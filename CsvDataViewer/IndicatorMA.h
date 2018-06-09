#pragma once


// CIndicatorMA ダイアログ

class CIndicatorMA : public CDialogEx
{
	DECLARE_DYNAMIC(CIndicatorMA)

public:
	CIndicatorMA(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CIndicatorMA();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IND_MA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelColor();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	DWORD m_dwSpan;
	COLORREF m_Color;

	virtual BOOL OnInitDialog();
};
