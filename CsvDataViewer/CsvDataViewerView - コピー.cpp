ZeroGraphViewer

// ZeroGraphViewer.cpp:CZeroGraphViewer クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "CsvDataViewer.h"
#endif

#include "CsvDataViewerDoc.h"
#include "ZeroGraphViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "MainFrm.h"

extern CMainFrame* g_pMainFrame;
// CZeroGraphViewer

IMPLEMENT_DYNCREATE(CZeroGraphViewer, CView)

BEGIN_MESSAGE_MAP(CZeroGraphViewer, CView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CZeroGraphViewer::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_20EMA, &CZeroGraphViewer::On20EMA)
	ON_COMMAND(ID_50EMA, &CZeroGraphViewer::On50EMA)
	ON_COMMAND(ID_OSCILLATOR, &CZeroGraphViewer::OnStochasticOscillator)
	ON_COMMAND(ID_200EMA, &CZeroGraphViewer::On200EMA)
	ON_COMMAND(ID_13EMA, &CZeroGraphViewer::On13ema)
	ON_COMMAND(ID_21EMA, &CZeroGraphViewer::On21ema)
	ON_UPDATE_COMMAND_UI(ID_20EMA, &CZeroGraphViewer::OnUpdate20ema)
	ON_UPDATE_COMMAND_UI(ID_50EMA, &CZeroGraphViewer::OnUpdate50ema)
	ON_UPDATE_COMMAND_UI(ID_200EMA, &CZeroGraphViewer::OnUpdate200ema)
	ON_UPDATE_COMMAND_UI(ID_13EMA, &CZeroGraphViewer::OnUpdate13ema)
	ON_UPDATE_COMMAND_UI(ID_21EMA, &CZeroGraphViewer::OnUpdate21ema)
	ON_MESSAGE(WM_APP + 1001, &CZeroGraphViewer::OnRegiterRateMessage)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CZeroGraphViewer コンストラクション/デストラクション

CZeroGraphViewer::CZeroGraphViewer()
{
}

CZeroGraphViewer::~CZeroGraphViewer()
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	//	もっと良い場所ある？
	
	//if( pDoc->m_bCurRateMode != FALSE )
	{
		WriteIndicatorSetting();
	}
}

BOOL CZeroGraphViewer::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。
	
	//	インジゲーター設定の初期化
	//	そのうち、設定ファイル作る
	m_b13EMA = FALSE;
	m_b21EMA = FALSE;
	m_b20EMA = FALSE;
	m_b50EMA = FALSE;
	m_b200EMA = FALSE;
	m_bOscillator = FALSE;


	return CView::PreCreateWindow(cs);
}


void CZeroGraphViewer::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CCsvDataViewerDoc* pDoc = GetDocument();
	
	//if( pDoc->m_bCurRateMode != FALSE )
	{
		ReadIndicatorSetting();
	}

	HBRUSH m_hbrush_black = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH m_hbrush_red = CreateSolidBrush(RGB(0xFF, 0, 0));
	HBRUSH m_hbrush_blue = CreateSolidBrush(RGB(0, 0, 0xFF));

}

// インクルード
#include "MT4_Supoort.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>  // ※istringstreamを使うために必要
// 名前空間
using namespace std;
//

#define BAR_WIDTH			(5)
#define BAR_SPAN			(2)
double g_BarHeightPerPixel = 1;
#define BAR_HEIGHT_PER_PIXEL	(g_BarHeightPerPixel)

#define GRAPH_MARGIN		(20)
#define GRAPH_TOP			(GRAPH_MARGIN)
#define GRAPH_VIEW_TOP		(GRAPH_TOP+GRAPH_MARGIN)
#define GRAPH_LEFT			(GRAPH_MARGIN)
DWORD	g_dwGraphScalePerWSize = 90;
#define GRAPH_SCALE_PERWSIZE	(g_dwGraphScalePerWSize)	//	クライアント領域のうち何％をグラフで使用するか

DWORD g_dwGraphBottom = 500;
DWORD g_dwGraphRight = 800;
#define GRAPH_BOTTOM		(g_dwGraphBottom-GRAPH_MARGIN)
#define GRAPH_VIEW_BOTTOM	(GRAPH_BOTTOM+GRAPH_MARGIN)
#define GRAPH_RIGHT			(g_dwGraphRight)
#define GRAPH_HEIGHT		(GRAPH_BOTTOM-GRAPH_TOP)
#define GRAPH_WIDTH			(GRAPH_RIGHT-GRAPH_LEFT)

DWORD g_dwViewBottom = 500;
DWORD g_dwViewRight = 800;
#define VIEW_BOTTOM			(g_dwViewBottom)
#define VIEW_RIGHT			(g_dwViewRight)

DWORD g_dwOscillatorBottom = 0;
DWORD g_dwOscillatorTop = 0;
#define OSCILLATOR_BOTTOM	(g_dwOscillatorBottom)
#define OSCILLATOR_TOP		(g_dwOscillatorTop)
#define OSCILLATOR_HEIGHT	(OSCILLATOR_BOTTOM-OSCILLATOR_TOP)
#define OSCILLATOR_LEFT		GRAPH_LEFT
#define OSCILLATOR_RIGHT	GRAPH_RIGHT
#define OSCILLATOR_WIDTH	(OSCILLATOR_RIGHT-OSCILLATOR_LEFT)

#define EMA_VALUE	(20)

double CZeroGraphViewer::CalcEMAValue(int idx)
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return 0;

	double nRet = 0;
	bool bFirst = false;
	int nCurIdx = idx;
	double sumCl = 0;
	int i;

	if (idx < EMA_VALUE - 1){
		return 0;
	}

	if (idx <= EMA_VALUE - 1) {
		bFirst = true;
	}

	sumCl = 0;
	for (i = idx; i > idx - EMA_VALUE; i--) {
		sumCl += pDoc->m_TickInfo[i].Cl;
	}
	if (bFirst == false) {
		sumCl += pDoc->m_TickInfo[idx].Cl;
		nRet = sumCl / (EMA_VALUE + 1);
	}
	else {
		nRet = sumCl / EMA_VALUE;
	}

	return nRet;
}

