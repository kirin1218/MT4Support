// IndicatorMACD.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "IndicatorMACD.h"
#include "afxdialogex.h"


// CIndicatorMACD �_�C�A���O

IMPLEMENT_DYNAMIC(CIndicatorMACD, CDialogEx)

CIndicatorMACD::CIndicatorMACD(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IND_MACD, pParent)
{

}

CIndicatorMACD::~CIndicatorMACD()
{
}

void CIndicatorMACD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIndicatorMACD, CDialogEx)
	ON_BN_CLICKED(IDOK, &CIndicatorMACD::OnBnClickedOk)
END_MESSAGE_MAP()


// CIndicatorMACD ���b�Z�[�W �n���h���[


BOOL CIndicatorMACD::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CEdit*)GetDlgItem( IDC_ED_SSPAN ))->SetWindowTextA( "12" );
	((CEdit*)GetDlgItem( IDC_ED_LSPAN ))->SetWindowTextA( "26" );
	((CEdit*)GetDlgItem( IDC_ED_SIGNAL ))->SetWindowTextA( "9" );

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CIndicatorMACD::OnBnClickedOk()
{
	char cEdit[64];
	
	((CEdit*)GetDlgItem( IDC_ED_SSPAN ))->GetWindowTextA( cEdit, sizeof(cEdit) );
	m_dwSSpan = atoi( cEdit );

	((CEdit*)GetDlgItem( IDC_ED_LSPAN ))->GetWindowTextA( cEdit, sizeof(cEdit) );
	m_dwLSpan = atoi( cEdit );

	((CEdit*)GetDlgItem( IDC_ED_SIGNAL ))->GetWindowTextA( cEdit, sizeof(cEdit) );
	m_dwSignal = atoi( cEdit );

	if( m_dwLSpan != 0 && m_dwSSpan != 0 && m_dwSignal != 0 ){
		// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
		CDialogEx::OnOK();
	}
}
