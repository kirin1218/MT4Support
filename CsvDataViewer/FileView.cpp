
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "CsvDataViewer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




//! 指定フォルダのファイルを取得する
//! ex: std::vector<std::string> path = util::Directory::GetFiles("C:\\aa\\");

void Directory::GetFiles(const std::string& dir_path, const std::string& filfilter)
{
	char cDirPath[1024];
	char* pDirName;
	lstrcpy( cDirPath, dir_path.c_str() );
	cDirPath[lstrlen(cDirPath)-1] = 0x00;

	//	フォルダ名を覚える
	pDirName = (char*)_mbsrchr((unsigned char*)cDirPath, (unsigned int)'\\');
	m_DirData.FileData.Name = pDirName+1;

	GetFiles(dir_path, "*.*", filfilter, &m_DirData );

	return; 
}

//! 指定フォルダのファイルを取得する
//! ex: std::vector<std::string> path = util::Directory::GetFiles("C:\\aa\\", "*.png");
void Directory::GetFiles(const std::string& dir_path, const std::string& dirfilter, const std::string& filfilter, DIR_DATA* pDirData )
{
	WIN32_FIND_DATAA fd;

	std::string ss = dir_path + dirfilter;
	HANDLE hFind = FindFirstFileA(ss.c_str(), &fd);

	// 検索失敗
	if(hFind == INVALID_HANDLE_VALUE)
	{
		throw std::exception("util::Directory::GetFiles failed");
	}

	//std::vector<std::string> fileList;
	do
	{
		// フォルダは除く
		if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			if(fd.cFileName[0] != '.' ){
				DIR_DATA DirData;
				DirData.FileData.FullName = dir_path + fd.cFileName + "\\";
				DirData.FileData.bIsFile = FALSE;
				DirData.FileData.Name = fd.cFileName;
				GetFiles(DirData.FileData.FullName, dirfilter, filfilter, &DirData );

				pDirData->dirList.push_back(DirData);
			}
		}else{
			// 隠しファイルは除く
			if(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
				continue;

			char* pExt = (char*)_mbsrchr((unsigned char*)fd.cFileName, (unsigned int)'.');
			if( pExt != NULL ){
				if( _memicmp( pExt, filfilter.c_str(), filfilter.size() ) == 0 ){
					FILE_DATA fildata;
					fildata.Name = fd.cFileName;
					fildata.FullName = dir_path + fd.cFileName;
					fildata.bIsFile = TRUE;
					pDirData->fileList.push_back(fildata);
				}
			}
		}
	} while(FindNextFileA(hFind, &fd));

	FindClose(hFind);

	return;
}

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_MESSAGE(WM_APP + 1110, &CFileView::OnSelectTreeItem)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar メッセージ ハンドラー

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ビューの作成:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("ファイル ビューを作成できませんでした\n");
		return -1;      // 作成できない場合
	}

	// ビューのイメージの読み込み:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ロックされています*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// すべてのコマンドが、親フレーム経由ではなくこのコントロール経由で渡されます:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	m_wndFileView.SetTreeType( TREE_TYPE_TRIFILE );
	m_wndFileView.SetTreeViewHandle( m_hWnd );

	//MakeTRIFileList();
	Directory cdir;
	std::string CurDir(_LOG_DIR_);// + "\\";
	CurDir += "\\";

	cdir.GetFiles( CurDir, ".tri"  );

	// 静的ツリー ビュー データ (ダミー コード) を入力します
	TRITreeList( &cdir );

	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

BOOL CFileView::IsExistFile( DIR_DATA* pdirdata )
{
	BOOL bRet = FALSE;

	if(pdirdata->fileList.size()>0) {
		bRet = TRUE;
	}else{
		std::vector<struct _DIR_DATA>::iterator itr = pdirdata->dirList.begin();

		if(pdirdata->dirList.size()>0) {
			for(;itr != pdirdata->dirList.end(); itr++) {
				if( (*itr).fileList.size() > 0 ){
					bRet = TRUE;
					break;
				}
			}
		}
	}

	return bRet;
}

