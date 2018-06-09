
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>  // ※istringstreamを使うために必要

// 名前空間
using namespace std;

#include "MT4_Supoort.h"

/////////////////////////////////////////////////////////////////////////////
// COutputList ウィンドウ

class CTradeListCtrl : public CListCtrl
{
// コンストラクション
public:
	CTradeListCtrl();

// 実装
public:
	virtual ~CTradeListCtrl();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	vector<TRADE_INFO> vTradeInfo;

	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


/////////////////////////////////////////////////////////////////////////////
// CCmpGListCtrl ウィンドウ

class CCmpGListCtrl : public CListCtrl
{
// コンストラクション
public:
	CCmpGListCtrl();

// 実装
public:
	virtual ~CCmpGListCtrl();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
public:
	vector<COMP_STICK_LIST> vCmpSTickInfo;

	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////
// CRateChkListCtrl ウィンドウ

class CRateChkListCtrl : public CListCtrl
{
// コンストラクション
public:
	CRateChkListCtrl();

// 実装
public:
	virtual ~CRateChkListCtrl();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	vector<RATE_CHECK_INFO> vRateChkInfo;

	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
};

class COutputWnd : public CDockablePane
{
// コンストラクション
public:
	COutputWnd();

	void UpdateFonts();

// 属性
protected:
	CMFCTabCtrl	m_wndTabs;

	//COutputList m_wndOutputBuild;
	//COutputList m_wndOutputDebug;
	//COutputList m_wndOutputFind;
	CRateChkListCtrl m_RateChkInfoList;
	CTradeListCtrl m_TradeInfoList;
	CCmpGListCtrl m_CmpGraphList;
protected:
	//void FillBuildWindow();
	//void FillDebugWindow();
	//void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

// 実装
public:
	virtual ~COutputWnd();
	void UpdateTradeInfoListCtrl(vector<TRADE_INFO>* pTrInfoList);
	void UpdateRateChkInfoListCtrl(vector<RATE_CHECK_INFO>* pRateChkInfoList );
	void UpdateCompSTickInfoListCtrl(vector<COMP_STICK_LIST>* pSTickInfoList );

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

