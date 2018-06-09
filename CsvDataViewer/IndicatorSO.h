#pragma once


// CIndicatorSO ダイアログ

class CIndicatorSO : public CDialogEx
{
	DECLARE_DYNAMIC(CIndicatorSO)

public:
	CIndicatorSO(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CIndicatorSO();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IND_SO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	DWORD m_dwKSpan;
	DWORD m_dwDSpan;
	DWORD m_dwSSpan;
	virtual BOOL OnInitDialog();
};
