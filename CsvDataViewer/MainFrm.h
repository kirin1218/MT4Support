
// MainFrm.h : CMainFrame クラスのインターフェイス
//

#pragma once
#include "FileView.h"
#include "PairView.h"
#include "IndicatorView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "CsvDataViewerDoc.h"
#include "CsvDataViewerView.h"
#include "CompTickChartView.h"
#include "MT4_Supoort.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 実装
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // コントロール バー用メンバー
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	CPairView		  m_wndPairView;
	CIndicatorView	  m_wndIndicatorView;
	CClassView        m_wndClassView;
	COutputWnd        m_wndOutput;
	CPropertiesWnd    m_wndProperties;

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnFileOpen();
	afx_msg void OnTestmode();

	void UpdateTradeInfoListCtrl( vector<TRADE_INFO>* pList );
	void UpdateRateChkInfoListCtrl( vector<RATE_CHECK_INFO>* pRCInfoList );
	void UpdateCompSTickInfoListCtrl( vector<COMP_STICK_LIST>* pSTickInfoList );
	void SetTriFilePath(const char*);
	void OpenCurrentChart(char* pPairName);
	void OpenTickRateGraph( VIEW_TICK_DATA* pTickRateData );
	std::string m_strTriFile;
	void ViewShiftSelectTrade( SYSTEMTIME*, SYSTEMTIME* );
	void ViewShiftSelectRCI( DWORD dwType, DWORD dwRate, SYSTEMTIME* pOpenTime, SYSTEMTIME* pCloseTime );
	void ViewShiftSelectCmpSTick( COMP_STICK_LIST* pCompSTickInfo );

	CCsvDataViewerView* ExistingView();
	HWND m_hRateGraphWindow;
	void SetRateGraphWindow( HWND hWnd );
	void ResetIndicatorTree();
	afx_msg void OnTick70tick();
	afx_msg void OnTick15min();
	afx_msg void OnTick30min();
	afx_msg void OnTick1min();
	afx_msg void OnTick1hour();
	afx_msg void OnUpdateTick15min(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTick1hour(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTick1min(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTick30min(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTick70tick(CCmdUI *pCmdUI);
	void SetTickType( DWORD dwTickType );
	afx_msg void OnTick5min();
	afx_msg void OnUpdateTick5min(CCmdUI *pCmdUI);
	void OpenCompTickChartFile( const char* pPath );
	afx_msg void OnAnalyzeTick();
};


