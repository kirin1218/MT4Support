
#pragma once

#include "ViewTree.h"
#include "MT4_Supoort.h"

class CIndicatorViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CIndicatorView : public CDockablePane
{
// コンストラクション
public:
	CIndicatorView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// 属性
protected:

	CViewTree m_wndIndicatorView;
	CImageList m_IndicatorViewImages;
	CIndicatorViewToolBar m_wndToolBar;

protected:
	//void TRIIndicatorView(DIR_DATA* pdirdata, HTREEITEM hParent);

	//BOOL IsExistFile(DIR_DATA* pdirdata);

// 実装
public:
	virtual ~CIndicatorView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnLClickTreeItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRClickTreeItem(WPARAM wParam, LPARAM lParam);
public:
	void AddIndicator( INDICATOR_SETTING* pIS );
	afx_msg void OnAddBb();
	afx_msg void OnUpdateAddBb(CCmdUI *pCmdUI);
	HTREEITEM FindSameTreeItem( char* pTreeName );
	afx_msg void OnIndicatorviewDel();
	afx_msg void OnAddMa();
	afx_msg void OnAddAdx();
	afx_msg void OnAddIchimoku();
	void ViewTreeList();
	afx_msg void OnAddRSI();
	void DelOtherOscillator();
	afx_msg void OnAddSO();
	afx_msg void OnAddMACD();
	afx_msg void OnAddRCI();
};