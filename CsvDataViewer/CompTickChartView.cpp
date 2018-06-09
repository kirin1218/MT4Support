// CompTickChartView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "CompTickChartView.h"
#include "CompTickChartDoc.h"

// CCompTickChartView

IMPLEMENT_DYNCREATE(CCompTickChartView, CView)

CCompTickChartView::CCompTickChartView()
{

}

CCompTickChartView::~CCompTickChartView()
{
}

BEGIN_MESSAGE_MAP(CCompTickChartView, CView)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CCompTickChartView 描画
void CCompTickChartView::DrawGraph(CDC* pDC, BOOL bDrawLeft )
{
	CCompTickChartDoc* pDoc = (CCompTickChartDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CPen* oldp;

	CPen* pPenFrame;
	RECT* pRcG;
	DWORD dwGraphWidth;
	int nLGraphStartChange;
	DWORD dwSelSId;
	DWORD dwSelEId;


	if( m_bSelectLeft == bDrawLeft ){
		pPenFrame = &m_pen_frame_red;
	}else{
		pPenFrame = &m_pen_frame;
	}

	if( bDrawLeft != FALSE ){
		pRcG = &m_rcLeftGraph;

		pRcG->left = m_LGraphLeft;
		pRcG->top = m_GraphTop;
		pRcG->right = m_LGraphRight;
		pRcG->bottom = m_GraphBottom;

		dwGraphWidth = m_LGraphWidth;

		nLGraphStartChange = m_nLGraphStartChange;

		dwSelSId = m_dwSelLSId;
		dwSelEId = m_dwSelLEId;
	}else{
		pRcG = &m_rcRightGraph;

		pRcG->left = m_RGraphLeft;
		pRcG->top = m_GraphTop;
		pRcG->right = m_RGraphRight;
		pRcG->bottom = m_GraphBottom;

		dwGraphWidth = m_RGraphWidth;

		nLGraphStartChange = m_nRGraphStartChange;

		dwSelSId = m_dwSelRSId;
		dwSelEId = m_dwSelREId;
	}

	//******************************************************************
	//	グラフの外枠を描画
	//******************************************************************
	oldp = pDC->SelectObject(pPenFrame);	//ペンの変更

	//	外枠を描画


	pDC->MoveTo(pRcG->left, pRcG->top);pDC->LineTo(pRcG->right, pRcG->top);
	pDC->MoveTo(pRcG->right, pRcG->top);pDC->LineTo(pRcG->right, pRcG->bottom);
	pDC->MoveTo(pRcG->right, pRcG->bottom);pDC->LineTo(pRcG->left, pRcG->bottom);
	pDC->MoveTo(pRcG->left, pRcG->bottom);pDC->LineTo(pRcG->left, pRcG->top);


	pDC->SelectObject(oldp);	//ペンを元に戻す(必須)

	//	表示できるバーの数を計算する
			//	1個の表示に必要な幅・・・m_BarWidth+m_BarSpan
	DWORD dwViewBars = (dwGraphWidth) / (m_BarWidth + m_BarSpan);

	DWORD dwMaxRecords;// = sizeof(g_tickinfo) / sizeof(g_tickinfo[0]);
	dwMaxRecords = pDoc->m_TickInfo.size();

	if((int)dwMaxRecords > nLGraphStartChange && dwMaxRecords - nLGraphStartChange > (dwGraphWidth) / (m_BarWidth + m_BarSpan)) {
		dwMaxRecords -= nLGraphStartChange;
	}
	else if((dwGraphWidth) / (m_BarWidth + m_BarSpan) > dwMaxRecords) {
		;;
	}
	else {
		dwMaxRecords = (dwGraphWidth) / (m_BarWidth + m_BarSpan);
	}

	DWORD dwEndIdx = (dwViewBars < dwMaxRecords) ? (dwMaxRecords - dwViewBars - 1) : 0;

	if((dwGraphWidth) / (m_BarWidth + m_BarSpan) > dwMaxRecords) {
		dwMaxRecords -= 1;
	}
	else {
		dwMaxRecords -= 2;	//	-2ってなんぞ？思い出せない
	}
	//	最大値と最小値を検索する
	DWORD dwMaxVal = 0;
	DWORD dwMinVal = 777777;	//	メンバーにしてみました。

	for(int i = dwMaxRecords; i >= (int)dwEndIdx; i--) {
		DWORD Hi = EValFractionCut(pDoc->m_TickInfo[i].Hi, pDoc->m_dwPair);
		DWORD Lo = EValFractionCut(pDoc->m_TickInfo[i].Lo, pDoc->m_dwPair);

		if(dwMaxVal < Hi)		dwMaxVal = Hi;
		if(dwMinVal > Lo)		dwMinVal = Lo;
	}

	//	見た目上上下にゆとりを持たせる
	dwMaxVal += 100;
	dwMinVal -= 100;

	m_HeightPerEVal = (double)m_GraphHeight / (double)(dwMaxVal - dwMinVal);


	//******************************************************************
	//	区切りの数値に点線を引く
	//******************************************************************
	DWORD dwLineY = (dwMinVal + 100) - (dwMinVal % 100);

	oldp = pDC->SelectObject(&m_pen_dot_black);	//ペンの変更

	for(DWORD i = dwLineY; i < dwMaxVal; i += 100) {
		char wLine[32];
		DWORD dwY = (DWORD)(pRcG->bottom - (i - dwMinVal)*m_HeightPerEVal);

		pDC->MoveTo(pRcG->left, dwY);
		pDC->LineTo(pRcG->right, dwY);

		if(pDoc->m_dwPair == PAIR_EURUSD || pDoc->m_dwPair == PAIR_GBPUSD || pDoc->m_dwPair == PAIR_USDCHF) {
			sprintf_s(wLine, "%.4f", EVal2Rate(i, pDoc->m_dwPair));
		}
		else {
			sprintf_s(wLine, "%.2f", EVal2Rate(i, pDoc->m_dwPair));
		}
		TextOutA(pDC->m_hDC, pRcG->right + 3, dwY - 2, wLine, lstrlen(wLine));
	}

	pDC->SelectObject(oldp);	//ペンを元に戻す(必須)


	char cDate[32];
	wsprintf(cDate, "%02d/%02d", pDoc->m_TickInfo[dwEndIdx].sDt.wMonth, pDoc->m_TickInfo[dwEndIdx].sDt.wDay);
	TextOutA(pDC->m_hDC, pRcG->left + 2, pRcG->top + 2, cDate, lstrlen(cDate));

	//*******************************************************************
	int nCurX = pRcG->right - (m_BarSpan + m_BarWidth);
	CRect rc;
	int nPrevHour = -1;
	int nPrevMin = -1;


	for(int i = dwMaxRecords; i >= (int)dwEndIdx; i--) {
		DWORD open = EValFractionCut(pDoc->m_TickInfo[i].Op, pDoc->m_dwPair);
		DWORD Hi = EValFractionCut(pDoc->m_TickInfo[i].Hi, pDoc->m_dwPair);
		DWORD Lo = EValFractionCut(pDoc->m_TickInfo[i].Lo, pDoc->m_dwPair);
		DWORD close = EValFractionCut(pDoc->m_TickInfo[i].Cl, pDoc->m_dwPair);

		//******************************************************************
		//	10分おきに時刻を表示
		//******************************************************************
		if((nPrevHour == -1 && nPrevMin == -1)
			|| (nPrevHour > pDoc->m_TickInfo[i].sDt.wHour)
			|| (nPrevHour == pDoc->m_TickInfo[i].sDt.wHour && nPrevMin > pDoc->m_TickInfo[i].sDt.wMinute)
			) {
			if(pDoc->m_TickInfo[i].sDt.wMinute % 10 == 0) {
				char cHour[32];
				char cMin[32];

				wsprintf(cHour, "%02d", pDoc->m_TickInfo[i].sDt.wHour);
				TextOutA(pDC->m_hDC, nCurX, pRcG->bottom + 10, cHour, sizeof("XX") - 1);

				wsprintf(cMin, "%02d", pDoc->m_TickInfo[i].sDt.wMinute);
				TextOutA(pDC->m_hDC, nCurX, pRcG->bottom + 30, cMin, sizeof("XX") - 1);

				nPrevHour = pDoc->m_TickInfo[i].sDt.wHour;
				nPrevMin = pDoc->m_TickInfo[i].sDt.wMinute;
			}
		}
		//******************************************************************

		//******************************************************************
		//	ローソクの描画
		//******************************************************************
		HBRUSH hBrush_old = NULL;

		if(close == open) {
			oldp = pDC->SelectObject(&m_pen_black);	//ペンの変更
		}
		else if(close > open) {
			oldp = pDC->SelectObject(&m_pen_red);	//ペンの変更
			hBrush_old = (HBRUSH)pDC->SelectObject(m_hbrush_red);
		}
		else/* if ( close < open ) */
		{
			oldp = pDC->SelectObject(&m_pen_blue);	//ペンの変更
			hBrush_old = (HBRUSH)pDC->SelectObject(m_hbrush_blue);
		}

		//	実体の描画
		rc.left = nCurX;
		rc.top = pRcG->bottom - (DWORD)((close - dwMinVal)*m_HeightPerEVal);
		rc.right = rc.left + m_BarWidth;
		rc.bottom = pRcG->bottom - (DWORD)((open - dwMinVal)*m_HeightPerEVal);
		if(rc.top == rc.bottom)	rc.bottom += 1;

		pDC->Rectangle(&rc);

		if(hBrush_old != NULL)	pDC->SelectObject(hBrush_old);	//ペンを元に戻す(必須)
		pDC->SelectObject(oldp);	//ペンを元に戻す(必須)

		//	ヒゲの描画
		oldp = pDC->SelectObject(&m_pen_black);	//ペンの変更

		pDC->MoveTo(nCurX + m_BarWidth / 2, rc.top);
		pDC->LineTo(nCurX + m_BarWidth / 2, pRcG->bottom - (DWORD)((Hi - dwMinVal)*m_HeightPerEVal));
		pDC->MoveTo(nCurX + m_BarWidth / 2, rc.bottom);
		pDC->LineTo(nCurX + m_BarWidth / 2, pRcG->bottom - (DWORD)((Lo - dwMinVal)*m_HeightPerEVal));

		pDC->SelectObject(oldp);	//ペンを元に戻す(必須)
		//******************************************************************

		if(dwSelSId > 0 && dwSelEId > 0) {
			if(dwSelSId == i) {
				DWORD dwMin = 7777777;
				DWORD dwMax = 0;

				for(DWORD j = dwSelSId; j <= dwSelEId; j++) {
					DWORD Hi = pDoc->m_TickInfo[j].Hi;
					DWORD Lo = pDoc->m_TickInfo[j].Lo;

					if(dwMax < Hi) {
						dwMax = Hi;
					}
					if(dwMin > Lo) {
						dwMin = Lo;
					}
				}

				CPen* oldp = pDC->SelectObject(&m_pen_red);	//ペンの変更

				int nBottom = m_GraphBottom - (int)((dwMin - dwMinVal)*m_HeightPerEVal);
				int nTop = m_GraphBottom - (int)((dwMax - dwMinVal)*m_HeightPerEVal);

				//	外枠を描画
				pDC->MoveTo(nCurX, nTop);
				pDC->LineTo(nCurX + (dwSelEId - dwSelSId + 1)*(m_BarWidth + m_BarSpan), nTop);
				pDC->MoveTo(nCurX + (dwSelEId - dwSelSId + 1)*(m_BarWidth + m_BarSpan), nTop);
				pDC->LineTo(nCurX + (dwSelEId - dwSelSId + 1)*(m_BarWidth + m_BarSpan), nBottom);
				pDC->MoveTo(nCurX + (dwSelEId - dwSelSId + 1)*(m_BarWidth + m_BarSpan), nBottom);
				pDC->LineTo(nCurX, nBottom);
				pDC->MoveTo(nCurX, nBottom);
				pDC->LineTo(nCurX, nTop);

				pDC->SelectObject(oldp);	//ペンを元に戻す(必須)
			}
		}

		nCurX -= m_BarWidth + m_BarSpan;


	}
}


void CCompTickChartView::DoDrawMem(CDC* pDC)
{
	CCompTickChartDoc* pDoc = (CCompTickChartDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->GetPathName().IsEmpty() == TRUE)
		return;

	CRect rc;
    GetClientRect(&rc);
	pDC->FillSolidRect(&rc, (COLORREF)0x00ffffff);

	//	描画処理の開始
	SetGraphParameter();
	
	if( pDoc->m_TickInfo.size() > 0 ){
		DrawGraph( pDC, TRUE );

		DrawGraph( pDC, FALSE );		
	}
}
void CCompTickChartView::OnDraw(CDC* pDC)
{
    CDC dcMem;
    CBitmap bmpMem;
    CRect rc;
    GetClientRect(&rc);

    dcMem.CreateCompatibleDC(pDC);
    bmpMem.CreateCompatibleBitmap(pDC, rc.right, rc.bottom);
    CBitmap* pOldBmp = dcMem.SelectObject(&bmpMem);

	DoDrawMem(&dcMem);
	//DoDrawMem(pDC);

    pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, SRCCOPY);
}


