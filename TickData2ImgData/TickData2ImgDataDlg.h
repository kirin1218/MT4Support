
// TickData2ImgDataDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxwin.h"

#include "MT4_Supoort.h"

// インクルード
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>  // ※istringstreamを使うために必要
// 名前空間
using namespace std;

//#define IMG_SIZE		(224)
#define IMG_SIZE		(256)
#define TICKBAR_WIDTH	(3)
#define TICK_SPACE		(1)
#define IMG_DATA_SIZE	((DWORD)((IMG_SIZE+(TICKBAR_WIDTH+TICK_SPACE)-1)/(TICKBAR_WIDTH+TICK_SPACE)))

// CTickData2ImgDataDlg ダイアログ
class CTickData2ImgDataDlg : public CDialogEx
{
// コンストラクション
public:
	CTickData2ImgDataDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TICKDATA2IMGDATA_DIALOG };
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
	CEdit m_EdSrc;

	vector<ADF_DATA> m_TickDataList;
	vector<TICK_INFO> m_70TickDataList;
	vector< vector< vector<BYTE> >> m_ImgList;//(IMG_SIZE, vector<BYTE>(IMG_SIZE) );
	vector<BYTE>m_LabelList;
	DWORD MakeTickDataList( std::vector<std::string>& PathList );
	DWORD Make70TickDataList( DWORD dwPair );
	DWORD ConvertTick2Img();
	int GetLabelNo( DWORD dwStart, DWORD dwStartVal );
	vector< vector<COLORREF> > MakeImg( DWORD dwStart, DWORD dwEnd, DWORD dwOverLine, DWORD dwCenter, DWORD dwUnderLine );
	BOOL ImgFileOut();
};
