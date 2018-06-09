#pragma once


// CIndicatorRCI ダイアログ

class CIndicatorRCI : public CDialogEx
{
	DECLARE_DYNAMIC(CIndicatorRCI)

public:
	CIndicatorRCI(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CIndicatorRCI();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IND_RCI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	DWORD m_dwSpan1;
	DWORD m_dwSpan2;
	DWORD m_dwSpan3;
	virtual BOOL OnInitDialog();
};