void CFileView::TRIFileView( DIR_DATA* pdirdata, HTREEITEM hParent )
{
	
	if(pdirdata->fileList.size()>0 || IsExistFile(pdirdata) != FALSE ){
		HTREEITEM hSrc = m_wndFileView.InsertItem(pdirdata->FileData.Name.c_str(), 0, 0, hParent);

		std::vector<FILE_DATA>::iterator itr = pdirdata->fileList.begin();

		for(;itr != pdirdata->fileList.end(); itr++) {
			HTREEITEM hTree = m_wndFileView.InsertItem( (*itr).Name.c_str(), 1, 1, hSrc);
			FILE_DATA* pData = &(*itr);
			m_wndFileView.SetItemData( hTree, (DWORD_PTR)hTree);
			m_wndFileView.SetFileData((DWORD_PTR)hTree, (*itr).FullName );

			//FILE_DATA* pFileData = (FILE_DATA*)m_wndFileView.GetItemData(hTree);
		}

		std::vector<struct _DIR_DATA>::iterator itr2 = pdirdata->dirList.begin();

		if(pdirdata->dirList.size()>0) {
			for(;itr2 != pdirdata->dirList.end(); itr2++) {
				TRIFileView(&(*itr2), hSrc);
			}
		}		
	}

	return;
}

void CFileView::TRITreeList( Directory* pdir )
{
	HTREEITEM hRoot = m_wndFileView.InsertItem( pdir->m_DirData.FileData.Name.c_str(), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	//std::vector<std::string> fileList;
	//std::vector<struct _DIR_DATA> dirList;
	std::vector<struct _DIR_DATA>::iterator itr = pdir->m_DirData.dirList.begin();

	if(pdir->m_DirData.dirList.size()>0){
		for( ;itr!= pdir->m_DirData.dirList.end(); itr++ ){
			TRIFileView( &(*itr), hRoot );
		}
	}

	//HTREEITEM hSrc = m_wndFileView.InsertItem(_T("FakeApp ソース ファイル"), 0, 0, hRoot);
	//m_wndFileView.InsertItem(_T("FakeApp.cpp"), 1, 1, hSrc);
	//m_wndFileView.InsertItem(_T("FakeApp.rc"), 1, 1, hSrc);
	//m_wndFileView.InsertItem(_T("FakeAppDoc.cpp"), 1, 1, hSrc);
	//m_wndFileView.InsertItem(_T("FakeAppView.cpp"), 1, 1, hSrc);
	//m_wndFileView.InsertItem(_T("MainFrm.cpp"), 1, 1, hSrc);
	//m_wndFileView.InsertItem(_T("StdAfx.cpp"), 1, 1, hSrc);

	//HTREEITEM hInc = m_wndFileView.InsertItem(_T("FakeApp ヘッダー ファイル"), 0, 0, hRoot);

	//m_wndFileView.InsertItem(_T("FakeApp.h"), 2, 2, hInc);
	//m_wndFileView.InsertItem(_T("FakeAppDoc.h"), 2, 2, hInc);
	//m_wndFileView.InsertItem(_T("FakeAppView.h"), 2, 2, hInc);
	//m_wndFileView.InsertItem(_T("Resource.h"), 2, 2, hInc);
	//m_wndFileView.InsertItem(_T("MainFrm.h"), 2, 2, hInc);
	//m_wndFileView.InsertItem(_T("StdAfx.h"), 2, 2, hInc);

	//HTREEITEM hRes = m_wndFileView.InsertItem(_T("FakeApp リソース ファイル"), 0, 0, hRoot);

	//m_wndFileView.InsertItem(_T("FakeApp.ico"), 2, 2, hRes);
	//m_wndFileView.InsertItem(_T("FakeApp.rc2"), 2, 2, hRes);
	//m_wndFileView.InsertItem(_T("FakeAppDoc.ico"), 2, 2, hRes);
	//m_wndFileView.InsertItem(_T("FakeToolbar.bmp"), 2, 2, hRes);

	//m_wndFileView.Expand(hRoot, TVE_EXPAND);
	//m_wndFileView.Expand(hSrc, TVE_EXPAND);
	//m_wndFileView.Expand(hInc, TVE_EXPAND);
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// クリックされた項目の選択:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("プロパティ..."));

}

void CFileView::OnFileOpen()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnFileOpenWith()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnDummyCompile()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnEditCut()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnEditCopy()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnEditClear()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // 描画のデバイス コンテキスト

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ロックされました*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("ビットマップを読み込めませんでした: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}




afx_msg LRESULT CFileView::OnSelectTreeItem(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hTree = m_wndFileView.GetNextItem(TVI_ROOT, TVGN_CARET);
	DWORD_PTR dwItemData = m_wndFileView.GetItemData( hTree );

	if(dwItemData > 0 ){
		std::string FilePath = m_wndFileView.m_TreeData[dwItemData];

		CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

		if(pMainFrame != NULL )
		{
			((CMainFrame*)pMainFrame)->SetTriFilePath(FilePath.c_str());

		}
	}

	return 0;
}
