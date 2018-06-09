// MT4SptDll.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "MT4_Supoort.h"

#include <stdio.h>
#include <mbstring.h>
#include <Shlwapi.h>

BOOL g_bTestMode = FALSE;
//BOOL g_bInit = FALSE;
DWORD g_dwProcId = 0;
DWORD g_dwThreadId = 0;

PSECURITY_DESCRIPTOR CreateSecurityDescriptor() {
	PSECURITY_DESCRIPTOR pd;
	SECURITY_ATTRIBUTES sa;
	if ((pd = (PSECURITY_DESCRIPTOR)LocalAlloc(LMEM_FIXED,SECURITY_DESCRIPTOR_MIN_LENGTH)) != NULL) {
		InitializeSecurityDescriptor(pd, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(pd, TRUE, (PACL)NULL, FALSE);
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = pd;
		sa.bInheritHandle = TRUE;
	}

	return pd;
}

void FreeSecurityDescriptor(PSECURITY_DESCRIPTOR pd) {
	if( pd )	LocalFree(pd);
}

DWORD MakeMapId2(){
	static DWORD dwCounter = 0;
	static HANDLE hMod;

	if(hMod == NULL)	hMod = GetModuleHandle(NULL);

	InterlockedIncrement( &dwCounter );


	return (DWORD)hMod + dwCounter;
}

HANDLE g_hSignalMap = NULL;
DWORD* g_pSignalMapData = NULL;

BOOL CallFunctionSignal(DWORD dwFuncNo, DWORD dwMapId, DWORD dwTimeOut ) 
{
	if(g_hSignalMap==NULL|| g_pSignalMapData==NULL)	return FALSE;

	while( InterlockedExchange(g_pSignalMapData,1) == 1)	Sleep(0);

	DWORD* pdwSetEvent = g_pSignalMapData + sizeof(DWORD);
	DWORD* pdwFunNo = g_pSignalMapData + sizeof(DWORD)*2;
	DWORD* pdwMapId = g_pSignalMapData + sizeof(DWORD)*3;

	*pdwFunNo = dwFuncNo;
	*pdwMapId = dwMapId;

	*pdwSetEvent = 1;

	//	Agent側で処理が終わり0になるのを待つ
	while( *pdwSetEvent == 1 )	Sleep(0);

	InterlockedExchange(g_pSignalMapData, 0);

	return TRUE;
}

//	処理を別プロセスに依頼し処理を待機する
//	とりあえずデータをMap、通知をMessageで行うが、そのうちRemoteThreadにしたいなぁ
BOOL CallFunction( DWORD dwFuncNo, DWORD dwMapId ) {
	BOOL bRet = FALSE;
	DWORD_PTR dwSendRet;
	HWND hWnd;
	char cWindName[256];

	if( dwFuncNo == MT4_INITILIZE || dwFuncNo == MT4_FINISHPROCESS ){
		if(g_bTestMode == FALSE) {
			lstrcpy(cWindName, AW_WINDOW_NAME);
		}
		else {
			lstrcpy(cWindName, AW_TETWINDOW_NAME);
		}
		hWnd = FindWindow("#32770", cWindName);

		if(hWnd != NULL) {
			if(dwFuncNo == MT4_INITILIZE ) {
				if(g_hSignalMap == NULL){
					PSECURITY_DESCRIPTOR pd = CreateSecurityDescriptor();

					if((g_hSignalMap = CreateFileMapping(NULL, pd, PAGE_READWRITE, 0, sizeof(DWORD)*4, cWindName)) != NULL) {
						if((g_pSignalMapData = (DWORD*)MapViewOfFile(g_hSignalMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
							memset(g_pSignalMapData, 0x00, sizeof(DWORD)*3);
						}
					}
					FreeSecurityDescriptor(pd);
				}
			}

			if (SendMessageTimeoutA(hWnd, WM_APP + 1000, (WPARAM)dwFuncNo, (LPARAM)dwMapId, SMTO_ABORTIFHUNG, 100000, &dwSendRet) != 0) {
				bRet = TRUE;
			}			

			if(dwFuncNo == MT4_FINISHPROCESS) {
				if(g_pSignalMapData != NULL) {
					UnmapViewOfFile(g_pSignalMapData);
					g_pSignalMapData = NULL;
				}
				if(g_hSignalMap != NULL) {
					CloseHandle(g_hSignalMap);
					g_hSignalMap = NULL;
				}
			}
		}
	}else{
		bRet = CallFunctionSignal( dwFuncNo, dwMapId, 100000 );
	}


	return bRet;
}


//	MT4起動時の初期化
MT4_EXPFUNC BOOL WINAPI MT4_Initilize( int nPair )
{
	BOOL bRet = FALSE;
	DWORD dwTick;
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];

	g_dwProcId = GetCurrentProcessId();
	g_dwThreadId = GetCurrentThreadId();

	dwTick = MakeMapId(g_dwProcId, g_dwThreadId);

	//if( InterlockedExchange( &g_bInit, 1 ) == 0  )
	{	//	初期化は基本一度でOK
		PSECURITY_DESCRIPTOR pd = CreateSecurityDescriptor();

		//	引数渡し用の領域を用意する
		PMT4_INITILIZE_DATA pMapData;

		wsprintf(cMapName, "MT4_Initilize_%d", dwTick);

		if ((hMap = CreateFileMapping(NULL, pd, PAGE_READWRITE, 0, sizeof(MT4_INITILIZE_DATA), cMapName)) != NULL) {
			if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
				pMapData = (PMT4_INITILIZE_DATA)pMap;

				memset(pMap, 0x00, sizeof(MT4_INITILIZE_DATA));

				pMapData->dwPair = nPair;
				//pMapData->dwPair = -1;

				CallFunction(MT4_INITILIZE, dwTick);

				if (pMapData->dwRet > 0)	bRet = TRUE;

				UnmapViewOfFile(pMap);
			}
			CloseHandle(hMap);
		}

		FreeSecurityDescriptor(pd);
	}
	return bRet;
}



//	Ask情報を70Tickチャートに登録する
MT4_EXPFUNC BOOL WINAPI MT4_RegisterRate( int nPair, double dRate )
{
	BOOL bRet = FALSE;
	DWORD dwTick = MakeMapId(g_dwProcId, g_dwThreadId);
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];
	PSECURITY_DESCRIPTOR pd = CreateSecurityDescriptor();


	//	引数渡し用の領域を用意する
	PMT4_REGISTERRATE_DATA pMapData;

	wsprintf(cMapName, "MT4_RegisterRate_%d", dwTick);

	if ((hMap = CreateFileMapping(NULL, pd, PAGE_READWRITE, 0, sizeof(MT4_REGISTERRATE_DATA), cMapName)) != NULL) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_REGISTERRATE_DATA)pMap;

			memset(pMap, 0x00, sizeof(MT4_REGISTERRATE_DATA));

			pMapData->dwPair = nPair;
			pMapData->dwRate = Rate2EVal( dRate, nPair );

			CallFunction(MT4_REGISTERRATE, dwTick);

			if (pMapData->dwRet > 0)	bRet = TRUE;

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	FreeSecurityDescriptor(pd);

	return bRet;
}

