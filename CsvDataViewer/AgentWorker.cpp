#include "stdafx.h"
#if 0
#include <Windows.h>
#include "MT4_Supoort.h"

#include <vector>       // ヘッダファイルインクルード
using namespace std;         //  名前空間指定

vector<vector<TICK_INFO>>	g_TickInfo;
vector<TICK_INFO>	g_TickInfo_usdjpy;
vector<TICK_INFO>	g_TickInfo_eurjpy;
vector<TICK_INFO>	g_TickInfo_eurusd;

vector<TRADE_INFO> g_tradeInfo;

LRESULT CALLBACK AgentWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

//	MT4から依頼された処理を変わりの行う

//	初期化
DWORD WINAPI InitilizeAgentWorker( LPVOID pParam ) {
	HINSTANCE hInst = (HINSTANCE)pParam;
	HWND hwnd;
	MSG msg;
	WNDCLASS winc;

	winc.style = CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc = AgentWndProc;
	winc.cbClsExtra = winc.cbWndExtra = 0;
	winc.hInstance = hInst;
	winc.hIcon = NULL;
	winc.hCursor = NULL;
	winc.hbrBackground = NULL;
	winc.lpszMenuName = NULL;
	winc.lpszClassName = "AgentWroker Window";

	if (!RegisterClass(&winc)) return 0;

	g_TickInfo.push_back(g_TickInfo_usdjpy);
	g_TickInfo.push_back(g_TickInfo_eurjpy);
	g_TickInfo.push_back(g_TickInfo_eurusd);

	hwnd = CreateWindow(
		"AgentWroker Window", "AgentWroker Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInst, NULL
	);

	if (hwnd == NULL) return 0;

	while (GetMessage(&msg, hwnd, 0, 0)) {
		DispatchMessage(&msg);
	}

	return 1;
}


//	データの初期化
void MT4_Initilize(DWORD dwMapId)
{
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];


	PMT4_INITILIZE_DATA pMapData;

	wsprintf(cMapName, "MT4_Initilize_%d", dwMapId);

	if( (hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cMapName )) != INVALID_HANDLE_VALUE ){
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_INITILIZE_DATA)pMap;

			g_TickInfo[pMapData->dwPair].clear();

			pMapData->dwRet = 1;

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	return;
}

