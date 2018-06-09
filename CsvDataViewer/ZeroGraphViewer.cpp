// ZeroGraphViewer.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "ZeroGraphViewer.h"
#include "ZeroGraphDoc.h"

// CZeroGraphViewer

IMPLEMENT_DYNCREATE(CZeroGraphViewer, CView)

BEGIN_MESSAGE_MAP(CZeroGraphViewer, CView)
END_MESSAGE_MAP()

CZeroGraphViewer::CZeroGraphViewer()
{

}

CZeroGraphViewer::~CZeroGraphViewer()
{
}

BOOL CZeroGraphViewer::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
		m_pen_frame.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	m_pen_black.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	m_pen_dot_black.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	m_pen_dot_red.CreatePen(PS_DOT, 1, RGB(0xFF, 0, 0));
	m_pen_red.CreatePen(PS_SOLID, 1, RGB(0xFF, 0, 0));
	m_pen_blue.CreatePen(PS_SOLID, 1, RGB(0, 0, 0xFF));
	m_pen_pink.CreatePen(PS_SOLID, 1, RGB(0xFF, 0, 0xFF));

	return CView::PreCreateWindow(cs);
}


void CZeroGraphViewer::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
	//ReadIndicatorSetting();

	HBRUSH m_hbrush_black = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH m_hbrush_red = CreateSolidBrush(RGB(0xFF, 0, 0));
	HBRUSH m_hbrush_blue = CreateSolidBrush(RGB(0, 0, 0xFF));
}


// �C���N���[�h
#include "MT4_Supoort.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>  // ��istringstream���g�����߂ɕK�v
// ���O���
using namespace std;
//


// CZeroGraphViewer �`��

/////////////////////////////////////////////////////
//��=��(((X1-Xm)^2+(X2-Xm)^2�E�E�E(Xn-Xm)^2)/N-1)
//N�E�E�E�C�ӂ̊���(25�����g�����Ƃ������ł�)
//X1�`Xn�E�E�E1�`N���܂ł̊e�I�l
//Xm�E�E�EX1�`Xn�̕��ϒl 

