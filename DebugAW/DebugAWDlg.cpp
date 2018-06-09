
// DebugAWDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DebugAW.h"
#include "DebugAWDlg.h"
#include "afxdialogex.h"

#include "MT4_Supoort.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDebugAWDlg ダイアログ



CDebugAWDlg::CDebugAWDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEBUGAW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDebugAWDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDebugAWDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDebugAWDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDebugAWDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_INIT, &CDebugAWDlg::OnBnClickedBtnInit)
	ON_BN_CLICKED(IDC_BTN_REGTICK, &CDebugAWDlg::OnRegisterTickInfo)
	ON_BN_CLICKED(IDC_BTN_CLOSECHECK, &CDebugAWDlg::OnBnClickedBtnClosecheck)
	ON_BN_CLICKED(IDC_BTN_REGASK, &CDebugAWDlg::OnBtnRegAsk)
	ON_BN_CLICKED(IDC_BTN_ORDERCHECK, &CDebugAWDlg::OnBtnOrderCheck)
	ON_BN_CLICKED(IDC_BTN_ORDER, &CDebugAWDlg::OnBnClickedBtnOrder)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CDebugAWDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_REF, &CDebugAWDlg::OnBtnRefs)
	ON_BN_CLICKED(IDC_BTN_REGASKFILE, &CDebugAWDlg::OnBtnRegAskFromFile)
END_MESSAGE_MAP()


// CDebugAWDlg メッセージ ハンドラー

BOOL CDebugAWDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CDebugAWDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CDebugAWDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CDebugAWDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDebugAWDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	//CDialogEx::OnOK();
}


void CDebugAWDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}

HMODULE WINAPI LoadLibrarySptDll() 
{
	HMODULE hMod = NULL;
	char  cModPath[1024];
	char* pFileName;

	GetModuleFileName(NULL, cModPath, sizeof(cModPath));

	if ((pFileName = (char*)_mbsrchr( (unsigned char*)cModPath, (unsigned int)'\\')) != NULL) {
		pFileName++;
		lstrcpy( pFileName, "MT4SptDll.dll" );

		hMod = LoadLibraryA(cModPath);
	}

	return hMod;
}

#if 0
_MT4_Initilize@4
_MT4_RegisterRate@12
_MT4_DbgRegisterTickInfo@36
_MT4_TradeCheck@8
_MT4_RegisterTradeInfo@36
_MT4_CheckCloseTrade@8
#endif
//	データの初期化
void CDebugAWDlg::OnBnClickedBtnInit()
{
	HMODULE hMod = LoadLibrarySptDll();

	if( hMod != NULL ){
		MT4_INITILIZE_FUNC* pFunc = (MT4_INITILIZE_FUNC*)GetProcAddress(hMod, "_MT4_Initilize@4");
		
		if (pFunc != NULL) {
			pFunc(0);
		}

		//FreeLibrary( hMod );
	}
}

//	ティック情報を無理やり登録してもらう：：：完全にデバッグ検査用
void CDebugAWDlg::OnRegisterTickInfo()
{
	HMODULE hMod = LoadLibrarySptDll();
	DWORD dwPair;
	double dHi, dLo, dOp, dCl;
	char cEdit[64];


	if (((CButton*)GetDlgItem(IDC_RDO_UJ3))->GetCheck() != 0) {
		dwPair = 0;
	}else if (((CButton*)GetDlgItem(IDC_RDO_EJ3))->GetCheck() != 0) {
		dwPair = 1;
	}else if (((CButton*)GetDlgItem(IDC_RDO_EU3))->GetCheck() != 0) {
		dwPair = 2;
	}

	((CEdit*)GetDlgItem(IDC_ED_HI))->GetWindowTextA(cEdit, sizeof(cEdit));
	if (cEdit[0] == 0x00)	return;
	dHi = atof(cEdit);

	((CEdit*)GetDlgItem(IDC_ED_LO))->GetWindowTextA(cEdit, sizeof(cEdit));
	if (cEdit[0] == 0x00)	return;
	dLo = atof(cEdit);

	((CEdit*)GetDlgItem(IDC_ED_OPEN))->GetWindowTextA(cEdit, sizeof(cEdit));
	if (cEdit[0] == 0x00)	return;
	dOp = atof(cEdit);

	((CEdit*)GetDlgItem(IDC_ED_CLOSE))->GetWindowTextA(cEdit, sizeof(cEdit));
	if (cEdit[0] == 0x00)	return;
	dCl = atof(cEdit);
		
	if (hMod != NULL) {
		MT4_DBGREGISTERTICKINFO_FUNC* pFunc = (MT4_DBGREGISTERTICKINFO_FUNC*)GetProcAddress(hMod, "_MT4_DbgRegisterTickInfo@36");

		if (pFunc != NULL) {
			pFunc( dwPair, dHi, dLo, dOp, dCl );
		}

		//FreeLibrary(hMod);
	}
}


