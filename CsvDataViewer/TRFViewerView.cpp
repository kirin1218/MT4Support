// TRFViewerView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "TRFViewerView.h"
#include "TRFViewerDoc.h"

#include "MainFrm.h"

extern CMainFrame* g_pMainFrame;

// CTRFViewerView

IMPLEMENT_DYNCREATE(CTRFViewerView, CView)

CTRFViewerView::CTRFViewerView()
{

}

CTRFViewerView::~CTRFViewerView()
{
}

BEGIN_MESSAGE_MAP(CTRFViewerView, CView)
	ON_MESSAGE(WM_APP + 1014, &CTRFViewerView::OnReloadRateGraphRequest)
END_MESSAGE_MAP()

//
//#define BAR_WIDTH			(5)
//#define m_BarSpan			(2)
//double g_TRFHeightPerPixel = 1;
//#define BAR_HEIGHT_PER_PIXEL	(g_TRFHeightPerPixel)
//
//#define GRAPH_MARGIN		(50)
//#define GRAPH_TOP			(GRAPH_MARGIN)
//#define GRAPH_VIEW_TOP		(GRAPH_TOP+GRAPH_MARGIN)
//#define GRAPH_LEFT			(GRAPH_MARGIN)
//#define GRAPH_SCALE_PERWSIZE	(90)	//	�N���C�A���g�̈�̂����������O���t�Ŏg�p���邩
//
//DWORD g_dwTRFGraphBottom = 500;
//DWORD g_dwTRFGraphRight = 800;
//#define GRAPH_BOTTOM		(g_dwTRFGraphBottom)
//#define GRAPH_VIEW_BOTTOM	(GRAPH_BOTTOM+GRAPH_MARGIN)
//#define GRAPH_RIGHT			(g_dwTRFGraphRight)
//#define GRAPH_HEIGHT		(GRAPH_BOTTOM-GRAPH_TOP)
//#define GRAPH_WIDTH			(GRAPH_RIGHT-GRAPH_LEFT)

// CTRFViewerView �`��

void CTRFViewerView::OnDraw(CDC* pDC)
{
	CTRFViewerDoc* pDoc = (CTRFViewerDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->GetPathName().IsEmpty() == TRUE)
		return;

	//	�`�揈���̊J�n
	SetGraphParameter();

	//g_dwTRFGraphBottom = ( cRc.bottom * m_dwGraphScalePerWSize /100 );
	//g_dwTRFGraphRight = m_GraphLeft + (cRc.right - m_GraphMargin * 2);

	CPen pen_frame(PS_SOLID, 2, RGB(0, 0, 0));
	CPen pen_black(PS_SOLID, 1, RGB(0, 0, 0));
//	CPen pen_dot_black(PS_DOT, 1, RGB(0, 0, 0));
//	CPen pen_red(PS_SOLID, 1, RGB(0xFF, 0, 0));
//	CPen pen_blue(PS_SOLID, 1, RGB(0, 0, 0xFF));

//	HBRUSH hbrush_black = CreateSolidBrush(RGB(0, 0, 0));
//	HBRUSH hbrush_red = CreateSolidBrush(RGB(0xFF, 0, 0));
//	HBRUSH hbrush_blue = CreateSolidBrush(RGB(0, 0, 0xFF));
//	HBRUSH hBrush_old;
	CPen* oldp;

	//******************************************************************
	//	�O���t�̊O�g��`��
	//******************************************************************
	oldp = pDC->SelectObject(&m_pen_frame);	//�y���̕ύX

												//	�O�g��`��
	pDC->MoveTo(m_GraphLeft, m_GraphTop);pDC->LineTo(m_GraphRight, m_GraphTop);
	pDC->MoveTo(m_GraphRight, m_GraphTop);pDC->LineTo(m_GraphRight, m_GraphBottom);
	pDC->MoveTo(m_GraphRight, m_GraphBottom);pDC->LineTo(m_GraphLeft, m_GraphBottom);
	pDC->MoveTo(m_GraphLeft, m_GraphBottom);pDC->LineTo(m_GraphLeft, m_GraphTop);

	pDC->SelectObject(oldp);	//�y�������ɖ߂�(�K�{)

	//******************************************************************


	if( pDoc->m_vRateList.size() > 0 ){
		//	�\���ł���o�[�̐����v�Z����
		//	1�̕\���ɕK�v�ȕ��E�E�Em_BarWidth+m_BarSpan
		DWORD dwPntSpan = m_GraphWidth / (pDoc->m_vRateList.size()+2);

		DWORD dwMaxRecords;// = sizeof(g_tickinfo) / sizeof(g_tickinfo[0]);
		dwMaxRecords = pDoc->m_vRateList.size();
		DWORD dwEndIdx = pDoc->m_vRateList.size();
		DWORD dwMaxVal =0;

		DWORD Hi,Lo,Op,Cl;

		m_dwMinVal = 777777;
		for (DWORD i = 0; i < pDoc->m_vRateList.size(); i++ ) {
			if (dwMaxVal < pDoc->m_vRateList[i] )		dwMaxVal = pDoc->m_vRateList[i];
			if (m_dwMinVal > pDoc->m_vRateList[i])	m_dwMinVal = pDoc->m_vRateList[i];
		}

		Hi = dwMaxVal;
		Lo = m_dwMinVal;
		Op = pDoc->m_vRateList[0];
		Cl = pDoc->m_vRateList[pDoc->m_vRateList.size()-1];

		char cDate[32];
		wsprintf(cDate, "High:%d", Hi );
		TextOutA(pDC->m_hDC, m_GraphLeft+2, m_GraphTop + 2, cDate, lstrlen(cDate) );

		wsprintf(cDate, "Low :%d", Lo );
		TextOutA(pDC->m_hDC, m_GraphLeft+2, m_GraphTop + 2 + 30, cDate, lstrlen(cDate) );

		wsprintf(cDate, "Open :%d", Op );
		TextOutA(pDC->m_hDC, m_GraphLeft+(m_GraphRight-m_GraphLeft)/2, m_GraphTop + 2, cDate, lstrlen(cDate) );

		wsprintf(cDate, "Close:%d", Cl );
		TextOutA(pDC->m_hDC, m_GraphLeft+(m_GraphRight-m_GraphLeft)/2, m_GraphTop + 2 + 30, cDate, lstrlen(cDate) );

		//	�����ڏ�㉺�ɂ�Ƃ����������
		dwMaxVal += 100;
		m_dwMinVal -= 100;

		m_HeightPerEVal = (double)m_GraphHeight / (double)(dwMaxVal - m_dwMinVal);
		//g_TRFHeightPerPixel *= 2;


		//******************************************************************
		//	��؂�̐��l�ɓ_��������
		//******************************************************************
		//DWORD dwLineY = (m_dwMinVal+100) - (m_dwMinVal%100);

		//oldp = pDC->SelectObject(&m_pen_dot_black);	//�y���̕ύX

		//for (DWORD i = dwLineY; i < dwMaxVal; i += 100 ) {
		//	char wLine[32];
		//	DWORD dwY = (DWORD)(m_GraphBottom - (i - m_dwMinVal)*m_dwGraphScalePerWSize);

		//	pDC->MoveTo(m_GraphLeft, dwY);
		//	pDC->LineTo(m_GraphRight, dwY);

		//	if (pDoc->m_dwPair == PAIR_EURUSD) {
		//		sprintf_s(wLine, "%.4f", EVal2Rate( i, pDoc->m_dwPair ) );
		//	}else{
		//		sprintf_s(wLine, "%.2f", EVal2Rate( i, pDoc->m_dwPair ) );
		//	}
		//	TextOutA(pDC->m_hDC, m_GraphRight + 3, dwY - 2, wLine, lstrlen(wLine) );
		//}

		//pDC->SelectObject(oldp);	//�y�������ɖ߂�(�K�{)


		//char cDate[32];
		//wsprintf(cDate, "%02d/%02d", pDoc->m_TickInfo[dwEndIdx].sDt.wMonth, pDoc->m_TickInfo[dwEndIdx].sDt.wDay );
		//TextOutA(pDC->m_hDC, m_GraphLeft+2, m_GraphTop + 2, cDate, lstrlen(cDate) );

		//*******************************************************************
		int nCurX = m_GraphLeft + dwPntSpan;
		CRect rc;
		DWORD dwMaxRateCnt = pDoc->m_vRateList.size();


		for (DWORD i = 0; i < dwMaxRateCnt-1; i++ ) {
			//******************************************************************
			//	10�������Ɏ�����\��
			//******************************************************************
			//if ( ( nPrevHour == -1 && nPrevMin == -1 )
			//|| (nPrevHour > pDoc->m_TickInfo[i].sDt.wHour)
			//|| (nPrevHour == pDoc->m_TickInfo[i].sDt.wHour && nPrevMin > pDoc->m_TickInfo[i].sDt.wMinute)
			//) {
			//	if (pDoc->m_TickInfo[i].sDt.wMinute % 10 == 0) {
			//		char cHour[32];
			//		char cMin[32];

			//		wsprintf(cHour, "%02d", pDoc->m_TickInfo[i].sDt.wHour);
			//		TextOutA(pDC->m_hDC, nCurX, m_GraphBottom + 10, cHour, sizeof("XX") - 1);

			//		wsprintf(cMin, "%02d", pDoc->m_TickInfo[i].sDt.wMinute);
			//		TextOutA(pDC->m_hDC, nCurX, m_GraphBottom + 30, cMin, sizeof("XX") - 1);

			//		nPrevHour = pDoc->m_TickInfo[i].sDt.wHour;
			//		nPrevMin = pDoc->m_TickInfo[i].sDt.wMinute;
			//	}
			//}
			//******************************************************************
			oldp = pDC->SelectObject(&m_pen_black);	//�y���̕ύX

			int nPnt = m_GraphBottom - (int)((pDoc->m_vRateList[i] - m_dwMinVal)*m_dwGraphScalePerWSize);
			int nNextPnt = m_GraphBottom - (int)((pDoc->m_vRateList[i+1] - m_dwMinVal)*m_dwGraphScalePerWSize);

			pDC->MoveTo( nCurX, nPnt);
			pDC->LineTo(nCurX+dwPntSpan, nNextPnt);

			pDC->SelectObject(&oldp);

			nCurX += dwPntSpan;
		}
	}

	g_pMainFrame->SetRateGraphWindow( this->m_hWnd );
}


// CTRFViewerView �f�f

#ifdef _DEBUG
void CTRFViewerView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CTRFViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTRFViewerView ���b�Z�[�W �n���h���[


void CTRFViewerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
		m_pen_frame.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	m_pen_black.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	m_pen_dot_black.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	m_pen_red.CreatePen(PS_SOLID, 1, RGB(0xFF, 0, 0));
	m_pen_blue.CreatePen(PS_SOLID, 1, RGB(0, 0, 0xFF));
	m_pen_pink.CreatePen(PS_SOLID, 1, RGB(0xFF, 0, 0xFF));
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
	g_pMainFrame->SetRateGraphWindow( this->m_hWnd );
}


afx_msg LRESULT CTRFViewerView::OnReloadRateGraphRequest(WPARAM wParam, LPARAM lParam)
{
	CTRFViewerDoc* pDoc = (CTRFViewerDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return 0;

	char cOpenFile[1024];

	GetTempPath( sizeof(cOpenFile), cOpenFile );

	wsprintf( cOpenFile, "%s%08x.trg", cOpenFile, (DWORD)wParam );

	pDoc->ReadTRF( cOpenFile );

	Invalidate();

	return 0;
}