double CalcEMAValue(DWORD dwPairIdx, DWORD dwCalcIdx )
{
	double nRet = 0;
	bool bFirst = false;
	int nCurIdx = dwCalcIdx;
	double sumCl = 0;
	int i;

	if (dwCalcIdx < EMA_VALUE - 1) {
		return 0;
	}

	if (dwCalcIdx <= EMA_VALUE - 1) {
		bFirst = true;
	}

	sumCl = 0;
	for (i = (int)dwCalcIdx; i >= (int)dwCalcIdx + 1 - EMA_VALUE; i--) {
		sumCl += g_TickInfo[dwPairIdx][i].Cl;
	}
	if (bFirst == false) {
		sumCl += g_TickInfo[dwPairIdx][dwCalcIdx].Cl;
		nRet = sumCl / (EMA_VALUE + 1);
	}
	else {
		nRet = sumCl / EMA_VALUE;
	}

	return nRet;
}
#if 0
bool TickInfoWrite( DWORD dwPairIdx, DWORD dwWriteIdx ) {
	char cLogFilePath[1024];
	HANDLE hFile;
	DWORD dwWrite;

	if (DEBUG_FILEOUT < 1)	return false;

	SYSTEMTIME* pSt = &g_TickInfo[dwPairIdx][dwWriteIdx].sDt;
	wsprintf(cLogFilePath, "%s\\%s_%02d%02d%02d.aw", _LOG_DIR_, GetPairName(dwPairIdx), pSt->wYear, pSt->wMonth, pSt->wDay);

	if ((hFile = CreateFile(cLogFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
		SetFilePointer( hFile, 0, 0, FILE_END);

		WriteFile(hFile, &g_TickInfo[dwPairIdx][dwWriteIdx], sizeof(TICK_INFO), &dwWrite, NULL);

		CloseHandle(hFile);
	}

	return true;
}
BOOL WriteTradeInfo( TRADE_INFO* ti, BOOL bOpen ) {
	char cLogFilePath[1024];
	char cWriteBuffer[1024];
	HANDLE hFile;
	DWORD dwWrite;
	SYSTEMTIME CurTime;
	SYSTEMTIME* pSt;

	if (DEBUG_FILEOUT < 1)	return false;

	if (bOpen != FALSE) {
		pSt = &ti->openDt;
	}
	else {
		pSt = &ti->closeDt;
	}

	GetLocalTime(&CurTime);
	wsprintf(cLogFilePath, "%s\\TradeInfo_%d%02d%02d.aw", _LOG_DIR_,  pSt->wYear, pSt->wMonth, pSt->wDay);


	if ((hFile = CreateFile(cLogFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
		SetFilePointer(hFile, 0, 0, FILE_END);

		_stprintf_s(cWriteBuffer, "%02d:%02d:%02d,%s,%s,%s,%.3f,%.3f,%f,%02d:%02d:%02d,%02d:%02d:%02d\r\n",
			CurTime.wHour, CurTime.wMinute, CurTime.wSecond,
			bOpen ? "注文履歴" : "決算履歴",
			GetPairName(ti->dwPair),
			(ti->dwOrderType == 1) ? "買い注文" : "売り注文",
			ti->dOpenPrice,
			bOpen ? 0 : ti->dClosePrice,
			ti->dStopLoss,
			ti->openDt.wHour, ti->openDt.wMinute, ti->openDt.wSecond,
			bOpen ? 0 : ti->closeDt.wHour, bOpen ? 0 : ti->closeDt.wMinute, bOpen ? 0 : ti->closeDt.wSecond
		);

		WriteFile(hFile, cWriteBuffer, lstrlen(cWriteBuffer), &dwWrite, NULL);

		CloseHandle(hFile);
	}

	return true;
}
#endif

//	Ask値の登録
//	ファイルへの記録もここで行う
void MT4_RegisterRate(DWORD dwMapId) {
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];
	TICK_INFO AddData;


	PMT4_REGISTERRATE_DATA pMapData;

	wsprintf(cMapName, "MT4_RegisterRate_%d", dwMapId);

	if ((hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cMapName)) != INVALID_HANDLE_VALUE) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_REGISTERRATE_DATA)pMap;

			DWORD i = pMapData->dwPair;

			if ( i < PAIR_COUNT ) {
				DWORD dwEnd = g_TickInfo[i].size()-1;
				SYSTEMTIME systime;

				GetLocalTime(&systime);
				
				//	Tickを切り替える(初回もこっちに入れる
				if( g_TickInfo[i].size() == 0 || g_TickInfo[i][dwEnd].Tc >= REM_TICK_CNT) {
					if (g_TickInfo[i].size() > 0 ) {
						memcpy(&g_TickInfo[i][dwEnd].eDt, &systime, sizeof(SYSTEMTIME));

						if (g_TickInfo[i].size() >= EMA_VALUE) {
							g_TickInfo[i][dwEnd].ema = CalcEMAValue(i, dwEnd);
						}

						//	更新時にティック情報をファイルに書き込む
						//	とりあえずAskだけ
//						TickInfoWrite(i, dwEnd);

						//	最大数に達していたら、先頭を削除する
						if (g_TickInfo[i].size() >= MAX_TICK_INFOCNT) {
							g_TickInfo[i].erase(g_TickInfo[i].begin());
						}
					}

					memset(&AddData, 0x00, sizeof(TICK_INFO));

					AddData.Hi = pMapData->dAsk;
					AddData.Op = pMapData->dAsk;
					AddData.Cl = pMapData->dAsk;
					AddData.Lo = pMapData->dAsk;
					memcpy(&AddData.sDt, &systime, sizeof(SYSTEMTIME));
					AddData.ema = 0;
					AddData.Tc = 1;

					g_TickInfo[i].push_back(AddData);
				}
				//	最後のTickデータを更新する
				else {
					//	最高または最低を必要があれば更新
					if (g_TickInfo[i][dwEnd].Hi < pMapData->dAsk) {
						g_TickInfo[i][dwEnd].Hi = pMapData->dAsk;
					}
					else if (g_TickInfo[i][dwEnd].Lo > pMapData->dAsk) {
						g_TickInfo[i][dwEnd].Lo = pMapData->dAsk;
					}

					//	最終は常に更新する
					g_TickInfo[i][dwEnd].Cl = pMapData->dAsk;
					g_TickInfo[i][dwEnd].Tc++;
				}

				pMapData->dwRet = 1;
			}
			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	return;
}

#define ABS(a,b)	(a>b)?(a-b):(b-a)


void MT4_TradeCheck(DWORD dwMapId)
{
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];


	PMT4_TRADECHECK_DATA pMapData;

	wsprintf(cMapName, "MT4_TradeCheck_%d", dwMapId);

	if ((hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cMapName)) != INVALID_HANDLE_VALUE) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_TRADECHECK_DATA)pMap;

			pMapData->dwRet = 0;

			if (g_TickInfo[pMapData->dwPair].size() > 1) {
				vector<TICK_INFO>::iterator itr_e = g_TickInfo[pMapData->dwPair].end()-1;

				double dCurAsk = (*itr_e).Cl;

				//	一個前からさかのぼっていく
				double dPrevDojiVal = 0;
				DWORD dojiCnt = 0;
				DWORD dwMagni = (pMapData->dwPair == PAIR_EURUSD) ? (10000) : (100);
				double dRange = (pMapData->dwPair == PAIR_EURUSD) ? (0.0003) : (0.03);

				for (itr_e--; itr_e != g_TickInfo[pMapData->dwPair].begin(); itr_e--) {
					//	連続する同時線を囲む
					double dAbsOpCl = ABS((*itr_e).Op, (*itr_e).Cl);
					double dAbsHiLo = ABS((*itr_e).Hi, (*itr_e).Lo);
					double dChkA = dPrevDojiVal * dwMagni - 1;
					double dChkB = (*itr_e).Op * dwMagni && (*itr_e).Op * dwMagni <= dPrevDojiVal * dwMagni + 1;
					double dChkC = (*itr_e).Cl * dwMagni && (*itr_e).Cl * dwMagni <= dPrevDojiVal * dwMagni + 1;

					if (dPrevDojiVal > 0) {
						if (dAbsHiLo * dwMagni <= 3.0
							&& (dChkA <= dChkB)
							&& (dChkA <= dChkC)
							) {
							dojiCnt++;
						}
						else {
							if (dojiCnt >= 5) {
								//	0:注文しない 1:買い注文 2:売り注文
								if (dPrevDojiVal + dRange < dCurAsk) {
									pMapData->dTradePrice = dCurAsk;
									pMapData->dwRet = 1;
								}
								else if (dPrevDojiVal - dRange > dCurAsk) {
									pMapData->dTradePrice = dCurAsk;
									pMapData->dwRet = 2;
								}
							}
							break;
						}
					}
					else if (dAbsOpCl * dwMagni <= 1.0 && dAbsHiLo * dwMagni <= 3.0) {
						dPrevDojiVal = ((*itr_e).Op < (*itr_e).Cl) ? ((*itr_e).Op + ((*itr_e).Cl - (*itr_e).Op) / 2) : ((*itr_e).Cl + ((*itr_e).Op - (*itr_e).Cl) / 2);
						dojiCnt++;
					}
					else {
						break;
					}
				}
			}


			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	return;
}

