
// ConvetLogFileDlg.h : ヘッダー ファイル
//

#pragma once

// インクルード
#include "MT4_Supoort.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>  // ※istringstreamを使うために必要
// 名前空間
using namespace std;

// CConvetLogFileDlg ダイアログ
class CConvetLogFileDlg : public CDialogEx
{
// コンストラクション
public:
	CConvetLogFileDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONVETLOGFILE_DIALOG };
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
	afx_msg void OnBnClickedBtnRefs();

	bool ReadCsvData(const char* filename, vector<vector<string>>& vec_table);
	bool ReadTickFileVer1(char* filepath, vector<TICK_INFO_VER1>& vec_table);
	bool WriteTickFileVer1to2(char* filepath, vector<TICK_INFO_VER1>& vec_table);
	bool WriteTickFileVer0to2(char* filepath, vector<vector<string>>& vec_table);
};
