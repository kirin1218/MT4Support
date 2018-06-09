// IndicatorRCI.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "IndicatorRCI.h"
#include "afxdialogex.h"


// CIndicatorRCI ダイアログ

IMPLEMENT_DYNAMIC(CIndicatorRCI, CDialogEx)

CIndicatorRCI::CIndicatorRCI(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IND_RCI, pParent)
{

}

CIndicatorRCI::~CIndicatorRCI()
{
}

void CIndicatorRCI::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIndicatorRCI, CDialogEx)
	ON_BN_CLICKED(IDOK, &CIndicatorRCI::OnBnClickedOk)
END_MESSAGE_MAP()


// CIndicatorRCI メッセージ ハンドラー


void CIndicatorRCI::OnBnClickedOk()
{
	char cEdit[64];
	
	((CEdit*)GetDlgItem( IDC_ED_SPAN1 ))->GetWindowTextA( cEdit, sizeof(cEdit) );
	m_dwSpan1 = atoi( cEdit );

	((CEdit*)GetDlgItem( IDC_ED_SPAN2 ))->GetWindowTextA( cEdit, sizeof(cEdit) );
	m_dwSpan2 = atoi( cEdit );

	((CEdit*)GetDlgItem( IDC_ED_SPAN3 ))->GetWindowTextA( cEdit, sizeof(cEdit) );
	m_dwSpan3 = atoi( cEdit );

	if( m_dwSpan1 != 0 ){
		CDialogEx::OnOK();
	}
}


BOOL CIndicatorRCI::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください
	((CEdit*)GetDlgItem( IDC_ED_SPAN1 ))->SetWindowTextA( "9" );
	((CEdit*)GetDlgItem( IDC_ED_SPAN2 ))->SetWindowTextA( "26" );
	((CEdit*)GetDlgItem( IDC_ED_SPAN3 ))->SetWindowTextA( "52" );
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