void CZeroGraphViewer::WrieteRectangle(CDC* pDC, int nStartX, int nStartIdx, int nEndIdx, int nPenStyle, int nWidth, COLORREF lColor ) 
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DWORD dwMin = 7777777;
	DWORD dwMax = 0;

	for (int i = nStartIdx; i <= nEndIdx; i++) {
		DWORD Hi = pDoc->m_TickInfo[i].Hi;
		DWORD Lo = pDoc->m_TickInfo[i].Lo;

		if (dwMax < Hi) {
			dwMax = Hi;
		}
		if (dwMin > Lo) {
			dwMin = Lo;
		}
	}

	CPen pen( nPenStyle, nWidth, lColor );
	CPen* oldp = pDC->SelectObject(&pen);	//ペンの変更

	int nBottom = GRAPH_BOTTOM - (int)((dwMin - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);
	int nTop = GRAPH_BOTTOM - (int)((dwMax - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);

	//	外枠を描画
	pDC->MoveTo(nStartX, nTop);
	pDC->LineTo(nStartX + (nEndIdx - nStartIdx + 1)*(BAR_WIDTH + BAR_SPAN), nTop);
	pDC->MoveTo(nStartX + (nEndIdx - nStartIdx + 1)*(BAR_WIDTH + BAR_SPAN), nTop);
	pDC->LineTo(nStartX + (nEndIdx - nStartIdx + 1)*(BAR_WIDTH + BAR_SPAN), nBottom);
	pDC->MoveTo(nStartX + (nEndIdx - nStartIdx + 1)*(BAR_WIDTH + BAR_SPAN), nBottom);
	pDC->LineTo(nStartX, nBottom);
	pDC->MoveTo(nStartX, nBottom);
	pDC->LineTo(nStartX, nTop);

	pDC->SelectObject(oldp);	//ペンを元に戻す(必須)
}

void CZeroGraphViewer::FrameDojiArea(CDC* pDC, int nStartX, int nStartIdx, int nEndIdx, DWORD dwFrameWidth, double dGainCut, double dLossCut ) 
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	double dMin = 77777;
	double dMax = 0;
	int nLoIdx;
	int nHiIdx;

	for (int i = nStartIdx; i <= nEndIdx; i++) {
		double Hi = pDoc->m_TickInfo[i].Hi;
		double Lo = pDoc->m_TickInfo[i].Cl;

		if (dMax < Hi) {
			dMax = Hi;
			nHiIdx = i;
		}
		if (dMin > Lo) {
			dMin = Lo;
			nLoIdx = i;
		}
	}

	CPen pen_dot_red(PS_DOT, (dwFrameWidth==0)?1:2, (dwFrameWidth == 1)?RGB(0xFF, 0, 0):(dwFrameWidth == 1)?RGB(0xFF, 0, 0):((dwFrameWidth == 2) ? RGB(0, 0, 0xFF): (0, 0xFF, 0) ));
	CPen* oldp = pDC->SelectObject(&pen_dot_red);	//ペンの変更

	int nBottom = GRAPH_TOP + GRAPH_MARGIN + GRAPH_HEIGHT - (int)((dMin - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);
	int nTop = GRAPH_TOP + GRAPH_MARGIN + GRAPH_HEIGHT - (int)((dMax - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);
	//	外枠を描画
	pDC->MoveTo(nStartX, nTop);pDC->LineTo(nStartX + (nEndIdx - nStartIdx + 1)*(BAR_WIDTH + BAR_SPAN), nTop);
	pDC->MoveTo(nStartX + (nEndIdx - nStartIdx + 1)*(BAR_WIDTH + BAR_SPAN), nTop);pDC->LineTo(nStartX + (nEndIdx - nStartIdx + 1)*(BAR_WIDTH + BAR_SPAN), nBottom);
	pDC->MoveTo(nStartX + (nEndIdx - nStartIdx + 1)*(BAR_WIDTH + BAR_SPAN), nBottom);pDC->LineTo(nStartX, nBottom);
	pDC->MoveTo(nStartX, nBottom);pDC->LineTo(nStartX, nTop);


	//	指値と逆指値の線を引く
	if (dGainCut > 0 && dLossCut > 0) {
		CPen pen_dot_lossgain(PS_DOT, 1, RGB(0xFF, 0, 0xFF));
		int nGainCut = GRAPH_TOP + GRAPH_MARGIN + GRAPH_HEIGHT - (int)((dGainCut - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);
		int nLossCut = GRAPH_TOP + GRAPH_MARGIN + GRAPH_HEIGHT - (int)((dLossCut - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);

		oldp = pDC->SelectObject(&pen_dot_lossgain);	//ペンの変更

		pDC->MoveTo(nStartX, nGainCut);pDC->LineTo(nStartX + (nEndIdx - nStartIdx + 1)*(BAR_WIDTH + BAR_SPAN), nGainCut);
		pDC->MoveTo(nStartX, nLossCut);pDC->LineTo(nStartX + (nEndIdx - nStartIdx + 1)*(BAR_WIDTH + BAR_SPAN), nLossCut);

		pDC->SelectObject(oldp);	//ペンを元に戻す(必須)
	}

}

vector<INDICATOR_SETTING>& CZeroGraphViewer::GetActiveIndicatorSetting()
{
	return m_IndiSetting;
}

void CZeroGraphViewer::DrawIndicator(CDC* pDC, DWORD i, int nCurrentX )
{
	vector<INDICATOR_SETTING>::iterator itr = m_IndiSetting.begin();

	for( ;itr != m_IndiSetting.end(); itr++ ){
		//	ボリジャーバンド
		if( (*itr).dwIndiType == INDICATOR_TYPE_BB ){
			DrawBollingerBands( pDC, i, (*itr).bb.dwSpan, (*itr).bb.dwSigma, nCurrentX );
		}
		else if( (*itr).dwIndiType == INDICATOR_TYPE_MA ){
			;
		}
		else if( (*itr).dwIndiType == INDICATOR_TYPE_ADX ){
			DrawADX( pDC, i, (*itr).adx.dwSpan, nCurrentX );
		}
		else if( (*itr).dwIndiType == INDICATOR_TYPE_RSI ){
			DrawRSI( pDC, i, (*itr).rsi.dwSpan, nCurrentX );
		}
		else if( (*itr).dwIndiType == INDICATOR_TYPE_SO ){
			DrawSO( pDC, i, (*itr).so.dwKSpan, (*itr).so.dwDSpan, (*itr).so.dwSSpan, nCurrentX );
		}
		else if( (*itr).dwIndiType == INDICATOR_TYPE_MACD ){
			DrawMACD( pDC, i, (*itr).macd.dwSSpan, (*itr).macd.dwLSpan, (*itr).macd.dwSignal, nCurrentX );
		}
		else if( (*itr).dwIndiType == INDICATOR_TYPE_ICHIMOKU ){
			DrawIchimoku( pDC, i, nCurrentX );
		}
	}

	return;
}

BOOL CalcDM( DWORD* pdwPDM, DWORD* pdwMDM, DWORD dwNowHi, DWORD dwPrevHi, DWORD dwNowLo, DWORD dwPrevLo )
{
	BOOL bRet = FALSE;

	DWORD dwPDM = -1;
	DWORD dwMDM = -1;

	if( dwNowHi == 0
	 || dwPrevHi == 0
	 || dwNowLo == 0
	 || dwPrevLo == 0
	){
		return FALSE;
	}

	if( dwNowHi > dwPrevHi ){
		dwPDM = dwNowHi - dwPrevHi;
	}else{
		dwPDM = 0;
	}
	if( dwPrevLo > dwNowLo ){
		dwMDM = dwPrevLo - dwNowLo;
	}else{
		dwMDM = 0;
	}

	if( dwPDM > dwMDM ){
		dwMDM = 0;
	}else if( dwMDM > dwPDM ){
		dwPDM = 0;
	}else{
		dwPDM = 0;
		dwMDM = 0;
	}

	if( pdwPDM != NULL )	*pdwPDM = dwPDM;
	if( pdwMDM != NULL )	*pdwMDM = dwMDM;

	return bRet;
}

double CZeroGraphViewer::CalcDX( DWORD i, DWORD dwSpan )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return  0;

	DWORD dwAdx = 0;

	double dPDI = 0;
	double dMDI = 0;
	DWORD dwPDM = 0;
	DWORD dwMDM = 0;
	DWORD dwSumPDM = 0;
	DWORD dwSumMDM = 0;
	double dATR = 0;
	double dDX;

	DWORD l;
	for( l = i - (dwSpan - 1); l <= i; l++ ){
		dwPDM = 0;
		dwMDM = 0;
		CalcDM( &dwPDM, &dwMDM, pDoc->m_TickInfo[l].Hi, pDoc->m_TickInfo[l-1].Hi, pDoc->m_TickInfo[l].Lo, pDoc->m_TickInfo[l-1].Lo );

		if( dwPDM == -1 || dwMDM == -1 ){
			return -1;
		}

		dwSumPDM += dwPDM;
		dwSumMDM += dwMDM;

		dATR += ((pDoc->m_TickInfo[l].Hi>pDoc->m_TickInfo[l-1].Cl)?(pDoc->m_TickInfo[l].Hi):(pDoc->m_TickInfo[l-1].Cl)) - ((pDoc->m_TickInfo[l].Lo<pDoc->m_TickInfo[l-1].Cl)?(pDoc->m_TickInfo[l].Lo):(pDoc->m_TickInfo[l-1].Cl));
	}

	//dATR /= (double)dwSpan;

	dPDI = (double)dwSumPDM*100/dATR;
	dMDI = (double)dwSumMDM*100/dATR;

	dDX = ((dPDI>dMDI)?(dPDI-dMDI):(dMDI-dPDI))*100/(dPDI+dMDI);

	return dDX;
}

DWORD CZeroGraphViewer::CalcADX( DWORD i, DWORD dwSpan )
{
	double dTemp;
	double dSumDX = 0;
	DWORD l;
	for( l = i - (dwSpan - 1); l <= i; l++ ){
		dTemp = CalcDX( l, dwSpan );
		if( dTemp == (DWORD)-1 ){
			return 0;
		}

		dSumDX += dTemp;
	}

	dSumDX /= (double)dwSpan;

	return (DWORD)dSumDX;
}



BOOL CZeroGraphViewer::GetIchimokuData( DWORD i, ICHIMOKU_INFO* pRetInfo )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return FALSE;

	BOOL bRet = FALSE;

	//	転換値：(当日含む過去9日間の高値+当日含む過去9日間の安値)÷2
	if( i >= 9 - 1 ){
		DWORD dwHi = 0;
		DWORD dwLo = 7777777;

		for( DWORD j = i; (int)j >= (int)(i - ( 9 - 1 )); j-- ){
			if( dwHi < pDoc->m_TickInfo[j].Hi )	dwHi = pDoc->m_TickInfo[j].Hi;
			if( dwLo > pDoc->m_TickInfo[j].Lo )	dwLo = pDoc->m_TickInfo[j].Lo;
		}

		pRetInfo->dwConvVal = ( dwHi + dwLo )/2;

		bRet = TRUE;
	}
	
	//	基準値：(当日含む過去26日間の高値+当日含む過去26日間の安値)÷2
	if( i >= 26 - 1 ){
		DWORD dwHi = 0;
		DWORD dwLo = 7777777;

		for( DWORD j = i; (int)j >= (int)(i - ( 26 - 1 )); j-- ){
			if( dwHi < pDoc->m_TickInfo[j].Hi )	dwHi = pDoc->m_TickInfo[j].Hi;
			if( dwLo > pDoc->m_TickInfo[j].Lo )	dwLo = pDoc->m_TickInfo[j].Lo;
		}

		pRetInfo->dwBaseVal = ( dwHi + dwLo )/2;

		bRet = TRUE;
	}

	//	遅行スパン：当日の終値を26日後ろにずらしたもの
	if( i + 26 < pDoc->m_TickInfo.size() ){
		pRetInfo->dwLaggingSpan = pDoc->m_TickInfo[i+26].Cl;

		bRet = TRUE;
	}

	//	先行スパン１：(転換値の値+基準値の値)÷2・・・26日先にずらして書く
	if( i >= 26 - 1 + 26 ){
		DWORD dwHi = 0;
		DWORD dwLo = 7777777;
		DWORD dwConvVal;
		DWORD dwBaseVal;
		DWORD dwStart = i - 26;

		for( DWORD j = dwStart; (int)j >= (int)(dwStart - ( 9 - 1 )); j-- ){
			if( dwHi < pDoc->m_TickInfo[j].Hi )	dwHi = pDoc->m_TickInfo[j].Hi;
			if( dwLo > pDoc->m_TickInfo[j].Lo )	dwLo = pDoc->m_TickInfo[j].Lo;
		}

		dwConvVal = ( dwHi + dwLo )/2;

		dwHi = 0;
		dwLo = 7777777;

		for( DWORD j = dwStart; (int)j >= (int)(dwStart - ( 26 - 1 )); j-- ){
			if( dwHi < pDoc->m_TickInfo[j].Hi )	dwHi = pDoc->m_TickInfo[j].Hi;
			if( dwLo > pDoc->m_TickInfo[j].Lo )	dwLo = pDoc->m_TickInfo[j].Lo;
		}

		dwBaseVal = ( dwHi + dwLo )/2;

		pRetInfo->dwLeadingSpan1 = (dwConvVal+dwBaseVal)/2;

		bRet = TRUE;
	}


	//	先行スパン２：(過去52日間の高値+過去52日間の安値)÷2・・・26日先にずらして書く
	if( i >= 52 - 1 + 26 ){
		DWORD dwHi = 0;
		DWORD dwLo = 7777777;
		DWORD dwStart = i - 26;

		for( DWORD j = dwStart; (int)j >= (int)(dwStart - ( 52 - 1 )); j-- ){
			if( dwHi < pDoc->m_TickInfo[j].Hi )	dwHi = pDoc->m_TickInfo[j].Hi;
			if( dwLo > pDoc->m_TickInfo[j].Lo )	dwLo = pDoc->m_TickInfo[j].Lo;
		}

		pRetInfo->dwLeadingSpan2 = ( dwHi + dwLo )/2;

		bRet = TRUE;
	}
	return bRet;
}

//	一目均衡表
//	転換値：(当日含む過去9日間の高値+当日含む過去9日間の安値)÷2
//	基準値：(当日含む過去26日間の高値+当日含む過去26日間の安値)÷2
//	遅行スパン：当日の終値を26日後ろにずらしたもの
//	先行スパン１：(転換値の値+基準値の値)÷2・・・26日先にずらして書く
//	先行スパン２：(過去52日間の高値+過去52日間の安値)÷2・・・26日先にずらして書く
void CZeroGraphViewer::DrawIchimoku(CDC* pDC, DWORD i, int nCurrentX )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return;

	ICHIMOKU_INFO ichi_info1;
	ICHIMOKU_INFO ichi_info2;

	memset( &ichi_info1, 0x00, sizeof(ICHIMOKU_INFO) );
	memset( &ichi_info2, 0x00, sizeof(ICHIMOKU_INFO) );

	if( GetIchimokuData( i, &ichi_info1 ) != FALSE ){
		if( GetIchimokuData( i+1, &ichi_info2 ) != FALSE ){
			CPen* oldp;
			int nStartX, nStartY, nEndX, nEndY;

			//	転換値:オレンジ(0xFF,0xA5,0x00)
			if( ichi_info1.dwConvVal != 0 && ichi_info2.dwConvVal != 0 ){
				CPen Pen_Conv(PS_SOLID, 1, RGB(0xFF,0xA5,0x00));

				nStartX = nCurrentX + BAR_WIDTH / 2;
				nStartY = GRAPH_BOTTOM - (DWORD)((ichi_info1.dwConvVal - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);
				nEndX = nStartX + BAR_WIDTH + BAR_SPAN;
				nEndY = GRAPH_BOTTOM - (DWORD)((ichi_info2.dwConvVal - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);

				oldp = pDC->SelectObject(&Pen_Conv);	//ペンの変更

				pDC->MoveTo(nStartX, nStartY);
				pDC->LineTo(nEndX, nEndY);

				pDC->SelectObject(oldp);
			}

			//	基準値:緑(0x00,0x80,0x00)
			if( ichi_info1.dwBaseVal != 0 && ichi_info2.dwBaseVal != 0 ){
				CPen Pen_Base(PS_SOLID, 1, RGB(0x00,0x80,0x00));

				nStartX = nCurrentX + BAR_WIDTH / 2;
				nStartY = GRAPH_BOTTOM - (DWORD)((ichi_info1.dwBaseVal - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);
				nEndX = nStartX + BAR_WIDTH + BAR_SPAN;
				nEndY = GRAPH_BOTTOM - (DWORD)((ichi_info2.dwBaseVal - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);

				oldp = pDC->SelectObject(&Pen_Base);	//ペンの変更

				pDC->MoveTo(nStartX, nStartY);
				pDC->LineTo(nEndX, nEndY);

				pDC->SelectObject(oldp);
			}

			//	遅行スパン:紫(0x80,0x00,0x80)
			if( ichi_info1.dwLaggingSpan != 0 && ichi_info2.dwLaggingSpan != 0 ){
				CPen Pen_Lag(PS_SOLID, 1, RGB(0x80,0x00,0x80));

				nStartX = nCurrentX + BAR_WIDTH / 2;
				nStartY = GRAPH_BOTTOM - (DWORD)((ichi_info1.dwLaggingSpan - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);
				nEndX = nStartX + BAR_WIDTH + BAR_SPAN;
				nEndY = GRAPH_BOTTOM - (DWORD)((ichi_info2.dwLaggingSpan - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);

				oldp = pDC->SelectObject(&Pen_Lag);	//ペンの変更

				pDC->MoveTo(nStartX, nStartY);
				pDC->LineTo(nEndX, nEndY);

				pDC->SelectObject(oldp);
			}

			//	先行スパン１:青(0x00,0x00,0xFF)
			if( ichi_info1.dwLeadingSpan1 != 0 && ichi_info2.dwLeadingSpan1 != 0 ){
				CPen Pen_Lead(PS_SOLID, 1, RGB(0x00,0x00,0xFF));

				nStartX = nCurrentX + BAR_WIDTH / 2;
				nStartY = GRAPH_BOTTOM - (DWORD)((ichi_info1.dwLeadingSpan1 - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);
				nEndX = nStartX + BAR_WIDTH + BAR_SPAN;
				nEndY = GRAPH_BOTTOM - (DWORD)((ichi_info2.dwLeadingSpan1 - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);

				oldp = pDC->SelectObject(&Pen_Lead);	//ペンの変更

				pDC->MoveTo(nStartX, nStartY);
				pDC->LineTo(nEndX, nEndY);

				pDC->SelectObject(oldp);
			}

			//	先行スパン２:ピンク(0xFF,0xC0,0xCB)
			if( ichi_info1.dwLeadingSpan2 != 0 && ichi_info2.dwLeadingSpan2 != 0 ){
				CPen Pen_Lead2(PS_SOLID, 1, RGB(0xFF,0xC0,0xCB));

				nStartX = nCurrentX + BAR_WIDTH / 2;
				nStartY = GRAPH_BOTTOM - (DWORD)((ichi_info1.dwLeadingSpan2 - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);
				nEndX = nStartX + BAR_WIDTH + BAR_SPAN;
				nEndY = GRAPH_BOTTOM - (DWORD)((ichi_info2.dwLeadingSpan2 - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);

				oldp = pDC->SelectObject(&Pen_Lead2);	//ペンの変更

				pDC->MoveTo(nStartX, nStartY);
				pDC->LineTo(nEndX, nEndY);

				pDC->SelectObject(oldp);
			}

			//	先行スパン１-先行スパン2の雲：グレー(0x80,0x80,0x80)
			if( ichi_info1.dwLeadingSpan1 != 0 && ichi_info1.dwLeadingSpan2 != 0 ){
				CPen Pen_Cloud(PS_DOT, 1, RGB(0x80,0x80,0x80));

				nStartX = nCurrentX + BAR_WIDTH / 2;
				nStartY = GRAPH_BOTTOM - (DWORD)((ichi_info1.dwLeadingSpan1 - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);
				nEndX = nStartX;
				nEndY = GRAPH_BOTTOM - (DWORD)((ichi_info1.dwLeadingSpan2 - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);


				oldp = pDC->SelectObject(&Pen_Cloud);	//ペンの変更

				pDC->MoveTo(nStartX, nStartY);
				pDC->LineTo(nEndX, nEndY);

				pDC->SelectObject(oldp);
			}
		}
	}
}

double CZeroGraphViewer::CalcRSI( DWORD i, DWORD dwSpan, double dPrevUpAve, double dPrevDownAve )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return 0;

	double dRet = 0;
	double dUp = 0;
	double dDown = 0;

	if( dwSpan > i )	return 0;

	if( dPrevUpAve == 0 && dPrevDownAve == 0 ){
		for( DWORD j = i; (int)j > (int)i - (int)dwSpan; j-- ){
			if( pDoc->m_TickInfo[j-1].Cl < pDoc->m_TickInfo[j].Cl ){
				dUp += pDoc->m_TickInfo[j].Cl - pDoc->m_TickInfo[j-1].Cl;
			}else{
				dDown += pDoc->m_TickInfo[j-1].Cl - pDoc->m_TickInfo[j].Cl;
			}
		}
	}else{
		dUp = dPrevUpAve*(dwSpan-1);
		dDown = dPrevDownAve*(dwSpan-1);

		if( pDoc->m_TickInfo[i-1].Cl < pDoc->m_TickInfo[i].Cl ){
			dUp += pDoc->m_TickInfo[i].Cl - pDoc->m_TickInfo[i-1].Cl;
		}else{
			dDown += pDoc->m_TickInfo[i-1].Cl - pDoc->m_TickInfo[i].Cl;
		}
	}

	dUp /= dwSpan;
	dDown /= dwSpan;

	dRet = dUp*100/(dUp+dDown);
	
	return dRet;
}

void CZeroGraphViewer::DrawRSI(CDC* pDC, DWORD i, DWORD dwSpan, int nCurrentX )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return;

	if( i < dwSpan*2 ){
		return;
	}
	if( i >= (pDoc->m_TickInfo.size()-1 ) ){
		return;
	}

	CPen pen_frame(PS_SOLID, 2, RGB(0, 0, 0));
	CPen pen_dot_black(PS_DOT, 1, RGB(0, 0, 0));
	CPen* oldp;

	//******************************************************************
	//	グラフの外枠を描画
	//******************************************************************
	oldp = pDC->SelectObject(&pen_frame);	//ペンの変更
	//OSCILLATOR_TOP
												//	外枠を描画
	pDC->MoveTo(OSCILLATOR_LEFT, OSCILLATOR_TOP);pDC->LineTo(OSCILLATOR_RIGHT, OSCILLATOR_TOP);
	pDC->MoveTo(OSCILLATOR_RIGHT, OSCILLATOR_TOP);pDC->LineTo(OSCILLATOR_RIGHT, OSCILLATOR_BOTTOM);
	pDC->MoveTo(OSCILLATOR_RIGHT, OSCILLATOR_BOTTOM);pDC->LineTo(OSCILLATOR_LEFT, OSCILLATOR_BOTTOM);
	pDC->MoveTo(OSCILLATOR_LEFT, OSCILLATOR_BOTTOM);pDC->LineTo(OSCILLATOR_LEFT, OSCILLATOR_TOP);

	pDC->SelectObject(oldp);	//ペンを元に戻す(必須)

	//******************************************************************

	//******************************************************************
	//	区切りの数値に点線を引く
	//******************************************************************
	double dHeightPerPixel = (double)OSCILLATOR_HEIGHT / (double)(100);

	oldp = pDC->SelectObject(&pen_dot_black);	//ペンの変更

	for (DWORD i = 20; i < 100; i += 20 ) {
		char wLine[32];
		DWORD dwY = (DWORD)(OSCILLATOR_BOTTOM - i*dHeightPerPixel);

		pDC->MoveTo(OSCILLATOR_LEFT, dwY);
		pDC->LineTo(OSCILLATOR_RIGHT, dwY);

		sprintf_s(wLine, "%d", i );
		
		TextOutA(pDC->m_hDC, OSCILLATOR_RIGHT + 3, dwY-10, wLine, lstrlen(wLine) );
	}

	pDC->SelectObject(oldp);	//ペンを元に戻す(必須)


	double dRSI = CalcRSI( i, dwSpan, 0, 0 );
	double dNextRSI = CalcRSI( i+1, dwSpan, 0, 0 );

	if( dRSI == 0 || dNextRSI == 0 ){
		return;
	}

	CPen Pen(PS_SOLID, 1, RGB(255, 0, 255));
	int nStartX, nStartY, nEndX, nEndY;

	nStartX = nCurrentX + BAR_WIDTH / 2;
	nStartY = OSCILLATOR_BOTTOM - (DWORD)((double)dRSI*dHeightPerPixel);
	nEndX = nStartX + BAR_WIDTH + BAR_SPAN;
	nEndY = OSCILLATOR_BOTTOM - (DWORD)((double)dNextRSI*dHeightPerPixel);

	oldp = pDC->SelectObject(&Pen);	//ペンの変更

	pDC->MoveTo(nStartX, nStartY);
	pDC->LineTo(nEndX, nEndY);

	pDC->SelectObject(oldp);
}

void CCsvDataViewerDoc::CalcStochasticOscillatorValue( TICK_INDICATOR_INFO* pTickIndicate, DWORD dwStart, DWORD dwKSpan, DWORD dwDSpan, DWORD dwSDSpan)
{
	DWORD i;
	DWORD dwEnd;

	DWORD dwHi;
	DWORD dwLo;
	DWORD dwSlowDSum = 0;


	if ( dwStart < dwKSpan + (dwDSpan-1))	return;

	//	%Kの計算
	CalcKLKH(dwStart, dwKSpan, &dwHi, &dwLo);
	pTickIndicate->dwSO_K = (m_TickInfo[dwStart].Cl - dwLo)*100/(dwHi-dwLo);
	//DebugPrint("CL=%.3f,Hi=%.3f,Lo=%.3f,K=%.3f\r\n", m_TickInfo[dwStart].Cl, dHi, dLo, pTickIndicate->dSO_K );
	//	%Dの計算
	pTickIndicate->dwSO_D = CalcD( dwStart, dwKSpan, dwDSpan, dwSDSpan );

	//	Slow%Dの計算
	dwEnd = dwStart + 1 - dwSDSpan;
	for (i = dwStart; i >= dwEnd; i--) {
		dwSlowDSum += CalcD( i, dwKSpan, dwDSpan, dwSDSpan);
	}
	pTickIndicate->dwSO_SlowD = dwSlowDSum / dwSDSpan;

	return;

}

BOOL CZeroGraphViewer::CalcStochasticOscillator( DWORD i, DWORD dwKSpan, DWORD dwDSpan, DWORD dwSDSpan, DWORD* pdwSO_K, DWORD* pdwSO_D, DWORD* pdwSO_Slow )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return FALSE;

	if( i >= (pDoc->m_TickInfo.size()-1 ) ){
		return FALSE;
	}

	BOOL bRet = FALSE;

	DWORD dwHi = 0;
	DWORD dwLo = 0;
	DWORD dwSO_K;
	DWORD dwSO_D;
	DWORD dwSO_SlowD;
	DWORD dwEnd;
	DWORD dwSlowDSum = 0;
	
	if ( i < dwKSpan + (dwDSpan-1))	return FALSE;

	//	%Kの計算
	pDoc->CalcKLKH(i, dwKSpan, &dwHi, &dwLo);
	dwSO_K = ( pDoc->m_TickInfo[i].Cl - dwLo)*100/(dwHi-dwLo);
	//	%Dの計算
	dwSO_D = pDoc->CalcD( i, dwKSpan, dwDSpan, dwSDSpan );

	//	Slow%Dの計算
	dwEnd = i + 1 - dwSDSpan;
	for ( DWORD j = i; j >= dwEnd; j--) {
		dwSlowDSum += pDoc->CalcD( j, dwKSpan, dwDSpan, dwSDSpan);
	}
	dwSO_SlowD = dwSlowDSum / dwSDSpan;

	if( pdwSO_K != NULL )		*pdwSO_K = dwSO_K;
	if( pdwSO_D != NULL )		*pdwSO_D = dwSO_D;
	if( pdwSO_Slow != NULL )	*pdwSO_Slow = dwSO_SlowD;

	return TRUE;
}

//*******************************************************************************************
//%K
//%K＝{ （C－L9）÷（H9－L9） }×100％
//C：当日終値
//L9：過去x日間の最安値。xとしては、14, 9, 5 などが使用されることが多い。
//H9：過去x日間の最高値
//
//
//%D
//%D＝（H3÷L3）×100％
//H3：（C－L9）のy日間合計。（C－L9）の単純移動平均。yとしては3が使われることが多い。
//L3：（H9－L9）のy日間合計。（H9－L9）の単純移動平均。
//
//Slow%D
//Slow%D＝%Dのz日の単純移動平均。zとしては、3が使われることが多い。
//*******************************************************************************************
void CZeroGraphViewer::DrawSO(CDC* pDC, DWORD i, DWORD dwKSpan, DWORD dwDSpan, DWORD dwSDSpan, int nCurrentX )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return;

	DWORD dwSO_K = 0;
	DWORD dwNextSO_K = 0;
	DWORD dwSO_D = 0;
	DWORD dwNextSO_D = 0;
	DWORD dwSO_SlowD = 0;
	DWORD dwNextSO_SlowD = 0;

	if( CalcStochasticOscillator( i, dwKSpan, dwDSpan, dwSDSpan, &dwSO_K, &dwSO_D, &dwSO_SlowD ) == FALSE )	return;

	if( CalcStochasticOscillator( i+1, dwKSpan, dwDSpan, dwSDSpan, &dwNextSO_K, &dwNextSO_D, &dwNextSO_SlowD ) == FALSE )	return;

	CPen pen_frame(PS_SOLID, 2, RGB(0, 0, 0));
	CPen pen_dot_black(PS_DOT, 1, RGB(0, 0, 0));
	CPen K_Pen(PS_SOLID, 1, RGB(0, 128, 0));
	CPen D_Pen(PS_SOLID, 1, RGB(255, 0, 255));
	CPen SD_Pen(PS_SOLID, 1, RGB(255, 153, 0));

	CPen* oldp;

	//******************************************************************
	//	グラフの外枠を描画
	//******************************************************************
	oldp = pDC->SelectObject(&pen_frame);	//ペンの変更
	//OSCILLATOR_TOP
												//	外枠を描画
	pDC->MoveTo(OSCILLATOR_LEFT, OSCILLATOR_TOP);pDC->LineTo(OSCILLATOR_RIGHT, OSCILLATOR_TOP);
	pDC->MoveTo(OSCILLATOR_RIGHT, OSCILLATOR_TOP);pDC->LineTo(OSCILLATOR_RIGHT, OSCILLATOR_BOTTOM);
	pDC->MoveTo(OSCILLATOR_RIGHT, OSCILLATOR_BOTTOM);pDC->LineTo(OSCILLATOR_LEFT, OSCILLATOR_BOTTOM);
	pDC->MoveTo(OSCILLATOR_LEFT, OSCILLATOR_BOTTOM);pDC->LineTo(OSCILLATOR_LEFT, OSCILLATOR_TOP);

	pDC->SelectObject(oldp);	//ペンを元に戻す(必須)

	//******************************************************************

	//******************************************************************
	//	区切りの数値に点線を引く
	//******************************************************************
	double dHeightPerPixel = (double)OSCILLATOR_HEIGHT / (double)(100);

	oldp = pDC->SelectObject(&pen_dot_black);	//ペンの変更

	for (DWORD i = 20; i < 100; i += 20 ) {
		char wLine[32];
		DWORD dwY = (DWORD)(OSCILLATOR_BOTTOM - i*dHeightPerPixel);

		pDC->MoveTo(OSCILLATOR_LEFT, dwY);
		pDC->LineTo(OSCILLATOR_RIGHT, dwY);

		sprintf_s(wLine, "%d", i );
		
		TextOutA(pDC->m_hDC, OSCILLATOR_RIGHT + 3, dwY-10, wLine, lstrlen(wLine) );
	}

	pDC->SelectObject(oldp);	//ペンを元に戻す(必須)

	int nStartX, nStartY, nEndX, nEndY;


#if 0	//	Kの描画
	nStartX = nCurrentX + BAR_WIDTH / 2;
	nStartY = VIEW_BOTTOM - dwSO_K;
	nEndX = nStartX + BAR_WIDTH + BAR_SPAN;
	nEndY = VIEW_BOTTOM - dwNextSO_K;

	oldp = pDC->SelectObject(&K_Pen);	//ペンの変更

	pDC->MoveTo(nStartX, nStartY);
	pDC->LineTo(nEndX, nEndY);

	pDC->SelectObject(oldp);
#endif


	nStartX = nCurrentX + BAR_WIDTH / 2;
	nStartY = VIEW_BOTTOM - dwSO_D;
	nEndX = nStartX + BAR_WIDTH + BAR_SPAN;
	nEndY = VIEW_BOTTOM - dwNextSO_D;

	oldp = pDC->SelectObject(&D_Pen);	//ペンの変更

	pDC->MoveTo(nStartX, nStartY);
	pDC->LineTo(nEndX, nEndY);

	pDC->SelectObject(oldp);


	nStartX = nCurrentX + BAR_WIDTH / 2;
	nStartY = VIEW_BOTTOM - dwSO_SlowD;
	nEndX = nStartX + BAR_WIDTH + BAR_SPAN;
	nEndY = VIEW_BOTTOM - dwNextSO_SlowD;

	oldp = pDC->SelectObject(&SD_Pen);	//ペンの変更

	pDC->MoveTo(nStartX, nStartY);
	pDC->LineTo(nEndX, nEndY);

	pDC->SelectObject(oldp);
}

DWORD CZeroGraphViewer::CalcSMAValue( DWORD dwSpan, DWORD idx )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return 0;

	DWORD dwRet = 0;

	DWORD dwSum = 0;
	for( DWORD i = idx - (dwSpan - 1); i <= idx; i++ ){
		dwSum += pDoc->m_TickInfo[i].Cl;
	}
	dwRet = dwSum / dwSpan;

	return dwRet;
}

DWORD CZeroGraphViewer::CalcEMAValue( DWORD dwSpan, DWORD idx )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return 0;

	DWORD dwRet = 0;

	//	最初の値
	if( idx == dwSpan - 1 ){
		DWORD dwSum = 0;
		for( DWORD i = idx - (dwSpan - 1); i <= idx; i++ ){
			dwSum += pDoc->m_TickInfo[i].Cl;
		}
		dwRet = dwSum / dwSpan;
	}
	//	二つ目以降の値
	else{
		double dAlpha =  2/ (double)( dwSpan + 1 );
		dwRet = (DWORD)(((double)m_EMAMap[dwSpan][idx-1])*(1-dAlpha) + ((double)pDoc->m_TickInfo[idx].Cl)*dAlpha);
	}

	return dwRet;
}

DWORD CZeroGraphViewer::CalcEMAArray( DWORD dwSpan )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return 0;

	map<DWORD,DWORD> mEMA;
	if( m_EMAMap.find(dwSpan) == m_EMAMap.end() ){
		m_EMAMap[dwSpan] = mEMA;
	}

	for( DWORD i = dwSpan-1; i < pDoc->m_TickInfo.size(); i++ ){
		if( m_EMAMap[dwSpan].find(i) == m_EMAMap[dwSpan].end() ){
			m_EMAMap[dwSpan][i] = CalcEMAValue( dwSpan, i );
		}
	}


	return m_EMAMap[dwSpan].size();
}

void CZeroGraphViewer::DrawMACD(CDC* pDC, DWORD i, DWORD dwSSpan, DWORD dwLSpan, DWORD dwSignal, int nCurrentX )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return;

	int nSignalVal, nNextSignalVal;
	int nMACD, nNextMACD;

	if( m_EMAMap.find(dwSSpan) == m_EMAMap.end() 
	 || m_EMAMap[dwSSpan].find(i) == m_EMAMap[dwSSpan].end()
	 || m_EMAMap[dwSSpan].find(i+1) == m_EMAMap[dwSSpan].end()
	){
		if( CalcEMAArray( dwSSpan ) <= i ){
			return;
		}
	}

	if( m_EMAMap.find(dwLSpan) == m_EMAMap.end() 
	 || m_EMAMap[dwLSpan].find(i) == m_EMAMap[dwLSpan].end()
	 || m_EMAMap[dwLSpan].find(i+1) == m_EMAMap[dwLSpan].end()
	){
		if( CalcEMAArray( dwLSpan ) <= i ){
			return;
		}
	}

	nMACD = (int)m_EMAMap[dwSSpan][i] - (int)m_EMAMap[dwLSpan][i];
	nNextMACD = (int)m_EMAMap[dwSSpan][i+1] - (int)m_EMAMap[dwLSpan][i+1];

	//	シグナル
	int nSum;

	nSum = 0;
	for( DWORD j = i - (dwSignal - 1); j <= i; j++ ){
		nSum += ( CalcSMAValue( dwSSpan, j ) - CalcSMAValue( dwLSpan, j ));
	}
	nSignalVal = nSum / (int)dwSignal;

	nSum = 0;
	for( DWORD j = i - (dwSignal - 1) + 1; j <= i + 1; j++ ){
		nSum += ( CalcSMAValue( dwSSpan, j ) - CalcSMAValue( dwLSpan, j ));
	}
	nNextSignalVal = nSum / (int)dwSignal;


	CPen pen_frame(PS_SOLID, 2, RGB(0, 0, 0));
	CPen pen_dot_black(PS_DOT, 1, RGB(0, 0, 0));
	CPen MACD_Pen(PS_SOLID, 1, RGB(0, 128, 0));
	CPen Signal_Pen(PS_SOLID, 1, RGB(255, 153, 0));

	CPen* oldp;

	//******************************************************************
	//	グラフの外枠を描画
	//******************************************************************
	oldp = pDC->SelectObject(&pen_frame);	//ペンの変更
	//OSCILLATOR_TOP
												//	外枠を描画
	pDC->MoveTo(OSCILLATOR_LEFT, OSCILLATOR_TOP);pDC->LineTo(OSCILLATOR_RIGHT, OSCILLATOR_TOP);
	pDC->MoveTo(OSCILLATOR_RIGHT, OSCILLATOR_TOP);pDC->LineTo(OSCILLATOR_RIGHT, OSCILLATOR_BOTTOM);
	pDC->MoveTo(OSCILLATOR_RIGHT, OSCILLATOR_BOTTOM);pDC->LineTo(OSCILLATOR_LEFT, OSCILLATOR_BOTTOM);
	pDC->MoveTo(OSCILLATOR_LEFT, OSCILLATOR_BOTTOM);pDC->LineTo(OSCILLATOR_LEFT, OSCILLATOR_TOP);

	pDC->SelectObject(oldp);	//ペンを元に戻す(必須)

	//******************************************************************

	//******************************************************************
	//	区切りの数値に点線を引く
	//******************************************************************
	double dHeightPerPixel = (double)OSCILLATOR_HEIGHT / (double)(800);
	DWORD dwCenterY = OSCILLATOR_BOTTOM - OSCILLATOR_HEIGHT/2;
	
	oldp = pDC->SelectObject(&pen_dot_black);	//ペンの変更

	for (int i = -400; i <= 400; i += 100 ) {
		char wLine[32];
		DWORD dwY = (DWORD)((int)dwCenterY + i*dHeightPerPixel);

		pDC->MoveTo(OSCILLATOR_LEFT, dwY);
		pDC->LineTo(OSCILLATOR_RIGHT, dwY);

		sprintf_s(wLine, "%d", i );
		
		TextOutA(pDC->m_hDC, OSCILLATOR_RIGHT + 3, dwY-10, wLine, lstrlen(wLine) );
	}

	pDC->SelectObject(oldp);	//ペンを元に戻す(必須)

	int nStartX, nStartY, nEndX, nEndY;

	nStartX = nCurrentX + BAR_WIDTH / 2;
	nStartY = (int)dwCenterY + nMACD;
	nEndX = nStartX + BAR_WIDTH + BAR_SPAN;
	nEndY = (int)dwCenterY + nNextMACD;

	oldp = pDC->SelectObject(&MACD_Pen);	//ペンの変更

	pDC->MoveTo(nStartX, nStartY);
	pDC->LineTo(nEndX, nEndY);

	pDC->SelectObject(oldp);


	nStartX = nCurrentX + BAR_WIDTH / 2;
	nStartY = (int)dwCenterY + nSignalVal;
	nEndX = nStartX + BAR_WIDTH + BAR_SPAN;
	nEndY = (int)dwCenterY + nNextSignalVal;

	oldp = pDC->SelectObject(&Signal_Pen);	//ペンの変更

	pDC->MoveTo(nStartX, nStartY);
	pDC->LineTo(nEndX, nEndY);

	pDC->SelectObject(oldp);
}


void CZeroGraphViewer::DrawADX(CDC* pDC, DWORD i, DWORD dwSpan, int nCurrentX )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return;

	if( i < dwSpan*2 ){
		return;
	}
	if( i >= (pDoc->m_TickInfo.size()-1 ) ){
		return;
	}

	CPen pen_frame(PS_SOLID, 2, RGB(0, 0, 0));
	CPen pen_dot_black(PS_DOT, 1, RGB(0, 0, 0));
	CPen* oldp;

	//******************************************************************
	//	グラフの外枠を描画
	//******************************************************************
	oldp = pDC->SelectObject(&pen_frame);	//ペンの変更
	//OSCILLATOR_TOP
												//	外枠を描画
	pDC->MoveTo(OSCILLATOR_LEFT, OSCILLATOR_TOP);pDC->LineTo(OSCILLATOR_RIGHT, OSCILLATOR_TOP);
	pDC->MoveTo(OSCILLATOR_RIGHT, OSCILLATOR_TOP);pDC->LineTo(OSCILLATOR_RIGHT, OSCILLATOR_BOTTOM);
	pDC->MoveTo(OSCILLATOR_RIGHT, OSCILLATOR_BOTTOM);pDC->LineTo(OSCILLATOR_LEFT, OSCILLATOR_BOTTOM);
	pDC->MoveTo(OSCILLATOR_LEFT, OSCILLATOR_BOTTOM);pDC->LineTo(OSCILLATOR_LEFT, OSCILLATOR_TOP);

	pDC->SelectObject(oldp);	//ペンを元に戻す(必須)

	//******************************************************************

	//******************************************************************
	//	区切りの数値に点線を引く
	//******************************************************************
	double dHeightPerPixel = (double)OSCILLATOR_HEIGHT / (double)(100);

	oldp = pDC->SelectObject(&pen_dot_black);	//ペンの変更

	for (DWORD i = 20; i < 100; i += 20 ) {
		char wLine[32];
		DWORD dwY = (DWORD)(OSCILLATOR_BOTTOM - i*dHeightPerPixel);

		pDC->MoveTo(OSCILLATOR_LEFT, dwY);
		pDC->LineTo(OSCILLATOR_RIGHT, dwY);

		sprintf_s(wLine, "%d", i );
		
		TextOutA(pDC->m_hDC, OSCILLATOR_RIGHT + 3, dwY-10, wLine, lstrlen(wLine) );
	}

	pDC->SelectObject(oldp);	//ペンを元に戻す(必須)


	DWORD dwADX = CalcADX( i, dwSpan );
	DWORD dwNextADX = CalcADX( i+1, dwSpan );

	if( dwADX == 0 || dwNextADX == 0 ){
		return;
	}

	CPen Pen(PS_SOLID, 1, RGB(255, 0, 255));
	int nStartX, nStartY, nEndX, nEndY;

	nStartX = nCurrentX + BAR_WIDTH / 2;
	nStartY = OSCILLATOR_BOTTOM - (DWORD)((double)dwADX*dHeightPerPixel);
	nEndX = nStartX + BAR_WIDTH + BAR_SPAN;
	nEndY = OSCILLATOR_BOTTOM - (DWORD)((double)dwNextADX*dHeightPerPixel);

	oldp = pDC->SelectObject(&Pen);	//ペンの変更

	pDC->MoveTo(nStartX, nStartY);
	pDC->LineTo(nEndX, nEndY);

	pDC->SelectObject(oldp);
}

/////////////////////////////////////////////////////
//σ=√(((X1-Xm)^2+(X2-Xm)^2・・・(Xn-Xm)^2)/N-1)
//N・・・任意の期間(25日を使うことが多いです)
//X1～Xn・・・1～N日までの各終値
//Xm・・・X1～Xnの平均値 
double CZeroGraphViewer::CalcSigma( DWORD i , DWORD dwSpan )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return 0;

	if ( i + 1 < dwSpan )	return 0;

	DWORD l;
	double nAve = 0;
	double nMole = 0;

	for( l = i - dwSpan + 1; l <= i; l++ ){
		nAve += (double)pDoc->m_TickInfo[l].Cl;
	}
	nAve /= (double)dwSpan;

	for( l = i - dwSpan + 1; l <= i; l++ ){
		nMole += ((double)pDoc->m_TickInfo[l].Cl-nAve)*((double)pDoc->m_TickInfo[l].Cl-nAve);
	}

	nMole /= (double)dwSpan-1;

	return sqrt(nMole);
}

DWORD CZeroGraphViewer::CalcBollingerBandsSigma( DWORD i , DWORD dwSpan, int nSigma )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return 0;

	if ( i + 1 < dwSpan )	return 0;

	DWORD l;
	double nAve = 0;
	double nMole = 0;

	for( l = i - dwSpan + 1; l <= i; l++ ){
		nAve += (double)pDoc->m_TickInfo[l].Cl;
	}
	nAve /= (double)dwSpan;

	return (DWORD)(nAve + (CalcSigma(i, dwSpan)*nSigma));
}


void CZeroGraphViewer::DrawBollingerBands(CDC* pDC, DWORD i, DWORD dwSpan, DWORD dwSigma, int nCurrentX )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if ( i + 1 < dwSpan )	return;
	if( i >= (pDoc->m_TickInfo.size()-1 ) )	return;

	DWORD dwBB = 0;
	DWORD dwNextBB = 0;

	for( int nSigma = -(int)dwSigma; nSigma <= (int)dwSigma; nSigma++ ){
		//if( nSigma != 2 && nSigma != -2 )	continue;

		dwBB = CalcBollingerBandsSigma( i, dwSpan, nSigma );
		dwNextBB = CalcBollingerBandsSigma( i+1, dwSpan, nSigma );

		int nStartX, nStartY, nEndX, nEndY;
	
		nStartX = nCurrentX + BAR_WIDTH / 2;
		nStartY = GRAPH_BOTTOM - (DWORD)((dwBB - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);
		nEndX = nStartX + BAR_WIDTH + BAR_SPAN;
		nEndY = GRAPH_BOTTOM - (DWORD)((dwNextBB - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);

		CPen pen_frame(PS_SOLID, 1, RGB(0xFf,0,0) );
		CPen* oldp = pDC->SelectObject(&pen_frame);	//ペンの変更

		pDC->MoveTo( nStartX, nStartY );
		pDC->LineTo( nEndX, nEndY );

		pDC->SelectObject(oldp);
	}
#if 0
	//	実体の描画
	RECT rc;
	double dSigma = CalcSigma( i, dwSpan );

	rc.left = nCurrentX;
	rc.top = GRAPH_BOTTOM - (DWORD)(dSigma);
	rc.right = rc.left + BAR_WIDTH;
	rc.bottom = GRAPH_BOTTOM;

	CPen pen_frame(PS_SOLID, 1, RGB(0,0xFF,0) );
	CPen* oldp = pDC->SelectObject(&pen_frame);	//ペンの変更

	pDC->Rectangle(&rc);

	pDC->SelectObject(oldp);
#endif
}

//	指数移動平均線の描画
void CZeroGraphViewer::DrawEMALine(CDC* pDC, DWORD i, DWORD dwEMAVal, int nCurrentX )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//	CurrentからNextまでの線なので+1したものがオーバーしないようにする
	if (!(i + 1 < pDoc->m_TickIndicteInfo.size()))	return;

	DWORD dwEma = 0;
	DWORD dwNextEma = 0;
	COLORREF lColor;
	if(dwEMAVal == 13) {
		dwEma = pDoc->m_TickIndicteInfo[i].dw13ema;
		dwNextEma = pDoc->m_TickIndicteInfo[i + 1].dw13ema;
		lColor = RGB(222, 130, 167);
	}else if(dwEMAVal == 21) {
		dwEma = pDoc->m_TickIndicteInfo[i].dw21ema;
		dwNextEma = pDoc->m_TickIndicteInfo[i + 1].dw21ema;
		lColor = RGB(91, 179, 181);
	}else if (dwEMAVal == 20) {
		dwEma = pDoc->m_TickIndicteInfo[i].dw20ema;
		dwNextEma = pDoc->m_TickIndicteInfo[i+1].dw20ema;
		lColor = RGB(0, 128, 0);
	}
	else if (dwEMAVal == 50) {
		dwEma = pDoc->m_TickIndicteInfo[i].dw50ema;
		dwNextEma = pDoc->m_TickIndicteInfo[i + 1].dw50ema;
		lColor = RGB(255, 0, 255);
	}
	else if (dwEMAVal == 200) {
		dwEma = pDoc->m_TickIndicteInfo[i].dw200ema;
		dwNextEma = pDoc->m_TickIndicteInfo[i + 1].dw200ema;
		lColor = RGB(255, 153, 0);
	}

	//	値が不正な場合はさようなら
	if (dwEma == 0 || dwNextEma == 0) {
		return;
	}

	int nStartX, nStartY, nEndX, nEndY;
	
	nStartX = nCurrentX + BAR_WIDTH / 2;
	nStartY = GRAPH_BOTTOM - (DWORD)((dwEma - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);
	nEndX = nStartX + BAR_WIDTH + BAR_SPAN;
	nEndY = GRAPH_BOTTOM - (DWORD)((dwNextEma - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);

	CPen pen_frame(PS_SOLID, 1, lColor );
	CPen* oldp = pDC->SelectObject(&pen_frame);	//ペンの変更

	pDC->MoveTo( nStartX, nStartY );
	pDC->LineTo( nEndX, nEndY );

	pDC->SelectObject(oldp);
}


int CZeroGraphViewer::GetTradeInfo( DWORD dwTicket )
{
	int nRet = -1;
	TRADE_INFO* pRet = NULL;
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return nRet;

	for( DWORD i = 0; i < pDoc->m_TradeInfo.size(); i++ ){
		if( pDoc->m_TradeInfo[i].dwTicket == dwTicket ){
			nRet = i;

			break;
		}
	}

	return nRet;
}


BOOL CZeroGraphViewer::IsConvergeSigma( BOOL bHiCheck, DWORD dwIdx )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return FALSE;

	BOOL bConverge = FALSE;

	DWORD dwHiSigma2 = CalcBollingerBandsSigma( dwIdx, 21, 2 );
	DWORD dwLoSigma2 = CalcBollingerBandsSigma( dwIdx, 21, -2 );
	DWORD dwPrevHiSigma2 = CalcBollingerBandsSigma( dwIdx-1, 21, 2 );
	DWORD dwPrevLoSigma2 = CalcBollingerBandsSigma( dwIdx-1, 21, -2 );
	DWORD i;

	if( bHiCheck != FALSE ){
		if( dwPrevHiSigma2 > dwHiSigma2 ){
			dwHiSigma2 = dwPrevHiSigma2;
			for( i = dwIdx -1; i >= 0; i-- ){
				dwPrevHiSigma2 = CalcBollingerBandsSigma( i-1, 21, 2 );

				if( dwHiSigma2 == dwPrevHiSigma2 ){
					;
				}else if( dwHiSigma2 > dwPrevHiSigma2 ){
					bConverge = TRUE;
					break;
				}else{
					break;
				}
			}
		}
	}else{
		if( dwPrevLoSigma2 < dwLoSigma2 ){
			dwLoSigma2 = dwPrevLoSigma2;
			for( i = dwIdx -1; i >= 0; i-- ){
				dwPrevLoSigma2 = CalcBollingerBandsSigma( i-1, 21, -2 );

				if( dwLoSigma2 == dwPrevLoSigma2 ){
					;
				}else if( dwLoSigma2 < dwPrevLoSigma2 ){
					bConverge = TRUE;
					break;
				}else{
					break;
				}
			}
		}
	}

	return bConverge;
}


//	ボリジャーバンドを使った判定
DWORD CZeroGraphViewer::BandsWalkRange( CDC* pDC, int nStart, int idx )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return 0;

	DWORD dwRet = 0;
	static DWORD dwPrevChkIdx = 0;
	DWORD dwEndIdx = 0;

	if( idx < 50 )	return 0;

	//	σ2が縮小傾向にあるか？を超えているか？
	DWORD dwHiSigma2 = CalcBollingerBandsSigma( idx, 21, 2 );
	DWORD dwLoSigma2 = CalcBollingerBandsSigma( idx, 21, -2 );
#define NEED_SIGMA_PIPS	(3)

	if( dwHiSigma2 - dwLoSigma2 > Pips2EVal(NEED_SIGMA_PIPS)*2 ){
		DWORD dwPrevHiSigma2 = CalcBollingerBandsSigma( idx-1, 21, 2 );
		DWORD dwPrevLoSigma2 = CalcBollingerBandsSigma( idx-1, 21, -2 );
		DWORD dwPrevPrevHiSigma2 = CalcBollingerBandsSigma( idx-2, 21, 2 );
		DWORD dwPrevPrevLoSigma2 = CalcBollingerBandsSigma( idx-2, 21, -2 );
		DWORD dwWalkCnt = 0;

		DWORD i = 0;

		//	σの縮まりはじめを検知
		if( IsConvergeSigma( FALSE, idx ) != FALSE ){
			if( dwPrevChkIdx == idx )	return 0;
			if( pDoc->m_TickInfo[idx-1].Cl >=  dwPrevHiSigma2
			 || pDoc->m_TickInfo[idx-2].Cl >=  dwPrevPrevHiSigma2
			){
				dwHiSigma2 = dwPrevHiSigma2;
				//	遡っていき、スクイーズしている場所を探す
				for( i = idx -1; i >= 0; i-- ){
					dwPrevHiSigma2 = CalcBollingerBandsSigma( i-1, 21, 2 );
					if( dwPrevHiSigma2 > dwHiSigma2 ){
						break;
					}
					if( dwEndIdx == 0 ){
						if( pDoc->m_TickInfo[i].Op < pDoc->m_TickInfo[i].Cl ){
							dwEndIdx = i;
						}
					}else{
						/*if( pDoc->m_TickInfo[i].Op > pDoc->m_TickInfo[i].Cl ){
							i++;
							break;
						}*/
					}
						
					if( pDoc->m_TickInfo[i].Cl >=  dwHiSigma2 ){
						dwWalkCnt++;
					}
					dwHiSigma2 = dwPrevHiSigma2;
				}
			}
			dwPrevChkIdx = idx;
		}else if( IsConvergeSigma( TRUE, idx ) != FALSE ){
			if( dwPrevChkIdx == idx )	return 0;

			if( pDoc->m_TickInfo[idx-1].Cl <=  dwPrevLoSigma2
			 || pDoc->m_TickInfo[idx-2].Cl <=  dwPrevPrevLoSigma2
			){
				dwLoSigma2 = dwPrevLoSigma2;
				//	遡っていき、スクイーズしている場所を探す
				for( i = idx -1; i >= 0; i-- ){
					dwPrevLoSigma2 = CalcBollingerBandsSigma( i-1, 21, -2 );
					if( dwPrevLoSigma2 < dwLoSigma2 ){
						break;
					}
					if( dwEndIdx == 0 ){
						if( pDoc->m_TickInfo[i].Cl < pDoc->m_TickInfo[i].Op ){
							dwEndIdx = i;
						}
					}else{
						/*if( pDoc->m_TickInfo[i].Cl > pDoc->m_TickInfo[i].Op ){
							i++;
							break;
						}*/
					}
					if( pDoc->m_TickInfo[i].Cl <=  dwLoSigma2 ){
						dwWalkCnt++;
					}
					dwLoSigma2 = dwPrevLoSigma2;
				}
			}
			dwPrevChkIdx = idx;
		}

		if( dwWalkCnt >= 1 ){
			for( ; i<=dwEndIdx; i++ ){
				dwHiSigma2 = CalcBollingerBandsSigma( i, 21, 2 );
				dwLoSigma2 = CalcBollingerBandsSigma( i, 21, -2 );
				DWORD dwNextHiSigma2 = CalcBollingerBandsSigma( i+1, 21, 2 );
				DWORD dwNextLoSigma2 = CalcBollingerBandsSigma( i+1, 21, -2 );

				//	エクスパンション
				if( dwHiSigma2 < dwNextHiSigma2 
					&& dwLoSigma2 > dwNextLoSigma2 
				){
					break;
				}
			}

			//int nStartPos = nStart - (BAR_WIDTH + BAR_SPAN)*( idx - i );
			WrieteRectangle( pDC, nStart, i, idx, PS_SOLID, 2, RGB(153,255,0) );

			dwRet = i;
		}
	}

	return dwRet;
}

void CZeroGraphViewer::BollingerSigmaTrans( CDC* pDC, int nCurX, int i ){
	DWORD dwHiSigma2 = CalcBollingerBandsSigma( i, 21, 2 );
	DWORD dwLoSigma2 = CalcBollingerBandsSigma( i, 21, -2 );
	DWORD dwPrevHiSigma2 = CalcBollingerBandsSigma( i-1, 21, 2 );
	DWORD dwPrevLoSigma2 = CalcBollingerBandsSigma( i-1, 21, -2 );
	CPen pen_dot;
	CPen* oldp;
	DWORD dwBolliSign = 0;
	static DWORD dwPrevBolliSign = 0;
			
	//	エクスパンション
	if( dwPrevHiSigma2 < dwHiSigma2 
	 && dwPrevLoSigma2 > dwLoSigma2 
	){
		pen_dot.CreatePen( PS_SOLID, 1, RGB(0xFF, 0, 0) );

		dwBolliSign = 1;
	}
	//	スクイーズ
	else 
	if( dwPrevHiSigma2 > dwHiSigma2 
	 && dwPrevLoSigma2 < dwLoSigma2 
	){
		pen_dot.CreatePen( PS_SOLID, 1, RGB(0, 0, 0xFF));

		dwBolliSign = 2;
	}/*else 
	if( dwPrevHiSigma2 < dwHiSigma2 
	 || dwPrevLoSigma2 > dwLoSigma2 
	){
		pen_dot.CreatePen( PS_DOT, 1, RGB(0xFF, 0, 0) );

		dwBolliSign = 3;
	}else 
	if( dwPrevHiSigma2 > dwHiSigma2 
	 || dwPrevLoSigma2 < dwLoSigma2 
	){
		pen_dot.CreatePen( PS_DOT, 1, RGB(0, 0, 0xFF));

		dwBolliSign = 4;
	}*/else{
		dwBolliSign = 0;
	}

	if( dwBolliSign != 0 && dwPrevBolliSign != dwBolliSign ){
		oldp = pDC->SelectObject(&pen_dot);	//ペンの変更

		pDC->MoveTo(nCurX, GRAPH_TOP);
		pDC->LineTo(nCurX, GRAPH_BOTTOM);

		pDC->SelectObject(oldp);	//ペンを元に戻す(必須)

		dwPrevBolliSign = dwBolliSign;
	}
}

void CZeroGraphViewer::WriteTrandTerm(CDC* pDC, int nCurX, int i, DWORD dwEndIdx ) 
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int j, k;
	BOOL bFindEnd = FALSE;

	for(j = 0; j < (int)pDoc->m_TradeInfo.size(); j++) {
		BOOL bFind = FALSE;
		FILETIME ft_tick;
		FILETIME ft_trade;
		LONGLONG llTick;
		LONGLONG llTrade_st;
		LONGLONG llTrade_end;


#if 1
		SystemTimeToFileTime(&pDoc->m_TradeInfo[j].closeDt, &ft_trade);
		llTrade_end = *(LONGLONG*)&ft_trade;
		SystemTimeToFileTime(&pDoc->m_TradeInfo[j].openDt, &ft_trade);
		llTrade_st = *(LONGLONG*)&ft_trade;

		SystemTimeToFileTime(&pDoc->m_TickInfo[i].sDt, &ft_tick);
		llTick = *(LONGLONG*)&ft_tick;
		if(llTick <= llTrade_end) {
			if(i + 1 <= (int)pDoc->m_TickInfo.size() - 1) {
				SystemTimeToFileTime(&pDoc->m_TickInfo[i + 1].sDt, &ft_tick);
				llTick = *(LONGLONG*)&ft_tick;
				if(llTick > llTrade_end) {
					bFind = TRUE;
				}
			}
			else {
				bFind = TRUE;
			}
		}
		if(bFind != FALSE) {
			bFind = FALSE;
			for(k = i; k >= (int)dwEndIdx; k--) {
				SystemTimeToFileTime(&pDoc->m_TickInfo[k].sDt, &ft_tick);
				llTick = *(LONGLONG*)&ft_tick;
				if(llTick <= llTrade_st) {
					if(k - 1 >= (int)dwEndIdx) {
						SystemTimeToFileTime(&pDoc->m_TickInfo[k + 1].sDt, &ft_tick);
						llTick = *(LONGLONG*)&ft_tick;
						if(llTrade_st < llTick) {
							bFind = TRUE;
						}
					}
					else {
						bFind = TRUE;
					}
				}
				if(bFind != FALSE) {
					COLORREF lColor;
					if(pDoc->m_TradeInfo[j].dOpenPrice == pDoc->m_TradeInfo[j].dClosePrice) {
						lColor = RGB(0, 0, 0);
					}
					else if(pDoc->m_TradeInfo[j].dOpenPrice < pDoc->m_TradeInfo[j].dClosePrice) {
						if(pDoc->m_TradeInfo[j].dwOrderType == 1) {
							lColor = RGB(0, 0, 0xFF);
						}
						else {
							lColor = RGB(0xFF, 0, 0);
						}
					}
					else {
						if(pDoc->m_TradeInfo[j].dwOrderType == 1) {
							lColor = RGB(0xFF, 0, 0);
						}
						else {
							lColor = RGB(0, 0, 0xFF);
						}
					}
					int nStartPos = nCurX - (BAR_WIDTH + BAR_SPAN)*(i - k);

					WrieteRectangle(pDC, nStartPos, k, i, PS_SOLID, 2, lColor);

					bFindEnd = TRUE;

					break;
				}
			}
		}
#endif
		if(bFindEnd == TRUE)	break;
	}
}

BOOL CZeroGraphViewer::IncludeOscillator()
{
	BOOL bRet = FALSE;

	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return FALSE;

	vector<INDICATOR_SETTING>::iterator itr = m_IndiSetting.begin();

	for( ;itr != m_IndiSetting.end(); itr++ ){
		if( IS_OSCILLATOR( (*itr).dwIndiType ) ){
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

bool is_cloud( DWORD dwParam1, DWORD dwParam2, DWORD dwParam3, DWORD dwParam4 )
{
	bool bRet = false;

	if(dwParam1 < dwParam2) {
		if( dwParam1 < dwParam3 && dwParam3 < dwParam2
		 || dwParam1 < dwParam4 && dwParam4 < dwParam2
		){
			bRet = true;
		}
	}
	else {
		if( dwParam2 < dwParam3 && dwParam3 < dwParam1
		 || dwParam2 < dwParam4 && dwParam4 < dwParam1
		){
			bRet = true;
		}
	}

	return bRet;
}

int g_nGraphStartChange = 0;
// CZeroGraphViewer 描画

DWORD g_dwPrevBolliSign = 0;

void CZeroGraphViewer::OnDraw(CDC* pDC)
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->GetPathName().IsEmpty() == TRUE)
		return;

	VIEW_TICK_DATA vtd;

	pDoc->ReadTradeFile();

	//	描画処理の開始
	CRect cRc;

	GetClientRect(&cRc);

	if( IncludeOscillator() != FALSE ){
		g_dwGraphScalePerWSize = 70;
	}else{
		g_dwGraphScalePerWSize = 100;
	}

	//g_dwGraphBottom = GRAPH_TOP + (cRc.bottom - 100 - GRAPH_MARGIN * 2) - 100;
	g_dwGraphBottom = ( cRc.bottom * GRAPH_SCALE_PERWSIZE /100 ) - GRAPH_MARGIN;
	g_dwOscillatorBottom = cRc.bottom - GRAPH_MARGIN;
	g_dwOscillatorTop = ( cRc.bottom * GRAPH_SCALE_PERWSIZE /100 ) + GRAPH_MARGIN;
	g_dwGraphRight = cRc.right - 50;

	g_dwViewBottom = cRc.bottom - GRAPH_MARGIN;
	g_dwViewRight = cRc.right - GRAPH_MARGIN;
	
	CPen pen_frame(PS_SOLID, 2, RGB(0, 0, 0));
	CPen pen_black(PS_SOLID, 1, RGB(0, 0, 0));
	CPen pen_dot_black(PS_DOT, 1, RGB(0, 0, 0));
	CPen pen_red(PS_SOLID, 1, RGB(0xFF, 0, 0));
	CPen pen_blue(PS_SOLID, 1, RGB(0, 0, 0xFF));

	HBRUSH hBrush_old;
	CPen* oldp;

	//******************************************************************
	//	グラフの外枠を描画
	//******************************************************************
	oldp = pDC->SelectObject(&pen_frame);	//ペンの変更

												//	外枠を描画
	pDC->MoveTo(GRAPH_LEFT, GRAPH_TOP);pDC->LineTo(GRAPH_RIGHT, GRAPH_TOP);
	pDC->MoveTo(GRAPH_RIGHT, GRAPH_TOP);pDC->LineTo(GRAPH_RIGHT, GRAPH_BOTTOM);
	pDC->MoveTo(GRAPH_RIGHT, GRAPH_BOTTOM);pDC->LineTo(GRAPH_LEFT, GRAPH_BOTTOM);
	pDC->MoveTo(GRAPH_LEFT, GRAPH_BOTTOM);pDC->LineTo(GRAPH_LEFT, GRAPH_TOP);

	pDC->SelectObject(oldp);	//ペンを元に戻す(必須)

	//******************************************************************


	if( pDoc->m_TickInfo.size() > 0 ){
		//	表示できるバーの数を計算する
		//	1個の表示に必要な幅・・・BAR_WIDTH+BAR_SPAN
		DWORD dwViewBars = (GRAPH_WIDTH) / (BAR_WIDTH + BAR_SPAN);

		DWORD dwMaxRecords;// = sizeof(g_tickinfo) / sizeof(g_tickinfo[0]);
		dwMaxRecords = pDoc->m_TickInfo.size();

		if ((int)dwMaxRecords > g_nGraphStartChange && dwMaxRecords - g_nGraphStartChange > (GRAPH_WIDTH) / (BAR_WIDTH + BAR_SPAN)) {
			dwMaxRecords -= g_nGraphStartChange;
		}
		else if( (GRAPH_WIDTH) / (BAR_WIDTH + BAR_SPAN) > dwMaxRecords ){
			;;
		}else {
			dwMaxRecords = (GRAPH_WIDTH) / (BAR_WIDTH + BAR_SPAN);
			//g_nGraphStartChange = dwMaxRecords;
		}

		DWORD dwEndIdx = (dwViewBars < dwMaxRecords) ? (dwMaxRecords - dwViewBars - 1) : 0;

		if( (GRAPH_WIDTH) / (BAR_WIDTH + BAR_SPAN) > dwMaxRecords ){
			dwMaxRecords -= 1;
		}else{
			dwMaxRecords -= 2;	//	-2ってなんぞ？思い出せない
		}
		//	最大値と最小値を検索する
		DWORD dwMaxVal = 0;
		m_dwMinVal = 777777;	//	メンバーにしてみました。

		for (int i = dwMaxRecords; i >= (int)dwEndIdx; i--) {
			DWORD Hi = EValFractionCut( pDoc->m_TickInfo[i].Hi, pDoc->m_dwPair );
			DWORD Lo = EValFractionCut( pDoc->m_TickInfo[i].Lo, pDoc->m_dwPair );

			if (dwMaxVal < Hi)		dwMaxVal = Hi;
			if (m_dwMinVal > Lo)	m_dwMinVal = Lo;
		}

		//	見た目上上下にゆとりを持たせる
		dwMaxVal += 100;
		m_dwMinVal -= 100;

		g_BarHeightPerPixel = (double)GRAPH_HEIGHT / (double)(dwMaxVal - m_dwMinVal);


		//******************************************************************
		//	区切りの数値に点線を引く
		//******************************************************************
		DWORD dwLineY = (m_dwMinVal+100) - (m_dwMinVal%100);

		oldp = pDC->SelectObject(&pen_dot_black);	//ペンの変更

		for (DWORD i = dwLineY; i < dwMaxVal; i += 100 ) {
			char wLine[32];
			DWORD dwY = (DWORD)(GRAPH_BOTTOM - (i - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);

			pDC->MoveTo(GRAPH_LEFT, dwY);
			pDC->LineTo(GRAPH_RIGHT, dwY);

			if (pDoc->m_dwPair == PAIR_EURUSD) {
				sprintf_s(wLine, "%.4f", EVal2Rate( i, pDoc->m_dwPair ) );
			}else{
				sprintf_s(wLine, "%.2f", EVal2Rate( i, pDoc->m_dwPair ) );
			}
			TextOutA(pDC->m_hDC, GRAPH_RIGHT + 3, dwY - 2, wLine, lstrlen(wLine) );
		}

		pDC->SelectObject(oldp);	//ペンを元に戻す(必須)


		char cDate[32];
		wsprintf(cDate, "%02d/%02d", pDoc->m_TickInfo[dwEndIdx].sDt.wMonth, pDoc->m_TickInfo[dwEndIdx].sDt.wDay );
		TextOutA(pDC->m_hDC, GRAPH_LEFT+2, GRAPH_TOP + 2, cDate, lstrlen(cDate) );

		//*******************************************************************
		int nCurX = GRAPH_RIGHT - (BAR_SPAN + BAR_WIDTH);
		CRect rc;
		int nPrevHour = -1;
		int nPrevMin = -1;


		for (int i = dwMaxRecords; i >= (int)dwEndIdx; i--) {
			DWORD open = EValFractionCut(pDoc->m_TickInfo[i].Op, pDoc->m_dwPair );
			DWORD Hi = EValFractionCut(pDoc->m_TickInfo[i].Hi, pDoc->m_dwPair);
			DWORD Lo = EValFractionCut(pDoc->m_TickInfo[i].Lo, pDoc->m_dwPair);
			DWORD close = EValFractionCut(pDoc->m_TickInfo[i].Cl, pDoc->m_dwPair);

			//******************************************************************
			//	10分おきに時刻を表示
			//******************************************************************
			if ( ( nPrevHour == -1 && nPrevMin == -1 )
			|| (nPrevHour > pDoc->m_TickInfo[i].sDt.wHour)
			|| (nPrevHour == pDoc->m_TickInfo[i].sDt.wHour && nPrevMin > pDoc->m_TickInfo[i].sDt.wMinute)
			) {
				if (pDoc->m_TickInfo[i].sDt.wMinute % 10 == 0) {
					char cHour[32];
					char cMin[32];

					wsprintf(cHour, "%02d", pDoc->m_TickInfo[i].sDt.wHour);
					TextOutA(pDC->m_hDC, nCurX, GRAPH_BOTTOM + 10, cHour, sizeof("XX") - 1);

					wsprintf(cMin, "%02d", pDoc->m_TickInfo[i].sDt.wMinute);
					TextOutA(pDC->m_hDC, nCurX, GRAPH_BOTTOM + 30, cMin, sizeof("XX") - 1);

					nPrevHour = pDoc->m_TickInfo[i].sDt.wHour;
					nPrevMin = pDoc->m_TickInfo[i].sDt.wMinute;
				}
			}
			//******************************************************************

			//******************************************************************
			//	ローソクの描画
			//******************************************************************
			hBrush_old = NULL;

			if ( close == open ) {
				oldp = pDC->SelectObject(&pen_black);	//ペンの変更
			}
			else if ( close > open ) {
				oldp = pDC->SelectObject(&pen_red);	//ペンの変更
				hBrush_old = (HBRUSH)pDC->SelectObject(m_hbrush_red);
			}
			else/* if ( close < open ) */
			{
				oldp = pDC->SelectObject(&pen_blue);	//ペンの変更
				hBrush_old = (HBRUSH)pDC->SelectObject(m_hbrush_blue);
			}

			//	実体の描画
			rc.left = nCurX;
			rc.top = GRAPH_BOTTOM - (DWORD)((close - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);
			rc.right = rc.left + BAR_WIDTH;
			rc.bottom = GRAPH_BOTTOM - (DWORD)((open - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL);
			if (rc.top == rc.bottom)	rc.bottom += 1;

			pDC->Rectangle(&rc);

			memcpy( &vtd.ti, &pDoc->m_TickInfo[i], sizeof(TICK_INFO) );
			vtd.rc.left = rc.left;
			vtd.rc.top = rc.top;
			vtd.rc.right = rc.right;
			vtd.rc.bottom = rc.bottom;

			m_ViewTickData.push_back(vtd);

			if (hBrush_old != NULL)	pDC->SelectObject(hBrush_old);	//ペンを元に戻す(必須)
			pDC->SelectObject(oldp);	//ペンを元に戻す(必須)

			//	ヒゲの描画
			oldp = pDC->SelectObject(&pen_black);	//ペンの変更

			pDC->MoveTo(nCurX + BAR_WIDTH / 2, rc.top);
			pDC->LineTo(nCurX + BAR_WIDTH / 2, GRAPH_BOTTOM - (DWORD)((Hi - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL));
			pDC->MoveTo(nCurX + BAR_WIDTH / 2, rc.bottom);
			pDC->LineTo(nCurX + BAR_WIDTH / 2, GRAPH_BOTTOM - (DWORD)((Lo - m_dwMinVal)*BAR_HEIGHT_PER_PIXEL));

			pDC->SelectObject(oldp);	//ペンを元に戻す(必須)
			//******************************************************************
#if 0
//	テストシグマ超過
DWORD dwHiSigma = CalcBollingerBandsSigma( i, 21, 3 );
DWORD dwLoSigma = CalcBollingerBandsSigma( i, 21, -3 );
ICHIMOKU_INFO ichi_info;
BOOL bIchi = GetIchimokuData(i,&ichi_info);

//if( bIchi != FALSE )
{
	if( pDoc->m_TickInfo[i].Hi >= dwHiSigma ){
		//if( is_cloud( ichi_info.dwLeadingSpan1, ichi_info.dwLeadingSpan2, pDoc->m_TickInfo[i].Op, pDoc->m_TickInfo[i].Cl ) != false )
		{
			oldp = pDC->SelectObject(&pen_red);	//ペンの変更

			pDC->MoveTo(nCurX + BAR_WIDTH / 2, rc.top);
			pDC->LineTo(nCurX + BAR_WIDTH / 2, GRAPH_TOP );

			pDC->SelectObject(oldp);	//ペンを元に戻す(必須)
		}
	}else if( pDoc->m_TickInfo[i].Lo <= dwLoSigma ){
		//if( is_cloud( ichi_info.dwLeadingSpan1, ichi_info.dwLeadingSpan2, pDoc->m_TickInfo[i].Op, pDoc->m_TickInfo[i].Cl ) != false )
		{
			oldp = pDC->SelectObject(&pen_blue);	//ペンの変更

			pDC->MoveTo(nCurX + BAR_WIDTH / 2, rc.bottom);
			pDC->LineTo(nCurX + BAR_WIDTH / 2, GRAPH_BOTTOM );

			pDC->SelectObject(oldp);	//ペンを元に戻す(必須)
		}
	}
}
#endif
			WriteTrandTerm( pDC, nCurX, i, dwEndIdx );

#if 0
			RECT rc;
			FILETIME ftStart;
			FILETIME ftEnd;
			LONGLONG llTime;


			SystemTimeToFileTime( &pDoc->m_TickInfo[i].sDt, &ftStart );
			if( pDoc->m_TickInfo[i].Tc < 70 ){
				SYSTEMTIME CurTime;
				GetLocalTime( &CurTime );
				SystemTimeToFileTime( &CurTime, &ftEnd );
			}else{
				SystemTimeToFileTime( &pDoc->m_TickInfo[i].eDt, &ftEnd );
			}

			llTime = (LONGLONG)(*(LONGLONG*)&ftEnd - *(LONGLONG*)&ftStart );
			rc.left = nCurX;
			rc.top = GRAPH_BOTTOM - ((DWORD)(llTime/10000000));
			rc.right = rc.left + BAR_WIDTH;
			rc.bottom = GRAPH_BOTTOM;

			CPen pen_frame(PS_SOLID, 1, RGB(0,0,0) );
			CPen* oldp = pDC->SelectObject(&pen_frame);	//ペンの変更

			pDC->Rectangle(&rc);

			pDC->SelectObject(oldp);
#endif
			RECT rc;

			rc.left = nCurX;
			rc.top = GRAPH_BOTTOM - ((DWORD)(pDoc->m_TickInfo[i].dwDirectChangeCnt));
			rc.right = rc.left + BAR_WIDTH;
			rc.bottom = GRAPH_BOTTOM;

			CPen pen_frame(PS_SOLID, 1, RGB(0,0,0) );
			CPen* oldp = pDC->SelectObject(&pen_frame);	//ペンの変更

			pDC->Rectangle(&rc);

			pDC->SelectObject(oldp);

			nCurX -= BAR_WIDTH + BAR_SPAN;

		}

		nCurX = GRAPH_LEFT + BAR_SPAN;

		for (int i = dwEndIdx; i <= (int)dwMaxRecords; i++) {
			//BandsWalkRange( pDC, nCurX, i );
#if 0
			BollingerSigmaTrans( pDC, m_ViewTickData[m_ViewTickData.size()-1-(i-dwEndIdx)].rc.left, i );
#endif			
			

			//	インジゲーターを表示する
			DrawIndicator( pDC, i, nCurX );
			//if(m_b13EMA != FALSE)		DrawEMALine(pDC, i, 13, nCurX);
			//if(m_b21EMA != FALSE)		DrawEMALine(pDC, i, 21, nCurX);
			//if(m_b20EMA != FALSE)		DrawEMALine(pDC, i, 20, nCurX);
			//if(m_b50EMA != FALSE)		DrawEMALine(pDC, i, 50, nCurX);
			//if(m_b200EMA != FALSE)		DrawEMALine(pDC, i, 200, nCurX);
			//if(m_bOscillator != FALSE)	StchasticOscillatorLine(pDC, i, nCurX);

			nCurX += BAR_WIDTH + BAR_SPAN;
		}
		//再描画
		//UpdateWindow();
	}

	if( pDoc->m_bCurRateMode != FALSE ){
		HWND hWnd = ::FindWindow("#32770", AW_WINDOW_NAME);
		if( hWnd != NULL ){
			::PostMessage( hWnd, WM_APP+1001, (WPARAM)pDoc->m_dwPair, (LPARAM)m_hWnd );
		}
	}
}

// CZeroGraphViewer 印刷


void CZeroGraphViewer::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CZeroGraphViewer::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CZeroGraphViewer::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CZeroGraphViewer::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

void CZeroGraphViewer::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CZeroGraphViewer::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CZeroGraphViewer 診断

#ifdef _DEBUG
void CZeroGraphViewer::AssertValid() const
{
	CView::AssertValid();
}

void CZeroGraphViewer::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCsvDataViewerDoc* CZeroGraphViewer::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCsvDataViewerDoc)));
	return (CCsvDataViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CZeroGraphViewer メッセージ ハンドラー


BOOL CZeroGraphViewer::PreTranslateMessage(MSG* pMsg)
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if (pMsg->message == WM_KEYDOWN) {

		switch (pMsg->wParam) {
		case 37:	//	左ボタン
			if( pDoc != NULL ){
				if (g_nGraphStartChange < (int)pDoc->m_TickInfo.size() ){
					int nOnePageBars = (int)(GRAPH_WIDTH) / (BAR_WIDTH + BAR_SPAN);
					g_nGraphStartChange += ( nOnePageBars - nOnePageBars/10 );
					if (g_nGraphStartChange > ( (int)pDoc->m_TickInfo.size() - (int)(GRAPH_WIDTH) / (BAR_WIDTH + BAR_SPAN)  ) ){
						g_nGraphStartChange = (int)pDoc->m_TickInfo.size() - (int)(GRAPH_WIDTH) / (BAR_WIDTH + BAR_SPAN);
					}

					Invalidate();
				}
			}
			break;
		case 39:	//	右ボタン
			if( g_nGraphStartChange > 0 ){
				int nOnePageBars = (int)(GRAPH_WIDTH) / (BAR_WIDTH + BAR_SPAN);

				g_nGraphStartChange -= ( nOnePageBars - nOnePageBars/10 );
				if (g_nGraphStartChange < 0){
					g_nGraphStartChange = 0;
				}

				Invalidate();
			}
			break;
		case 116:	//	F5
			Invalidate();

			break;
		default:
			break;
		}
	}

	return CView::PreTranslateMessage(pMsg);
}


void CZeroGraphViewer::On20EMA()
{
	m_b20EMA = !m_b20EMA;

	Invalidate();
}


void CZeroGraphViewer::On50EMA()
{
	m_b50EMA = !m_b50EMA;

	Invalidate();
}

void CZeroGraphViewer::On200EMA()
{
	m_b200EMA = !m_b200EMA;

	Invalidate();
}

void CZeroGraphViewer::OnStochasticOscillator()
{
	m_bOscillator = !m_bOscillator;

	Invalidate();
}




void CZeroGraphViewer::On13ema()
{
	m_b13EMA = !m_b13EMA;

	Invalidate();
}


void CZeroGraphViewer::On21ema()
{
	m_b21EMA = !m_b21EMA;

	Invalidate();
}


void CZeroGraphViewer::OnUpdate20ema(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_b20EMA);
}


void CZeroGraphViewer::OnUpdate50ema(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_b50EMA);
}


void CZeroGraphViewer::OnUpdate200ema(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_b200EMA);
}


