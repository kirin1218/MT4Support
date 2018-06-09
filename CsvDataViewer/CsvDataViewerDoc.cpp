
// CsvDataViewerDoc.cpp : CCsvDataViewerDoc クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "CsvDataViewer.h"
#endif

#include "CsvDataViewerDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "MainFrm.h"
extern CMainFrame* g_pMainFrame;
// CCsvDataViewerDoc

IMPLEMENT_DYNCREATE(CCsvDataViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CCsvDataViewerDoc, CDocument)
//ON_COMMAND(ID_FILE_OPEN, &CCsvDataViewerDoc::OnFileOpen)
END_MESSAGE_MAP()



// CCsvDataViewerDoc コンストラクション/デストラクション

CCsvDataViewerDoc::CCsvDataViewerDoc()
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。
	m_bCurRateMode = FALSE;

	m_PrevTradeFile[0] = 0x00;

}

CCsvDataViewerDoc::~CCsvDataViewerDoc()
{
}

BOOL CCsvDataViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。
	//OnFileOpen();

	return TRUE;
}




// CCsvDataViewerDoc シリアル化

void CCsvDataViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 格納するコードをここに追加してください。
	}
	else
	{
		char cFilePath[1024];
		char cFileName[256];
		char* pFileName;
		lstrcpy( cFilePath, ar.m_strFileName );

		pFileName = (char*)_mbsrchr((unsigned char*)cFilePath, (unsigned int)'\\');
		lstrcpy(cFileName, pFileName+1);

		//	現在のレート表示
		if( StrStrI(cFileName, "current") != NULL ){
			char* pTemp = StrStrI(cFileName, "_current");
			*pTemp = 0x00;

			m_bCurRateMode = TRUE;
		}
		//	TKIファイルの読み込み
		else{
			;;
		}

		//	データの読み込み
		ReadTickFile( cFilePath );

		//CalculateIndicatorValue();

		//ReadTradeFile();

		//SetTitle(cFileName);
	}
}

#ifdef SHARED_HANDLERS

//縮小版のサポート
void CCsvDataViewerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// このコードを変更してドキュメントのデータを描画します
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 検索ハンドラーのサポート
void CCsvDataViewerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ドキュメントのデータから検索コンテンツを設定します。
	// コンテンツの各部分は ";" で区切る必要があります

	// 例:      strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CCsvDataViewerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CCsvDataViewerDoc 診断

#ifdef _DEBUG
void CCsvDataViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCsvDataViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCsvDataViewerDoc コマンド

#if 0
void CCsvDataViewerDoc::OnFileOpen()
{
	;;
}
#endif

void DebugPrint(char* pszFormat, ...);

bool CCsvDataViewerDoc::ReadCsvData(const char* filename, vector<vector<string>>& vec_table)
{
	const char delimiter = ',';
	// ファイルを開く
	fstream filestream(filename);
	if(!filestream.is_open())
	{
		// ファイルが開けなかった場合は終了する
		return false;
	}

	vec_table.clear();

	// ファイルを読み込む
	while(!filestream.eof())
	{
		// １行読み込む
		string buffer;
		filestream >> buffer;

		// ファイルから読み込んだ１行の文字列を区切り文字で分けてリストに追加する
		vector<string> record;              // １行分の文字列のリスト
		istringstream streambuffer(buffer); // 文字列ストリーム
		string token;                       // １セル分の文字列
		while(getline(streambuffer, token, delimiter))
		{
			// １セル分の文字列をリストに追加する
			record.push_back(token);
		}

		// １行分の文字列を出力引数のリストに追加する
		vec_table.push_back(record);
	}

	if(vec_table.size() > 0) {
		return true;
	}
	else {
		return false;
	}
}

