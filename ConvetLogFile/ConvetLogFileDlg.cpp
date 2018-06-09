
// ConvetLogFileDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ConvetLogFile.h"
#include "ConvetLogFileDlg.h"
#include "afxdialogex.h"

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


// CConvetLogFileDlg ダイアログ



CConvetLogFileDlg::CConvetLogFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONVETLOGFILE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConvetLogFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CConvetLogFileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CConvetLogFileDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_REFS, &CConvetLogFileDlg::OnBnClickedBtnRefs)
END_MESSAGE_MAP()


// CConvetLogFileDlg メッセージ ハンドラー

BOOL CConvetLogFileDlg::OnInitDialog()
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

void CConvetLogFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CConvetLogFileDlg::OnPaint()
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
HCURSOR CConvetLogFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CConvetLogFileDlg::WriteTickFileVer0to2(char* filepath, vector<vector<string>>& vec_table ) {
	bool bRet = false;
	char cNewFilePath[1024];
	char* pExt;
	HANDLE hFile;
	DWORD dwWrite;
	TICK_INFO_VER2 writedata;
	char cDate[32];

	if (vec_table.size() == 0)	return false;

	lstrcpy(cNewFilePath, filepath);

	if ((pExt = (char*)_mbsrchr((unsigned char*)cNewFilePath, (const unsigned int)'.')) != NULL) {
		lstrcpy(pExt, "_Ver2.tki");

		if ((hFile = CreateFile(cNewFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL))
			!= INVALID_HANDLE_VALUE) {

			memset(&writedata, 0x00, sizeof(TICK_INFO_VER2));

			//	ファイル名から日付を取得
			char* pFileName;
			char* pCur;
			if ((pFileName = (char*)_mbsrchr((unsigned char*)filepath, (const unsigned int)'\\')) != NULL) {
				if ((pFileName = StrStrI(pFileName, "-cd_")) != NULL) {
					pFileName += sizeof("-cd_") - 1;

					lstrcpy(cDate, pFileName);
					int i = 0;
					while (cDate[i] != 0x00) {
						if (cDate[i] == '.') {
							cDate[i] = 0x00;
						}
						i++;
					}

					pCur = cDate;
					writedata.sDt.wYear = atoi(pCur);
					pCur += lstrlen(pCur) + 1;
					writedata.sDt.wMonth = atoi(pCur);
					pCur += lstrlen(pCur) + 1;
					writedata.sDt.wDay = atoi(pCur);
				}
			}

			//	CSV：一列目から時刻を取得
			lstrcpy( cDate, vec_table[0][0].c_str() );
			int i = 0;
			while (cDate[i] != 0x00) {
				if (cDate[i] == ':') {
					cDate[i] = 0x00;
				}
				i++;
			}

			pCur = cDate;
			writedata.sDt.wHour = atoi(pCur);
			pCur += lstrlen(pCur) + 1;
			writedata.sDt.wMinute = atoi(pCur);
			pCur += lstrlen(pCur) + 1;
			writedata.sDt.wSecond = atoi(pCur);

			memcpy(&writedata.eDt, &writedata.sDt, sizeof(SYSTEMTIME));

			for (i = 0;(DWORD)i < vec_table.size() && vec_table[i].size() >= 6;i++) {
				writedata.Ver = 2;
				writedata.Op = atof(vec_table[i][1].c_str());
				writedata.Hi = atof(vec_table[i][2].c_str());
				writedata.Lo = atof(vec_table[i][3].c_str());
				writedata.Cl = atof(vec_table[i][4].c_str());
				writedata.Tc = 70;
				writedata.ema = atof(vec_table[i][5].c_str());

				WriteFile(hFile, &writedata, sizeof(TICK_INFO_VER2), &dwWrite, NULL);
			}


			bRet = true;

			CloseHandle(hFile);
		}
	}
	return bRet;

}