void CZeroGraphViewer::OnUpdate13ema(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_b13EMA);
}


void CZeroGraphViewer::OnUpdate21ema(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_b21EMA);
}


afx_msg LRESULT CZeroGraphViewer::OnRegiterRateMessage(WPARAM wParam, LPARAM lParam)
{
	DWORD dwRate = (DWORD)lParam;
	TICK_INFO AddData;

	if( dwRate == 0 )	return 0;

	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return 0;

	DWORD dwEnd = pDoc->m_TickInfo.size() - 1;
	SYSTEMTIME systime;

	GetLocalTime(&systime);

	//	Tickを切り替える(初回もこっちに入れる
	if(pDoc->m_TickInfo.size() == 0 || pDoc->m_TickInfo[dwEnd].Tc >= 70 ) {
		if(pDoc->m_TickInfo.size() > 0) {
			memcpy(&pDoc->m_TickInfo[dwEnd].eDt, &systime, sizeof(SYSTEMTIME));

			//	最大数に達していたら、先頭を削除する
			if(pDoc->m_TickInfo.size() >= MAX_TICK_INFOCNT) {
				pDoc->m_TickInfo.erase(pDoc->m_TickInfo.begin());
			}
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

		pDoc->m_TickInfo.push_back(AddData);
	}
	//	最後のTickデータを更新する
	else {
		//	最高または最低を必要があれば更新
		if(pDoc->m_TickInfo[dwEnd].Hi < dwRate ) {
			pDoc->m_TickInfo[dwEnd].Hi = dwRate;
		}
		else if(pDoc->m_TickInfo[dwEnd].Lo > dwRate) {
			pDoc->m_TickInfo[dwEnd].Lo = dwRate;
		}

		//	最終は常に更新する
		if(pDoc->m_TickInfo[dwEnd].Cl != dwRate) {
			pDoc->m_TickInfo[dwEnd].Cl = dwRate;
		}

		pDoc->m_TickInfo[dwEnd].Tc++;

	}

	if( pDoc->m_TickInfo[dwEnd].Tc % 10 == 0 ){
		Invalidate();
	}

	return 0;
}


//	設定ファイルからIndicatorの設定を読み込む
//	設定ファイルは./Indicator_USDJPY.ini
void CZeroGraphViewer::ReadIndicatorSetting()
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_IndiSetting.clear();

	char cIniPath[1024];
	char* pFileName;

	GetModuleFileName( NULL, cIniPath, sizeof(cIniPath) );
	pFileName = (char*)_mbsrchr((unsigned char*)cIniPath, (unsigned int)'\\');

	wsprintf( pFileName, "\\Indicator_%s.ini", GetPairName(pDoc->m_dwPair) );

	//	一覧の取得
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	hFind = FindFirstFile( cIniPath, &wfd );

	if( hFind != INVALID_HANDLE_VALUE ){
		FindClose( hFind );

		INDICATOR_SETTING IS;
		char* pIniData;

		if( ( pIniData = (char*)LocalAlloc( LMEM_FIXED, wfd.nFileSizeLow )) != NULL) {
			GetPrivateProfileString( "INDICATOR_LIST", NULL, NULL, pIniData, wfd.nFileSizeLow, cIniPath );

			char* pCur = pIniData;

			while( *pCur != 0x00 ){
				DWORD dwIndiType = GetPrivateProfileInt( pCur, "TYPE", -1, cIniPath );
				
				if( dwIndiType != INDICATOR_TYPE_UNKNOWN ){
					BOOL bAdd = FALSE;
					memset( &IS, 0x00, sizeof(INDICATOR_SETTING) );
					IS.dwIndiType = dwIndiType;

					switch (dwIndiType) {
					case INDICATOR_TYPE_BB:
						IS.bb.dwSpan = GetPrivateProfileInt( pCur, "SPAN", 0, cIniPath );
						IS.bb.dwSigma = GetPrivateProfileInt( pCur, "SIGMA", 0, cIniPath );

						if( IS.bb.dwSpan > 0 && IS.bb.dwSigma > 0 ){
							bAdd = TRUE;
						}
						break;
					case INDICATOR_TYPE_MA:
						IS.ma.dwSpan = GetPrivateProfileInt( pCur, "SPAN", 0, cIniPath );
						IS.ma.color = GetPrivateProfileInt( pCur, "COLOR", 0, cIniPath );

						if( IS.ma.dwSpan > 0 ){
							bAdd = TRUE;
						}
						break;
					case INDICATOR_TYPE_ADX:
						IS.adx.dwSpan = GetPrivateProfileInt( pCur, "SPAN", 0, cIniPath );

						if( IS.adx.dwSpan > 0 ){
							bAdd = TRUE;
						}
						break;
					case INDICATOR_TYPE_ICHIMOKU:
						bAdd = TRUE;
						break;
					case INDICATOR_TYPE_RSI:
						IS.rsi.dwSpan = GetPrivateProfileInt( pCur, "SPAN", 0, cIniPath );

						if( IS.rsi.dwSpan > 0 ){
							bAdd = TRUE;
						}
						break;
					case INDICATOR_TYPE_SO:
						IS.so.dwKSpan = GetPrivateProfileInt( pCur, "KSPAN", 0, cIniPath );
						IS.so.dwDSpan = GetPrivateProfileInt( pCur, "DSPAN", 0, cIniPath );
						IS.so.dwSSpan = GetPrivateProfileInt( pCur, "SSPAN", 0, cIniPath );

						if( IS.so.dwKSpan > 0 && IS.so.dwDSpan > 0 ){
							bAdd = TRUE;
						}
						break;
					case INDICATOR_TYPE_MACD:
						IS.macd.dwSSpan = GetPrivateProfileInt( pCur, "SSPAN", 0, cIniPath );
						IS.macd.dwLSpan = GetPrivateProfileInt( pCur, "LSPAN", 0, cIniPath );
						IS.macd.dwSignal = GetPrivateProfileInt( pCur, "SIGNAL", 0, cIniPath );

						if( IS.macd.dwSSpan > 0 && IS.macd.dwLSpan > 0 && IS.macd.dwSignal > 0 ){
							bAdd = TRUE;
						}
						break;
					default:
						break;
					}

					GetPrivateProfileString( pCur, "DISPLAYNAME", "", IS.DisplayName, sizeof(IS.DisplayName), cIniPath );

					if( bAdd != FALSE ){
						m_IndiSetting.push_back(IS);
					}
				}
				pCur += lstrlen(pCur)+1;
			}

			LocalFree( pIniData );
		}
	}

}

