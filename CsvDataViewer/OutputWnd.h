
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>  // ��istringstream���g�����߂ɕK�v

// ���O���
using namespace std;

#include "MT4_Supoort.h"

/////////////////////////////////////////////////////////////////////////////
// COutputList �E�B���h�E

class CTradeListCtrl : public CListCtrl
{
// �R���X�g���N�V����
public:
	CTradeListCtrl();

// ����
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
// CCmpGListCtrl �E�B���h�E

class CCmpGListCtrl : public CListCtrl
{
// �R���X�g���N�V����
public:
	CCmpGListCtrl();

// ����
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
// CRateChkListCtrl �E�B���h�E

class CRateChkListCtrl : public CListCtrl
{
// �R���X�g���N�V����
public:
	CRateChkListCtrl();

// ����
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
// �R���X�g���N�V����
public:
	COutputWnd();

	void UpdateFonts();

// ����
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

// ����
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

