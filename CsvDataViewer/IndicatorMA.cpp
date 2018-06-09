// IndicatorMA.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "IndicatorMA.h"
#include "afxdialogex.h"


// CIndicatorMA ダイアログ

IMPLEMENT_DYNAMIC(CIndicatorMA, CDialogEx)

CIndicatorMA::CIndicatorMA(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IND_MA, pParent)
{

}

CIndicatorMA::~CIndicatorMA()
{
}

void CIndicatorMA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIndicatorMA, CDialogEx)
	ON_BN_CLICKED(IDC_SEL_COLOR, &CIndicatorMA::OnBnClickedSelColor)
	ON_BN_CLICKED(IDOK, &CIndicatorMA::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CIndicatorMA::OnBnClickedCancel)
END_MESSAGE_MAP()


// CIndicatorMA メッセージ ハンドラー


void CIndicatorMA::OnBnClickedSelColor()
{
	CColorDialog Dlg;

	if( Dlg.DoModal() == IDOK ){
		COLORREF color = Dlg.GetColor();
		char cData[32];

		wsprintf( cData, "%X", GetRValue( color ) );
		//DWORD dwTest = strtol( cData, NULL, 16 );
		((CEdit*)GetDlgItem( IDC_ED_R ) )->SetWindowTextA( cData );
		wsprintf( cData, "%X", GetGValue( color ) );
		((CEdit*)GetDlgItem( IDC_ED_G ) )->SetWindowTextA( cData );
		wsprintf( cData, "%X", GetBValue( color ) );
		((CEdit*)GetDlgItem( IDC_ED_B ) )->SetWindowTextA( cData );
	}
}


void CIndicatorMA::OnBnClickedOk()
{
	char cEdit[64];

	((CEdit*)GetDlgItem( IDC_ED_SPAN ))->GetWindowTextA( cEdit, sizeof(cEdit) );
	m_dwSpan = atoi( cEdit );

	char cData[32];
	DWORD dwR,dwG,dwB;

	((CEdit*)GetDlgItem( IDC_ED_R ) )->GetWindowTextA( cData, sizeof(cData) );
	dwR = strtol( cData, NULL, 16 );
	((CEdit*)GetDlgItem( IDC_ED_G ) )->GetWindowTextA( cData, sizeof(cData) );
	dwG = strtol( cData, NULL, 16 );
	((CEdit*)GetDlgItem( IDC_ED_B ) )->GetWindowTextA( cData, sizeof(cData) );
	dwB = strtol( cData, NULL, 16 );

	m_Color = RGB( dwR, dwG, dwB );

	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnOK();
}


void CIndicatorMA::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}


BOOL CIndicatorMA::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