double CZeroGraphViewer::CalcSigma( DWORD i , DWORD dwSpan )
{
	CZeroGraphDoc* pDoc = (CZeroGraphDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return 0;

	if ( i + 1 < dwSpan )	return 0;

	DWORD l;
	double nAve = 0;
	double nMole = 0;

	for( l = i - dwSpan + 1; l <= i; l++ ){
		nAve += (double)pDoc->m_vSMAList[l];
	}
	nAve /= (double)dwSpan;

	for( l = i - dwSpan + 1; l <= i; l++ ){
		nMole += ((double)pDoc->m_vSMAList[l]-nAve)*((double)pDoc->m_vSMAList[l]-nAve);
	}

	nMole /= (double)dwSpan-1;

	return sqrt(nMole);
}

DWORD CZeroGraphViewer::CalcBollingerBandsSigma( DWORD i , DWORD dwSpan, int nSigma )
{
	CZeroGraphDoc* pDoc = (CZeroGraphDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return 0;

	if ( i + 1 < dwSpan )	return 0;

	DWORD l;
	double nAve = 0;
	double nMole = 0;

	for( l = i - dwSpan + 1; l <= i; l++ ){
		nAve += (double)pDoc->m_vSMAList[l];
	}
	nAve /= (double)dwSpan;

	return (DWORD)(nAve + (CalcSigma(i, dwSpan)*nSigma));
}

void CZeroGraphViewer::DrawBollingerBands(CDC* pDC, DWORD i, DWORD dwSpan, DWORD dwSigma, int nCurrentX )
{
	CZeroGraphDoc* pDoc = (CZeroGraphDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if ( i + 1 < dwSpan )	return;
	if( i >= (pDoc->m_vSMAList.size()-1 ) )	return;

	DWORD dwBB = 0;
	DWORD dwNextBB = 0;

	for( int nSigma = -(int)dwSigma; nSigma <= (int)dwSigma; nSigma++ ){
		//if( nSigma != 2 && nSigma != -2 )	continue;

		dwBB = CalcBollingerBandsSigma( i, dwSpan, nSigma );
		dwNextBB = CalcBollingerBandsSigma( i+1, dwSpan, nSigma );

		int nStartX, nStartY, nEndX, nEndY;
	
		nStartX = nCurrentX + m_BarWidth / 2;
		nStartY = m_GraphBottom - (DWORD)((dwBB - m_dwMinVal)*m_HeightPerEVal);
		nEndX = nStartX + m_BarWidth + m_BarSpan;
		nEndY = m_GraphBottom - (DWORD)((dwNextBB - m_dwMinVal)*m_HeightPerEVal);

		CPen pen_frame(PS_SOLID, 1, RGB(0xFf,0,0) );
		CPen* oldp = pDC->SelectObject(&m_pen_frame);	//�y���̕ύX

		pDC->MoveTo( nStartX, nStartY );
		pDC->LineTo( nEndX, nEndY );

		pDC->SelectObject(oldp);
	}
#if 0
	//	���̂̕`��
	RECT rc;
	double dSigma = CalcSigma( i, dwSpan );

	rc.left = nCurrentX;
	rc.top = m_GraphBottom - (DWORD)(dSigma);
	rc.right = rc.left + m_BarWidth;
	rc.bottom = m_GraphBottom;

	CPen pen_frame(PS_SOLID, 1, RGB(0,0xFF,0) );
	CPen* oldp = pDC->SelectObject(&m_pen_frame);	//�y���̕ύX

	pDC->Rectangle(&rc);

	pDC->SelectObject(oldp);
#endif
}


void CZeroGraphViewer::OnDraw(CDC* pDC)
{
	CZeroGraphDoc* pDoc = (CZeroGraphDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->GetPathName().IsEmpty() == TRUE)
		return;

	//	�`�揈���̊J�n
	SetGraphParameter();

	CPen pen_frame(PS_SOLID, 2, RGB(0, 0, 0));
	CPen pen_black(PS_SOLID, 1, RGB(0, 0, 0));
	CPen pen_dot_black(PS_DOT, 1, RGB(0, 0, 0));
	CPen pen_dot_red(PS_DOT, 1, RGB(0xFF, 0, 0));
	CPen pen_red(PS_SOLID, 1, RGB(0xFF, 0, 0));
	CPen pen_blue(PS_SOLID, 1, RGB(0, 0, 0xFF));

	HBRUSH hBrush_old;
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
		DWORD dwViewBars = (m_GraphWidth) / (m_BarWidth + m_BarSpan);

		DWORD dwMaxRecords;// = sizeof(g_tickinfo) / sizeof(g_tickinfo[0]);
		dwMaxRecords = pDoc->m_vRateList.size();

		if ((int)dwMaxRecords > m_nZeroGraphStartChange && dwMaxRecords - m_nZeroGraphStartChange > (m_GraphWidth) / (m_BarWidth + m_BarSpan)) {
			dwMaxRecords -= m_nZeroGraphStartChange;
		}
		else if( (m_GraphWidth) / (m_BarWidth + m_BarSpan) > dwMaxRecords ){
			;;
		}else {
			dwMaxRecords = (m_GraphWidth) / (m_BarWidth + m_BarSpan);
			//m_nGraphStartChange = dwMaxRecords;
		}

		DWORD dwEndIdx = (dwViewBars < dwMaxRecords) ? (dwMaxRecords - dwViewBars - 1) : 0;

		if( (m_GraphWidth) / (m_BarWidth + m_BarSpan) > dwMaxRecords ){
			dwMaxRecords -= 1;
		}else{
			dwMaxRecords -= 2;	//	-2���ĂȂ񂼁H�v���o���Ȃ�
		}
		//	�ő�l�ƍŏ��l����������
		DWORD dwMaxVal = 0;
		m_dwMinVal = 777777;	//	�����o�[�ɂ��Ă݂܂����B

		for (int i = dwMaxRecords; i >= (int)dwEndIdx; i--) {
			DWORD Rate = EValFractionCut( pDoc->m_vRateList[i], pDoc->m_dwPair );

			if (dwMaxVal < Rate)		dwMaxVal = Rate;
			if (m_dwMinVal > Rate)		m_dwMinVal = Rate;
		}

		//	�����ڏ�㉺�ɂ�Ƃ����������
		dwMaxVal += 20;
		m_dwMinVal -= 20;

		m_HeightPerEVal = (double)m_GraphHeight / (double)(dwMaxVal - m_dwMinVal);


		//******************************************************************
		//	��؂�̐��l�ɓ_��������
		//******************************************************************
		DWORD dwLineY = (m_dwMinVal+10) - (m_dwMinVal%10);

		

		for (DWORD i = dwLineY; i < dwMaxVal; i += 10 ) {
			char wLine[32];
			DWORD dwY = (DWORD)(m_GraphBottom - (i - m_dwMinVal)*m_HeightPerEVal);

			if( i % 100 == 0 ){
				oldp = pDC->SelectObject(&m_pen_dot_red);	//�y���̕ύX
			}else{
				oldp = pDC->SelectObject(&m_pen_dot_black);	//�y���̕ύX
			}

			pDC->MoveTo(m_GraphLeft, dwY);
			pDC->LineTo(m_GraphRight, dwY);

			if (pDoc->m_dwPair == PAIR_EURUSD || pDoc->m_dwPair == PAIR_GBPUSD || pDoc->m_dwPair == PAIR_USDCHF ){
				sprintf_s(wLine, "%.4f", EVal2Rate( i, pDoc->m_dwPair ) );
			}else{
				sprintf_s(wLine, "%.2f", EVal2Rate( i, pDoc->m_dwPair ) );
			}

			TextOutA(pDC->m_hDC, m_GraphRight + 3, dwY - 2, wLine, lstrlen(wLine) );

			pDC->SelectObject(oldp);	//�y�������ɖ߂�(�K�{)
		}

		

		DWORD dwPntSpan = m_GraphWidth / (pDoc->m_vRateList.size()+2);
		//char cDate[32];
		//wsprintf(cDate, "%02d/%02d", pDoc->m_TickInfo[dwEndIdx].sDt.wMonth, pDoc->m_TickInfo[dwEndIdx].sDt.wDay );
		//TextOutA(pDC->m_hDC, m_GraphLeft+2, m_GraphTop + 2, cDate, lstrlen(cDate) );

		//*******************************************************************
		int nCurX = m_GraphRight - (m_BarSpan + m_BarWidth);
		//int nCurX = m_GraphLeft + dwPntSpan;
		CRect rc;
		int nPrevHour = -1;
		int nPrevMin = -1;


		for (int i = dwMaxRecords; i >= (int)dwEndIdx+1; i--) {
			//DWORD open = EValFractionCut(pDoc->m_TickInfo[i].Op, pDoc->m_dwPair );
			//DWORD Hi = EValFractionCut(pDoc->m_TickInfo[i].Hi, pDoc->m_dwPair);
			//DWORD Lo = EValFractionCut(pDoc->m_TickInfo[i].Lo, pDoc->m_dwPair);
			//DWORD close = EValFractionCut(pDoc->m_TickInfo[i].Cl, pDoc->m_dwPair);

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

			if( i % 200 == 0 ){
				char cCnt[32];
				wsprintf(cCnt, "%02d", i );
				TextOutA(pDC->m_hDC, nCurX, m_GraphBottom + 10, cCnt, lstrlen(cCnt) );
			}


			//******************************************************************

			//******************************************************************
			//	���[�\�N�̕`��
			//******************************************************************
			hBrush_old = NULL;

			//if ( close == open ) {
			//	oldp = pDC->SelectObject(&m_pen_black);	//�y���̕ύX
			//}
			//else if ( close > open ) {
			//	oldp = pDC->SelectObject(&m_pen_red);	//�y���̕ύX
			//	hBrush_old = (HBRUSH)pDC->SelectObject(m_hbrush_red);
			//}
			//else/* if ( close < open ) */
			//{
			//	oldp = pDC->SelectObject(&m_pen_blue);	//�y���̕ύX
			//	hBrush_old = (HBRUSH)pDC->SelectObject(m_hbrush_blue);
			//}

			////	���̂̕`��
			//rc.left = nCurX;
			//rc.top = m_GraphBottom - (DWORD)((close - m_dwMinVal)*m_HeightPerEVal);
			//rc.right = rc.left + m_BarWidth;
			//rc.bottom = m_GraphBottom - (DWORD)((open - m_dwMinVal)*m_HeightPerEVal);
			//if (rc.top == rc.bottom)	rc.bottom += 1;

			//pDC->Rectangle(&rc);

			//memcpy( &vtd.ti, &pDoc->m_TickInfo[i], sizeof(TICK_INFO) );
			//vtd.rc.left = rc.left;
			//vtd.rc.top = rc.top;
			//vtd.rc.right = rc.right;
			//vtd.rc.bottom = rc.bottom;

			//m_ViewTickData.push_back(vtd);

			//if (hBrush_old != NULL)	pDC->SelectObject(hBrush_old);	//�y�������ɖ߂�(�K�{)
			//pDC->SelectObject(oldp);	//�y�������ɖ߂�(�K�{)

			////	�q�Q�̕`��
			//oldp = pDC->SelectObject(&m_pen_black);	//�y���̕ύX

			//pDC->MoveTo(nCurX + m_BarWidth / 2, rc.top);
			//pDC->LineTo(nCurX + m_BarWidth / 2, m_GraphBottom - (DWORD)((Hi - m_dwMinVal)*m_HeightPerEVal));
			//pDC->MoveTo(nCurX + m_BarWidth / 2, rc.bottom);
			//pDC->LineTo(nCurX + m_BarWidth / 2, m_GraphBottom - (DWORD)((Lo - m_dwMinVal)*m_HeightPerEVal));


			//pDC->SelectObject(oldp);	//�y�������ɖ߂�(�K�{)

			int nPnt;
			int nPrevtPnt;

			oldp = pDC->SelectObject(&m_pen_black);	//�y���̕ύX

			nPnt = m_GraphBottom - (int)((pDoc->m_vRateList[i] - m_dwMinVal)*m_HeightPerEVal);
			nPrevtPnt = m_GraphBottom - (int)((pDoc->m_vRateList[i-1] - m_dwMinVal)*m_HeightPerEVal);

			pDC->MoveTo( nCurX, nPnt);
			pDC->LineTo( nCurX-(m_BarSpan + m_BarWidth), nPrevtPnt);

			pDC->SelectObject(&oldp);

			oldp = pDC->SelectObject(&m_pen_blue);	//�y���̕ύX

			nPnt = m_GraphBottom - (int)((pDoc->m_vSMAList[i] - m_dwMinVal)*m_HeightPerEVal);
			nPrevtPnt = m_GraphBottom - (int)((pDoc->m_vSMAList[i-1] - m_dwMinVal)*m_HeightPerEVal);

			pDC->MoveTo( nCurX, nPnt);
			pDC->LineTo( nCurX-(m_BarSpan + m_BarWidth), nPrevtPnt);

			pDC->SelectObject(&oldp);

			//******************************************************************

			//WriteTrandTerm( pDC, nCurX, i, dwEndIdx );

			//RECT rc;

			//rc.left = nCurX;
			//rc.top = m_GraphBottom - ((DWORD)(pDoc->m_TickInfo[i].dwDirectChangeCnt));
			//rc.right = rc.left + m_BarWidth;
			//rc.bottom = m_GraphBottom;

			//CPen pen_frame(PS_SOLID, 1, RGB(0,0,0) );
			//CPen* oldp = pDC->SelectObject(&m_pen_frame);	//�y���̕ύX

			//pDC->Rectangle(&rc);

			//pDC->SelectObject(oldp);

			nCurX -= m_BarWidth + m_BarSpan;

		}

		nCurX = m_GraphLeft + m_BarSpan;

		for (int i = dwEndIdx; i <= (int)dwMaxRecords; i++) {
			//	�C���W�Q�[�^�[��\������
			//DrawIndicator( pDC, i, nCurX );

			nCurX += m_BarWidth + m_BarSpan;

			//DrawBollingerBands( pDC, i, 50, 3, nCurX );
		}
		//�ĕ`��
		//UpdateWindow();
	}
}


// CZeroGraphViewer �f�f

#ifdef _DEBUG
void CZeroGraphViewer::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CZeroGraphViewer::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CZeroGraphViewer ���b�Z�[�W �n���h���[





BOOL CZeroGraphViewer::PreTranslateMessage(MSG* pMsg)
{
{
	CZeroGraphDoc* pDoc = (CZeroGraphDoc*)GetDocument();
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B
	if (pMsg->message == WM_KEYDOWN) {

		switch (pMsg->wParam) {
		case 37:	//	���{�^��
			if( pDoc != NULL ){
				if (m_nZeroGraphStartChange < (int)pDoc->m_vRateList.size() ){
					int nOnePageBars = (int)(m_GraphWidth) / (m_BarWidth + m_BarSpan);
					m_nZeroGraphStartChange += ( nOnePageBars - nOnePageBars/10 );
					if (m_nZeroGraphStartChange > ( (int)pDoc->m_vRateList.size() - (int)(m_GraphWidth) / (int)(m_BarWidth + m_BarSpan)  ) ){
						m_nZeroGraphStartChange = (int)pDoc->m_vRateList.size() - (int)(m_GraphWidth) / (int)(m_BarWidth + m_BarSpan);
					}

					Invalidate();
				}
			}
			break;
		case 39:	//	�E�{�^��
			if( m_nZeroGraphStartChange > 0 ){
				int nOnePageBars = (int)(m_GraphWidth) / (m_BarWidth + m_BarSpan);

				m_nZeroGraphStartChange -= ( nOnePageBars - nOnePageBars/10 );
				if (m_nZeroGraphStartChange < 0){
					m_nZeroGraphStartChange = 0;
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
	return CView::PreTranslateMessage(pMsg);
}
