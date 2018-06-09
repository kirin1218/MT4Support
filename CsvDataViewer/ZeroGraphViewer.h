#pragma once


// CZeroGraphViewer ビュー

class CZeroGraphViewer : public CView
{
	DECLARE_DYNCREATE(CZeroGraphViewer)

protected:
	CZeroGraphViewer();           // 動的生成で使用される protected コンストラクター
	virtual ~CZeroGraphViewer();

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
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void OnInitialUpdate();

	void DrawBollingerBands(CDC* pDC, DWORD i, DWORD dwSpan, DWORD dwSigma, int nCurrentX);
	DWORD CalcBollingerBandsSigma(DWORD i, DWORD dwSpan, int nSigma);
	double CalcSigma(DWORD i, DWORD dwSpan);
	DWORD m_dwMinVal;

	CPen m_pen_frame;//(PS_SOLID, 2, RGB(0, 0, 0));
	CPen m_pen_black;//(PS_SOLID, 1, RGB(0, 0, 0));
	CPen m_pen_dot_black;//(PS_DOT, 1, RGB(0, 0, 0));
	CPen m_pen_red;//(PS_SOLID, 1, RGB(0xFF, 0, 0));
	CPen m_pen_blue;//(PS_SOLID, 1, RGB(0, 0, 0xFF));
	CPen m_pen_pink;
	CPen m_pen_dot_red;

	DWORD m_BarWidth;//=1;
//	#define BAR_WIDTH			(1)
	DWORD m_BarSpan;//=1;
//	#define BAR_SPAN			(1)
	double m_HeightPerEVal;//= 1;
//	#define HEIGHT_PER_EVAL	(m_HeightPerEVal)

	DWORD m_GraphMargin;//= 20;
//	#define GRAPH_MARGIN		(20)
	DWORD m_GraphTop;//= m_GraphMargin;
//	#define GRAPH_TOP			(GRAPH_MARGIN)
	DWORD m_GraphViewTop;//= m_GraphTop+m_GraphMargin;
//	#define GRAPH_VIEW_TOP		(GRAPH_TOP+GRAPH_MARGIN)
	DWORD m_GraphLeft;//= m_GraphMargin;
//	#define GRAPH_LEFT			(GRAPH_MARGIN)
	DWORD	m_dwGraphScalePerWSize;//= 90;
//	#define GRAPH_SCALE_PERWSIZE	(m_dwGraphScalePerWSize)	//	クライアント領域のうち何％をグラフで使用するか

//	DWORD m_dwGraphBottom;//= 500;
//	DWORD m_dwGraphRight;//= 800;
	DWORD m_GraphBottom;//= m_dwGraphBottom - m_GraphMargin;
//	#define GRAPH_BOTTOM		(m_dwGraphBottom-GRAPH_MARGIN)
	DWORD m_GraphViewBottom;//= m_GraphBottom + m_GraphMargin;
//	#define GRAPH_VIEW_BOTTOM	(GRAPH_BOTTOM+GRAPH_MARGIN)
	DWORD m_GraphRight;
//	#define GRAPH_RIGHT			(m_dwGraphRight)
	DWORD m_GraphHeight;//= m_GraphBottom - m_GraphTop;
//	#define GRAPH_HEIGHT		(GRAPH_BOTTOM-GRAPH_TOP)
	DWORD m_GraphWidth;//= m_GraphBottom-m_GraphLeft;
//	#define GRAPH_WIDTH			(GRAPH_RIGHT-GRAPH_LEFT)

	DWORD m_ViewBottom;//= 500;
	DWORD m_ViewRight;//= 800;
//	#define VIEW_BOTTOM			(m_dwViewBottom)
//	#define VIEW_RIGHT			(m_dwViewRight)

	DWORD m_OscillatorBottom;//= 0;
	DWORD m_OscillatorTop;//= 0;
//	#define OSCILLATOR_BOTTOM	(m_dwOscillatorBottom)
//	#define OSCILLATOR_TOP		(m_dwOscillatorTop)
	DWORD m_OscillatorHeight;//= m_OscillatorBottom-m_OscillatorTop;
	//#define OSCILLATOR_HEIGHT	(OSCILLATOR_BOTTOM-OSCILLATOR_TOP)
	DWORD m_OscillatorLeft;//= m_GraphLeft;
//	#define OSCILLATOR_LEFT		GRAPH_LEFT
	DWORD m_OscillatorRight;//= m_GraphRight;
//	#define OSCILLATOR_RIGHT	GRAPH_RIGHT
	DWORD m_OscillatorWidth;//= m_GraphRight - m_GraphLeft;
//	#define OSCILLATOR_WIDTH	(OSCILLATOR_RIGHT-OSCILLATOR_LEFT)

	int m_nZeroGraphStartChange = 0;
	virtual BOOL PreTranslateMessage(MSG* pMsg);


private:
	void SetGraphParameter(){
		CRect cRc;

		GetClientRect(&cRc);

		m_GraphMargin = 20;
		m_BarWidth = 1;
		m_BarSpan = 1;
		m_dwGraphScalePerWSize = 100;


		m_GraphBottom = ( cRc.bottom * m_dwGraphScalePerWSize /100 ) - m_GraphMargin;
		m_OscillatorBottom = cRc.bottom - m_GraphMargin;
		m_OscillatorTop = ( cRc.bottom * m_dwGraphScalePerWSize /100 ) + m_GraphMargin;
		m_GraphRight = cRc.right - 50;

		m_ViewBottom = cRc.bottom - m_GraphMargin;
		m_ViewRight = cRc.right - m_GraphMargin;

		m_GraphTop = m_GraphMargin;
		m_GraphViewTop = m_GraphTop + m_GraphMargin;
		m_GraphLeft = m_GraphMargin;

		m_GraphViewBottom = m_GraphBottom + m_GraphMargin;
		m_GraphHeight = m_GraphBottom - m_GraphTop;
		m_GraphWidth = m_GraphRight - m_GraphLeft;

		m_OscillatorHeight = m_OscillatorBottom - m_OscillatorTop;
		m_OscillatorLeft = m_GraphLeft;
		m_OscillatorRight = m_GraphRight;
		m_OscillatorWidth = m_GraphRight - m_GraphLeft;
	}

};


