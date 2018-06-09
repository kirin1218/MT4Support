// IndicatorSO.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "IndicatorSO.h"
#include "afxdialogex.h"


// CIndicatorSO ダイアログ

IMPLEMENT_DYNAMIC(CIndicatorSO, CDialogEx)

CIndicatorSO::CIndicatorSO(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IND_SO, pParent)
{

}

CIndicatorSO::~CIndicatorSO()
{
}

void CIndicatorSO::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIndicatorSO, CDialogEx)
	ON_BN_CLICKED(IDOK, &CIndicatorSO::OnBnClickedOk)
END_MESSAGE_MAP()


// CIndicatorSO メッセージ ハンドラー


void CIndicatorSO::OnBnClickedOk()
{
	char cEdit[64];
	
	((CEdit*)GetDlgItem( IDC_ED_KSPAN ))->GetWindowTextA( cEdit, sizeof(cEdit) );
	m_dwKSpan = atoi( cEdit );

	((CEdit*)GetDlgItem( IDC_ED_DSPAN ))->GetWindowTextA( cEdit, sizeof(cEdit) );
	m_dwDSpan = atoi( cEdit );

	((CEdit*)GetDlgItem( IDC_ED_SSPAN ))->GetWindowTextA( cEdit, sizeof(cEdit) );
	m_dwSSpan = atoi( cEdit );

	if( m_dwKSpan != 0 && m_dwDSpan != 0 /*&& m_dwSSpan != 0*/ ){
		CDialogEx::OnOK();
	}
}


BOOL CIndicatorSO::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CEdit*)GetDlgItem( IDC_ED_KSPAN ))->SetWindowTextA( "9" );
	((CEdit*)GetDlgItem( IDC_ED_DSPAN ))->SetWindowTextA( "3" );
	((CEdit*)GetDlgItem( IDC_ED_SSPAN ))->SetWindowTextA( "3" );

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
