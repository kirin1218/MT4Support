#pragma once


// CIndicatorADX ダイアログ

class CIndicatorADX : public CDialogEx
{
	DECLARE_DYNAMIC(CIndicatorADX)

public:
	CIndicatorADX(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CIndicatorADX();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IND_ADX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	DWORD m_dwSpan;
	virtual BOOL OnInitDialog();
};
