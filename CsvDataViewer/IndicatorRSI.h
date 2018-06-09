#pragma once


// CIndicatorRSI ダイアログ

class CIndicatorRSI : public CDialogEx
{
	DECLARE_DYNAMIC(CIndicatorRSI)

public:
	CIndicatorRSI(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CIndicatorRSI();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IND_RSI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwSpan;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