void CDebugAWDlg::OnBnClickedBtnOrdercheck()
{
	HMODULE hMod = LoadLibrarySptDll();
	DWORD dwPair;
	int nBreakSign;
	int nRet;

	if (((CButton*)GetDlgItem(IDC_RDO_UJ4))->GetCheck() != 0) {
		dwPair = 0;
	}
	else if (((CButton*)GetDlgItem(IDC_RDO_EJ4))->GetCheck() != 0) {
		dwPair = 1;
	}
	else if (((CButton*)GetDlgItem(IDC_RDO_EU4))->GetCheck() != 0) {
		dwPair = 2;
	}

	if (hMod != NULL) {
		MT4_TRADECHECK_FUNC* pFunc = (MT4_TRADECHECK_FUNC*)GetProcAddress(hMod, "_MT4_TradeCheck@8");

		if (pFunc != NULL) {
			nRet = pFunc(dwPair,&nBreakSign);
		}

		//FreeLibrary(hMod);
	}
}

void CDebugAWDlg::OnBtnRegAsk()
{
	HMODULE hMod = LoadLibrarySptDll();
	DWORD dwPair;
	BOOL bRet;
	char cEdit[64];
	double dAsk;

	if (((CButton*)GetDlgItem(IDC_RDO_UJ))->GetCheck() != 0) {
		dwPair = 0;
	}
	else if (((CButton*)GetDlgItem(IDC_RDO_EJ))->GetCheck() != 0) {
		dwPair = 1;
	}
	else if (((CButton*)GetDlgItem(IDC_RDO_EU))->GetCheck() != 0) {
		dwPair = 2;
	}

	((CEdit*)GetDlgItem(IDC_ED_ASKVAL))->GetWindowTextA(cEdit, sizeof(cEdit));
	if (cEdit[0] == 0x00)	return;
	dAsk = atof(cEdit);

	if (hMod != NULL) {
		MT4_REGISTERRATE_FUNC* pFunc = (MT4_REGISTERRATE_FUNC*)GetProcAddress(hMod, "_MT4_RegisterRate@12");

		if (pFunc != NULL) {
			bRet = pFunc(dwPair, dAsk);
		}

		//FreeLibrary(hMod);
	}
}


void CDebugAWDlg::OnBnClickedBtnClosecheck()
{
	HMODULE hMod = LoadLibrarySptDll();
	DWORD dwPair;
	BOOL bRet;
	char cEdit[64];
	DWORD dwTicket;

	if (((CButton*)GetDlgItem(IDC_RDO_UJ4))->GetCheck() != 0) {
		dwPair = 0;
	}
	else if (((CButton*)GetDlgItem(IDC_RDO_EJ4))->GetCheck() != 0) {
		dwPair = 1;
	}
	else if (((CButton*)GetDlgItem(IDC_RDO_EU4))->GetCheck() != 0) {
		dwPair = 2;
	}

	((CEdit*)GetDlgItem(IDC_ED_TICKET))->GetWindowTextA(cEdit, sizeof(cEdit));
	if (cEdit[0] == 0x00)	return;
	dwTicket = atoi(cEdit);

	if (hMod != NULL) {
		MT4_CHECKCLOSETRADE_FUNC* pFunc = (MT4_CHECKCLOSETRADE_FUNC*)GetProcAddress(hMod, "_MT4_CheckCloseTrade@8");

		if (pFunc != NULL) {
			bRet = pFunc(dwPair, dwTicket);
		}

		//FreeLibrary(hMod);
	}
}

