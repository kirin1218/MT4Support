#pragma once


// CTRFViewerView ビュー

class CTRFViewerView : public CView
{
	DECLARE_DYNCREATE(CTRFViewerView)

protected:
	CTRFViewerView();           // 動的生成で使用される protected コンストラクター
	virtual ~CTRFViewerView();

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

public:
	DWORD m_dwMinVal;
	CPen m_pen_frame;//(PS_SOLID, 2, RGB(0, 0, 0));
	CPen m_pen_black;//(PS_SOLID, 1, RGB(0, 0, 0));
	CPen m_pen_dot_black;//(PS_DOT, 1, RGB(0, 0, 0));
	CPen m_pen_red;//(PS_SOLID, 1, RGB(0xFF, 0, 0));
	CPen m_pen_blue;//(PS_SOLID, 1, RGB(0, 0, 0xFF));
	CPen m_pen_pink;

	virtual void OnInitialUpdate();
protected:
	afx_msg LRESULT OnReloadRateGraphRequest(WPARAM wParam, LPARAM lParam);

private:
	DWORD m_BarWidth;
	DWORD m_BarSpan;
	double m_HeightPerEVal;

	DWORD m_GraphMargin;
	DWORD m_GraphTop;
	DWORD m_GraphViewTop;
	DWORD m_GraphLeft;
	DWORD m_dwGraphScalePerWSize;
	DWORD m_GraphBottom;
	DWORD m_GraphViewBottom;
	DWORD m_GraphRight;
	DWORD m_GraphHeight;
	DWORD m_GraphWidth;

	DWORD m_ViewBottom;
	DWORD m_ViewRight;

	DWORD m_OscillatorBottom;
	DWORD m_OscillatorTop;
	DWORD m_OscillatorHeight;
	DWORD m_OscillatorLeft;
	DWORD m_OscillatorRight;
	DWORD m_OscillatorWidth;

	void SetGraphParameter(){
		CRect cRc;

		GetClientRect(&cRc);

		m_GraphMargin = 20;
		m_BarWidth = 5;
		m_BarSpan = 2;

		m_GraphBottom = ( cRc.bottom * m_dwGraphScalePerWSize /100 ) - m_GraphMargin;
		m_GraphRight = cRc.right - 50;

		m_ViewBottom = cRc.bottom - m_GraphMargin;
		m_ViewRight = cRc.right - m_GraphMargin;

		m_GraphTop = m_GraphMargin;
		m_GraphViewTop = m_GraphTop + m_GraphMargin;
		m_GraphLeft = m_GraphMargin;

		m_GraphViewBottom = m_GraphBottom + m_GraphMargin;
		m_GraphHeight = m_GraphBottom - m_GraphTop;
		m_GraphWidth = m_GraphRight - m_GraphLeft;
	}
};