bool CConvetLogFileDlg::WriteTickFileVer1to2(char* filepath, vector<TICK_INFO_VER1>& vec_table) {
	bool bRet = false;
	char cNewFilePath[1024];
	char* pExt;
	HANDLE hFile;
	DWORD dwWrite;
	TICK_INFO_VER2 writedata;

	lstrcpy(cNewFilePath, filepath);

	if ((pExt = (char*)_mbsrchr( (unsigned char*)cNewFilePath, (const unsigned int )'.')) != NULL) {
		lstrcpy( pExt, "_Ver2.tki" );

		if ((hFile = CreateFile(cNewFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL))
			!= INVALID_HANDLE_VALUE) {
			vector<TICK_INFO_VER1>::iterator itr = vec_table.begin();

			for (;itr != vec_table.end(); itr++) {
				writedata.Ver = 2;
				writedata.Op = (*itr).Op;
				writedata.Hi = (*itr).Hi;
				writedata.Lo = (*itr).Lo;
				writedata.Cl = (*itr).Cl;
				writedata.Tc = (*itr).Tc;
				writedata.ema = (*itr).ema;
				memcpy(&writedata.sDt, &(*itr).sDt, sizeof(SYSTEMTIME) );
				memcpy(&writedata.eDt, &(*itr).eDt, sizeof(SYSTEMTIME) );

				WriteFile(hFile, &writedata, sizeof(TICK_INFO_VER2), &dwWrite, NULL);

				bRet = true;
			}
			
			CloseHandle(hFile);
		}
	}
	return bRet;
}

bool CConvetLogFileDlg::ReadTickFileVer1(char* filepath, vector<TICK_INFO_VER1>& vec_table) {
	bool bRet = false;

	HANDLE hFile;
	TICK_INFO_VER1 readata;
	DWORD dwRead;

	if ((hFile = CreateFile(filepath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL))
		!= INVALID_HANDLE_VALUE
		) {

		while( ReadFile(hFile, &readata, sizeof(TICK_INFO_VER1), &dwRead, NULL) != FALSE && sizeof(TICK_INFO_VER1) == dwRead) {
			if (*((DWORD*)&readata) != 1) {
				vec_table.push_back(readata);
			}
		}
		CloseHandle(hFile);
	}

	if (vec_table.size() > 0) {
		bRet = true;
	}

	return bRet;
}

//
//  CSVを取得する
//      filename    読み込むファイル名
//      table       読み込んだCSVの内容
//      delimiter   区切り文字(今回はデフォルトでカンマ)
//
bool CConvetLogFileDlg::ReadCsvData(const char* filename, vector<vector<string>>& vec_table)
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

void CConvetLogFileDlg::OnBnClickedOk()
{
	//	
	char cEdit[1024];

	((CEdit*)GetDlgItem(IDC_ED_PATH))->GetWindowText(cEdit, sizeof(cEdit));

	if (GetFileAttributes(cEdit) != INVALID_FILE_ATTRIBUTES) {
		if (((CButton*)GetDlgItem(IDC_RDO_LOGASK))->GetCheck() != 0) {
			vector<vector<string>> RateFileTable;

			if (ReadCsvData(cEdit, RateFileTable) != false) {
				;
			}
		}
		//	Tickファイル
		else if (((CButton*)GetDlgItem(IDC_RDO_LOGTICK))->GetCheck() != 0) {
			vector<vector<string>> TickFileTable0;
			vector<TICK_INFO_VER1> TickFileTable;

			if (strstr(cEdit, "-cd_") != NULL) {
				if (ReadCsvData(cEdit, TickFileTable0) != false) {
					WriteTickFileVer0to2(cEdit, TickFileTable0);
				}
			}
			else {
				if (ReadTickFileVer1(cEdit, TickFileTable) != false) {
					WriteTickFileVer1to2(cEdit, TickFileTable);
				}
			}
		}
		else if (((CButton*)GetDlgItem(IDC_RDO_LOGTRADE))->GetCheck() != 0) {
			;
		}
	}

	//CDialogEx::OnOK();
}

void CConvetLogFileDlg::OnBnClickedBtnRefs()
{
	static char szFilter[] = "CsvFile(*.csv)|*.csv|AskDataFile(*.adf)| *.adf|TikcInfoFile(*.tki) | *tki|TradeInfoFile(*.tri) | *.tri||";

	CFileDialog fd(
		TRUE,   // TRUE:Open, FAlSE:Close
		".csv",     // デフォルトの拡張子
		"",     // デフォルトのファイル名
		OFN_FILEMUSTEXIST | // 既存のファイルのみ
		OFN_HIDEREADONLY | // ReadOnlyのCheckBoxを表示しない
		OFN_LONGNAMES,  // 長いファイル名を使う
		szFilter,
		NULL    // 親ウィンドウ
	);


	fd.m_ofn.lpstrInitialDir = (LPCSTR)_LOG_DIR_;

	if (fd.DoModal() == IDOK) {
		((CEdit*)GetDlgItem(IDC_ED_PATH))->SetWindowTextA(fd.GetPathName());
	}
}
