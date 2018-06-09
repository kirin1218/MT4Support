// IndicatorRSI.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "IndicatorRSI.h"
#include "afxdialogex.h"


// CIndicatorRSI �_�C�A���O

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


// CIndicatorRSI ���b�Z�[�W �n���h���[


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

	// TODO: �����ɏ�������ǉ����Ă�������

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
