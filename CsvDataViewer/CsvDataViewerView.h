
// CsvDataViewerView.h : CCsvDataViewerView クラスのインターフェイス
//

#pragma once

#include <map>
#include <vector>
using namespace std;

class CCsvDataViewerView : public CView
{
protected: // シリアル化からのみ作成します。
	CCsvDataViewerView();
	DECLARE_DYNCREATE(CCsvDataViewerView)

// 属性
public:
	CCsvDataViewerDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 実装
public:
	virtual ~CCsvDataViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	double CalcEMAValue(int idx);
	void WriteRectangle(CDC* pDC, int nStartX, int nStartIdx, int nEndIdx, int nPenStyle, int nWidth, COLORREF lColor );
	void WriteRectangle2(CDC* pDC, int nStartX, int nStartIdx, int nEndIdx, int nPenStyle, int nWidth, COLORREF lColor, DWORD dwHigh=0, DWORD dwLow=0 ) ;
	void FrameDojiArea(CDC* pDC, int nStartX, int nStartIdx, int nEndIdx, DWORD dwLineWdth, double dGainCut = 0, double dLossCut = 0 );
	void DrawEMALine(CDC* pDC, DWORD i, DWORD dwEMAVal, int nCurrentX);
	void DrawStchasticOscillator(CDC* pDC, DWORD i, int nCurrentX);
	int GetTradeInfo(DWORD dwTicket);
	afx_msg void On20EMA();
	afx_msg void On50EMA();
	afx_msg void OnStochasticOscillator();
	afx_msg void On200EMA();

	//	Menu設定
public:
	vector<INDICATOR_SETTING> m_IndiSetting;

	BOOL m_b13EMA;
	BOOL m_b21EMA;
	BOOL m_b20EMA;
	BOOL m_b50EMA;
	BOOL m_b200EMA;
	BOOL m_bOscillator;

public:
	DWORD m_dwMinVal;	//	表示中または表示予定の値のうち最小値を保持する
	afx_msg void On13ema();
	afx_msg void On21ema();
	afx_msg void OnUpdate20ema(CCmdUI *pCmdUI);
	afx_msg void OnUpdate50ema(CCmdUI *pCmdUI);
	afx_msg void OnUpdate200ema(CCmdUI *pCmdUI);
	afx_msg void OnUpdate13ema(CCmdUI *pCmdUI);
	afx_msg void OnUpdate21ema(CCmdUI *pCmdUI);
protected:
	afx_msg LRESULT OnRegiterRateMessage(WPARAM wParam, LPARAM lParam);
public:
	void ReadIndicatorSetting();
	void WriteIndicatorSetting();

	virtual void OnInitialUpdate();

	vector<INDICATOR_SETTING>& GetIndicatorSetting(){
		return m_IndiSetting;
	}

	void SetIndicatorDisplayName( INDICATOR_SETTING* pIndiSetting );

	void SetIndicatorSetting( INDICATOR_SETTING* pIndiSetting ){
		m_IndiSetting.push_back(*pIndiSetting);
		
		Invalidate();

		WriteIndicatorSetting();
	}
	void DeleteIndicatorSetting( DWORD );

	void DrawIndicator(CDC* pDC, DWORD i, int nCurrentX );
	void DrawBollingerBands(CDC* pDC, DWORD i, DWORD dwSpan, DWORD dwSigma, int nCurrentX );
	DWORD CalcBollingerBandsSigma( DWORD i , DWORD dwSpan, int nSigma );
	double CalcSigma( DWORD i , DWORD dwSpan );
	void DrawADX(CDC* pDC, DWORD i, DWORD dwSpan, int nCurrentX);
	double CalcRSI( DWORD i, DWORD dwSpan, double dPrevUpAve, double dPrevDownAve );
	void DrawRSI(CDC* pDC, DWORD i, DWORD dwSpan, int nCurrentX);
	double GetRCIPRank( DWORD dwStart, DWORD dwPrice, DWORD dwSpan );
	double CalcRCI( DWORD i, DWORD dwSpan );
	void DrawRCI(CDC* pDC, DWORD i, DWORD dwSpan1, DWORD dwSpan2, DWORD dwSpan3, int nCurrentX );
	BOOL CalcStochasticOscillator( DWORD i, DWORD dwKSpan, DWORD dwDSpan, DWORD dwSDSpan, DWORD* pdwSO_K, DWORD* pdwSO_D, DWORD* pdwSO_Slow );
	void DrawSO(CDC* pDC, DWORD i, DWORD dwKSpan, DWORD dwDSpan, DWORD dwSSpan, int nCurrentX );
	void DrawIchimoku(CDC* pDC, DWORD i, int nCurrentX );
	void DrawMACD(CDC* pDC, DWORD i, DWORD dwSSpan, DWORD dwLSpan, DWORD dwSignal, int nCurrentX );
	BOOL GetIchimokuData( DWORD i, ICHIMOKU_INFO* pRetInfo );
	double CalcDX( DWORD i, DWORD dwSpan );
	DWORD CalcADX( DWORD i, DWORD dwSpan );
	DWORD BandsWalkRange( CDC* pDC, int dwStart, int idx );
	void BollingerSigmaTrans( CDC* pDC, int nCurX, int i );
	BOOL IsConvergeSigma( BOOL bHiCheck, DWORD idx );
	void WriteTrandTerm(CDC* pDC, int nCurX, int i, DWORD dwEndIdx ) ;
	void WriteRateCheckInfo(CDC* pDC, int nCurX, int i, DWORD dwEndIdx ) ;
	vector<VIEW_TICK_DATA> m_ViewTickData;
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void ViewShiftSelectTrade( SYSTEMTIME* pOpenTime, SYSTEMTIME* pCloseTime );
	void ViewShiftSelectRCI( DWORD dwType, DWORD dwRate, SYSTEMTIME* pOpenTime, SYSTEMTIME* pCloseTime );
	BOOL IncludeOscillator();
	vector<INDICATOR_SETTING>& GetActiveIndicatorSetting();
	HBRUSH m_hbrush_black;
	HBRUSH m_hbrush_red;
	HBRUSH m_hbrush_blue;
	CPen m_pen_frame;//(PS_SOLID, 2, RGB(0, 0, 0));
	CPen m_pen_black;//(PS_SOLID, 1, RGB(0, 0, 0));
	CPen m_pen_dot_black;//(PS_DOT, 1, RGB(0, 0, 0));
	CPen m_pen_red;//(PS_SOLID, 1, RGB(0xFF, 0, 0));
	CPen m_pen_blue;//(PS_SOLID, 1, RGB(0, 0, 0xFF));
	CPen m_pen_pink;
	CPen m_pen_orange;//.CreatePen(PS_SOLID, 1, RGB(255, 153, 0));
	CPen m_pen_green;//.CreatePen(PS_SOLID, 1, RGB(0, 128, 0));