void CZeroGraphViewer::WriteIndicatorSetting()
{

	if( m_IndiSetting.size() == 0 )	return;

	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	char cIniPath[1024];
	char* pFileName;

	GetModuleFileName( NULL, cIniPath, sizeof(cIniPath) );
	pFileName = (char*)_mbsrchr((unsigned char*)cIniPath, (unsigned int)'\\');

	wsprintf( pFileName, "\\Indicator_%s.ini", GetPairName(pDoc->m_dwPair) );
	
	WritePrivateProfileString( NULL, NULL, NULL, cIniPath );

	vector<INDICATOR_SETTING>::iterator itr = m_IndiSetting.begin();

	for( ;itr != m_IndiSetting.end(); itr++ ){
		char cSection[64] = "";
		char cData[32];

		lstrcpy( cSection, (*itr).DisplayName );

		switch ( (*itr).dwIndiType ) {
		case INDICATOR_TYPE_BB:
			wsprintf( cData, "%d", (*itr).bb.dwSpan );
			WritePrivateProfileString( cSection, "SPAN", cData, cIniPath );
			wsprintf( cData, "%d", (*itr).bb.dwSigma );
			WritePrivateProfileString( cSection, "SIGMA", cData, cIniPath );
			break;
		case INDICATOR_TYPE_MA:
			wsprintf( cData, "%d", (*itr).ma.dwSpan );
			WritePrivateProfileString( cSection, "SPAN", cData, cIniPath );
			wsprintf( cData, "%d", (*itr).ma.color );
			WritePrivateProfileString( cSection, "COLOR", cData, cIniPath );
			break;
		case INDICATOR_TYPE_ADX:
			wsprintf( cData, "%d", (*itr).adx.dwSpan );
			WritePrivateProfileString( cSection, "SPAN", cData, cIniPath );
			break;
		case INDICATOR_TYPE_ICHIMOKU:
			break;
		case INDICATOR_TYPE_RSI:
			wsprintf( cData, "%d", (*itr).rsi.dwSpan );
			WritePrivateProfileString( cSection, "SPAN", cData, cIniPath );
			break;
		case INDICATOR_TYPE_SO:
			wsprintf( cData, "%d", (*itr).so.dwKSpan );
			WritePrivateProfileString( cSection, "KSPAN", cData, cIniPath );
			wsprintf( cData, "%d", (*itr).so.dwDSpan );
			WritePrivateProfileString( cSection, "DSPAN", cData, cIniPath );
			wsprintf( cData, "%d", (*itr).so.dwSSpan );
			WritePrivateProfileString( cSection, "SSPAN", cData, cIniPath );
			break;
		case INDICATOR_TYPE_MACD:
			wsprintf( cData, "%d", (*itr).macd.dwSSpan );
			WritePrivateProfileString( cSection, "SSPAN", cData, cIniPath );
			wsprintf( cData, "%d", (*itr).macd.dwLSpan );
			WritePrivateProfileString( cSection, "LSPAN", cData, cIniPath );
			wsprintf( cData, "%d", (*itr).macd.dwSignal );
			WritePrivateProfileString( cSection, "SIGNAL", cData, cIniPath );
			break;
		default:
			break;
		}

		if( cSection[0] != 0x00 ){
			WritePrivateProfileString( "INDICATOR_LIST", cSection, "1", cIniPath );
			wsprintf( cData, "%d", (*itr).dwIndiType );
			WritePrivateProfileString( cSection, "TYPE", cData, cIniPath );
			WritePrivateProfileString( cSection, "DISPLAYNAME", (*itr).DisplayName, cIniPath );
		}
	}
}

