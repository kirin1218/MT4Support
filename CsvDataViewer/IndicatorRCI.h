#pragma once


// CIndicatorRCI �_�C�A���O

class CIndicatorRCI : public CDialogEx
{
	DECLARE_DYNAMIC(CIndicatorRCI)

public:
	CIndicatorRCI(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CIndicatorRCI();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IND_RCI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	DWORD m_dwSpan1;
	DWORD m_dwSpan2;
	DWORD m_dwSpan3;
	virtual BOOL OnInitDialog();
};
