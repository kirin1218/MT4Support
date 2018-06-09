
#include "stdafx.h"
#include "mainfrm.h"
#include "IndicatorView.h"
#include "Resource.h"
#include "CsvDataViewer.h"
#include "IndicatorBB.h"
#include "IndicatorMA.h"
#include "IndicatorADX.h"
#include "IndicatorRSI.h"
#include "IndicatorSO.h"
#include "IndicatorMACD.h"
#include "IndicatorRCI.h"

#include "CsvDataViewerView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CIndicatorView

CIndicatorView::CIndicatorView()
{
}

CIndicatorView::~CIndicatorView()
{
}

BEGIN_MESSAGE_MAP(CIndicatorView, CDockablePane)
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
	ON_MESSAGE(WM_APP + 1110, &CIndicatorView::OnLClickTreeItem)
	ON_MESSAGE(WM_APP + 1111, &CIndicatorView::OnRClickTreeItem)
	ON_COMMAND(ID_ADD_BB, &CIndicatorView::OnAddBb)
	ON_UPDATE_COMMAND_UI(ID_ADD_BB, &CIndicatorView::OnUpdateAddBb)
	ON_COMMAND(ID_INDICATORVIEW_DEL, &CIndicatorView::OnIndicatorviewDel)
	ON_COMMAND(ID_ADD_MA, &CIndicatorView::OnAddMa)
	ON_COMMAND(ID_ADD_ADX, &CIndicatorView::OnAddAdx)
	ON_COMMAND(ID_ADD_ICHI, &CIndicatorView::OnAddIchimoku)
	ON_COMMAND(ID_ADD_RSI, &CIndicatorView::OnAddRSI)
	ON_COMMAND(ID_ADD_SO, &CIndicatorView::OnAddSO)
	ON_COMMAND(ID_ADD_MACD, &CIndicatorView::OnAddMACD)
	ON_COMMAND(ID_ADD_RCI, &CIndicatorView::OnAddRCI)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar メッセージ ハンドラー

int CIndicatorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ビューの作成:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndIndicatorView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("ファイル ビューを作成できませんでした\n");
		return -1;      // 作成できない場合
	}

	// ビューのイメージの読み込み:
	m_IndicatorViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndIndicatorView.SetImageList(&m_IndicatorViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ロックされています*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// すべてのコマンドが、親フレーム経由ではなくこのコントロール経由で渡されます:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	m_wndIndicatorView.SetTreeType( TREE_TYPE_TRIFILE );
	m_wndIndicatorView.SetTreeViewHandle( m_hWnd );

	// 静的ツリー ビュー データ (ダミー コード) を入力します
	ViewTreeList();

	AdjustLayout();

	return 0;
}