void CDebugAWDlg::OnBtnOrderCheck()
{
	HMODULE hMod = LoadLibrarySptDll();
	DWORD dwPair;
	BOOL bRet;
	int nBreakSign;

	if (((CButton*)GetDlgItem(IDC_RDO_UJ4))->GetCheck() != 0) {
		dwPair = 0;
	}
	else if (((CButton*)GetDlgItem(IDC_RDO_EJ4))->GetCheck() != 0) {
		dwPair = 1;
	}
	else if (((CButton*)GetDlgItem(IDC_RDO_EU4))->GetCheck() != 0) {
		dwPair = 2;
	}

	if (hMod != NULL) {
		MT4_TRADECHECK_FUNC* pFunc = (MT4_TRADECHECK_FUNC*)GetProcAddress(hMod, "_MT4_TradeCheck@8");

		if (pFunc != NULL) {
			bRet = pFunc(dwPair, &nBreakSign);
		}

		//FreeLibrary(hMod);
	}
}

void CDebugAWDlg::OnBnClickedBtnOrder()
{
	HMODULE hMod = LoadLibrarySptDll();
	DWORD dwPair;

	if (((CButton*)GetDlgItem(IDC_RDO_UJ4))->GetCheck() != 0) {
		dwPair = 0;
	}
	else if (((CButton*)GetDlgItem(IDC_RDO_EJ4))->GetCheck() != 0) {
		dwPair = 1;
	}
	else if (((CButton*)GetDlgItem(IDC_RDO_EU4))->GetCheck() != 0) {
		dwPair = 2;
	}

	if (hMod != NULL) {
		MT4_REGISTERTRADEINFO_FUNC* pFunc = (MT4_REGISTERTRADEINFO_FUNC*)GetProcAddress(hMod, "_MT4_RegisterTradeInfo@36");

		if (pFunc != NULL) {
			//bRet = pFunc(dwPair, &dPrice);
		}

		//FreeLibrary(hMod);
	}
}


void CDebugAWDlg::OnBnClickedBtnClose()
{

	HMODULE hMod = LoadLibrarySptDll();
	DWORD dwPair;
	BOOL bRet;
	double dPrice;
	DWORD dwTicket;
	char cEdit[256];

	if (((CButton*)GetDlgItem(IDC_RDO_UJ4))->GetCheck() != 0) {
		dwPair = 0;
	}
	else if (((CButton*)GetDlgItem(IDC_RDO_EJ4))->GetCheck() != 0) {
		dwPair = 1;
	}
	else if (((CButton*)GetDlgItem(IDC_RDO_EU4))->GetCheck() != 0) {
		dwPair = 2;
	}

	((CEdit*)GetDlgItem(IDC_ED_TICKET))->GetWindowTextA(cEdit, sizeof(cEdit));
	if (cEdit[0] == 0x00)	return;
	dwTicket = atoi(cEdit);

	((CEdit*)GetDlgItem(IDC_ED_TRADE_VAL))->GetWindowTextA(cEdit, sizeof(cEdit));
	if (cEdit[0] == 0x00)	return;
	dPrice = atof(cEdit);

	if (hMod != NULL) {
		MT4_REGISTERCLOSETRADE_FUNC* pFunc = (MT4_REGISTERCLOSETRADE_FUNC*)GetProcAddress(hMod, "_MT4_RegisterCloseTrade@16");

		if (pFunc != NULL) {
			bRet = pFunc(dwPair, dwTicket, dPrice);
		}

		//FreeLibrary(hMod);
	}

}