void CZeroGraphViewer::DeleteIndicatorSetting( DWORD dwDelId )
{
	vector<INDICATOR_SETTING>::iterator itr = m_IndiSetting.begin();

	for( ;itr != m_IndiSetting.end(); itr++ ){
		if( (*itr).dwSettingId == dwDelId ){
			m_IndiSetting.erase(itr);

			Invalidate();

			break;
		}
	}
}

void CZeroGraphViewer::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	vector<VIEW_TICK_DATA>::iterator itr = m_ViewTickData.begin();

	for( ;itr != m_ViewTickData.end(); itr++ ){
		if( (*itr).rc.left <= point.x && point.x <= (*itr).rc.right ){
			//if( (*itr).rc.top - 5 <= point.y && point.y <= (*itr).rc.bottom + 5 )
			{
				g_pMainFrame->OpenTickRateGraph( &(*itr) );

			}
		}
	}
	CView::OnLButtonDblClk(nFlags, point);
}

void CZeroGraphViewer::ViewShiftSelectTrade( SYSTEMTIME* pOpenTime, SYSTEMTIME* pCloseTime )
{
	CCsvDataViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	FILETIME ft_trade;
	LONGLONG llTrade_st;
	LONGLONG llTrade_end;

	int nOnePageBars = (int)(GRAPH_WIDTH) / (BAR_WIDTH + BAR_SPAN);
	//g_nGraphStartChange += ( nOnePageBars - nOnePageBars/10 );
	int nPosition;// = (pDoc->m_TickInfo.size() *  (BAR_WIDTH + BAR_SPAN));

	SystemTimeToFileTime( pCloseTime, &ft_trade);
	llTrade_end = *(LONGLONG*)&ft_trade;

	SystemTimeToFileTime( pOpenTime, &ft_trade);
	llTrade_st = *(LONGLONG*)&ft_trade;

	for( DWORD i = 0; i < pDoc->m_TickInfo.size(); i++ ){
		FILETIME ft_tick;
		LONGLONG llTick;

		SystemTimeToFileTime( &pDoc->m_TickInfo[i].sDt, &ft_tick);
		llTick = *(LONGLONG*)&ft_tick;

		if( llTrade_st <= llTick && llTick <= llTrade_end ) {
			nPosition = (int)( pDoc->m_TickInfo.size() - i );
			nPosition -= nOnePageBars/2;

			if( nPosition < 0 ){
				nPosition = 0;
			}else if( nPosition + nOnePageBars/2 > (int)pDoc->m_TickInfo.size() ){
				nPosition = (int)pDoc->m_TickInfo.size();
			}

			g_nGraphStartChange = nPosition;

			Invalidate();

			break;
		}
	}

	return;
}