MT4_EXPFUNC BOOL WINAPI MT4_DbgRegisterTickInfo(int nPair, double dHi, double dLo, double dOp, double dCl)
{
	BOOL bRet = FALSE;
	DWORD dwTick = MakeMapId(g_dwProcId, g_dwThreadId);
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];
	PSECURITY_DESCRIPTOR pd = CreateSecurityDescriptor();


	//	引数渡し用の領域を用意する
	PMT4_DBGREGISTERTICKINFO_DATA pMapData;

	wsprintf(cMapName, "MT4_DbgRegisterTickInfo_%d", dwTick);

	if ((hMap = CreateFileMapping(NULL, pd, PAGE_READWRITE, 0, sizeof(MT4_DBGREGISTERTICKINFO_DATA), cMapName)) != NULL) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_DBGREGISTERTICKINFO_DATA)pMap;

			memset(pMap, 0x00, sizeof(MT4_DBGREGISTERTICKINFO_DATA));

			pMapData->dwPair = nPair;
			pMapData->dwHi = Rate2EVal( dHi, nPair);
			pMapData->dwLo = Rate2EVal( dLo, nPair);
			pMapData->dwOp = Rate2EVal( dOp, nPair);
			pMapData->dwCl = Rate2EVal( dCl, nPair);

			CallFunction(MT4_DBGREGISTERTICKINFO, dwTick);

			if (pMapData->dwRet > 0)	bRet = TRUE;

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	FreeSecurityDescriptor(pd);

	return bRet;
}

