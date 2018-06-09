
#pragma once

#include "ViewTree.h"


class CPairViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPairView : public CDockablePane
{
// �R���X�g���N�V����
public:
	CPairView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// ����
protected:

	CViewTree m_wndPairView;
	CImageList m_PairViewImages;
	CPairViewToolBar m_wndToolBar;

protected:
	//void TRIPairView(DIR_DATA* pdirdata, HTREEITEM hParent);
	void PairTreeList();
	//BOOL IsExistFile(DIR_DATA* pdirdata);

// ����
public:
	virtual ~CPairView();

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
	afx_msg LRESULT OnSelectTreeItem(WPARAM wParam, LPARAM lParam);
};