//	ファイル参照ダイアログ
void CDebugAWDlg::OnBtnRefs()
{
	CFileDialog fd(
		TRUE,   // TRUE:Open, FAlSE:Close
		".aw",     // デフォルトの拡張子
		"",     // デフォルトのファイル名
		OFN_FILEMUSTEXIST | // 既存のファイルのみ
		OFN_HIDEREADONLY | // ReadOnlyのCheckBoxを表示しない
		OFN_LONGNAMES,  // 長いファイル名を使う
		"AskFile      (*.aw)|*.aw|",
		NULL    // 親ウィンドウ
	);

	fd.m_ofn.lpstrInitialDir = (LPCSTR)_LOG_DIR_;

	if (fd.DoModal() == IDOK) {
		((CEdit*)GetDlgItem(IDC_ED_ASKFILE))->SetWindowTextA(fd.GetPathName());
	}
}

#include <random>
void CDebugAWDlg::OnBtnRegAskFromFile()
{
	char cEdit[1024];


	((CEdit*)GetDlgItem(IDC_ED_ASKFILE))->GetWindowTextA(cEdit, sizeof(cEdit));
	if (cEdit[0] == 0x00)	return;

	//	ファイルからAsk情報を読み込む
	vector<vector<string>> RateFileTable;

	if (ReadCsvData(cEdit, RateFileTable) != false) {
		HMODULE hMod = LoadLibrarySptDll();
		DWORD dwPair;
		BOOL bRet;
		char* pFileName;
		double dAsk;

		if ((pFileName = (char*)_mbsrchr((unsigned char*)cEdit, (unsigned int)'\\')) != NULL) {
			pFileName++;

			dwPair = GetPairIdFromStrI( pFileName );

			if( dwPair == PAIR_UNKNOWN ) {
				if (((CButton*)GetDlgItem(IDC_RDO_UJ2))->GetCheck() != 0) {
					dwPair = 0;
				}
				else if (((CButton*)GetDlgItem(IDC_RDO_EJ2))->GetCheck() != 0) {
					dwPair = 1;
				}
				else if (((CButton*)GetDlgItem(IDC_RDO_EU2))->GetCheck() != 0) {
					dwPair = 2;
				}
			}


			if (hMod != NULL) {
				MT4_REGISTERRATE_FUNC* pFunc = (MT4_REGISTERRATE_FUNC*)GetProcAddress(hMod, "_MT4_RegisterRate@12");

				if (pFunc != NULL) {
					vector<vector<string>>::iterator itr = RateFileTable.begin();

//					std::random_device rnd;     // 非決定的な乱数生成器

					for (;itr != RateFileTable.end();itr++) {
						string strAsk = (*itr).at(1);
						dAsk = atof(strAsk.c_str());

						bRet = pFunc(dwPair, dAsk);
//						Sleep(rnd()%300);
					}
				}

				//FreeLibrary(hMod);
			}
		}

	}
}

//
//  CSVを取得する
//      filename    読み込むファイル名
//      table       読み込んだCSVの内容
//      delimiter   区切り文字(今回はデフォルトでカンマ)
//
bool CDebugAWDlg::ReadCsvData(const char* filename, vector<vector<string>>& vec_table )
{
	const char delimiter = ',';
	// ファイルを開く
	fstream filestream(filename);
	if (!filestream.is_open())
	{
		// ファイルが開けなかった場合は終了する
		return false;
	}

	vec_table.clear();

	// ファイルを読み込む
	while (!filestream.eof())
	{
		// １行読み込む
		string buffer;
		filestream >> buffer;

		// ファイルから読み込んだ１行の文字列を区切り文字で分けてリストに追加する
		vector<string> record;              // １行分の文字列のリスト
		istringstream streambuffer(buffer); // 文字列ストリーム
		string token;                       // １セル分の文字列
		while (getline(streambuffer, token, delimiter))
		{
			// １セル分の文字列をリストに追加する
			record.push_back(token);
		}

		// １行分の文字列を出力引数のリストに追加する
		vec_table.push_back(record);
	}

	if (vec_table.size() > 0) {
		return true;
	}
	else {
		return false;
	}
}