
#include "stdafx.h"
#include "mainfrm.h"
#include "PairView.h"
#include "Resource.h"
#include "CsvDataViewer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CPairView

CPairView::CPairView()
{
}

CPairView::~CPairView()
{
}

BEGIN_MESSAGE_MAP(CPairView, CDockablePane)
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
	ON_MESSAGE(WM_APP + 1110, &CPairView::OnSelectTreeItem)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar メッセージ ハンドラー

int CPairView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ビューの作成:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndPairView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("ファイル ビューを作成できませんでした\n");
		return -1;      // 作成できない場合
	}

	// ビューのイメージの読み込み:
	m_PairViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndPairView.SetImageList(&m_PairViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ロックされています*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// すべてのコマンドが、親フレーム経由ではなくこのコントロール経由で渡されます:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	m_wndPairView.SetTreeType( TREE_TYPE_PAIR );
	m_wndPairView.SetTreeViewHandle( m_hWnd );

	// 静的ツリー ビュー データ (ダミー コード) を入力します
	PairTreeList();

	AdjustLayout();

	return 0;
}

void CPairView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}


void CPairView::PairTreeList( )
{
	HTREEITEM hRoot = m_wndPairView.InsertItem( "通貨ペア一覧", 0, 0);
	//m_wndPairView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	m_wndPairView.InsertItem(_T("USDJPY"), 0, 0, hRoot);
	m_wndPairView.InsertItem(_T("EURJPY"), 0, 0, hRoot);
	m_wndPairView.InsertItem(_T("EURUSD"), 0, 0, hRoot);
	m_wndPairView.InsertItem(_T("AUDJPY"), 0, 0, hRoot);
	m_wndPairView.InsertItem(_T("CHFJPY"), 0, 0, hRoot);
	m_wndPairView.InsertItem(_T("GBPJPY"), 0, 0, hRoot);
	m_wndPairView.InsertItem(_T("GBPUSD"), 0, 0, hRoot);
	m_wndPairView.InsertItem(_T("NZDJPY"), 0, 0, hRoot);
	m_wndPairView.InsertItem(_T("USDCHF"), 0, 0, hRoot);
	m_wndPairView.InsertItem(_T("ZARJPY"), 0, 0, hRoot);
	m_wndPairView.Expand(hRoot, TVE_EXPAND);
}

void CPairView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndPairView;
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

void CPairView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPairView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CPairView::OnProperties()
{
	AfxMessageBox(_T("プロパティ..."));

}

void CPairView::OnFileOpen()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CPairView::OnFileOpenWith()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CPairView::OnDummyCompile()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CPairView::OnEditCut()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CPairView::OnEditCopy()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CPairView::OnEditClear()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CPairView::OnPaint()
{
	CPaintDC dc(this); // 描画のデバイス コンテキスト

	CRect rectTree;
	m_wndPairView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CPairView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndPairView.SetFocus();
}

void CPairView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ロックされました*/);

	m_PairViewImages.DeleteImageList();

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

	m_PairViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_PairViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndPairView.SetImageList(&m_PairViewImages, TVSIL_NORMAL);
}




afx_msg LRESULT CPairView::OnSelectTreeItem(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hTree = m_wndPairView.GetNextItem(TVI_ROOT, TVGN_CARET);
	CString strPairName = m_wndPairView.GetItemText(hTree);
	char cPairName[64];
	
	if(strPairName.IsEmpty() == FALSE ){
		CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

		lstrcpy(cPairName, strPairName );
		if(pMainFrame != NULL )
		{
			((CMainFrame*)pMainFrame)->OpenCurrentChart(cPairName);

		}
	}

	return 0;
}
