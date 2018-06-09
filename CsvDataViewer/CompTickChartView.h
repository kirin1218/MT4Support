#pragma once
#include"MT4_Supoort.h"

// CCompTickChartView ビュー

class CCompTickChartView : public CView
{
	DECLARE_DYNCREATE(CCompTickChartView)

protected:
	CCompTickChartView();           // 動的生成で使用される protected コンストラクター
	virtual ~CCompTickChartView();

public:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドされます。
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void OnInitialUpdate();
	void ViewShiftSelectCmpSTick( COMP_STICK_LIST* pCompSTickInfo );
	void DrawGraph(CDC* pDC, BOOL bDrawLeft );
	void DoDrawMem(CDC* pDC);

	DWORD m_BarWidth;
	DWORD m_BarSpan;
	double m_HeightPerEVal;

	DWORD m_GraphMargin;
	DWORD m_GraphTop;
	DWORD m_GraphViewTop;
	DWORD m_RGraphLeft;
	DWORD m_LGraphLeft;
	DWORD m_dwGraphScalePerWSize;
	DWORD m_GraphBottom;
	DWORD m_GraphViewBottom;
	DWORD m_RGraphRight;
	DWORD m_LGraphRight;
	DWORD m_GraphHeight;
	DWORD m_RGraphWidth;
	DWORD m_LGraphWidth;

	DWORD m_ViewBottom;
	DWORD m_ViewRight;

	//DWORD m_OscillatorBottom;
	//DWORD m_OscillatorTop;
	//DWORD m_OscillatorHeight;
	//DWORD m_OscillatorLeft;
	//DWORD m_OscillatorRight;
	//DWORD m_OscillatorWidth;

	HBRUSH m_hbrush_black;
	HBRUSH m_hbrush_red;
	HBRUSH m_hbrush_blue;
	CPen m_pen_frame;//(PS_SOLID, 2, RGB(0, 0, 0));
	CPen m_pen_frame_red;//(PS_SOLID, 2, RGB(0, 0, 0));
	CPen m_pen_black;//(PS_SOLID, 1, RGB(0, 0, 0));
	CPen m_pen_dot_black;//(PS_DOT, 1, RGB(0, 0, 0));
	CPen m_pen_red;//(PS_SOLID, 1, RGB(0xFF, 0, 0));
	CPen m_pen_blue;//(PS_SOLID, 1, RGB(0, 0, 0xFF));
	CPen m_pen_pink;
	CPen m_pen_orange;//.CreatePen(PS_SOLID, 1, RGB(255, 153, 0));
	CPen m_pen_green;//.CreatePen(PS_SOLID, 1, RGB(0, 128, 0));

	int m_nLGraphStartChange = 0;
	int m_nRGraphStartChange = 0;

	BOOL m_bSelectLeft = TRUE;
	RECT m_rcLeftGraph;
	RECT m_rcRightGraph;
	DWORD m_dwSelLSId = 0;
	DWORD m_dwSelLEId = 0;
	DWORD m_dwSelRSId = 0;
	DWORD m_dwSelREId = 0;

private:
	void SetGraphParameter(){
		CRect cRc;

		GetClientRect(&cRc);

		m_GraphMargin = 20;
		m_BarWidth = 5;
		m_BarSpan = 2;
		m_dwGraphScalePerWSize = 100;

		m_GraphBottom = ( cRc.bottom * m_dwGraphScalePerWSize /100 ) - 60;
		m_RGraphRight = cRc.right - 50;
		m_LGraphRight = (cRc.right/2) - 50;

		m_ViewBottom = cRc.bottom - m_GraphMargin;
		m_ViewRight = cRc.right - m_GraphMargin;

		m_GraphTop = m_GraphMargin;
		m_GraphViewTop = m_GraphTop + m_GraphMargin;
		m_LGraphLeft = m_GraphMargin;
		m_RGraphLeft = (cRc.right/2)+m_GraphMargin;

		m_GraphViewBottom = m_GraphBottom + m_GraphMargin;
		m_GraphHeight = m_GraphBottom - m_GraphTop;
		m_LGraphWidth = m_LGraphRight - m_LGraphLeft;
		m_RGraphWidth = m_RGraphRight - m_RGraphLeft;
	}
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


