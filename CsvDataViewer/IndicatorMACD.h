#pragma once


// CIndicatorMACD �_�C�A���O

class CIndicatorMACD : public CDialogEx
{
	DECLARE_DYNAMIC(CIndicatorMACD)

public:
	CIndicatorMACD(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CIndicatorMACD();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IND_MACD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	DWORD m_dwSSpan;
	DWORD m_dwLSpan;
	DWORD m_dwSignal;

};