void CZeroGraphViewer::SetIndicatorDisplayName( INDICATOR_SETTING* pIndiSetting ){
	if(pIndiSetting->dwIndiType == INDICATOR_TYPE_BB) {
		lstrcpy(pIndiSetting->DisplayName, "Bollinger Bands");
	}
	else if(pIndiSetting->dwIndiType == INDICATOR_TYPE_MA) {
		wsprintf(pIndiSetting->DisplayName, "Moving Average(%d)", pIndiSetting->ma.dwSpan);
	}
	else if(pIndiSetting->dwIndiType == INDICATOR_TYPE_ADX) {
		lstrcpy(pIndiSetting->DisplayName, "ADX");
	}
	else if(pIndiSetting->dwIndiType == INDICATOR_TYPE_ICHIMOKU) {
		lstrcpy(pIndiSetting->DisplayName, "一目均衡表");
	}
	else if(pIndiSetting->dwIndiType == INDICATOR_TYPE_RSI) {
		lstrcpy(pIndiSetting->DisplayName, "RSI");
	}
	else if(pIndiSetting->dwIndiType == INDICATOR_TYPE_SO) {
		lstrcpy(pIndiSetting->DisplayName, "SO");
	}
	else if(pIndiSetting->dwIndiType == INDICATOR_TYPE_MACD) {
		lstrcpy(pIndiSetting->DisplayName, "MACD");
	}
}