//	注文を行うかをチェックする
MT4_EXPFUNC int WINAPI MT4_TradeCheck(int nPair, int* pdBreasSign )
{
	int  nRet = 0;
	DWORD dwTick = MakeMapId(g_dwProcId, g_dwThreadId);
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];
	PSECURITY_DESCRIPTOR pd = CreateSecurityDescriptor();


	//	引数渡し用の領域を用意する
	PMT4_TRADECHECK_DATA pMapData;

	wsprintf(cMapName, "MT4_TradeCheck_%d", dwTick);

	if ((hMap = CreateFileMapping(NULL, pd, PAGE_READWRITE, 0, sizeof(MT4_TRADECHECK_DATA), cMapName)) != NULL) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_TRADECHECK_DATA)pMap;

			memset(pMap, 0x00, sizeof(MT4_TRADECHECK_DATA));

			pMapData->dwPair = nPair;

			CallFunction(MT4_TRADECHECK, dwTick);

			//	0:注文しない 1:売り注文 2:買い注文
			nRet = pMapData->dwRet;
			if (nRet > 0) {
				//	注文価格を引数にSet
				if (pdBreasSign != NULL)	*pdBreasSign = pMapData->dwRetBreakSign;
			}

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	FreeSecurityDescriptor(pd);

	return nRet;
}


//	行った注文情報を保持する
MT4_EXPFUNC BOOL WINAPI MT4_RegisterTradeInfo(int  nPair, int nTicket, double dOpenPrice, int nOrderType, double dStopLoss, _MqlDateTime* stOpenDate, DWORD dwBreakTrigger )
{
	BOOL  bRet = 0;
	DWORD dwTick = MakeMapId(g_dwProcId, g_dwThreadId);
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];
	PSECURITY_DESCRIPTOR pd = CreateSecurityDescriptor();


	//	引数渡し用の領域を用意する
	PMT4_REGISTERTRADEINFO_DATA pMapData;

	wsprintf(cMapName, "MT4_RegisterTradeInfo_%d", dwTick);

	if ((hMap = CreateFileMapping(NULL, pd, PAGE_READWRITE, 0, sizeof(MT4_REGISTERTRADEINFO_DATA), cMapName)) != NULL) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_REGISTERTRADEINFO_DATA)pMap;

			memset(pMap, 0x00, sizeof(MT4_REGISTERTRADEINFO_DATA));

			pMapData->dwPair = nPair;
			pMapData->dwTicket = nTicket;
			pMapData->dOpenPrice = dOpenPrice;
			pMapData->dwOrderType = nOrderType;
			pMapData->dStopLoss = dStopLoss;
			pMapData->stOpenDate.wYear = stOpenDate->year;
			pMapData->stOpenDate.wMonth = stOpenDate->mon;
			pMapData->stOpenDate.wDay = stOpenDate->day;
			pMapData->stOpenDate.wHour = stOpenDate->hour;
			pMapData->stOpenDate.wMinute = stOpenDate->min;
			pMapData->stOpenDate.wSecond = stOpenDate->sec;
			pMapData->dwBreakTrigger = dwBreakTrigger;

			CallFunction(MT4_REGISTERTRADEINFO, dwTick);

			if (pMapData->dwRet > 0)	bRet = TRUE;

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	FreeSecurityDescriptor(pd);

	return bRet;
}

//	注文を継続するかをチェックする
MT4_EXPFUNC BOOL WINAPI MT4_CheckCloseTrade(int  nPair, int nTicket )
{
	BOOL  bRet = 0;
	DWORD dwTick = MakeMapId(g_dwProcId, g_dwThreadId);
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];
	PSECURITY_DESCRIPTOR pd = CreateSecurityDescriptor();


	//	引数渡し用の領域を用意する
	PMT4_CHECKCLOSETRADE_DATA pMapData;

	wsprintf(cMapName, "MT4_CheckCloseTrade_%d", dwTick);

	if ((hMap = CreateFileMapping(NULL, pd, PAGE_READWRITE, 0, sizeof(MT4_CHECKCLOSETRADE_DATA), cMapName)) != NULL) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_CHECKCLOSETRADE_DATA)pMap;

			memset(pMap, 0x00, sizeof(MT4_CHECKCLOSETRADE_DATA));

			pMapData->dwPair = nPair;
			pMapData->dwTicket = nTicket;

			CallFunction(MT4_CHECKCLOSETRADE, dwTick);

			if (pMapData->dwRet > 0)	bRet = TRUE;

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	FreeSecurityDescriptor(pd);

	return bRet;
}

