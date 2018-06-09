// IndicatorBB.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "IndicatorBB.h"
#include "afxdialogex.h"


// CIndicatorBB ダイアログ

IMPLEMENT_DYNAMIC(CIndicatorBB, CDialogEx)

CIndicatorBB::CIndicatorBB(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IND_BB, pParent)
{

}

CIndicatorBB::~CIndicatorBB()
{
}

void CIndicatorBB::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIndicatorBB, CDialogEx)
	ON_BN_CLICKED(IDOK, &CIndicatorBB::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CIndicatorBB::OnBnClickedCancel)
END_MESSAGE_MAP()


// CIndicatorBB メッセージ ハンドラー


void CIndicatorBB::OnBnClickedOk()
{
	char cEdit[64];

	((CEdit*)GetDlgItem( IDC_ED_SPAN ))->GetWindowTextA( cEdit, sizeof(cEdit) );
	m_dwSpan = atoi( cEdit );
	((CEdit*)GetDlgItem( IDC_ED_SIGMA ))->GetWindowTextA( cEdit, sizeof(cEdit) );
	m_dwSigma = atoi( cEdit );

	if( m_dwSpan != 0 && ( m_dwSigma == 2 || m_dwSigma == 3 ) ){
		CDialogEx::OnOK();
	}
}


void CIndicatorBB::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}


BOOL CIndicatorBB::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