void CIndicatorView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CIndicatorView::ViewTreeList( )
{
	m_wndIndicatorView.DeleteAllItems();

	HTREEITEM hRoot = m_wndIndicatorView.InsertItem( "Indicator", 0, 0);

	//	設定を読み込み選択中のインディケータを表示する
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());
	if(pMainFrame != NULL )
	{
		CCsvDataViewerView* pView = ((CMainFrame*)pMainFrame)->ExistingView();
		//CCsvDataViewerView* pView = (CCsvDataViewerView*)((CMainFrame*)pMainFrame)->GetActiveView();

		if( pView != NULL ){
			vector<INDICATOR_SETTING>& vIndicator = pView->GetActiveIndicatorSetting();
			vector<INDICATOR_SETTING>::iterator itr = vIndicator.begin();

			for(;itr!= vIndicator.end(); itr++ ){

				HTREEITEM hNewItem = m_wndIndicatorView.InsertItem( (*itr).DisplayName, 0, 0, hRoot );
				(*itr).dwSettingId = (DWORD)hNewItem;	//	重複しなければなんでもいい
				m_wndIndicatorView.SetItemData( hNewItem, (DWORD_PTR)(*itr).dwSettingId );
			}
		}
	}

	//m_wndIndicatorView.InsertItem(_T("USDJPY"), 0, 0, hRoot);
	//m_wndIndicatorView.InsertItem(_T("EURJPY"), 0, 0, hRoot);
	//m_wndIndicatorView.InsertItem(_T("EURUSD"), 0, 0, hRoot);
	//m_wndIndicatorView.InsertItem(_T("USDJPY"), 0, 0, hRoot);
	//m_wndIndicatorView.InsertItem(_T("EURJPY"), 0, 0, hRoot);
	//m_wndIndicatorView.InsertItem(_T("EURUSD"), 0, 0, hRoot);
	//m_wndIndicatorView.InsertItem(_T("AUDJPY"), 0, 0, hRoot);
	//m_wndIndicatorView.InsertItem(_T("CHFJPY"), 0, 0, hRoot);
	//m_wndIndicatorView.InsertItem(_T("GBPJPY"), 0, 0, hRoot);
	//m_wndIndicatorView.InsertItem(_T("GBPUSD"), 0, 0, hRoot);
	//m_wndIndicatorView.InsertItem(_T("NZDJPY"), 0, 0, hRoot);
	//m_wndIndicatorView.InsertItem(_T("USDCHF"), 0, 0, hRoot);
	//m_wndIndicatorView.InsertItem(_T("ZARJPY"), 0, 0, hRoot);

	m_wndIndicatorView.Expand(hRoot, TVE_EXPAND);
}

void CIndicatorView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndIndicatorView;
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

			CString strTreeItem = m_wndIndicatorView.GetItemText(hTreeItem);
			char cTreeItem[64];
	
			if(strTreeItem.IsEmpty() == FALSE ){
				CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

				lstrcpy( cTreeItem, strTreeItem );

				CMenu   cMenu;
				CMenu*	pSubMenu = NULL;
				POINT	pt;

				GetCursorPos( &pt );
		
				if( strcmp( cTreeItem, "インディケータ" ) == 0 ){
					//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_INDICTOR_RCLICK, point.x, point.y, this, TRUE);

					if( cMenu.LoadMenu(IDR_INDICTOR_RCLICK) ){
						if( ( pSubMenu = cMenu.GetSubMenu(0) ) != NULL){
							//ScreenToClient( &pt );
							if (!pSubMenu->TrackPopupMenu( TPM_RIGHTBUTTON, pt.x, pt.y, this) ){
								;
							}
						}
						// メニューを破棄
						cMenu.DestroyMenu();
					}			
				}else{
					if( cMenu.LoadMenu(IDR_INDICTOR_RCLICK) ){
						if( ( pSubMenu = cMenu.GetSubMenu(1) ) != NULL){
							//ScreenToClient( &pt );
							if (!pSubMenu->TrackPopupMenu( TPM_RIGHTBUTTON, pt.x, pt.y, this) ){
								;
							}
						}
						// メニューを破棄
						cMenu.DestroyMenu();
					}
				}

				return;
			}

		}
	}
	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CIndicatorView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndIndicatorView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CIndicatorView::OnProperties()
{
	AfxMessageBox(_T("プロパティ..."));

}

void CIndicatorView::OnFileOpen()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CIndicatorView::OnFileOpenWith()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CIndicatorView::OnDummyCompile()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CIndicatorView::OnEditCut()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CIndicatorView::OnEditCopy()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CIndicatorView::OnEditClear()
{
	// TODO: ここにコマンド ハンドラー コードを追加します
}

void CIndicatorView::OnPaint()
{
	CPaintDC dc(this); // 描画のデバイス コンテキスト

	CRect rectTree;
	m_wndIndicatorView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CIndicatorView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndIndicatorView.SetFocus();
}

void CIndicatorView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ロックされました*/);

	m_IndicatorViewImages.DeleteImageList();

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

	m_IndicatorViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_IndicatorViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndIndicatorView.SetImageList(&m_IndicatorViewImages, TVSIL_NORMAL);
}

