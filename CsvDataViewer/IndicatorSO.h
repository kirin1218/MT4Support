#pragma once


// CIndicatorSO �_�C�A���O

class CIndicatorSO : public CDialogEx
{
	DECLARE_DYNAMIC(CIndicatorSO)

public:
	CIndicatorSO(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CIndicatorSO();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IND_SO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	DWORD m_dwKSpan;
	DWORD m_dwDSpan;
	DWORD m_dwSSpan;
	virtual BOOL OnInitDialog();
};