// CCompTickChartView 診断

#ifdef _DEBUG
void CCompTickChartView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCompTickChartView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCompTickChartView メッセージ ハンドラー


LRESULT CCompTickChartView::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	return CView::DefWindowProc(message, wParam, lParam);
}


BOOL CCompTickChartView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	m_pen_frame.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	m_pen_frame_red.CreatePen(PS_SOLID, 2, RGB(0xFF, 0, 0));
	m_pen_black.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	m_pen_dot_black.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	m_pen_red.CreatePen(PS_SOLID, 1, RGB(0xFF, 0, 0));
	m_pen_blue.CreatePen(PS_SOLID, 1, RGB(0, 0, 0xFF));
	m_pen_pink.CreatePen(PS_SOLID, 1, RGB(0xFF, 0, 0xFF));
	m_pen_orange.CreatePen(PS_SOLID, 1, RGB(255, 153, 0));
	m_pen_green.CreatePen(PS_SOLID, 1, RGB(0, 128, 0));

	return CView::PreCreateWindow(cs);
}


void CCompTickChartView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
}


void CCompTickChartView::ViewShiftSelectCmpSTick( COMP_STICK_LIST* pCompSTickInfo )
{
	CCompTickChartDoc* pDoc = (CCompTickChartDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	int nOnePageBars;
	int nPosition;
	DWORD dwCenter;

	nOnePageBars = (int)(m_LGraphWidth) / (m_BarWidth + m_BarSpan);
	dwCenter = pCompSTickInfo->csi1.dwSId + ((pCompSTickInfo->csi1.dwEId - pCompSTickInfo->csi1.dwSId)/2);

	nPosition = (int)(pDoc->m_TickInfo.size() - dwCenter);
	nPosition -= nOnePageBars / 2;

	if(nPosition < 0) {
		nPosition = 0;
	}
	else if(nPosition + nOnePageBars / 2 > (int)pDoc->m_TickInfo.size()) {
		nPosition = (int)pDoc->m_TickInfo.size();
	}

	m_nLGraphStartChange = nPosition;
	m_dwSelLSId = pCompSTickInfo->csi1.dwSId;
	m_dwSelLEId = pCompSTickInfo->csi1.dwEId;
	
	nOnePageBars = (int)(m_RGraphWidth) / (m_BarWidth + m_BarSpan);
	dwCenter = pCompSTickInfo->csi2.dwSId + ((pCompSTickInfo->csi2.dwEId - pCompSTickInfo->csi2.dwSId)/2);

	nPosition = (int)(pDoc->m_TickInfo.size() - dwCenter);
	nPosition -= nOnePageBars / 2;

	if(nPosition < 0) {
		nPosition = 0;
	}
	else if(nPosition + nOnePageBars / 2 > (int)pDoc->m_TickInfo.size()) {
		nPosition = (int)pDoc->m_TickInfo.size();
	}

	m_nRGraphStartChange = nPosition;
	m_dwSelRSId = pCompSTickInfo->csi2.dwSId;
	m_dwSelREId = pCompSTickInfo->csi2.dwEId;

	Invalidate();
	
	return;
}

void CCompTickChartView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	if( m_bSelectLeft != FALSE ){
		if( m_rcRightGraph.left < point.x && point.x < m_rcRightGraph.right 
		 && m_rcRightGraph.top < point.y && point.y < m_rcRightGraph.bottom 
		){
			m_bSelectLeft = FALSE;

			Invalidate();
		}
	}else{
		if( m_rcLeftGraph.left < point.x && point.x < m_rcLeftGraph.right 
		 && m_rcLeftGraph.top < point.y && point.y < m_rcLeftGraph.bottom 
		){
			m_bSelectLeft = TRUE;

			Invalidate();
		}
	}
	CView::OnLButtonDown(nFlags, point);
}