#if 0

typedef struct _MT4_REGISTERTRADEINFO_DATA {
	DWORD	dwRet;
	DWORD	dwPair;
	DWORD	dwTicket;
	DWORD	dwOrderType;
	double	dOpenPrice;
	double	dStopLoss;
	SYSTEMTIME stOpenDate;
	BYTE	reserv[1024 - sizeof(DWORD) * 3 - sizeof(double) * 2 - sizeof(SYSTEMTIME)];
}MT4_REGISTERTRADEINFO_DATA, *PMT4_REGISTERTRADEINFO_DATA;
#endif

void MT4_RegisterTradeInfo(DWORD dwMapId)
{
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];
	TRADE_INFO AddData;

	PMT4_REGISTERTRADEINFO_DATA pMapData;

	wsprintf(cMapName, "MT4_RegisterTradeInfo_%d", dwMapId);

	memset(&AddData, 0x00, sizeof(TRADE_INFO));

	if ((hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cMapName)) != INVALID_HANDLE_VALUE) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_REGISTERTRADEINFO_DATA)pMap;

			AddData.dwPair = pMapData->dwPair;
			AddData.dwTicket = pMapData->dwTicket;
			AddData.dwOrderType = pMapData->dwOrderType;
			AddData.dOpenPrice = pMapData->dOpenPrice;
			AddData.dClosePrice = 0;
			AddData.dStopLoss = pMapData->dStopLoss;
			GetLocalTime(&AddData.openDt);

			g_tradeInfo.push_back(AddData);

