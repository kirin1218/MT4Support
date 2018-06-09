#pragma once


// CIndicatorMA �_�C�A���O

class CIndicatorMA : public CDialogEx
{
	DECLARE_DYNAMIC(CIndicatorMA)

public:
	CIndicatorMA(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CIndicatorMA();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IND_MA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelColor();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	DWORD m_dwSpan;
	COLORREF m_Color;

	virtual BOOL OnInitDialog();
};
