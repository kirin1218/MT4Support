// IndicatorADX.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "IndicatorADX.h"
#include "afxdialogex.h"


// CIndicatorADX ダイアログ

IMPLEMENT_DYNAMIC(CIndicatorADX, CDialogEx)

CIndicatorADX::CIndicatorADX(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IND_ADX, pParent)
{

}

CIndicatorADX::~CIndicatorADX()
{
}

void CIndicatorADX::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIndicatorADX, CDialogEx)
	ON_BN_CLICKED(IDOK, &CIndicatorADX::OnBnClickedOk)
END_MESSAGE_MAP()


// CIndicatorADX メッセージ ハンドラー


void CIndicatorADX::OnBnClickedOk()
{
	char cEdit[64];
	
	((CEdit*)GetDlgItem( IDC_ED_SPAN ))->GetWindowTextA( cEdit, sizeof(cEdit) );
	m_dwSpan = atoi( cEdit );

	if( m_dwSpan != 0 ){
		CDialogEx::OnOK();
	}
}


BOOL CIndicatorADX::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
