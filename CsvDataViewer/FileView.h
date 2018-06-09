
#pragma once

#include "ViewTree.h"




class Directory;

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// コンストラクション
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// 属性
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

protected:
	void TRIFileView(DIR_DATA* pdirdata, HTREEITEM hParent);
	void TRITreeList(Directory*);
	BOOL IsExistFile(DIR_DATA* pdirdata);

// 実装
public:
	virtual ~CFileView();

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




class Directory
{
public:
	DIR_DATA m_DirData;
	void GetFiles(const std::string& dir_path, const std::string& filfilter);
	void GetFiles(const std::string& dir_path, const std::string& dirfilter, const std::string& filfilter, DIR_DATA* pDirData);
};