//			WriteTradeInfo( &AddData, TRUE );

			//	0:注文しない 1:売り注文 2:買い注文
			pMapData->dwRet = 0;

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	return;
}

#if 0

typedef struct _MT4_CHECKCLOSETRADE_DATA {
	DWORD	dwRet;
	DWORD	dwPair;
	DWORD	dwTicket;
	BYTE	reserv[1024 - sizeof(DWORD) * 3];
}MT4_CHECKCLOSETRADE_DATA, *PMT4_CHECKCLOSETRADE_DATA;

#endif
void MT4_CheckCloseTrade(DWORD dwMapId)
{
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];
	double dChkB;


	PMT4_CHECKCLOSETRADE_DATA pMapData;

	wsprintf(cMapName, "MT4_CheckCloseTrade_%d", dwMapId);

	if ((hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cMapName)) != INVALID_HANDLE_VALUE) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_CHECKCLOSETRADE_DATA)pMap;

			vector<TRADE_INFO>::iterator itr = g_tradeInfo.begin();

			double dCurAsk = g_TickInfo[pMapData->dwPair][g_TickInfo[pMapData->dwPair].size() - 1].Cl;

			for (; itr != g_tradeInfo.end(); itr++) {
				if ((*itr).dwTicket == pMapData->dwTicket) {
					if ((*itr).dwOrderType == 1) {
						if ((*itr).bIKEIKE_Mode != FALSE) {
							if ((*itr).dIKEIKE_Val > dCurAsk) {
								//	イケイケモード終了のお知らせ
								pMapData->dwRet = 1;
							}
							else {
								//	終了価格を引き上げて関数終了
								(*itr).dIKEIKE_Val = dCurAsk;
								(*itr).dIKEIKE_Val -= (pMapData->dwPair == PAIR_EURUSD) ?(0.0002) : (0.02);
							}
						}
						else {
							if (dCurAsk > (*itr).dOpenPrice) {
								//	10pips超えたイケイケモード突入
								dChkB = (*itr).dOpenPrice;
								dChkB += (pMapData->dwPair == PAIR_EURUSD) ? (0.0008) : (0.08);
								if (dCurAsk > dChkB ) {
									(*itr).bIKEIKE_Mode = TRUE;
									(*itr).dIKEIKE_Val = dCurAsk;
									(*itr).dIKEIKE_Val -= (pMapData->dwPair == PAIR_EURUSD) ? (0.0002) : (0.02);
								}
							}
							else{
								dChkB = (*itr).dOpenPrice;
								dChkB -= (pMapData->dwPair == PAIR_EURUSD) ? (0.0003) : (0.03);
								if (dCurAsk <= dChkB ) {
									pMapData->dwRet = 1;
								}
							}
						}
					}
					else {
						if ((*itr).bIKEIKE_Mode != FALSE) {
							if ((*itr).dIKEIKE_Val < dCurAsk) {
								//	イケイケモード終了のお知らせ
								pMapData->dwRet = 1;
							}
							else {
								//	終了価格を引き上げて関数終了
								(*itr).dIKEIKE_Val = dCurAsk;
								(*itr).dIKEIKE_Val += (pMapData->dwPair == PAIR_EURUSD) ? (0.0002) : (0.02);
							}
						}
						else {
							if (dCurAsk < (*itr).dOpenPrice) {
								//	10pips超えたイケイケモード突入
								dChkB = (*itr).dOpenPrice;
								dChkB -= (pMapData->dwPair == PAIR_EURUSD) ? (0.0008) : (0.08);
								if (dCurAsk < dChkB ) {
									(*itr).bIKEIKE_Mode = TRUE;
									(*itr).dIKEIKE_Val += (pMapData->dwPair == PAIR_EURUSD) ? (0.0002) : (0.02);
								}
							}
							else{
								dChkB = (*itr).dOpenPrice;
								dChkB += (pMapData->dwPair == PAIR_EURUSD) ? (0.0003) : (0.03);
								if (dCurAsk >= dChkB ) {
									pMapData->dwRet = 1;
								}
							}
						}
					}

					break;
				}
			}


			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	return;
}

