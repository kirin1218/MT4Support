// IndicatorRSI.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "IndicatorRSI.h"
#include "afxdialogex.h"


// CIndicatorRSI ダイアログ

IMPLEMENT_DYNAMIC(CIndicatorRSI, CDialogEx)

CIndicatorRSI::CIndicatorRSI(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IND_RSI, pParent)
{

}

CIndicatorRSI::~CIndicatorRSI()
{
}

void CIndicatorRSI::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIndicatorRSI, CDialogEx)
	ON_BN_CLICKED(IDOK, &CIndicatorRSI::OnBnClickedOk)
END_MESSAGE_MAP()


// CIndicatorRSI メッセージ ハンドラー


void CIndicatorRSI::OnBnClickedOk()
{
	char cEdit[64];
	
	((CEdit*)GetDlgItem( IDC_ED_SPAN ))->GetWindowTextA( cEdit, sizeof(cEdit) );
	m_dwSpan = atoi( cEdit );

	if( m_dwSpan != 0 ){
		CDialogEx::OnOK();
	}
}


BOOL CIndicatorRSI::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