BOOL CCompTickChartView::PreTranslateMessage(MSG* pMsg)
{
	CCompTickChartDoc* pDoc = (CCompTickChartDoc*)GetDocument();
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if (pMsg->message == WM_KEYDOWN) {

		switch (pMsg->wParam) {
		case 37:	//	左ボタン
		case 39:	//	右ボタン
		{
			int* pGraphtStartChange;
			DWORD dwGraphWidth;

			if( m_bSelectLeft != FALSE ){
				pGraphtStartChange = &m_nLGraphStartChange;
				dwGraphWidth = m_LGraphWidth;
			}else{
				pGraphtStartChange = &m_nRGraphStartChange;
				dwGraphWidth = m_RGraphWidth;
			}
			if( pMsg->wParam == 37 ){
				if( pDoc != NULL ){
					if (*pGraphtStartChange < (int)pDoc->m_TickInfo.size() ){
						int nOnePageBars = (int)(dwGraphWidth) / (m_BarWidth + m_BarSpan);
						*pGraphtStartChange += ( nOnePageBars - nOnePageBars/10 );
						if (*pGraphtStartChange > ( (int)pDoc->m_TickInfo.size() - (int)(dwGraphWidth) / (int)(m_BarWidth + m_BarSpan)  ) ){
							*pGraphtStartChange = (int)pDoc->m_TickInfo.size() - (int)(dwGraphWidth) / (int)(m_BarWidth + m_BarSpan);
						}

						Invalidate();
					}
				}
				break;
			}else if( pMsg->wParam == 39 ){
				if( *pGraphtStartChange > 0 ){
					int nOnePageBars = (int)(dwGraphWidth) / (m_BarWidth + m_BarSpan);

					*pGraphtStartChange -= ( nOnePageBars - nOnePageBars/10 );
					if (*pGraphtStartChange < 0){
						*pGraphtStartChange = 0;
					}

					Invalidate();
				}
				break;
			}
		}
		case 116:	//	F5
			Invalidate();

			break;
		default:
			break;
		}
	}

	return CView::PreTranslateMessage(pMsg);
}
