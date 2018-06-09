#pragma once
#include "MT4_Supoort.h"



// CTestTradeDlg ダイアログ

class CTestTradeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestTradeDlg)

public:
	CTestTradeDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CTestTradeDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_TESTMODE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	void WriteSettingData(SETTING_PRAMETER* pTP, char* pDataPath, char* pTestName);
	void ReadSettingData(SETTING_PRAMETER* pSetting);
	char m_cTestName[256];
	DWORD m_dwPair;
};
