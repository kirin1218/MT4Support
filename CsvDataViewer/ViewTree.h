
#pragma once
#include <sstream>
#include <vector>
#include <string>
#include <exception>
#include <map>

enum{
	TREE_TYPE_TRIFILE = 0,
	TREE_TYPE_PAIR,
	TREE_TYPE_INDICATOR
};

typedef struct _FILE_DATA {
	std::string Name;
	std::string FullName;
	BOOL bIsFile;
}FILE_DATA, *PFILE_DATA;

typedef struct _DIR_DATA {
	FILE_DATA FileData;
	std::vector<FILE_DATA> fileList;
	std::vector<struct _DIR_DATA> dirList;
}DIR_DATA, *PDIR_DATA;

/////////////////////////////////////////////////////////////////////////////
// CViewTree ウィンドウ

class CViewTree : public CTreeCtrl
{
// コンストラクション
public:
	CViewTree();

// オーバーライド
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// 実装
public:
	virtual ~CViewTree();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void CViewTree::SetFileData(DWORD_PTR hTree, std::string& ItemPath);
	std::map< DWORD_PTR, std::string> m_TreeData;
	HWND m_hTreeView;
	void SetTreeViewHandle( HWND );
	DWORD m_TreeType;
	void SetTreeType( DWORD dwType ){
		m_TreeType = dwType;
	}
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
};