	map<DWORD,map<DWORD,DWORD>>	m_EMAMap;
	DWORD CalcEMAArray( DWORD dwSpan );
	DWORD CalcSMAValue( DWORD dwSpan, DWORD idx );
	DWORD CalcEMAValue( DWORD dwSpan, DWORD idx );
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void DoDrawMem(CDC* pDC);

	DWORD GetTickType();
	void SetTickType( DWORD dwTickType );
	DWORD m_dwTickType = TKI_70TICK;

	int g_nGraphStartChange = 0;
	//afx_msg void OnAnalyzeTick();
	void GetPathName( char* pPathName );

public:
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

//#define BAR_WIDTH			(5)
//#define BAR_SPAN			(2)
//		double g_BarHeightPerPixel = 1;
//#define BAR_HEIGHT_PER_PIXEL	(g_BarHeightPerPixel)
//
//#define GRAPH_MARGIN		(20)
//#define GRAPH_TOP			(GRAPH_MARGIN)
//#define GRAPH_VIEW_TOP		(GRAPH_TOP+GRAPH_MARGIN)
//#define GRAPH_LEFT			(GRAPH_MARGIN)
//		DWORD	g_dwGraphScalePerWSize = 90;
//#define GRAPH_SCALE_PERWSIZE	(g_dwGraphScalePerWSize)	//	クライアント領域のうち何％をグラフで使用するか
//
//		DWORD g_dwGraphBottom = 500;
//		DWORD g_dwGraphRight = 800;
//#define GRAPH_BOTTOM		(g_dwGraphBottom-GRAPH_MARGIN)
//#define GRAPH_VIEW_BOTTOM	(GRAPH_BOTTOM+GRAPH_MARGIN)
//#define GRAPH_RIGHT			(g_dwGraphRight)
//#define GRAPH_HEIGHT		(GRAPH_BOTTOM-GRAPH_TOP)
//#define GRAPH_WIDTH			(GRAPH_RIGHT-GRAPH_LEFT)
//
//		DWORD g_dwViewBottom = 500;
//		DWORD g_dwViewRight = 800;
//#define VIEW_BOTTOM			(g_dwViewBottom)
//#define VIEW_RIGHT			(g_dwViewRight)
//
//		DWORD g_dwOscillatorBottom = 0;
//		DWORD g_dwOscillatorTop = 0;
//#define OSCILLATOR_BOTTOM	(g_dwOscillatorBottom)
//#define OSCILLATOR_TOP		(g_dwOscillatorTop)
//#define OSCILLATOR_HEIGHT	(OSCILLATOR_BOTTOM-OSCILLATOR_TOP)
//#define OSCILLATOR_LEFT		GRAPH_LEFT
//#define OSCILLATOR_RIGHT	GRAPH_RIGHT
//#define OSCILLATOR_WIDTH	(OSCILLATOR_RIGHT-OSCILLATOR_LEFT)

private:
	void SetGraphParameter(){
		CRect cRc;

		GetClientRect(&cRc);

		m_GraphMargin = 20;
		m_BarWidth = 3;
		m_BarSpan = 1;
		
		if( IncludeOscillator() != FALSE ){
			m_dwGraphScalePerWSize = 70;
		}else{
			m_dwGraphScalePerWSize = 90;
		}

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

#ifndef _DEBUG  // CsvDataViewerView.cpp のデバッグ バージョン
inline CCsvDataViewerDoc* CCsvDataViewerView::GetDocument() const
   { return reinterpret_cast<CCsvDataViewerDoc*>(m_pDocument); }
#endif