afx_msg LRESULT CIndicatorView::OnLClickTreeItem(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hTree = m_wndIndicatorView.GetNextItem(TVI_ROOT, TVGN_CARET);

	return 0;
}

afx_msg LRESULT CIndicatorView::OnRClickTreeItem(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

HTREEITEM CIndicatorView::FindSameTreeItem( char* pTreeName )
{
	HTREEITEM hFind = NULL;

	HTREEITEM hItem = m_wndIndicatorView.GetRootItem();
	if( hItem != NULL ){
		hItem = m_wndIndicatorView.GetNextItem(hItem, TVGN_CHILD);

		while( hItem != NULL ){
			CString ItemText = m_wndIndicatorView.GetItemText( hItem );

			if( ItemText.Compare( pTreeName ) == 0 ){
				hFind = hItem;
				break;
			}

			hItem = m_wndIndicatorView.GetNextItem( hItem, TVGN_NEXT);
		}
	}

	return hFind;
}

void CIndicatorView::AddIndicator( INDICATOR_SETTING* pIS )
{
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());
	if(pMainFrame != NULL ){
		CCsvDataViewerView* pView = ((CMainFrame*)pMainFrame)->ExistingView();
		if( pView != NULL ){
			HTREEITEM hItem;
			DWORD dwTick = GetTickCount();

			pIS->dwSettingId = dwTick;

			pView->SetIndicatorDisplayName( pIS );

			if( ( hItem = FindSameTreeItem( pIS->DisplayName ) ) != NULL ){
				DWORD dwId = (DWORD)m_wndIndicatorView.GetItemData( hItem );
				pView->DeleteIndicatorSetting(dwId);
			}
			pView->SetIndicatorSetting(pIS);

			hItem = m_wndIndicatorView.GetRootItem();
			hItem = m_wndIndicatorView.InsertItem( pIS->DisplayName, hItem );
			m_wndIndicatorView.SetItemData( hItem, (DWORD_PTR)dwTick );

			HTREEITEM hRoot = m_wndIndicatorView.GetRootItem();
			m_wndIndicatorView.Expand( hRoot, TVE_EXPAND );
		}
	}
}

//	ボリジャーバンド
void CIndicatorView::OnAddBb()
{	
	CIndicatorBB Dlg;

	if( Dlg.DoModal() == IDOK ){
		INDICATOR_SETTING IS;

		IS.dwIndiType = INDICATOR_TYPE_BB;
		IS.bb.dwSpan = Dlg.m_dwSpan;
		IS.bb.dwSigma = Dlg.m_dwSigma;

		AddIndicator( &IS );
	}
}

void CIndicatorView::OnAddMa()
{
	CIndicatorMA Dlg;

	if( Dlg.DoModal() == IDOK ){
		INDICATOR_SETTING IS;

		IS.dwIndiType = INDICATOR_TYPE_MA;
		IS.ma.dwSpan = Dlg.m_dwSpan;
		IS.ma.color = Dlg.m_Color;

		AddIndicator( &IS );
	}
}

//	現仕様だと一個しか表示できんのよ
void CIndicatorView::DelOtherOscillator()
{

	return;

	BOOL bDel = FALSE;
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());
	if(pMainFrame != NULL )
	{
		CCsvDataViewerView* pView = ((CMainFrame*)pMainFrame)->ExistingView();
		//CCsvDataViewerView* pView = (CCsvDataViewerView*)((CMainFrame*)pMainFrame)->GetActiveView();

		if( pView != NULL ){
			vector<INDICATOR_SETTING>& vIndicator = pView->GetActiveIndicatorSetting();
			//vector<INDICATOR_SETTING>::iterator itr = vIndicator.begin();

			DWORD i = 0;

			while( i < vIndicator.size() ){
				if( IS_OSCILLATOR(vIndicator[i].dwIndiType ) != FALSE ){
					pView->DeleteIndicatorSetting( vIndicator[i].dwSettingId  );

					bDel = TRUE;
				}else{
					i++;
				}
			}
		}
	}

	if( bDel != FALSE ){
		ViewTreeList();
	}
	return;
}


