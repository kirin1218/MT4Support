#pragma once


// CIndicatorBB ダイアログ

class CIndicatorBB : public CDialogEx
{
	DECLARE_DYNAMIC(CIndicatorBB)

public:
	CIndicatorBB(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CIndicatorBB();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IND_BB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	DWORD m_dwSpan;
	DWORD m_dwSigma;
	virtual BOOL OnInitDialog();
};
