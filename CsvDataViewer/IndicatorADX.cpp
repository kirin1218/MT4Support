// IndicatorADX.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "IndicatorADX.h"
#include "afxdialogex.h"


// CIndicatorADX �_�C�A���O

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


// CIndicatorADX ���b�Z�[�W �n���h���[


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

	// TODO: �����ɏ�������ǉ����Ă�������

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
