// IndicatorBB.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "IndicatorBB.h"
#include "afxdialogex.h"


// CIndicatorBB �_�C�A���O

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


// CIndicatorBB ���b�Z�[�W �n���h���[


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
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialogEx::OnCancel();
}


BOOL CIndicatorBB::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