//	決算した注文を記録する
MT4_EXPFUNC BOOL WINAPI MT4_RegisterCloseTrade(int  nPair, int nTicket, double dClosePrice )
{
	BOOL  bRet = 0;
	DWORD dwTick = MakeMapId(g_dwProcId, g_dwThreadId);
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];
	PSECURITY_DESCRIPTOR pd = CreateSecurityDescriptor();


	//	引数渡し用の領域を用意する
	PMT4_REGISTERCLOSETRADE_DATA pMapData;

	wsprintf(cMapName, "MT4_RegisterCloseTrade_%d", dwTick);

	if ((hMap = CreateFileMapping(NULL, pd, PAGE_READWRITE, 0, sizeof(MT4_REGISTERCLOSETRADE_DATA), cMapName)) != NULL) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_REGISTERCLOSETRADE_DATA)pMap;

			memset(pMap, 0x00, sizeof(MT4_REGISTERCLOSETRADE_DATA));

			pMapData->dwPair = nPair;
			pMapData->dwTicket = nTicket;
			pMapData->dClosePrice = dClosePrice;

			CallFunction(MT4_REGISTERCLOSETRADE, dwTick);

			if (pMapData->dwRet > 0)	bRet = TRUE;

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	FreeSecurityDescriptor(pd);

	return bRet;
}

MT4_EXPFUNC void WINAPI MT4_SetTestModeFlag()
{
	g_bTestMode = TRUE;
}

