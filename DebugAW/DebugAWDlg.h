
// DebugAWDlg.h : ヘッダー ファイル
//

#pragma once

// インクルード
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>  // ※istringstreamを使うために必要
// 名前空間
using namespace std;


// CDebugAWDlg ダイアログ
class CDebugAWDlg : public CDialogEx
{
// コンストラクション
public:
	CDebugAWDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEBUGAW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnInit();
	afx_msg void OnRegisterTickInfo();
	afx_msg void OnBnClickedBtnOrdercheck();
	afx_msg void OnBnClickedBtnClosecheck();
	afx_msg void OnBtnRegAsk();
	afx_msg void OnBtnOrderCheck();
	afx_msg void OnBnClickedBtnOrder();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBtnRefs();
	afx_msg void OnBtnRegAskFromFile();

	bool ReadCsvData(const char* filename, vector<vector<string>>& vec_table);
};