bool CCsvDataViewerDoc::ReadTickFile( char* pTickFilePath ) {
	bool bRet = false;

	HANDLE hFile;
	TICK_INFO readata;
	DWORD dwRead;
	char cPath[1024];
	DWORD i=0;

	if( pTickFilePath != NULL ){
		lstrcpy( cPath, pTickFilePath );
	}else{
		lstrcpy( cPath, GetPathName() );
	}

	char* pExt;

	pExt = (char*)_mbsrchr((unsigned char*)cPath, (unsigned int)'.');

	
	m_TickInfo.clear();

	m_dwTickType = GetTickTypeFromStrI( cPath );
	m_dwPair = GetPairIdFromStrI( cPath );
	m_dSpread = GetSpread( m_dwPair )/2;


	if( StrStrI( cPath, "current" ) != NULL ) {
		CTime CurTime = CTime::GetCurrentTime();
		CTime time;
		char cPath[1024];

		for( int j = CurTime.GetDayOfWeek() - 1; j >= 0; j-- ){
			time = CurTime - CTimeSpan( j, 0, 0, 0);

			wsprintf(cPath, "%s\\ADF\\%s\\%02d\\%02d\\%02d.adf", _LOG_DIR_, GetPairName(m_dwPair), time.GetYear(), time.GetMonth(), time.GetDay());

			if( GetFileAttributes(cPath ) != INVALID_FILE_ATTRIBUTES ){
				HANDLE hFile;

				hFile = CreateFile(cPath, GENERIC_READ, FILE_SHARE_READ| FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if( hFile != INVALID_HANDLE_VALUE ){
					DWORD dwFileSize = GetFileSize( hFile, NULL );
					DWORD dwRead;
					char* pData;
					char* pCur;
					char* pPoint;
					char cTime[1024];
					char cRate[1024];

					if( ( pData = (char*)LocalAlloc( LMEM_FIXED, dwFileSize + 1 )) != NULL) {
						ReadFile( hFile, pData, dwFileSize, &dwRead, NULL );
						pData[dwFileSize-1] = 0x00;

						pCur = pData;

						while( *pCur != 0x00 ){
							pPoint = strchr(pCur, ',');
							memcpy(cTime, pCur, (DWORD)(pPoint-pCur) );
							cTime[(DWORD)(pPoint - pCur)] = 0x00;
							pCur = pPoint + 1;

							pPoint = strchr(pCur, '\r');
							if(pPoint != NULL ){
								memcpy(cRate, pCur, (DWORD)(pPoint - pCur));
								cRate[(DWORD)(pPoint - pCur)] = 0x00;
								pCur = pPoint + 1;

								if(*pCur == '\n') {
									pCur++;
								}

							}else{
								lstrcpy(cRate, pCur );
								pCur += lstrlen(cRate);
							}

							if(cTime[0] != 0x00 && cRate[0] != 0x00) {
								double dRate = atof(cRate);

								if(dRate > 0) {	//	原因は不明だけどadfに0が書かれていることがあるため
												//	データファイル内のAsk情報を登録する
									DWORD dwRate = Rate2EVal(dRate, m_dwPair);
									TICK_INFO AddData;

									if(dwRate == 0)	return FALSE;

									SYSTEMTIME systime;

									GetLocalTime(&systime);

									//	時分秒ミリ秒はファイル情報を使う
									DWORD l;

									for(l = 0; cTime[l] != 0x00; l++) {
										if(cTime[l] == ':' || cTime[l] == '.') {
											cTime[l] = 0x00;
										}
									}
									char* pCurTimePtr = cTime;
									systime.wHour = atoi(pCurTimePtr);		pCurTimePtr += lstrlen(pCurTimePtr)+1;
									systime.wMinute = atoi(pCurTimePtr);	pCurTimePtr += lstrlen(pCurTimePtr)+1;
									systime.wSecond = atoi(pCurTimePtr);	pCurTimePtr += lstrlen(pCurTimePtr)+1;
									systime.wMilliseconds = atoi(pCurTimePtr);

									DWORD dwEnd = m_TickInfo.size() - 1;


									//	Tickを切り替える(初回もこっちに入れる
									if(m_TickInfo.size() == 0 || m_TickInfo[dwEnd].bCl != FALSE ) {
										memset(&AddData, 0x00, sizeof(TICK_INFO));

										AddData.Ver = TICK_LATEST_VER;
										AddData.Hi = dwRate;
										AddData.Op = dwRate;
										AddData.Cl = dwRate;
										AddData.Lo = dwRate;

										memcpy(&AddData.sDt, &systime, sizeof(SYSTEMTIME));
										AddData.ema = 0;
										AddData.Tc = 1;

										m_TickInfo.push_back(AddData);
									}
									//	最後のTickデータを更新する
									else {
										//	最高または最低を必要があれば更新
										if(m_TickInfo[dwEnd].Hi < dwRate) {
											m_TickInfo[dwEnd].Hi = dwRate;
										}
										else if(m_TickInfo[dwEnd].Lo > dwRate) {
											m_TickInfo[dwEnd].Lo = dwRate;
										}

										//	最終は常に更新する
										if(m_TickInfo[dwEnd].Cl != dwRate) {
											m_TickInfo[dwEnd].Cl = dwRate;
										}

										m_TickInfo[dwEnd].Tc++;

										BOOL bTickEnd = FALSE;

										if(i == TKI_70TICK) {
											if( m_TickInfo[dwEnd].Tc > 70 ){
												bTickEnd = TRUE;
											}
										}else if(i == TKI_1MIN) {
											if( m_TickInfo[dwEnd].sDt.wMinute !=  systime.wMinute ){
												bTickEnd = TRUE;
											}
										}else if(i == TKI_5MIN) {
											;
										}else if(i == TKI_15MIN) {
											;
										}else if(i == TKI_1HOUR) {
											;
										}

										if( bTickEnd != FALSE ) {
											memcpy(&m_TickInfo[dwEnd].eDt, &systime, sizeof(SYSTEMTIME));

											m_TickInfo[dwEnd].bCl = TRUE;
										}
									}
								}
							}
						}

						//	一行ごとに
						LocalFree(pData );
					}

					CloseHandle(hFile);
				}
			}
		}
	}
	else{
		if ((hFile = CreateFile(cPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL))
			!= INVALID_HANDLE_VALUE
			) {
			SetFilePointer(hFile, 0, 0, FILE_BEGIN);

			while (ReadFile(hFile, &readata, sizeof(TICK_INFO), &dwRead, NULL) != FALSE && sizeof(TICK_INFO) == dwRead) {
				if( readata.Ver <= 2 ){
					readata.Op = Rate2EVal((*(double*)(&readata.Op)), m_dwPair);
					readata.Cl = Rate2EVal((*(double*)(&readata.Cl)), m_dwPair);
					readata.Hi = Rate2EVal((*(double*)(&readata.Hi)), m_dwPair);
					readata.Lo = Rate2EVal((*(double*)(&readata.Lo)), m_dwPair);
					m_TickInfo.push_back(readata);
				}else{
					if( ABS_CMP(readata.Hi, readata.Lo, 100) < 0
						&& ABS_CMP(readata.Op, readata.Op, 100) < 0
					){
						m_TickInfo.push_back(readata);
					}
				}
			}

			CloseHandle(hFile);
		}

		char cFilePath[1024];

		wsprintf( cFilePath, "%s\\TickInfo_%s.csv", _LOG_DIR_, GetPairName(m_dwPair) );
		HANDLE hFile;
		DWORD dwWrite;
		char cDate[1024];


		if((hFile = CreateFile(cFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
			SetFilePointer(hFile, 0, 0, FILE_END);
			for(DWORD i=0; i < m_TickInfo.size(); i++) {
				wsprintf( cDate, "%02d/%02d %02d:%02d:%02d.%03d,", m_TickInfo[i].sDt.wMonth, m_TickInfo[i].sDt.wDay, m_TickInfo[i].sDt.wHour, m_TickInfo[i].sDt.wMinute, m_TickInfo[i].sDt.wSecond, m_TickInfo[i].sDt.wMilliseconds );
				WriteFile(hFile, cDate, lstrlen(cDate), &dwWrite, NULL);
				wsprintf( cDate, "%02d/%02d %02d:%02d:%02d.%03d,", m_TickInfo[i].eDt.wMonth, m_TickInfo[i].eDt.wDay, m_TickInfo[i].eDt.wHour, m_TickInfo[i].eDt.wMinute, m_TickInfo[i].eDt.wSecond, m_TickInfo[i].eDt.wMilliseconds );
				WriteFile(hFile, cDate, lstrlen(cDate), &dwWrite, NULL);
				wsprintf( cDate, "%d,%d,%d,%d,%d,%d,%d,%d\r\n", m_TickInfo[i].Op, m_TickInfo[i].Hi, m_TickInfo[i].Lo, m_TickInfo[i].Cl, m_TickInfo[i].dwUpVal, m_TickInfo[i].dwUpCnt, m_TickInfo[i].dwDnVal, m_TickInfo[i].dwDnCnt );
				WriteFile(hFile, cDate, lstrlen(cDate), &dwWrite, NULL);
			}

			CloseHandle(hFile);
		}

		//DWORD dwEnd = m_TickInfo.size()-1;


		//for( DWORD i = dwEnd; i > 0; i-- ){
		//	for( DWORD j = i-1; j > 0; j-- ){
		//		DWORD dwMatch;
		//		for( dwMatch=0;; dwMatch++ ){
		//			if(  j-dwMatch > 0 ){
		//				if( MatchTickData( i-dwMatch, j-dwMatch ) == FALSE ){
		//					break;
		//				}
		//			}else{
		//				break;
		//			}
		//		}

		//		if( dwMatch >= 12 ){
		//			DebugPrint( "%d,%02d/%02d %02d:%02d,%02d/%02d %02d:%02d,", dwMatch, 
		//				m_TickInfo[i].sDt.wMonth, m_TickInfo[i].sDt.wDay, m_TickInfo[i].sDt.wHour, m_TickInfo[i].sDt.wMinute,
		//				m_TickInfo[j].sDt.wMonth, m_TickInfo[j].sDt.wDay, m_TickInfo[j].sDt.wHour, m_TickInfo[j].sDt.wMinute
		//			);
		//			DWORD k;
		//			for( k = i; k >= i-dwMatch; k-- ){
		//				TICK_MINI_DATA tData;

		//				GetTickMiniData( k, &tData);
		//				DebugPrint( "%d,", tData.dwDiff );
		//			}

		//			//for( k = j; k >= j-dwMatch; k-- ){
		//			//	TICK_MINI_DATA tData;

		//			//	GetTickMiniData( k, &tData);
		//			//	DebugPrint( "%d,", tData.dwDiff );
		//			//}

		//			DebugPrint( "\r\n" );
		//		}
		//	}
		//}
	}

	if (m_TickInfo.size() > 0) {
		bRet = true;
	}

	return bRet;
}

BOOL CCsvDataViewerDoc::MatchTickData( DWORD d1, DWORD d2 ) 
{
	BOOL bRet = FALSE;
	TICK_MINI_DATA tData1;
	TICK_MINI_DATA tData2;

	GetTickMiniData( d1, &tData1);
	GetTickMiniData( d2, &tData2);

	if(tData1.dwType == tData2.dwType ){
		if( 0 <= tData1.dwDiff && tData1.dwDiff < 20
		 && 0 <= tData2.dwDiff && tData2.dwDiff < 20
		){
			bRet = TRUE;
		}else 
		if( 20 <= tData1.dwDiff && tData1.dwDiff < 40
		 && 20 <= tData2.dwDiff && tData2.dwDiff < 40
		){
			bRet = TRUE;
		}else 
		if( 40 <= tData1.dwDiff && tData1.dwDiff < 60
		 && 40 <= tData2.dwDiff && tData2.dwDiff < 60
		){
			bRet = TRUE;
		}else 
		if( 60 <= tData1.dwDiff && tData1.dwDiff < 80
		 && 60 <= tData2.dwDiff && tData2.dwDiff < 80
		){
			bRet = TRUE;
		}else 
		if( 80 <= tData1.dwDiff
		 && 80 <= tData2.dwDiff
		){
			bRet = TRUE;
		}	
	}

	return bRet;
}

void CCsvDataViewerDoc::GetTickMiniData( DWORD i, TICK_MINI_DATA* pData ) {
	if( m_TickInfo[i].Cl > m_TickInfo[i].Op ){
		pData->dwType = 1;
		//pData->dwDiff = EValFractionCut( m_TickInfo[i].Cl - m_TickInfo[i].Op, 1 );
		pData->dwDiff = m_TickInfo[i].Cl - m_TickInfo[i].Op;
	}else{
		pData->dwType = 2;
		//pData->dwDiff = EValFractionCut( m_TickInfo[i].Op - m_TickInfo[i].Cl, 1 );
		pData->dwDiff = m_TickInfo[i].Op - m_TickInfo[i].Cl;
	}
}

bool CCsvDataViewerDoc::ReadRateChkFile() {
	bool bRet = false;

	HANDLE hFile;
	RATE_CHECK_INFO readata;
	DWORD dwRead;
	char cTradeFile[1024];
	char* pFileName;
	char* pExt;

	//	tkiのファイル名を取得
	lstrcpy( cTradeFile, GetPathName() );

	//	\TKI\usdjpy_20161027.tki::通常TKIファイル
	//	↓↓↓↓↓↓
	//	\TRI\usdjpy_20161027.tri
	//
	//	\TestMode_Result\20161029_142530\usdjpy.tki::テストモード時のTKIファイル
	//	↓↓↓↓↓↓
	//	\TestMode_Result\20161029_142530\usdjpy.tri
	//	TKIファイル名からTRIファイルを作成
	if( StrStrI( cTradeFile, "TestMode_Result") == NULL ){
		char* pTemp;
		pFileName = (char*)_mbsrchr((unsigned char*)cTradeFile, (unsigned int)'\\');
		pFileName++;

		if( ( pTemp = (char*)_mbschr((unsigned char*)pFileName, (unsigned int)'_')) != NULL ){
			lstrcpy( pFileName, pTemp+1 );
		}

		if( ( pFileName = StrStrI(cTradeFile, "TKI") ) != NULL) {
			memcpy(pFileName, "RCI", sizeof("RCI")-1 );
		}
	}

	if( ( pExt = (char*)_mbsrchr((unsigned char*)cTradeFile, (unsigned int)'.') ) != NULL ){
		lstrcpy( pExt, ".rci" );
	}

	if( GetFileAttributes(cTradeFile) == INVALID_FILE_ATTRIBUTES ){	
		lstrcpy( cTradeFile, g_pMainFrame->m_strTriFile.c_str() );
	}

	if( _stricmp( m_PrevRCIFile, cTradeFile ) == 0 )	return TRUE;

	m_RateCheckInfo.clear();

	if((hFile = CreateFile( cTradeFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL))
		!= INVALID_HANDLE_VALUE
		) {
		SetFilePointer(hFile, 0, 0, FILE_BEGIN);

		while(ReadFile(hFile, &readata, sizeof(RATE_CHECK_INFO), &dwRead, NULL) != FALSE && sizeof(RATE_CHECK_INFO) == dwRead) {
			//if( readata.dwPair == m_dwPair )
			{
				//if( readata.dOpenPrice > 0 && readata.dClosePrice > 0 )
				{
					m_RateCheckInfo.push_back(readata);
				}
			}
		}

		lstrcpy( m_PrevRCIFile, cTradeFile );
		CloseHandle(hFile);
	}

	if(m_RateCheckInfo.size() > 0) {
		g_pMainFrame->UpdateRateChkInfoListCtrl(&m_RateCheckInfo);

		bRet = true;
	}

	return bRet;
}

bool CCsvDataViewerDoc::ReadTradeFile() {
	bool bRet = false;

	HANDLE hFile;
	TRADE_INFO readata;
	DWORD dwRead;
	char cTradeFile[1024];
	char* pFileName;
	char* pExt;

	//	tkiのファイル名を取得
	lstrcpy( cTradeFile, GetPathName() );

	//	\TKI\usdjpy_20161027.tki::通常TKIファイル
	//	↓↓↓↓↓↓
	//	\TRI\usdjpy_20161027.tri
	//
	//	\TestMode_Result\20161029_142530\usdjpy.tki::テストモード時のTKIファイル
	//	↓↓↓↓↓↓
	//	\TestMode_Result\20161029_142530\usdjpy.tri
	//	TKIファイル名からTRIファイルを作成
	if( StrStrI( cTradeFile, "TestMode_Result") == NULL ){
		char* pTemp;
		pFileName = (char*)_mbsrchr((unsigned char*)cTradeFile, (unsigned int)'\\');
		pFileName++;

		if( ( pTemp = (char*)_mbschr((unsigned char*)pFileName, (unsigned int)'_')) != NULL ){
			lstrcpy( pFileName, pTemp+1 );
		}

		if( ( pFileName = StrStrI(cTradeFile, "TKI") ) != NULL) {
			memcpy(pFileName, "TRI", sizeof("TRI")-1 );
		}
	}

	if( ( pExt = (char*)_mbsrchr((unsigned char*)cTradeFile, (unsigned int)'.') ) != NULL ){
		lstrcpy( pExt, ".tri" );
	}

	if( GetFileAttributes(cTradeFile) == INVALID_FILE_ATTRIBUTES ){	
		lstrcpy( cTradeFile, g_pMainFrame->m_strTriFile.c_str() );
	}

	if( _stricmp( m_PrevTradeFile, cTradeFile ) == 0 )	return TRUE;

	m_TradeInfo.clear();

	if((hFile = CreateFile( cTradeFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL))
		!= INVALID_HANDLE_VALUE
		) {
		SetFilePointer(hFile, 0, 0, FILE_BEGIN);

		while(ReadFile(hFile, &readata, sizeof(TRADE_INFO), &dwRead, NULL) != FALSE && sizeof(TRADE_INFO) == dwRead) {
			if( readata.dwPair == m_dwPair ){
				if( readata.dOpenPrice > 0 && readata.dClosePrice > 0 ){
					m_TradeInfo.push_back(readata);
				}
			}
		}

		lstrcpy( m_PrevTradeFile, cTradeFile );
		CloseHandle(hFile);
	}

	if(m_TradeInfo.size() > 0) {
		g_pMainFrame->UpdateTradeInfoListCtrl(&m_TradeInfo);

		bRet = true;
	}

	return bRet;
}

//	2回目以降の計算式が胡散臭いかも
DWORD CCsvDataViewerDoc::CalcEMAValue( DWORD idx, DWORD dwEmaVal )
{
	DWORD dwRet = 0;
	bool bFirst = false;
	int nCurIdx = idx;
	DWORD sumCl = 0;
	int i;

	if (idx < dwEmaVal - 1) {
		return 0;
	}

	if (idx <= dwEmaVal - 1) {
		bFirst = true;
	}

	sumCl = 0;
	for (i = idx; i > (int)idx - (int)dwEmaVal; i--) {
		sumCl += m_TickInfo[i].Cl;
	}

	if (bFirst == false) {
		sumCl += m_TickInfo[idx].Cl;
		dwRet = sumCl / (dwEmaVal + 1);
	}
	else {
		dwRet = sumCl / dwEmaVal;
	}

	return dwRet;
}

DWORD CCsvDataViewerDoc::CalcSMAValue( DWORD idx, DWORD dwSmaVal )
{
	DWORD dwRet = 0;
	int nCurIdx = idx;
	DWORD sumCl = 0;
	int i;

	if (idx < dwSmaVal - 1) {
		return 0;
	}

	sumCl = 0;
	for (i = idx; i > (int)idx - (int)dwSmaVal; i--) {
		sumCl += m_TickInfo[i].Cl;
	}

	dwRet = sumCl / dwSmaVal;
	
	return dwRet;
}

bool CCsvDataViewerDoc::CalcKLKH( DWORD dwStart, DWORD dwKSpan, DWORD* pdwHi, DWORD* pdwLo )
{
	bool bRet = false;
	DWORD i;
	DWORD dwEnd;
	DWORD dwRet = 0;
	DWORD dwHi = 0;
	DWORD dwLo = 777777;

	if (pdwHi == NULL || pdwLo == NULL)	return false;

	dwEnd = dwStart - dwKSpan;

	for (i = dwStart; i > dwEnd; i--) {
		//DebugPrint( "%.3f=%.3f,", m_TickInfo[i].Hi, m_TickInfo[i].Lo);
		if (m_TickInfo[i].Hi > dwHi)	dwHi = m_TickInfo[i].Hi;
		if (m_TickInfo[i].Lo < dwLo)	dwLo = m_TickInfo[i].Lo;
	}

	//DebugPrint("[Hi]%.3f=[Lo]%.3f,", dHi, dLo);
	*pdwHi = dwHi;
	*pdwLo = dwLo;

	if( dwHi == 0 || dwLo == 0 ){
		int a;
		a = 3;
	}
	return true;
}

DWORD CCsvDataViewerDoc::CalcD(DWORD dwStart, DWORD dwKSpan, DWORD dwDSpan, DWORD dwSDSpan )
{
	DWORD i;
	DWORD dwEnd;
	DWORD dwDL = 0;
	DWORD dwDH = 0;
	DWORD dwHi;
	DWORD dwLo;

	dwEnd = dwStart + 1 - dwDSpan;
	for (i = dwStart; i >= dwEnd; i--) {
		CalcKLKH(i, dwDSpan, &dwHi, &dwLo);

		dwDH += m_TickInfo[i].Cl - dwLo;
		dwDL += dwHi - dwLo;
	}

	if(dwDL>0){
		return (dwDH * 100) / dwDL;
	}else{
		return 0;
	}
}
//
////*******************************************************************************************
////%K
////%K＝{ （C－L9）÷（H9－L9） }×100％
////C：当日終値
////L9：過去x日間の最安値。xとしては、14, 9, 5 などが使用されることが多い。
////H9：過去x日間の最高値
////
////
////%D
////%D＝（H3÷L3）×100％
////H3：（C－L9）のy日間合計。（C－L9）の単純移動平均。yとしては3が使われることが多い。
////L3：（H9－L9）のy日間合計。（H9－L9）の単純移動平均。
////
////Slow%D
////Slow%D＝%Dのz日の単純移動平均。zとしては、3が使われることが多い。
////*******************************************************************************************
//void CCsvDataViewerDoc::CalcStochasticOscillatorValue( TICK_INDICATOR_INFO* pTickIndicate, DWORD dwStart, DWORD dwKSpan, DWORD dwDSpan, DWORD dwSDSpan)
//{
//	DWORD i;
//	DWORD dwEnd;
//
//	DWORD dwHi;
//	DWORD dwLo;
//	DWORD dwSlowDSum = 0;
//
//
//	if ( dwStart < dwKSpan + (dwDSpan-1))	return;
//
//	//	%Kの計算
//	CalcKLKH(dwStart, dwKSpan, &dwHi, &dwLo);
//	pTickIndicate->dwSO_K = (m_TickInfo[dwStart].Cl - dwLo)*100/(dwHi-dwLo);
//	//DebugPrint("CL=%.3f,Hi=%.3f,Lo=%.3f,K=%.3f\r\n", m_TickInfo[dwStart].Cl, dHi, dLo, pTickIndicate->dSO_K );
//	//	%Dの計算
//	pTickIndicate->dwSO_D = CalcD( dwStart, dwKSpan, dwDSpan, dwSDSpan );
//
//	//	Slow%Dの計算
//	dwEnd = dwStart + 1 - dwSDSpan;
//	for (i = dwStart; i >= dwEnd; i--) {
//		dwSlowDSum += CalcD( i, dwKSpan, dwDSpan, dwSDSpan);
//	}
//	pTickIndicate->dwSO_SlowD = dwSlowDSum / dwSDSpan;
//
//	return;
//
//}

void CCsvDataViewerDoc::CalculateIndicatorValue() 
{
	TICK_INDICATOR_INFO AddInfo;
	for (DWORD i = 0; i < m_TickInfo.size(); i++) {
		memset(&AddInfo, 0, sizeof(TICK_INDICATOR_INFO));

		//if (m_TickInfo[i].Tc < REM_TICK_CNT) {
		//	break;	//	breakの方がいいかな？途中にあったら不正データやね
		//}
		AddInfo.dw13ema = CalcEMAValue(i, 13);
		AddInfo.dw21ema = CalcEMAValue(i, 21);
		AddInfo.dw20ema = CalcEMAValue(i, 20);
		AddInfo.dw50ema = CalcEMAValue(i, 50);
		AddInfo.dw200ema = CalcEMAValue(i, 200);

		//CalcStochasticOscillatorValue( &AddInfo, i, 10, 3, 5);
		CalcStochasticOscillatorValue(&AddInfo, i, 9, 3, 3);

		m_TickIndicteInfo.push_back(AddInfo);	//	vectorじゃなくてmapの方がええかな。。。
	}
}

BOOL CCsvDataViewerDoc::UpdateTKIFile( char* pTKIPath )
{
	BOOL bUpdate = FALSE;

	char cADFPath[1024];

	lstrcpy(cADFPath, pTKIPath);

	char* pExt;
	char* pDir;
	DWORD dwPair = -1;

	if( ( pDir = StrStrI( cADFPath, "\\TKI\\" ) ) != NULL ){
		memcpy( pDir, "\\ADF\\", sizeof( "\\ADF\\" ) -1 );

		pDir += sizeof("\\ADF\\")-1;

		if( ( pExt = (char*)_mbsrchr((unsigned char*)cADFPath, (unsigned int)'.') ) != NULL ){
			lstrcpy( pExt, ".adf" );

			dwPair = GetPairIdFromStrI( pDir );
			pDir += sizeof( "USDJPY" ) -1 + 1;

			if( dwPair != (DWORD)-1 ){
				char cTimeInfo[128];
				DWORD l;
				SYSTEMTIME systime;

				GetLocalTime(&systime);
				lstrcpy( cTimeInfo, pDir );

				for(l = 0; cTimeInfo[l] != 0x00; l++) {
					if(cTimeInfo[l] == '\\' || cTimeInfo[l] == '.') {
						cTimeInfo[l] = 0x00;
					}
				}
				char* pCurTimePtr = cTimeInfo;
				systime.wYear = atoi(pCurTimePtr);	pCurTimePtr += lstrlen(pCurTimePtr) + 1;
				systime.wMonth = atoi(pCurTimePtr);	pCurTimePtr += lstrlen(pCurTimePtr) + 1;
				systime.wDay = atoi(pCurTimePtr);

				vector<vector<string>> RateFileTable;
				vector<TICK_INFO> TickInfo;

				if( ReadCsvData(cADFPath, RateFileTable) != false) {
					vector<vector<string>>::iterator itr;

					for(itr = RateFileTable.begin(); itr != RateFileTable.end(); itr++) {
						vector<string>& str_vec = (*itr);

						if(str_vec.size() >= 2) {
							double dRate = atof(str_vec[1].c_str());

							if(dRate > 0) {	//	原因は不明だけどadfに0が書かれていることがあるため
											//	データファイル内のAsk情報を登録する
								DWORD dwRate = Rate2EVal(dRate, dwPair);
								TICK_INFO AddData;

								if(dwRate == 0)	return 0;

								DWORD dwEnd = TickInfo.size() - 1;


								//	Tickを切り替える(初回もこっちに入れる
								if(TickInfo.size() == 0 || TickInfo[dwEnd].Tc >= 70) {


									//	時分秒ミリ秒はファイル情報を使う
									lstrcpy(cTimeInfo, str_vec[0].c_str());

									for(l = 0; cTimeInfo[l] != 0x00; l++) {
										if(cTimeInfo[l] == ':' || cTimeInfo[l] == '.') {
											cTimeInfo[l] = 0x00;
										}
									}
									char* pCurTimePtr = cTimeInfo;
									systime.wHour = atoi(pCurTimePtr);		pCurTimePtr += lstrlen(pCurTimePtr) + 1;
									systime.wMinute = atoi(pCurTimePtr);	pCurTimePtr += lstrlen(pCurTimePtr) + 1;
									systime.wSecond = atoi(pCurTimePtr);	pCurTimePtr += lstrlen(pCurTimePtr) + 1;
									systime.wMilliseconds = atoi(pCurTimePtr);

									if(TickInfo.size() > 0) {
										memcpy(&TickInfo[dwEnd].eDt, &systime, sizeof(SYSTEMTIME));
									}

									memset(&AddData, 0x00, sizeof(TICK_INFO));

									AddData.Ver = TICK_LATEST_VER;
									AddData.Hi = dwRate;
									AddData.Op = dwRate;
									AddData.Cl = dwRate;
									AddData.Lo = dwRate;


									memcpy(&AddData.sDt, &systime, sizeof(SYSTEMTIME));
									AddData.ema = 0;
									AddData.Tc = 1;

									TickInfo.push_back(AddData);
								}
								//	最後のTickデータを更新する
								else {
									//	最高または最低を必要があれば更新
									if(TickInfo[dwEnd].Hi < dwRate) {
										TickInfo[dwEnd].Hi = dwRate;
									}
									else if(TickInfo[dwEnd].Lo > dwRate) {
										TickInfo[dwEnd].Lo = dwRate;
									}

									//	最終は常に更新する
									if(TickInfo[dwEnd].Cl != dwRate) {
										TickInfo[dwEnd].Cl = dwRate;
									}

									TickInfo[dwEnd].Tc++;
								}
							}
						}
					}

					HANDLE hFile;
					DWORD dwWrite;

					if((hFile = CreateFile(pTKIPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
						vector<TICK_INFO>::iterator itr_TickInfo = TickInfo.begin();

						for( ; itr_TickInfo != TickInfo.end(); itr_TickInfo++ ){
							TICK_INFO* pWriteData = &(*itr_TickInfo);
							WriteFile(hFile, pWriteData, sizeof(TICK_INFO), &dwWrite, NULL);
						}

						bUpdate = TRUE;

						CloseHandle(hFile);
					}

					TickInfo.clear();
					RateFileTable.clear();
				}
			}
		}
	}

	return bUpdate;
}


#define DELETE_EXCEPTION(e) do { if(e) { e->Delete(); } } while (0)
BOOL CCsvDataViewerDoc::OnOpenDocument(LPCTSTR lpszPathName)
{

	//UpdateTKIFile( (char*)lpszPathName );

#if 1
#ifdef _DEBUG
	if(IsModified())
		TRACE(traceAppMsg, 0, "Warning: OnOpenDocument replaces an unsaved document.\n");
#endif

	ENSURE(lpszPathName);

	CFileException* pfe = new CFileException;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead | CFile::shareDenyNone, pfe);
	if(pFile == NULL)
	{
		TRY
		{
			ReportSaveLoadException(lpszPathName, pfe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
			END_TRY
			DELETE_EXCEPTION(pfe);
		return FALSE;
	}

	DELETE_EXCEPTION(pfe);

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
	//if(pFile->GetLength() != 0)
		Serialize(loadArchive);     // load me
	loadArchive.Close();
	ReleaseFile(pFile, FALSE);
	}
		CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		DeleteContents();   // remove failed contents

		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
			END_TRY
			DELETE_EXCEPTION(e);
		return FALSE;
	}
	END_CATCH_ALL
#endif
//	this->SetPathName(lpszPathName);



	SetModifiedFlag(FALSE);

	return TRUE;
}


void CCsvDataViewerDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CDocument::SetPathName(lpszPathName, bAddToMRU);
}


void CCsvDataViewerDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	char cTitle[256];
	char* pTemp;

	lstrcpy( cTitle, lpszTitle );
	if( ( pTemp = StrStrI( cTitle, "_current" ) ) != NULL ){
		*pTemp = 0x00;
	}

	CDocument::SetTitle(cTitle);
}