MT4_EXPFUNC void WINAPI MT4_FinishProcess( DWORD dwPair )
{
	BOOL  bRet = 0;
	DWORD dwTick = MakeMapId(g_dwProcId, g_dwThreadId);
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];
	PSECURITY_DESCRIPTOR pd = CreateSecurityDescriptor();


	//	引数渡し用の領域を用意する
	PMT4_FINISHPROCESS_DATA pMapData;

	wsprintf(cMapName, "MT4_FinishProcess_%d", dwTick);

	if((hMap = CreateFileMapping(NULL, pd, PAGE_READWRITE, 0, sizeof(MT4_FINISHPROCESS_DATA), cMapName)) != NULL) {
		if((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_FINISHPROCESS_DATA)pMap;

			memset(pMap, 0x00, sizeof(MT4_FINISHPROCESS_DATA));

			pMapData->dwPair = dwPair;

			CallFunction(MT4_FINISHPROCESS, dwTick);

			if(pMapData->dwRet > 0)	bRet = TRUE;

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	FreeSecurityDescriptor(pd);

	return;
}




BOOL CheckDirectoryExist( char* pDir ){
	BOOL bRet = FALSE;

	if( GetFileAttributes( pDir ) == INVALID_FILE_ATTRIBUTES ){
		char cDir_Par[1024];
		char* pDirName;
		lstrcpy( cDir_Par, pDir);

		pDirName = (char*)_mbsrchr((unsigned char*)cDir_Par, (unsigned int)'\\');
		*pDirName = 0x00;
		if( CheckDirectoryExist(cDir_Par ) != FALSE ){
			bRet = CreateDirectory(pDir,NULL);
		}
	}else{
		bRet = TRUE;
	}

	return bRet;
}

#define MAX_CACHE_ADFFILE	(64)
struct _CACHE_ADFFILE
{
	DWORD dwThreadId;
	SYSTEMTIME LastWritetime;
	HANDLE hFile;
}g_CacheADF[MAX_CACHE_ADFFILE];

HANDLE GetChache(SYSTEMTIME* pTime, char* pPair )
{
	HANDLE hRet = NULL;
	DWORD dwThreadId = GetCurrentThreadId();
	DWORD i = 0;

	for( ; i<MAX_CACHE_ADFFILE; i++ ){
		if( g_CacheADF[i].dwThreadId == dwThreadId ){
			if( g_CacheADF[i].LastWritetime.wYear == pTime->wYear
			 && g_CacheADF[i].LastWritetime.wDay == pTime->wDay
			){
				hRet = g_CacheADF[i].hFile;
			}else{
				g_CacheADF[i].dwThreadId = 0;
				CloseHandle(g_CacheADF[i].hFile);
				g_CacheADF[i].hFile = NULL;

				//	日米のときはさらにスレッド生存チェックもしとく
				if( StrStrI( pPair, "usdjpy" ) != NULL ){
					for( DWORD j=0; j<MAX_CACHE_ADFFILE; j++) {
						if(g_CacheADF[i].dwThreadId != 0 && OpenThread(SYNCHRONIZE, FALSE, g_CacheADF[i].dwThreadId) == NULL  ){
							g_CacheADF[i].dwThreadId = 0;
							CloseHandle(g_CacheADF[i].hFile);
							g_CacheADF[i].hFile = NULL;
						}
					}
				}
			}
			break;
		}
	}

	return hRet;
}

void SetCache( HANDLE hFile, SYSTEMTIME* pTime )
{
	DWORD i = 0;

	for(; i<MAX_CACHE_ADFFILE; i++) {
		if( g_CacheADF[i].dwThreadId == 0 ){
			g_CacheADF[i].dwThreadId = GetCurrentThreadId();
			memcpy( &g_CacheADF[i].LastWritetime, pTime, sizeof(SYSTEMTIME) );
			g_CacheADF[i].hFile = hFile;
			break;
		}
	}

	return;
}

void ChangePairName( char* pOld, char* pNew )
{
	if( _memicmp("AUDJPY-cd", pOld, sizeof("AUDJPY-cd") - 1 ) == 0 ){
		lstrcpy( pNew, "AUDJPY" );
	}else if(_memicmp("CHFJPY-cd", pOld, sizeof("CHFJPY-cd") - 1) == 0) {
		lstrcpy(pNew, "CHFJPY");
	}
	else if(_memicmp("EURJPY-cd", pOld, sizeof("EURJPY-cd") - 1) == 0) {
		lstrcpy(pNew, "EURJPY");
	}
	else if(_memicmp("EURUSD-cd", pOld, sizeof("EURUSD-cd") - 1) == 0) {
		lstrcpy(pNew, "EURUSD");
	}
	else if(_memicmp("GBPJPY-cd", pOld, sizeof("GBPJPY-cd") - 1) == 0) {
		lstrcpy(pNew, "GBPJPY");
	}
	else if(_memicmp("GBPUSD-cd", pOld, sizeof("GBPUSD-cd") - 1) == 0) {
		lstrcpy(pNew, "GBPUSD");
	}
	else if(_memicmp("NZDJPY-cd", pOld, sizeof("NZDJPY-cd") - 1) == 0) {
		lstrcpy(pNew, "NZDJPY");
	}
	else if(_memicmp("USDCHF-cd", pOld, sizeof("USDCHF-cd") - 1) == 0) {
		lstrcpy(pNew, "USDCHF");
	}
	else if(_memicmp("USDJPY-cd", pOld, sizeof("USDJPY-cd") - 1) == 0) {
		lstrcpy(pNew, "USDJPY");
	}
	else if(_memicmp("ZARJPY-cd", pOld, sizeof("ZARJPY-cd") - 1) == 0) {
		lstrcpy(pNew, "ZARJPY");
	}else{
		lstrcpy( pNew, pOld );
	}
}
BOOL RateDataWrite( double dRate, char* pPair)
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	char cPath[1024];
	char cWriteData[1024];
	DWORD dwWrite;
	HANDLE hCacheHandle = NULL;

	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);

	hCacheHandle = GetChache( &CurTime, pPair );

	if( hCacheHandle != NULL ){
		hFile = hCacheHandle;
	}else{
		char cNewPairName[64];

		ChangePairName( pPair, cNewPairName );

		wsprintf(cPath, "%s\\ADF\\%s\\%02d\\%02d", _LOG_DIR_, cNewPairName, CurTime.wYear, CurTime.wMonth );

		if(CheckDirectoryExist(cPath) != FALSE) {
			wsprintf(cPath, "%s\\%02d.adf", cPath, CurTime.wDay);

			hFile = CreateFile(cPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if(hFile != INVALID_HANDLE_VALUE) {
				SetFilePointer(hFile, 0, 0, FILE_END);

				SetCache( hFile, &CurTime );
			}
		}
	}

	if(hFile != INVALID_HANDLE_VALUE) {
		sprintf_s(cWriteData, sizeof(cWriteData), "%02d:%02d:%02d.%03d,%f\r\n", CurTime.wHour, CurTime.wMinute, CurTime.wSecond, CurTime.wMilliseconds, dRate);

		WriteFile(hFile, cWriteData, lstrlen(cWriteData), &dwWrite, NULL);

		//CloseHandle(hFile);
	}

	return TRUE;
}

//	FXTFとは別にMT4にRateをFileOutするだけの関数
MT4_EXPFUNC BOOL WINAPI MT4_WriteRate( double dRate, wchar_t* pStrPair )
{
	BOOL bRet = FALSE;
	char cPair[64];

	char cTest[32];
	wsprintf(cTest, "%x", GetCurrentThreadId() );

	int ret = WideCharToMultiByte( CP_ACP, 0, pStrPair,	-1, cPair, sizeof(cPair), NULL, NULL);

	RateDataWrite( dRate, cPair);

	//MessageBoxA( NULL, cPair, cTest, MB_OK );

	return bRet;
}