void CIndicatorView::OnAddAdx()
{
	CIndicatorADX Dlg;

	if( Dlg.DoModal() == IDOK ){
		INDICATOR_SETTING IS;

		IS.dwIndiType = INDICATOR_TYPE_ADX;
		IS.bb.dwSpan = Dlg.m_dwSpan;

		//	現仕様だと一個しか表示できんのよ
		DelOtherOscillator();

		AddIndicator( &IS );
	}
}


void CIndicatorView::OnAddRSI()
{
	CIndicatorRSI Dlg;

	if( Dlg.DoModal() == IDOK ){
		INDICATOR_SETTING IS;

		IS.dwIndiType = INDICATOR_TYPE_RSI;
		IS.rsi.dwSpan = Dlg.m_dwSpan;

		//	現仕様だと一個しか表示できんのよ
		DelOtherOscillator();

		AddIndicator( &IS );
	}
}

void CIndicatorView::OnAddSO()
{
	CIndicatorSO Dlg;

	if( Dlg.DoModal() == IDOK ){
		INDICATOR_SETTING IS;

		IS.dwIndiType = INDICATOR_TYPE_SO;
		IS.so.dwKSpan = Dlg.m_dwKSpan;
		IS.so.dwDSpan = Dlg.m_dwDSpan;
		IS.so.dwSSpan = Dlg.m_dwSSpan;

		//	現仕様だと一個しか表示できんのよ
		DelOtherOscillator();

		AddIndicator( &IS );
	}
}


void CIndicatorView::OnAddMACD()
{
	CIndicatorMACD Dlg;

	if( Dlg.DoModal() == IDOK ){
		INDICATOR_SETTING IS;

		IS.dwIndiType = INDICATOR_TYPE_MACD;
		IS.macd.dwSSpan = Dlg.m_dwSSpan;
		IS.macd.dwLSpan = Dlg.m_dwLSpan;
		IS.macd.dwSignal = Dlg.m_dwSignal;

		//	現仕様だと一個しか表示できんのよ
		DelOtherOscillator();

		AddIndicator( &IS );
	}
}


void CIndicatorView::OnAddIchimoku()
{
	INDICATOR_SETTING IS;

	IS.dwIndiType = INDICATOR_TYPE_ICHIMOKU;
	//IS.bb.dwSpan = Dlg.m_dwSpan;

	AddIndicator( &IS );
}


void CIndicatorView::OnAddRCI()
{
	
	CIndicatorRCI Dlg;

	if( Dlg.DoModal() == IDOK ){
		INDICATOR_SETTING IS;

		IS.dwIndiType = INDICATOR_TYPE_RCI;
		IS.rci.dwSpan1 = Dlg.m_dwSpan1;
		IS.rci.dwSpan2 = Dlg.m_dwSpan2;
		IS.rci.dwSpan3 = Dlg.m_dwSpan3;

		//	現仕様だと一個しか表示できんのよ
		DelOtherOscillator();

		AddIndicator( &IS );
	}
}


void CIndicatorView::OnUpdateAddBb(CCmdUI *pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
}


void CIndicatorView::OnIndicatorviewDel()
{
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());
	if(pMainFrame != NULL )
	{
		CCsvDataViewerView* pView = ((CMainFrame*)pMainFrame)->ExistingView();
		//CCsvDataViewerView* pView = (CCsvDataViewerView*)((CMainFrame*)pMainFrame)->GetActiveView();

		if( pView != NULL ){
			HTREEITEM hTree = m_wndIndicatorView.GetNextItem(TVI_ROOT, TVGN_CARET);

			DWORD dwSettingId = (DWORD)m_wndIndicatorView.GetItemData( hTree );

			pView->DeleteIndicatorSetting( dwSettingId );

			m_wndIndicatorView.DeleteItem( hTree );
		}
	}
}
