#pragma once


// CIndicatorMACD ダイアログ

class CIndicatorMACD : public CDialogEx
{
	DECLARE_DYNAMIC(CIndicatorMACD)

public:
	CIndicatorMACD(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CIndicatorMACD();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IND_MACD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	DWORD m_dwSSpan;
	DWORD m_dwLSpan;
	DWORD m_dwSignal;

};
