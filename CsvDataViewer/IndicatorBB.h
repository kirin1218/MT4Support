#pragma once


// CIndicatorBB �_�C�A���O

class CIndicatorBB : public CDialogEx
{
	DECLARE_DYNAMIC(CIndicatorBB)

public:
	CIndicatorBB(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CIndicatorBB();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IND_BB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	DWORD m_dwSpan;
	DWORD m_dwSigma;
	virtual BOOL OnInitDialog();
};
