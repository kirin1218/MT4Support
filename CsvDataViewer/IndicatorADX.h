#pragma once


// CIndicatorADX �_�C�A���O

class CIndicatorADX : public CDialogEx
{
	DECLARE_DYNAMIC(CIndicatorADX)

public:
	CIndicatorADX(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CIndicatorADX();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IND_ADX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	DWORD m_dwSpan;
	virtual BOOL OnInitDialog();
};
