#pragma once


// CIndicatorRSI �_�C�A���O

class CIndicatorRSI : public CDialogEx
{
	DECLARE_DYNAMIC(CIndicatorRSI)

public:
	CIndicatorRSI(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CIndicatorRSI();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IND_RSI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwSpan;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