#if 0

typedef struct _MT4_REGISTERCLOSETRADE_DATA {
	DWORD	dwRet;
	DWORD	dwPair;
	DWORD	dwTicket;
	BYTE	reserv[1024 - sizeof(DWORD) * 3];
}MT4_REGISTERCLOSETRADE_DATA, *PMT4_REGISTERCLOSETRADE_DATA;

#endif
void MT4_RegisterCloseTrade(DWORD dwMapId)
{
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];
	SYSTEMTIME stCurTime;


	PMT4_REGISTERCLOSETRADE_DATA pMapData;

	wsprintf(cMapName, "MT4_RegisterCloseTrade_%d", dwMapId);

	GetLocalTime(&stCurTime);

	if ((hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cMapName)) != INVALID_HANDLE_VALUE) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_REGISTERCLOSETRADE_DATA)pMap;

			vector<TRADE_INFO>::iterator itr = g_tradeInfo.begin();

			for (; itr != g_tradeInfo.end(); itr++) {
				if ((*itr).dwTicket == pMapData->dwTicket) {
					(*itr).dClosePrice = pMapData->dClosePrice;
					memcpy(&(*itr).closeDt, &stCurTime, sizeof(SYSTEMTIME));

//					WriteTradeInfo(&(*itr), FALSE);

					break;
				}
			}

					//	0:注文しない 1:売り注文 2:買い注文
			pMapData->dwRet = 0;

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	return;
}
LRESULT CALLBACK AgentWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_APP + 1000:
		{
			DWORD dwRet = 1;
			DWORD dwFuncNo = (DWORD)wp;
			DWORD dwMapId = (DWORD)lp;
			switch (dwFuncNo) {
				//	初期化
				case MT4_INITILIZE:
					MT4_Initilize(dwMapId);
				break;
				case MT4_REGISTERRATE:
					MT4_RegisterRate(dwMapId);
					break;
				case MT4_TRADECHECK:
					MT4_TradeCheck(dwMapId);
					break;
				case MT4_REGISTERTRADEINFO:
					MT4_RegisterTradeInfo(dwMapId);
					break;
				case MT4_CHECKCLOSETRADE:
					MT4_CheckCloseTrade(dwMapId);
					break;
				case MT4_REGISTERCLOSETRADE:
					MT4_RegisterCloseTrade(dwMapId);
					break;
				default:
					dwRet = 0;
				break;
			}

			return dwRet;
		}
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

#endif