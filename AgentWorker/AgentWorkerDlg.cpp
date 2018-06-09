
// AgentWorkerDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "AgentWorker.h"
#include "AgentWorkerDlg.h"
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

DWORD g_REM_TICK_CNT = 70;

#include "pushbullet\pushbullet.hpp"
//#define APIKEY "o.UuRDpiZWF33WVlDh62PJcG2S1Aq2p6vh"
#define APIKEY "o.jbEOSckGPpTjEKfAUAcs44kOnPc7C7aP"
//#define APIKEY "yukirin.my.angel@gmail.com"

BOOL SendPushBullet()
{
	Pushbullet pb(APIKEY);

	//pb.print();
	pb.push_note("Title", "Message body");
	//pb.push_link("Google", "Body", "http://google.com");
	//pb.push_address("Some place", "Szczecin, Poland");
	//pb.push_checklist("List", "[\"one\",\"two\",\"three\"]");

	return TRUE;
}

BOOL SendPushBullet_Chance( char* pMes )
{
	Pushbullet pb(APIKEY);

	pb.push_note("Chance", pMes);

	return TRUE;
}


BOOL SendPushBullet_Attack( char* pMes )
{
	Pushbullet pb(APIKEY);

	pb.push_note("Attack!!!", pMes);

	return TRUE;
}


BOOL SendPushBullet_Finish( char* pMes )
{
	Pushbullet pb(APIKEY);

	pb.push_note("Finish", pMes );

	return TRUE;
}

// CAgentWorkerDlg ダイアログ



CAgentWorkerDlg::CAgentWorkerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_AGENTWORKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAgentWorkerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TICKUJ, m_ListUJ);
	DDX_Control(pDX, IDC_LIST_TICKEJ, m_ListEJ);
	DDX_Control(pDX, IDC_LIST_TICKEU, m_ListEU);
	DDX_Control(pDX, IDC_LIST_TRADE, m_ListTrade);
}

BEGIN_MESSAGE_MAP(CAgentWorkerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CAgentWorkerDlg::OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_TICKUJ, &CAgentWorkerDlg::OnNMCustomdrawListTickuj)
END_MESSAGE_MAP()


// CAgentWorkerDlg メッセージ ハンドラー
void CAgentWorkerDlg::InitilizeTickData( DWORD dwPair )
{
	if( m_TickInfo.find( dwPair ) == m_TickInfo.end() ){
		map<DWORD,vector<TICK_INFO>> newPairInfo;

		m_TickInfo[dwPair] = newPairInfo;
	}

	for( DWORD i = TKI_70TICK; i < TKI_COUNT; i++ ){
		if( m_TickInfo[dwPair].find( i ) == m_TickInfo[dwPair].end() ){
			vector<TICK_INFO> newTickInfo;
			m_TickInfo[dwPair][i] = newTickInfo;
		}

		m_TickInfo[dwPair][i].clear();
	}
}

void CAgentWorkerDlg::InitilizeTickInfo( DWORD dwPair )
{
	CTime CurTime = CTime::GetCurrentTime();
	CTime time;
	char cPath[1024];

	InitilizeTickData( dwPair );

	for( int j = CurTime.GetDayOfWeek() - 1; j >= 0; j-- ){
		time = CurTime - CTimeSpan( j, 0, 0, 0);

		wsprintf(cPath, "%s\\ADF\\%s\\%02d\\%02d\\%02d.adf", _LOG_DIR_, GetPairName(dwPair), time.GetYear(), time.GetMonth(), time.GetDay());

		if( GetFileAttributes(cPath ) != INVALID_FILE_ATTRIBUTES ){
			HANDLE hFile;

			hFile = CreateFile(cPath, GENERIC_READ, FILE_SHARE_READ| FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if( hFile != INVALID_HANDLE_VALUE ){
				DWORD dwFileSize = GetFileSize( hFile, NULL );
				DWORD dwRead;
				char* pData;
				char* pCur;
				char* pPoint;
				char cTime[1024];
				char cRate[1024];

				if( ( pData = (char*)LocalAlloc( LMEM_FIXED, dwFileSize + 1 )) != NULL) {
					ReadFile( hFile, pData, dwFileSize, &dwRead, NULL );
					pData[dwFileSize-1] = 0x00;

					pCur = pData;

					while( *pCur != 0x00 ){
						pPoint = strchr(pCur, ',');
						memcpy(cTime, pCur, (DWORD)(pPoint-pCur) );
						cTime[(DWORD)(pPoint - pCur)] = 0x00;
						pCur = pPoint + 1;

						pPoint = strchr(pCur, '\r');
						if(pPoint != NULL ){
							memcpy(cRate, pCur, (DWORD)(pPoint - pCur));
							cRate[(DWORD)(pPoint - pCur)] = 0x00;
							pCur = pPoint + 1;

							if(*pCur == '\n') {
								pCur++;
							}

						}else{
							lstrcpy(cRate, pCur );
							pCur += lstrlen(cRate);
						}

						if(cTime[0] != 0x00 && cRate[0] != 0x00) {
							double dRate = atof(cRate);

							if(dRate > 0) {	//	原因は不明だけどadfに0が書かれていることがあるため
											//	データファイル内のAsk情報を登録する
								DWORD dwRate = Rate2EVal(dRate, dwPair);
								//TICK_INFO AddData;

								if(dwRate == 0)	return;

								SYSTEMTIME systime;

								//GetLocalTime(&systime);
								systime.wYear = time.GetYear();
								systime.wMonth = time.GetMonth();
								systime.wDay = time.GetDay();

								//	時分秒ミリ秒はファイル情報を使う
								DWORD l;
								DWORD dwTID = 0;

								for(l = 0; cTime[l] != 0x00; l++) {
									if(cTime[l] == ':' || cTime[l] == '.') {
										cTime[l] = 0x00;
									}
								}
								char* pCurTimePtr = cTime;
								systime.wHour = atoi(pCurTimePtr);		pCurTimePtr += lstrlen(pCurTimePtr) + 1;
								systime.wMinute = atoi(pCurTimePtr);	pCurTimePtr += lstrlen(pCurTimePtr) + 1;
								systime.wSecond = atoi(pCurTimePtr);	pCurTimePtr += lstrlen(pCurTimePtr) + 1;
								systime.wMilliseconds = atoi(pCurTimePtr);

								RegisterRate_Main( dwPair, dwRate, &systime );
							}
						}
					}
					//	一行ごとに
					LocalFree(pData);
				}

				CloseHandle(hFile);
			}
		}
	}
}

BOOL CAgentWorkerDlg::OnInitDialog()
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
	char* pCmdLine = GetCommandLine();
	m_bAutoMode = FALSE;
	m_bTestMode = FALSE;
	m_bTaskMode = FALSE;

	//MessageBox("");

	//	テストモードの場合はウィンドウ名も変える
	if( StrStrI(pCmdLine, "-t") != NULL ){
		m_bTestMode = TRUE;
		SetWindowText(AW_TETWINDOW_NAME);
	}

	if(StrStrI(pCmdLine, "-a") != NULL) {
		m_bAutoMode = TRUE;
	}

	if(StrStrI(pCmdLine, "-s") != NULL) {
		m_bTaskMode = TRUE;
	}

	//m_TickInfo.push_back(m_TickInfo_usdjpy);
	//m_TickInfo.push_back(m_TickInfo_eurjpy);
	//m_TickInfo.push_back(m_TickInfo_eurusd);

	/*m_TickInfo[PAIR_USDJPY] = m_TickInfo_usdjpy;
	m_TickInfo[PAIR_EURJPY] = m_TickInfo_eurjpy;
	m_TickInfo[PAIR_EURUSD] = m_TickInfo_eurusd;
	m_TickInfo[PAIR_AUDJPY] = m_TickInfo_audjpy;
	m_TickInfo[PAIR_CHFJPY] = m_TickInfo_chfjpy;
	m_TickInfo[PAIR_GBPJPY] = m_TickInfo_gbpjpy;
	m_TickInfo[PAIR_GBPUSD] = m_TickInfo_gbpusd;
	m_TickInfo[PAIR_NZDJPY] = m_TickInfo_nzdjpy;
	m_TickInfo[PAIR_USDCHF] = m_TickInfo_usdchf;
	m_TickInfo[PAIR_ZARJPY] = m_TickInfo_zarjpy;*/

	//	とりあえずリストコントロールの初期化かな
	LVCOLUMN    lvc;
	char		cCap[][32] = { "時刻", "開始", "終了", "最大", "最小", "データ数", "備考", "" };
	DWORD		dwWdth[] = { 60, 70, 70, 70, 70, 70, 70, 0 };
	int         i;
	DWORD		dwStyle;

	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_FMT;    // 有効フラグ

	for (i = 0; cCap[i][0] != 0x00; i++) {
		lvc.iSubItem = i;            // サブアイテム番号
		lvc.pszText = cCap[i];   // 見出しテキスト
		lvc.cx = dwWdth[i];          // 横幅
		lvc.fmt = LVCFMT_RIGHT;
		m_ListUJ.InsertColumn(i, &lvc);
		m_ListEJ.InsertColumn(i, &lvc);
		m_ListEU.InsertColumn(i, &lvc);
	}

	char		cCapTrade[][32] = { "開始時刻", "ペア", "注文方法", "開始", "終了", "差分", "終了時刻", "" };
	DWORD		dwWdthTrade[] = { 100, 40, 50, 70, 70, 70, 100, 0 };
	for (i = 0; cCapTrade[i][0] != 0x00; i++) {
		lvc.iSubItem = i;            // サブアイテム番号
		lvc.pszText = cCapTrade[i];   // 見出しテキスト
		lvc.cx = dwWdthTrade[i];          // 横幅

		m_ListTrade.InsertColumn(i, &lvc);
	}

	dwStyle = m_ListUJ.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;

	m_ListUJ.SetExtendedStyle(dwStyle);
	m_ListEJ.SetExtendedStyle(dwStyle);
	m_ListEU.SetExtendedStyle(dwStyle);
	m_ListTrade.SetExtendedStyle(dwStyle);

	for( DWORD i = TKI_70TICK; i < TKI_COUNT; i++ ){
		m_hTickFile[i] = INVALID_HANDLE_VALUE;
	}
	m_hTradeFile = INVALID_HANDLE_VALUE;

	m_hCmdOpeThread = NULL;

	if(m_bTestMode!=FALSE){
		ShowWindow(SW_MINIMIZE);

		StartTest();

		ExitProcess(1);

		return FALSE;
	}
	
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

bool CAgentWorkerDlg::ReadADFFile(const char* filename, vector<ADF_DATA>& vec_table)
{
	CTime CurTime = CTime::GetCurrentTime();
	CTime time;
	char cADFPath[1024];
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	
	lstrcpy(cADFPath, filename);

	char* pDir;

	if( ( pDir = StrStrI( cADFPath, "\\ADF\\" ) ) != NULL ){
		pDir += sizeof("\\ADF\\USDJPY\\")-1;

		char cTimeInfo[128];
		DWORD l;

		lstrcpy( cTimeInfo, pDir );

		for(l = 0; cTimeInfo[l] != 0x00; l++) {
			if(cTimeInfo[l] == '\\' || cTimeInfo[l] == '.') {
				cTimeInfo[l] = 0x00;
			}
		}
		char* pCurTimePtr = cTimeInfo;
		systime.wYear = atoi(pCurTimePtr);	pCurTimePtr += lstrlen(pCurTimePtr) + 1;
		systime.wMonth = atoi(pCurTimePtr);	pCurTimePtr += lstrlen(pCurTimePtr) + 1;
		systime.wDay = atoi(pCurTimePtr);

		if(GetFileAttributes(filename) != INVALID_FILE_ATTRIBUTES) {
			HANDLE hFile;

			hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if(hFile != INVALID_HANDLE_VALUE) {
				DWORD dwFileSize = GetFileSize(hFile, NULL);
				DWORD dwRead;
				char* pData;
				char* pCur;
				char* pPoint;
				char cTime[1024];
				char cRate[1024];

				if((pData = (char*)LocalAlloc(LMEM_FIXED, dwFileSize + 1)) != NULL) {
					ADF_DATA AdfData;

					ReadFile(hFile, pData, dwFileSize, &dwRead, NULL);
					pData[dwFileSize - 1] = 0x00;

					pCur = pData;

					while(*pCur != 0x00) {
						pPoint = strchr(pCur, ',');
						memcpy(cTime, pCur, (DWORD)(pPoint - pCur));
						cTime[(DWORD)(pPoint - pCur)] = 0x00;
						pCur = pPoint + 1;

						pPoint = strchr(pCur, '\r');
						if(pPoint != NULL) {
							memcpy(cRate, pCur, (DWORD)(pPoint - pCur));
							cRate[(DWORD)(pPoint - pCur)] = 0x00;
							pCur = pPoint + 1;

							if(*pCur == '\n') {
								pCur++;
							}

						}
						else {
							lstrcpy(cRate, pCur);
							pCur += lstrlen(cRate);
						}

						if(cTime[0] != 0x00 && cRate[0] != 0x00) {
							double dRate = atof(cRate);

							if(dRate > 0) {
								AdfData.dRate = dRate;

								//	時分秒ミリ秒はファイル情報を使う
								DWORD l;
								DWORD dwTID = 0;

								memcpy( &AdfData.time, &systime, sizeof(SYSTEMTIME) );

								for(l = 0; cTime[l] != 0x00; l++) {
									if(cTime[l] == ':' || cTime[l] == '.') {
										cTime[l] = 0x00;
									}
								}
								char* pCurTimePtr = cTime;
								AdfData.time.wHour = atoi(pCurTimePtr);		pCurTimePtr += lstrlen(pCurTimePtr)+1;
								AdfData.time.wMinute = atoi(pCurTimePtr);	pCurTimePtr += lstrlen(pCurTimePtr)+1;
								AdfData.time.wSecond = atoi(pCurTimePtr);	pCurTimePtr += lstrlen(pCurTimePtr)+1;
								AdfData.time.wMilliseconds = atoi(pCurTimePtr);
										
								vec_table.push_back(AdfData);
							}
						}
					}
					//	一行ごとに
					LocalFree(pData);
				}
				CloseHandle(hFile);
			}
		}
	}


	if(vec_table.size() > 0) {
		return true;
	}
	else {
		return false;
	}
}

void CAgentWorkerDlg::WriteSettingData(SETTING_PRAMETER* pTP )
{
	char cIniPath[1024];
	char* pFileName;
	char cWriteBuf[256];

	GetModuleFileName(NULL, cIniPath, sizeof(cIniPath));
	pFileName = (char*)_mbsrchr((unsigned char*)cIniPath, (unsigned int)'\\');

	lstrcpy(pFileName, "\\AgentWorker_Test.ini");

	if(pTP != NULL) {
		_stprintf_s(cWriteBuf, "%d", pTP->dwTickCount);
		WritePrivateProfileString("GENERAL", "TICKCOUNT", cWriteBuf, cIniPath);

		_stprintf_s(cWriteBuf, "%d", pTP->dwGainPips);
		WritePrivateProfileString("GENERAL", "GAINPIPS", cWriteBuf, cIniPath);

		_stprintf_s(cWriteBuf, "%d", pTP->dwLosCutPips);
		WritePrivateProfileString("GENERAL", "LOSSCUTPIPS", cWriteBuf, cIniPath);

		_stprintf_s(cWriteBuf, "%d", pTP->dwIKEIKEEndPips);
		WritePrivateProfileString("GENERAL", "IKEIKEMODEENDPIPS", cWriteBuf, cIniPath);

		_stprintf_s(cWriteBuf, "%d", pTP->dwForceStopTickCnt);
		WritePrivateProfileString("GENERAL", "FORCESTOPTICKCNT", cWriteBuf, cIniPath);

		if(pTP->db.bEnable) {
			WritePrivateProfileString("DOUJIBREAK", "ENABLE", "1", cIniPath);
		}
		else {
			WritePrivateProfileString("DOUJIBREAK", "ENABLE", "0", cIniPath);
		}

		//	同時線として扱うOpとClの差
		_stprintf_s(cWriteBuf, "%f", pTP->db.dRgOpCl);
		WritePrivateProfileString("DOUJIBREAK", "DB_OPCLRANGE", cWriteBuf, cIniPath);
		//	同時線として扱うHiとLoの差
		_stprintf_s(cWriteBuf, "%f", pTP->db.dRgHiLo);
		WritePrivateProfileString("DOUJIBREAK", "DB_HILORANGE", cWriteBuf, cIniPath);
		//	ブレイクするために何個同時選が続いている必要があるか
		_stprintf_s(cWriteBuf, "%d", pTP->db.dwContNum);
		WritePrivateProfileString("DOUJIBREAK", "CONTINUTENUM", cWriteBuf, cIniPath);
		//	ブレイクに必要となる差分
		_stprintf_s(cWriteBuf, "%f", pTP->db.dSigTrig);
		WritePrivateProfileString("DOUJIBREAK", "SIGNALTRIGGER", cWriteBuf, cIniPath);

		if(pTP->mab.bEnable) {
			WritePrivateProfileString("EMABREAK", "ENABLE", "1", cIniPath);
		}
		else {
			WritePrivateProfileString("EMABREAK", "ENABLE", "0", cIniPath);
		}

		_stprintf_s(cWriteBuf, "%d", pTP->mab.dwMA1Val);
		WritePrivateProfileString("EMABREAK", "MAB_EMA1", cWriteBuf, cIniPath);
		_stprintf_s(cWriteBuf, "%d", pTP->mab.dwMA2Val);
		WritePrivateProfileString("EMABREAK", "MAB_EMA2", cWriteBuf, cIniPath);
		_stprintf_s(cWriteBuf, "%d", pTP->mab.dwSigTrig);
		WritePrivateProfileString("EMABREAK", "SIGNALTRIGGER", cWriteBuf, cIniPath);

		if(pTP->brsi.bEnable) {
			WritePrivateProfileString("BRSIBREAK", "ENABLE", "1", cIniPath);
		}
		else {
			WritePrivateProfileString("BRSIBREAK", "ENABLE", "0", cIniPath);
		}

		_stprintf_s(cWriteBuf, "%d", pTP->brsi.dwRSI);
		WritePrivateProfileString("BRSIBREAK", "RSI", cWriteBuf, cIniPath);
		_stprintf_s(cWriteBuf, "%d", pTP->brsi.dwRSISpan);
		WritePrivateProfileString("BRSIBREAK", "RSISPAN", cWriteBuf, cIniPath);
		_stprintf_s(cWriteBuf, "%d", pTP->brsi.dwProfit);
		WritePrivateProfileString("BRSIBREAK", "PROFIT", cWriteBuf, cIniPath);
		_stprintf_s(cWriteBuf, "%d", pTP->brsi.dwLossCut);
		WritePrivateProfileString("BRSIBREAK", "LOSSCUT", cWriteBuf, cIniPath);


		if(pTP->so.bEnable) {
			WritePrivateProfileString("SOBREAK", "ENABLE", "1", cIniPath);
		}
		else {
			WritePrivateProfileString("SOBREAK", "ENABLE", "0", cIniPath);
		}

		_stprintf_s(cWriteBuf, "%d", pTP->so.dwKSpan);
		WritePrivateProfileString("SOBREAK", "KSPAN", cWriteBuf, cIniPath);

		_stprintf_s(cWriteBuf, "%d", pTP->so.dwDSpan);
		WritePrivateProfileString("SOBREAK", "DSPAN", cWriteBuf, cIniPath);

		_stprintf_s(cWriteBuf, "%d", pTP->so.dwSlowDSpan);
		WritePrivateProfileString("SOBREAK", "SLOWDSPAN", cWriteBuf, cIniPath);

		if(pTP->rsi.bEnable) {
			WritePrivateProfileString("RSIBREAK", "ENABLE", "1", cIniPath);
		}
		else {
			WritePrivateProfileString("RSIBREAK", "ENABLE", "0", cIniPath);
		}

		_stprintf_s(cWriteBuf, "%d", pTP->rsi.dwSpan);
		WritePrivateProfileString("RSIBREAK", "SPAN", cWriteBuf, cIniPath);


		if(pTP->ichimoku.bEnable) {
			WritePrivateProfileString("ICHIMOKUBREAK", "ENABLE", "1", cIniPath);
		}
		else {
			WritePrivateProfileString("ICHIMOKUBREAK", "ENABLE", "0", cIniPath);
		}


		if(pTP->rb.bEnable) {
			WritePrivateProfileString("RANGEBREAK", "ENABLE", "1", cIniPath);
		}
		else {
			WritePrivateProfileString("RANGEBREAK", "ENABLE", "0", cIniPath);
		}

		_stprintf_s(cWriteBuf, "%d", pTP->rb.dwSpan);
		WritePrivateProfileString("RANGEBREAK", "SPANTICK", cWriteBuf, cIniPath);

		_stprintf_s(cWriteBuf, "%d", pTP->rb.dwWidth);
		WritePrivateProfileString("RANGEBREAK", "RANGEWIDTH", cWriteBuf, cIniPath);

		_stprintf_s(cWriteBuf, "%d", pTP->rb.dwVagueVal);
		WritePrivateProfileString("RANGEBREAK", "VAGUEVAL", cWriteBuf, cIniPath);
	}

	return;
}

void CAgentWorkerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAgentWorkerDlg::OnPaint()
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
HCURSOR CAgentWorkerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAgentWorkerDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnOK();
}

DWORD CommandOperateThread( LPVOID pParam ){
	CAgentWorkerDlg* pAWD = (CAgentWorkerDlg*)pParam;
	HANDLE hMap;
	DWORD* pdwCmd;
	BOOL bEnd = FALSE;

	char cMapName[256];

	if( pAWD->m_bTestMode == FALSE ){
		lstrcpy(cMapName, AW_WINDOW_NAME);
	}
	else {
		lstrcpy(cMapName, AW_TETWINDOW_NAME);
	}

	if((hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cMapName)) != NULL) {
		if((pdwCmd = (DWORD*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {

			DWORD* pdwSetEvent = pdwCmd + sizeof(DWORD);
			DWORD* pdwFunNo = pdwCmd + sizeof(DWORD) * 2;
			DWORD* pdwMapId = pdwCmd + sizeof(DWORD) * 3;

			while( bEnd==FALSE ){

				while( *pdwSetEvent == 0)	Sleep(0);

				DWORD dwMapId = *pdwMapId;

				switch(*pdwFunNo){
				case MT4_INITILIZE:
					pAWD->MT4_Initilize(dwMapId);
					break;
				case MT4_REGISTERRATE:
					pAWD->MT4_RegisterRate(dwMapId);
					break;
				case MT4_DBGREGISTERTICKINFO:
					pAWD->MT4_DbgRegisterTickInfo(dwMapId);
					break;
				case MT4_TRADECHECK:
					pAWD->MT4_TradeCheck(dwMapId);
					break;
				case MT4_REGISTERTRADEINFO:
					pAWD->MT4_RegisterTradeInfo(dwMapId);
					break;
				case MT4_CHECKCLOSETRADE:
					pAWD->MT4_CheckCloseTrade(dwMapId);
					break;
				case MT4_REGISTERCLOSETRADE:
					pAWD->MT4_RegisterCloseTrade(dwMapId);
					break;
				case MT4_FINISHPROCESS:
					pAWD->MT4_FinishProcess(dwMapId);
					break;
				}
				*pdwSetEvent = 0;
			}
			UnmapViewOfFile(pdwCmd);
		}
		CloseHandle(hMap);
	}

	return 1;
}

//	データの初期化
DWORD CAgentWorkerDlg::Initilize_Main(DWORD dwPair ){
	DWORD dwRet = 0;

	if( dwPair == (DWORD)-1) {
		for(DWORD i = 0; i < m_TickInfo.size(); i++) {
			InitilizeTickData(i);
		}
		memset( &LastTradeInfo, 0x00, sizeof(LastTradeInfo) );
	}
	else {
		InitilizeTickData(dwPair);
		
		memset( &LastTradeInfo[dwPair], 0x00, sizeof(TRADE_INFO) );

		m_dwExpansionTID[dwPair] = 0;
		m_dwSqueezeTID[dwPair] = 0;
		
		if( m_bTestMode == FALSE ){
			InitilizeTickInfo( dwPair );
		}
	}

	//	取引情報をクリア
	//m_tradeInfo.clear();

	//	設定ファイルからもろもろの設定を読み込む
	ReadSettingData(&(m_Setting[dwPair]), dwPair );

	dwRet = 1;

	return dwRet;
}

void CAgentWorkerDlg::MT4_Initilize(DWORD dwMapId)
{
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];


	PMT4_INITILIZE_DATA pMapData;

	wsprintf(cMapName, "MT4_Initilize_%d", dwMapId);

	if ((hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cMapName)) != INVALID_HANDLE_VALUE) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_INITILIZE_DATA)pMap;


			pMapData->dwRet = Initilize_Main( pMapData->dwPair);

			if(m_hCmdOpeThread == NULL ){
				m_hCmdOpeThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CommandOperateThread, (LPVOID)this, 0, NULL);
			}

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	return;
}

bool CAgentWorkerDlg::RateDataWrite(DWORD dwPairIdx, DWORD dwRate) {
	static char cLogFilePath_cache[PAIR_COUNT][1024];
	static HANDLE hFile_cache[PAIR_COUNT];

	HANDLE hFile = INVALID_HANDLE_VALUE;
	char cLogFilePath[1024];
	DWORD dwWrite;
	char cWriteData[1024];

	char* pCacheFilePath = NULL;
	HANDLE* phCachehFile = NULL;

	if (DEBUG_FILEOUT < 1)	return false;

	if( m_bTestMode != FALSE)	return false;

	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	wsprintf(cLogFilePath, "%s\\%s_%02d%02d%02d.adf", _LOG_DIR_, GetPairName(dwPairIdx), CurTime.wYear, CurTime.wMonth, CurTime.wDay);

	phCachehFile = &hFile_cache[dwPairIdx];
	pCacheFilePath = cLogFilePath_cache[dwPairIdx];


	if ( pCacheFilePath != NULL && _stricmp((const char*)cLogFilePath, (const char*)pCacheFilePath) != 0) {
		if (*phCachehFile != INVALID_HANDLE_VALUE && *phCachehFile != NULL ) {
			CloseHandle(*phCachehFile);
		}

		hFile = CreateFile(cLogFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile != INVALID_HANDLE_VALUE) {
			lstrcpy(pCacheFilePath, cLogFilePath);
			*phCachehFile = hFile;
		}
	}
	else {
		hFile = *phCachehFile;
	}

	if ( hFile != INVALID_HANDLE_VALUE) {
		//	実数かしてあるRate値をもとの小数の値に戻す
		double dRate = EVal2Rate(dwRate, dwPairIdx);

		SetFilePointer(hFile, 0, 0, FILE_END);

		if (dwPairIdx == PAIR_EURUSD || dwPairIdx == PAIR_GBPUSD || dwPairIdx == PAIR_USDCHF ){
			_stprintf_s(cWriteData, "%02d:%02d:%02d,%.5f\r\n", CurTime.wHour, CurTime.wMinute, CurTime.wSecond, dRate);
		}
		else {
			_stprintf_s(cWriteData, "%02d:%02d:%02d,%.3f\r\n", CurTime.wHour, CurTime.wMinute, CurTime.wSecond, dRate);
		}

		WriteFile(hFile, cWriteData, lstrlen(cWriteData), &dwWrite, NULL);

		//CloseHandle(hFile);
	}

	return true;
}

//通貨ペア,設定組み合わせ,取引合計,得回数、損回数、取引結果、得合計、損合計
void CAgentWorkerDlg::WriteTestResult( DWORD dwPair, DWORD dwTradeCnt, DWORD dwObtainCnt, DWORD dwLossCnt, int nRlstPips, DWORD dwObtainPips, DWORD dwLossPips )
{
	char cLogFilePath[1024];
	HANDLE hFile;
	DWORD dwWrite;
	char cWriteBuf[1024];
	char* pWriteBuf;

	if(DEBUG_FILEOUT < 1)	return;

	wsprintf(cLogFilePath, "%s\\TestMode_Result\\%s", _LOG_DIR_, m_Setting[dwPair].cTestName );

	CreateDirectory(cLogFilePath, NULL);

	wsprintf(cLogFilePath, "%s\\TestMode_Result\\%s\\TestResult_%s.csv", _LOG_DIR_, m_Setting[dwPair].cTestName, GetPairName(dwPair) );


	pWriteBuf = cWriteBuf;

	//	通貨ペア
	if(dwPair == PAIR_UNKNOWN){
		pWriteBuf += wsprintf(pWriteBuf, "%s,", "PAIR_UNKNOWN" );
	}else{
		pWriteBuf += wsprintf(pWriteBuf, "%s,", GetPairName(dwPair));
	}
	//	設定（共通）:Tick数,利益確定値,ロスカット値
	pWriteBuf += wsprintf(pWriteBuf, "%d,%d,%d,%d,%d,", m_Setting[dwPair].dwTickCount, m_Setting[dwPair].dwGainPips, m_Setting[dwPair].dwLosCutPips, m_Setting[dwPair].dwIKEIKEEndPips, m_Setting[dwPair].dwForceStopTickCnt );
	//	設定（DB）:有効無効,同時線定義（実体）,同時線定義（ヒゲ）,連続数,シグナルトリガー
	if( m_Setting[dwPair].db.bEnable != FALSE )	pWriteBuf += wsprintf(pWriteBuf, "%d,%d,%d,%d,%d,", m_Setting[dwPair].db.bEnable, (DWORD)m_Setting[dwPair].db.dRgOpCl, (DWORD)m_Setting[dwPair].db.dRgHiLo, m_Setting[dwPair].db.dwContNum, (DWORD)m_Setting[dwPair].db.dSigTrig );
	//	設定（MAB）:有効無効,MA1,MA2,シグナルトリガー
	if( m_Setting[dwPair].mab.bEnable != FALSE )	pWriteBuf += wsprintf(pWriteBuf, "%d,%d,%d,%d,", m_Setting[dwPair].mab.bEnable, (DWORD)m_Setting[dwPair].mab.dwMA1Val, (DWORD)m_Setting[dwPair].mab.dwMA2Val, m_Setting[dwPair].mab.dwSigTrig );
	//	設定（BRSI）:有効無効,RSI,RSISpan,Profit,Losscut
	if( m_Setting[dwPair].brsi.bEnable != FALSE )	pWriteBuf += wsprintf(pWriteBuf, "%d,%d,%d,%d,%d,", m_Setting[dwPair].brsi.bEnable, (DWORD)m_Setting[dwPair].brsi.dwRSI, (DWORD)m_Setting[dwPair].brsi.dwRSISpan, m_Setting[dwPair].brsi.dwProfit, m_Setting[dwPair].brsi.dwLossCut  );

	if( m_Setting[dwPair].so.bEnable != FALSE )	pWriteBuf += wsprintf(pWriteBuf, "%d,%d,%d,%d,", m_Setting[dwPair].so.bEnable, (DWORD)m_Setting[dwPair].so.dwKSpan, (DWORD)m_Setting[dwPair].so.dwDSpan, m_Setting[dwPair].so.dwSlowDSpan );

	if( m_Setting[dwPair].rsi.bEnable != FALSE )	pWriteBuf += wsprintf(pWriteBuf, "%d,%d,", m_Setting[dwPair].rsi.bEnable, (DWORD)m_Setting[dwPair].rsi.dwSpan );
	if( m_Setting[dwPair].ichimoku.bEnable != FALSE )	pWriteBuf += wsprintf(pWriteBuf, "%d,", m_Setting[dwPair].ichimoku.bEnable );

	//	結果:取引合計,得回数、損回数、取引結果、得合計、損合計
	pWriteBuf += wsprintf(pWriteBuf, "%d,%d,%d,%d,%d,%d\r\n", dwTradeCnt, dwObtainCnt, dwLossCnt, nRlstPips, dwObtainPips, dwLossPips );

	if((hFile = CreateFile(cLogFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
		SetFilePointer(hFile, 0, 0, FILE_END);

		WriteFile(hFile, cWriteBuf, lstrlen(cWriteBuf), &dwWrite, NULL);

		CloseHandle(hFile);
	}

	return;
}

bool CAgentWorkerDlg::TickInfoWrite(DWORD dwPairIdx, DWORD dwTickIdx, DWORD dwWriteIdx) {
	char cLogFilePath[1024];
	HANDLE hFile;
	DWORD dwWrite;

	if (DEBUG_FILEOUT < 1)	return false;

	SYSTEMTIME* pSt = &m_TickInfo[dwPairIdx][dwTickIdx][dwWriteIdx].sDt;
	if(m_bTestMode==FALSE){
		wsprintf(cLogFilePath, "%s\\TKI\\%s_%s_%02d%02d%02d.tki", _LOG_DIR_, GetPairName(dwPairIdx), GetTickName(dwTickIdx), pSt->wYear, pSt->wMonth, pSt->wDay);

		hFile = CreateFile(cLogFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		SetFilePointer(hFile, 0, 0, FILE_END);
	}
	//	\TestMode_Result\20161029_142530\usdjpy.tki
	else{
		wsprintf(cLogFilePath, "%s\\TestMode_Result\\%s", _LOG_DIR_, m_Setting[dwPairIdx].cTestName );

		CreateDirectory(cLogFilePath, NULL);

		if(m_bAutoMode != FALSE && m_bAnaTKIWrite == FALSE ) {
			return false;
		}

		wsprintf(cLogFilePath, "%s\\TestMode_Result\\%s\\%s_%s.tki", _LOG_DIR_, m_Setting[dwPairIdx].cTestName, GetPairName(dwPairIdx) , GetTickName(dwTickIdx) );

		if(m_hTickFile[dwTickIdx] == INVALID_HANDLE_VALUE){
			m_hTickFile[dwTickIdx] = CreateFile(cLogFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if(m_hTickFile[dwTickIdx] != INVALID_HANDLE_VALUE) {
				SetFilePointer(m_hTickFile[dwTickIdx], 0, 0, FILE_END);
			}
		}
		hFile = m_hTickFile[dwTickIdx];
	}

	if ( hFile != INVALID_HANDLE_VALUE) {
		TICK_INFO* pWriteData = &m_TickInfo[dwPairIdx][dwTickIdx][dwWriteIdx];		

		WriteFile(hFile, pWriteData, sizeof(TICK_INFO), &dwWrite, NULL);

		if(m_bTestMode == FALSE) {
			CloseHandle(hFile);
		}
	}

	return true;
}

BOOL CAgentWorkerDlg::WriteTradeInfo(TRADE_INFO* ti, BOOL bOpen) {
	char cLogFilePath[1024];
	char cWriteBuffer[1024];
	HANDLE hFile;
	DWORD dwWrite;
	SYSTEMTIME CurTime;
	SYSTEMTIME* pSt;

	if (DEBUG_FILEOUT < 1)	return false;

	if (bOpen != FALSE) {
		pSt = &ti->openDt;
	}
	else {
		pSt = &ti->closeDt;
	}

	GetLocalTime(&CurTime);
	if(m_bTestMode == FALSE) {
		wsprintf(cLogFilePath, "%s\\TRI\\%d%02d%02d.tri", _LOG_DIR_, pSt->wYear, pSt->wMonth, pSt->wDay);

		hFile = CreateFile(cLogFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		SetFilePointer(hFile, 0, 0, FILE_END);
	}
	//	\TestMode_Result\20161029_142530\usdjpy.tri
	else{
		wsprintf(cLogFilePath, "%s\\TestMode_Result\\%s", _LOG_DIR_, m_Setting[ti->dwPair].cTestName);

		CreateDirectory(cLogFilePath, NULL);
		if(m_bAutoMode != FALSE){
			char* pWriteBuf = cLogFilePath;
			pWriteBuf += wsprintf(pWriteBuf, "%s\\TestMode_Result\\%s\\%s_", _LOG_DIR_, m_Setting[ti->dwPair].cTestName, GetPairName(ti->dwPair));
			pWriteBuf += wsprintf(pWriteBuf, "%02d%02d%02d%02d%02d_", m_Setting[ti->dwPair].dwTickCount, m_Setting[ti->dwPair].dwGainPips, m_Setting[ti->dwPair].dwLosCutPips, m_Setting[ti->dwPair].dwIKEIKEEndPips, m_Setting[ti->dwPair].dwForceStopTickCnt );
			if( m_Setting[ti->dwPair].db.bEnable )	pWriteBuf += wsprintf(pWriteBuf, "%02d%02d%02d%02d%02d_", m_Setting[ti->dwPair].db.bEnable, (DWORD)m_Setting[ti->dwPair].db.dRgOpCl, (DWORD)m_Setting[ti->dwPair].db.dRgHiLo, m_Setting[ti->dwPair].db.dwContNum, (DWORD)m_Setting[ti->dwPair].db.dSigTrig);
			if( m_Setting[ti->dwPair].mab.bEnable )	pWriteBuf += wsprintf(pWriteBuf, "%02d%02d%02d%02d", m_Setting[ti->dwPair].mab.bEnable, (DWORD)m_Setting[ti->dwPair].mab.dwMA1Val, (DWORD)m_Setting[ti->dwPair].mab.dwMA2Val, m_Setting[ti->dwPair].mab.dwSigTrig);
			if( m_Setting[ti->dwPair].brsi.bEnable )	pWriteBuf += wsprintf(pWriteBuf, "%02d%02d%02d%02d%02d", m_Setting[ti->dwPair].brsi.bEnable, (DWORD)m_Setting[ti->dwPair].brsi.dwRSI, (DWORD)m_Setting[ti->dwPair].brsi.dwRSISpan, m_Setting[ti->dwPair].brsi.dwProfit, m_Setting[ti->dwPair].brsi.dwLossCut );
			if( m_Setting[ti->dwPair].so.bEnable )	pWriteBuf += wsprintf(pWriteBuf, "%02d%02d%02d%02d", m_Setting[ti->dwPair].so.bEnable, (DWORD)m_Setting[ti->dwPair].so.dwKSpan, (DWORD)m_Setting[ti->dwPair].so.dwDSpan, m_Setting[ti->dwPair].so.dwSlowDSpan);
			if( m_Setting[ti->dwPair].rsi.bEnable )	pWriteBuf += wsprintf(pWriteBuf, "%02d%02d", m_Setting[ti->dwPair].rsi.bEnable, (DWORD)m_Setting[ti->dwPair].rsi.dwSpan );
			if( m_Setting[ti->dwPair].ichimoku.bEnable )	pWriteBuf += wsprintf(pWriteBuf, "%02d", m_Setting[ti->dwPair].ichimoku.bEnable );
			pWriteBuf += wsprintf(pWriteBuf, ".tri");

		}else{
			wsprintf(cLogFilePath, "%s\\TestMode_Result\\%s\\%s.tri", _LOG_DIR_, m_Setting[ti->dwPair].cTestName, GetPairName(ti->dwPair));
		}

		if( m_hTradeFile == INVALID_HANDLE_VALUE ){
			m_hTradeFile = CreateFile(cLogFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if(m_hTradeFile != INVALID_HANDLE_VALUE) {
				SetFilePointer(m_hTradeFile, 0, 0, FILE_END);
			}
		}
		hFile = m_hTradeFile;
	}

	if ( hFile != INVALID_HANDLE_VALUE ) {
		WriteFile(hFile, ti, sizeof(TRADE_INFO), &dwWrite,  NULL);
		if(m_bTestMode == FALSE) {
			CloseHandle(hFile);
		}
	}

	if( bOpen != FALSE )	return true;
	//if(m_bTestMode != FALSE )	return true;
	
	if(m_bTestMode == FALSE) {
		wsprintf(cLogFilePath, "%s\\TRI_CSV\\TradeInfCsv_%d%02d%02d.csv", _LOG_DIR_, pSt->wYear, pSt->wMonth, pSt->wDay);
	}
	//	\TestMode_Result\20161029_142530\usdjpy.csv
	else{
		wsprintf(cLogFilePath, "%s\\TestMode_Result\\%s\\TradeInfCsv_%s.csv", _LOG_DIR_, m_Setting[ti->dwPair].cTestName, GetPairName(ti->dwPair));
	}
	
	if ((hFile = CreateFile(cLogFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
		char* pFormat;
		SetFilePointer(hFile, 0, 0, FILE_END);

		if (ti->dwPair == PAIR_EURUSD || ti->dwPair == PAIR_GBPUSD || ti->dwPair == PAIR_USDCHF ){
			pFormat = "%02d:%02d:%02d,%s,%s,%s,%.5f,%.5f,%02d:%02d:%02d,%02d:%02d:%02d,%d,";
		}
		else {
			pFormat = "%02d:%02d:%02d,%s,%s,%s,%.3f,%.3f,%02d:%02d:%02d,%02d:%02d:%02d,%d,";
		}

		int nCloseEVal = Rate2EVal(ti->dClosePrice,ti->dwPair);
		int nOpenEVal = Rate2EVal(ti->dOpenPrice,ti->dwPair);
		_stprintf_s(cWriteBuffer, pFormat,
			CurTime.wHour, CurTime.wMinute, CurTime.wSecond,
			bOpen ? "注文履歴" : "決算履歴",
			GetPairName(ti->dwPair),
			(ti->dwOrderType == 1) ? "買い注文" : "売り注文",
			ti->dOpenPrice,
			bOpen ? 0 : ti->dClosePrice,
			ti->openDt.wHour, ti->openDt.wMinute, ti->openDt.wSecond,
			bOpen ? 0 : ti->closeDt.wHour, bOpen ? 0 : ti->closeDt.wMinute, bOpen ? 0 : ti->closeDt.wSecond,
			(ti->dwOrderType == 1) ?(nCloseEVal-nOpenEVal):(nOpenEVal-nCloseEVal)
		);

		WriteFile(hFile, cWriteBuffer, lstrlen(cWriteBuffer), &dwWrite, NULL);

		if( ti->dwBreakTrigger & BREAKSIGN_BY_BBBREAK_BW ){
			_stprintf_s(cWriteBuffer, "%d,%d,%d,", ti->bbi.dwExpansionTID - ti->bbi.dwStartTID, ti->bbi.dwStartRate, (int)ti->bbi.dwHiRate-(int)ti->bbi.dwLowRate );
			WriteFile(hFile, cWriteBuffer, lstrlen(cWriteBuffer), &dwWrite, NULL);
			_stprintf_s(cWriteBuffer, "%d,%d,%d,", ((int)ti->bbi.dwStartPSigma-(int)ti->bbi.dwEndPSigma), ((int)ti->bbi.dwStartSMA-(int)ti->bbi.dwEndSMA), ((int)ti->bbi.dwStartMSigma-(int)ti->bbi.dwEndMSigma)  );
			WriteFile(hFile, cWriteBuffer, lstrlen(cWriteBuffer), &dwWrite, NULL);
		}

		WriteFile(hFile, "\r\n", lstrlen("\r\n"), &dwWrite, NULL);

		CloseHandle(hFile);
	}

	return true;
}


void CAgentWorkerDlg::UpdateTickInfoData( DWORD dwPair, DWORD dwCol) 
{

	CListCtrl* pListCtrl = NULL;
	
	if (dwPair == PAIR_USDJPY) {
		pListCtrl = &m_ListUJ;
	}
	else if (dwPair == PAIR_EURJPY) {
		pListCtrl = &m_ListEJ;
	}
	else if (dwPair == PAIR_EURUSD) {
		pListCtrl = &m_ListEU;
	}

	if ( pListCtrl != NULL && m_TickInfo[dwPair][TKI_70TICK].size() > 0) {
		char	cData[1024];
		char	cPriceFmt[][32] = { "%.3f", "%.3f", "%.5f" };
		DWORD i = m_TickInfo[dwPair][TKI_70TICK].size() - 1;
		if (dwCol == TI_COL_TYPE_DATE) {
			wsprintf(cData, "%02d:%02d:%02d", m_TickInfo[dwPair][TKI_70TICK][i].sDt.wHour, m_TickInfo[dwPair][TKI_70TICK][i].sDt.wMinute, m_TickInfo[dwPair][TKI_70TICK][i].sDt.wSecond);
			pListCtrl->SetItemText(0, dwCol, cData);
		}
		else if (dwCol == TI_COL_TYPE_OPEN) {
			_stprintf_s(cData, cPriceFmt[dwPair], m_TickInfo[dwPair][TKI_70TICK][i].Op);
			pListCtrl->SetItemText(0, dwCol, cData);
		}
		else if (dwCol == TI_COL_TYPE_CLOSE) {
			_stprintf_s(cData, cPriceFmt[dwPair], m_TickInfo[dwPair][TKI_70TICK][i].Cl);
			pListCtrl->SetItemText(0, dwCol, cData);
		}
		else if (dwCol == TI_COL_TYPE_HIGH) {
			_stprintf_s(cData, cPriceFmt[dwPair], m_TickInfo[dwPair][TKI_70TICK][i].Hi);
			pListCtrl->SetItemText(0, dwCol, cData);
		}
		else if (dwCol == TI_COL_TYPE_LOW) {
			_stprintf_s(cData, cPriceFmt[dwPair], m_TickInfo[dwPair][TKI_70TICK][i].Cl);
			pListCtrl->SetItemText(0, dwCol, cData);
		}
		else if (dwCol == TI_COL_TYPE_CNT) {
			_stprintf_s(cData, "%d", m_TickInfo[dwPair][TKI_70TICK][i].Tc);
			pListCtrl->SetItemText(0, dwCol, cData);
		}
	}
}

void CAgentWorkerDlg::SetTickInfoData( CListCtrl* pListCtrl, DWORD dwPair ) 
{
	int nMax;
	int nEnd;
	int i, l, idx;
	LVITEM       lvi;
	char cData[1024];
	char	cPriceFmt[][32] = { "%.3f", "%.3f", "%.5f" };
	CStatic* pStCnt = NULL;

	if( m_bTestMode != FALSE )	return;

	pListCtrl->DeleteAllItems();

	if (dwPair == PAIR_USDJPY) {
		pStCnt = (CStatic*)GetDlgItem(IDC_ST_UJ);
	}
	else if (dwPair == PAIR_EURJPY) {
		pStCnt = (CStatic*)GetDlgItem(IDC_ST_EJ);
	}
	else if (dwPair == PAIR_EURUSD) {
		pStCnt = (CStatic*)GetDlgItem(IDC_ST_EU);
	}
	if (pStCnt != NULL) {
		char cCnt[32];

		wsprintf(cCnt, "%d", m_TickInfo[dwPair][TKI_70TICK].size());

		pStCnt->SetWindowTextA(cCnt);
	}

	if ( m_TickInfo[dwPair][TKI_70TICK].size() > 0) {
		//	一個前からさかのぼっていく
		double dPrevDojiVal = 0;
		DWORD dojiCnt = 0;
		DWORD dwMagni = (dwPair == PAIR_EURUSD) ? (10000) : (100);
		double dRange = (dwPair == PAIR_EURUSD) ? (0.0003) : (0.03);

		nMax = m_TickInfo[dwPair][TKI_70TICK].size() - 1;
		if (nMax > 9) {
			nEnd = nMax - 10;
		}
		else {
			nEnd = 0;
		}

		for (i = nMax, l = 0; i >= nEnd; i--, l++) {
			lvi.mask = LVIF_TEXT;
			lvi.iItem = l;
			lvi.iSubItem = 0;
			wsprintf(cData, "%02d:%02d:%02d", m_TickInfo[dwPair][TKI_70TICK][i].sDt.wHour, m_TickInfo[dwPair][TKI_70TICK][i].sDt.wMinute, m_TickInfo[dwPair][TKI_70TICK][i].sDt.wSecond);
			lvi.pszText = cData;
			idx = pListCtrl->InsertItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = 1;
			_stprintf_s(cData, cPriceFmt[dwPair], EVal2Rate( m_TickInfo[dwPair][TKI_70TICK][i].Op, dwPair ) );
			lvi.pszText = cData;
			pListCtrl->SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = 2;
			_stprintf_s(cData, cPriceFmt[dwPair], EVal2Rate( m_TickInfo[dwPair][TKI_70TICK][i].Cl, dwPair));
			lvi.pszText = cData;
			pListCtrl->SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = 3;
			_stprintf_s(cData, cPriceFmt[dwPair], EVal2Rate( m_TickInfo[dwPair][TKI_70TICK][i].Hi, dwPair));
			lvi.pszText = cData;
			pListCtrl->SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = 4;
			_stprintf_s(cData, cPriceFmt[dwPair], EVal2Rate( m_TickInfo[dwPair][TKI_70TICK][i].Lo, dwPair));
			lvi.pszText = cData;
			pListCtrl->SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = 5;
			_stprintf_s(cData, "%d", m_TickInfo[dwPair][TKI_70TICK][i].Tc);
			lvi.pszText = cData;
			pListCtrl->SetItem(&lvi);


			if (l > 0) {
				//	連続する同時線を囲む
				double dAbsOpCl = ABS(m_TickInfo[dwPair][TKI_70TICK][i].Op, m_TickInfo[dwPair][TKI_70TICK][i].Cl);
				double dAbsHiLo = ABS(m_TickInfo[dwPair][TKI_70TICK][i].Hi, m_TickInfo[dwPair][TKI_70TICK][i].Lo);

				if (dPrevDojiVal == 0 && dAbsOpCl * dwMagni <= 1.0 && dAbsHiLo * dwMagni <= 3.0) {
					dPrevDojiVal = (m_TickInfo[dwPair][TKI_70TICK][i].Op < m_TickInfo[dwPair][TKI_70TICK][i].Cl) ? (m_TickInfo[dwPair][TKI_70TICK][i].Op + (m_TickInfo[dwPair][TKI_70TICK][i].Cl - m_TickInfo[dwPair][TKI_70TICK][i].Op) / 2) : (m_TickInfo[dwPair][TKI_70TICK][i].Cl + (m_TickInfo[dwPair][TKI_70TICK][i].Op - m_TickInfo[dwPair][TKI_70TICK][i].Cl) / 2);
					dojiCnt++;

					lvi.iItem = idx;
					lvi.iSubItem = 6;
					lvi.pszText = "同時";
					pListCtrl->SetItem(&lvi);
				}
				else if (dPrevDojiVal > 0) {
					double dChkA = dPrevDojiVal * dwMagni - 1;
					double dChkB = dPrevDojiVal * dwMagni + 1;
					double dChkC = m_TickInfo[dwPair][TKI_70TICK][i].Op * dwMagni;
					double dChkD = m_TickInfo[dwPair][TKI_70TICK][i].Cl * dwMagni;

					if (dAbsHiLo * dwMagni <= 3.0
						&& (dChkA <= dChkC && dChkC <= dChkB)
						&& (dChkA <= dChkD && dChkD <= dChkB)
						) {
						dojiCnt++;

						lvi.iItem = idx;
						lvi.iSubItem = 6;
						lvi.pszText = "同時連動";
						pListCtrl->SetItem(&lvi);
					}
					else {
						dPrevDojiVal = 0;
						dojiCnt = 0;
					}
				}
			}
		}
	}
}

void CAgentWorkerDlg::OutputTickInfo( DWORD dwPair )
{
	if (dwPair == PAIR_USDJPY) {
		SetTickInfoData(&m_ListUJ, PAIR_USDJPY);
	}
	else if (dwPair == PAIR_EURJPY) {
		SetTickInfoData(&m_ListEJ, PAIR_EURJPY);
	}
	else if (dwPair == PAIR_EURUSD) {
		SetTickInfoData(&m_ListEU, PAIR_EURUSD);
	}

}

void CAgentWorkerDlg::OutputTradeInfo() 
{
	int nMax;
	int i, j, idx;
	LVITEM       lvi;
	char cData[1024];

	if(m_bTestMode != FALSE)	return;

	m_ListTrade.DeleteAllItems();

	if (m_tradeInfo.size() > 0) {
		nMax = m_tradeInfo.size() - 1;
//	char		cCapTrade[][32] = { "開始時刻", "ペア", "開始", "終了", "差分", "終了時刻", "" };

		for(i = 0, j = 0; (DWORD)i < m_tradeInfo.size(); i++, j++) {
			j = 0;

			lvi.mask = LVIF_TEXT;
			lvi.iItem = i;
			lvi.iSubItem = j++;
			wsprintf(cData, "%02d:%02d:%02d", m_tradeInfo[i].openDt.wHour, m_tradeInfo[i].openDt.wMinute, m_tradeInfo[i].openDt.wSecond);
			lvi.pszText = cData;
			idx = m_ListTrade.InsertItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			lvi.pszText = GetPairName(m_tradeInfo[i].dwPair);
			m_ListTrade.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			lvi.pszText = (m_tradeInfo[i].dwOrderType == 1) ? "買い" : "売り";
			m_ListTrade.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			if(m_tradeInfo[i].dwPair == PAIR_EURUSD) {
				_stprintf_s(cData, "%.4f", m_tradeInfo[i].dOpenPrice);
			}
			else
			{
				_stprintf_s(cData, "%.2f", m_tradeInfo[i].dOpenPrice);
			}
			lvi.pszText = cData;
			m_ListTrade.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			if(m_tradeInfo[i].dClosePrice > 0) {
				if(m_tradeInfo[i].dwPair == PAIR_EURUSD) {
					_stprintf_s(cData, "%.4f", m_tradeInfo[i].dClosePrice);
				}
				else
				{
					_stprintf_s(cData, "%.2f", m_tradeInfo[i].dClosePrice);
				}
			}
			else {
				cData[0] = 0x00;
			}
			lvi.pszText = cData;
			m_ListTrade.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			if(m_tradeInfo[i].dClosePrice > 0) {
				DWORD PipsA, PipsB;

				PipsA = EVal2Pips(Rate2EVal(m_tradeInfo[i].dOpenPrice, m_tradeInfo[i].dwPair));
				PipsB = EVal2Pips(Rate2EVal(m_tradeInfo[i].dClosePrice, m_tradeInfo[i].dwPair));


				if(m_tradeInfo[i].dwOrderType == 1) {
					if(PipsA <= PipsB) {
						_stprintf_s(cData, "%d pips", PipsB - PipsA);
					}
					else
					{
						_stprintf_s(cData, "- %d pips ", PipsA - PipsB);
					}
				}
				else {
					if(PipsA <= PipsB) {
						_stprintf_s(cData, "- %d pips", PipsB - PipsA);
					}
					else
					{
						_stprintf_s(cData, "%d pips ", PipsA - PipsB);
					}
				}
			}
			else {
				cData[0] = 0x00;
			}
			lvi.pszText = cData;
			m_ListTrade.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			if(m_tradeInfo[i].dClosePrice > 0) {
				wsprintf(cData, "%02d:%02d:%02d", m_tradeInfo[i].closeDt.wHour, m_tradeInfo[i].closeDt.wMinute, m_tradeInfo[i].closeDt.wSecond);
			}
			else {
				cData[0] = 0x00;
			}
			lvi.pszText = cData;
			m_ListTrade.SetItem(&lvi);

		}
	}
}



BBBREAK_INFO g_BBBreakInfo;

void CAgentWorkerDlg::SigmaCheck( DWORD dwPair )
{
	DWORD dwCurIdx = m_TickInfo[dwPair][TKI_70TICK].size()-1;
static DWORD dwExpCnt = 0;
static DWORD dwSquCnt = 0;

	if( dwCurIdx == 0 )	return;

	if( m_TickInfo[dwPair][TKI_70TICK][dwCurIdx].bCl == FALSE )	return;

	DWORD dwHiSigma2 = CalcBollingerBandsSigma(dwPair, dwCurIdx, 21, 2);
	DWORD dwLoSigma2 = CalcBollingerBandsSigma(dwPair, dwCurIdx, 21, -2);
	DWORD dwPrevHiSigma2 = CalcBollingerBandsSigma(dwPair, dwCurIdx - 1, 21, 2);
	DWORD dwPrevLoSigma2 = CalcBollingerBandsSigma(dwPair, dwCurIdx - 1, 21, -2);

	//	エクスパンション
	if( dwPrevHiSigma2 < dwHiSigma2
	 && dwPrevLoSigma2 > dwLoSigma2
	) {
		dwSquCnt = 0;
		dwExpCnt++;
		if ( m_dwExpansionTID[dwPair] == 0 || m_dwExpansionTID[dwPair] <  m_dwSqueezeTID[dwPair] ){
			//if( dwExpCnt >= 7 )
			{
				m_dwExpansionTID[dwPair] = m_TickInfo[dwPair][TKI_70TICK][dwCurIdx].dwTID;

				if( m_dwSqueezeTID[dwPair] == g_BBBreakInfo.dwStartTID ){
					g_BBBreakInfo.dwExpansionTID = m_TickInfo[dwPair][TKI_70TICK][dwCurIdx].dwTID;
					g_BBBreakInfo.dwEndPSigma = CalcBollingerBandsSigma(dwPair, dwCurIdx, 21, 2);
					g_BBBreakInfo.dwEndSMA = CalcBollingerBandsSigma(dwPair, dwCurIdx, 21, 0);
					g_BBBreakInfo.dwEndMSigma = CalcBollingerBandsSigma(dwPair, dwCurIdx, 21, -2);

				}
			}
		}
	}else if( dwPrevHiSigma2 > dwHiSigma2
	 && dwPrevLoSigma2 < dwLoSigma2
	) {
		dwSquCnt++;
		dwExpCnt = 0;
		if ( m_dwSqueezeTID[dwPair] == 0 || m_dwSqueezeTID[dwPair] <  m_dwExpansionTID[dwPair] ){
			//if( dwSquCnt >= 7 )
			{
				m_dwSqueezeTID[dwPair] = m_TickInfo[dwPair][TKI_70TICK][dwCurIdx].dwTID;
				memset( &g_BBBreakInfo, 0x00, sizeof(BBBREAK_INFO) );

				g_BBBreakInfo.dwStartRate = m_TickInfo[dwPair][TKI_70TICK][dwCurIdx].Cl;
				g_BBBreakInfo.dwHiRate = m_TickInfo[dwPair][TKI_70TICK][dwCurIdx].Cl;
				g_BBBreakInfo.dwLowRate = m_TickInfo[dwPair][TKI_70TICK][dwCurIdx].Cl;
				g_BBBreakInfo.dwStartTID = m_TickInfo[dwPair][TKI_70TICK][dwCurIdx].dwTID;
				g_BBBreakInfo.dwStartPSigma = CalcBollingerBandsSigma(dwPair, dwCurIdx, 21, 2);
				g_BBBreakInfo.dwStartSMA = CalcBollingerBandsSigma(dwPair, dwCurIdx, 21, 0);
				g_BBBreakInfo.dwStartMSigma = CalcBollingerBandsSigma(dwPair, dwCurIdx, 21, -2);

			}
		}
	}

	return;
}

void CAgentWorkerDlg::RegisterRate_70Tick(DWORD dwPair, DWORD dwRate, SYSTEMTIME* pRegTime)
{
	DWORD dwTType = TKI_70TICK;
	DWORD dwTID = 0;
	DWORD dwEnd = m_TickInfo[dwPair][dwTType].size() - 1;
	//DWORD dwTID;

	TICK_INFO AddData;

	//	map<DWORD,map<DWORD,vector<TICK_INFO>>>

		//	Tickを切り替える(初回もこっちに入れる
	if(m_TickInfo[dwPair][dwTType].size() == 0 || m_TickInfo[dwPair][dwTType][dwEnd].bCl != FALSE) {
		if(m_TickInfo[dwPair][dwTType].size() > 0) {
			//	最大数に達していたら、先頭を削除する
			if(m_TickInfo[dwPair][dwTType].size() >= MAX_TICK_INFOCNT) {
				m_TickInfo[dwPair][dwTType].erase(m_TickInfo[dwPair][dwTType].begin());

				dwEnd = m_TickInfo[dwPair][dwTType].size() - 1;
			}
			dwTID = m_TickInfo[dwPair][dwTType][dwEnd].dwTID + 1;
		}

		memset(&AddData, 0x00, sizeof(TICK_INFO));

		AddData.Ver = TICK_LATEST_VER;
		AddData.dwTickType = dwTType;
		AddData.Hi = dwRate;
		AddData.Op = dwRate;
		AddData.Cl = dwRate;
		AddData.Lo = dwRate;
		memcpy(&AddData.sDt, pRegTime, sizeof(SYSTEMTIME));
		AddData.ema = 0;
		AddData.Tc = 1;
		AddData.dwTID = dwTID;
		AddData.dwRateAry[0] = dwRate;

		if(m_TickInfo[dwPair][dwTType].size() != 0) {
			if(m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].dwRateDirect == RATE_DIRECT_UP) {
				if(m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].dwRateAry[m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].Tc - 1] > dwRate) {
					AddData.dwRateDirect = RATE_DIRECT_DOWN;
					AddData.dwDirectChangeCnt++;
					AddData.dwDnCnt++;
					AddData.dwDnVal += m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].dwRateAry[m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].Tc - 1] - dwRate;
				}
				else {
					AddData.dwRateDirect = RATE_DIRECT_UP;
					AddData.dwUpCnt++;
					AddData.dwUpVal += dwRate - m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].dwRateAry[m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].Tc - 1];

				}
			}
			else if(m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].dwRateDirect == RATE_DIRECT_DOWN) {
				if(m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].dwRateAry[m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].Tc - 1] < dwRate) {
					AddData.dwRateDirect = RATE_DIRECT_UP;
					AddData.dwDirectChangeCnt++;
					AddData.dwUpCnt++;
					AddData.dwUpVal += dwRate - m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].dwRateAry[m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].Tc - 1];
				}
				else {
					AddData.dwRateDirect = RATE_DIRECT_DOWN;
					AddData.dwDnCnt++;
					AddData.dwDnVal += m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].dwRateAry[m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].Tc - 1] - dwRate;
				}
			}
		}
		m_TickInfo[dwPair][dwTType].push_back(AddData);

		//	リストデータを更新する
		OutputTickInfo(dwPair);
	}
	//	最後のTickデータを更新する
	else {
		static DWORD dwPrevTick = 0;
		BOOL bUpdateView = TRUE;

		if(dwPrevTick != 0 && dwPrevTick * 100 < GetTickCount())	bUpdateView = FALSE;

		//	最高または最低を必要があれば更新
		if(m_TickInfo[dwPair][dwTType][dwEnd].Hi < dwRate) {
			m_TickInfo[dwPair][dwTType][dwEnd].Hi = dwRate;

			if(bUpdateView)	UpdateTickInfoData(dwPair, TI_COL_TYPE_HIGH);
		}
		else if(m_TickInfo[dwPair][dwTType][dwEnd].Lo > dwRate) {
			m_TickInfo[dwPair][dwTType][dwEnd].Lo = dwRate;

			if(bUpdateView)	UpdateTickInfoData(dwPair, TI_COL_TYPE_LOW);
		}

		//	最終は常に更新する
		if(m_TickInfo[dwPair][dwTType][dwEnd].Cl != dwRate) {
			m_TickInfo[dwPair][dwTType][dwEnd].Cl = dwRate;

			if(bUpdateView)	UpdateTickInfoData(dwPair, TI_COL_TYPE_CLOSE);
		}

		if(m_TickInfo[dwPair][dwTType][dwEnd].Tc != 0) {
			if(m_TickInfo[dwPair][dwTType][dwEnd].dwRateDirect == RATE_DIRECT_UP) {
				if(m_TickInfo[dwPair][dwTType][dwEnd].dwRateAry[m_TickInfo[dwPair][dwTType][dwEnd].Tc - 1] > dwRate) {
					m_TickInfo[dwPair][dwTType][dwEnd].dwRateDirect = RATE_DIRECT_DOWN;
					m_TickInfo[dwPair][dwTType][dwEnd].dwDirectChangeCnt++;
					m_TickInfo[dwPair][dwTType][dwEnd].dwDnCnt++;
					m_TickInfo[dwPair][dwTType][dwEnd].dwDnVal += m_TickInfo[dwPair][dwTType][dwEnd].dwRateAry[m_TickInfo[dwPair][dwTType][dwEnd].Tc - 1] - dwRate;
				}else{
					m_TickInfo[dwPair][dwTType][dwEnd].dwUpCnt++;
					m_TickInfo[dwPair][dwTType][dwEnd].dwUpVal += dwRate - m_TickInfo[dwPair][dwTType][dwEnd].dwRateAry[m_TickInfo[dwPair][dwTType][dwEnd].Tc - 1];
				}
			}
			else if(m_TickInfo[dwPair][dwTType][dwEnd].dwRateDirect == RATE_DIRECT_DOWN) {
				if(m_TickInfo[dwPair][dwTType][dwEnd].dwRateAry[m_TickInfo[dwPair][dwTType][dwEnd].Tc - 1] < dwRate) {
					m_TickInfo[dwPair][dwTType][dwEnd].dwRateDirect = RATE_DIRECT_UP;
					m_TickInfo[dwPair][dwTType][dwEnd].dwDirectChangeCnt++;
					m_TickInfo[dwPair][dwTType][dwEnd].dwUpCnt++;
					m_TickInfo[dwPair][dwTType][dwEnd].dwUpVal += dwRate - m_TickInfo[dwPair][dwTType][dwEnd].dwRateAry[m_TickInfo[dwPair][dwTType][dwEnd].Tc - 1];
				}else{
					m_TickInfo[dwPair][dwTType][dwEnd].dwDnCnt++;
					m_TickInfo[dwPair][dwTType][dwEnd].dwDnVal += m_TickInfo[dwPair][dwTType][dwEnd].dwRateAry[m_TickInfo[dwPair][dwTType][dwEnd].Tc - 1] - dwRate;
				}
			}
			else if(m_TickInfo[dwPair][dwTType][dwEnd].dwRateDirect == RATE_DIRECT_STAY) {
				if(m_TickInfo[dwPair][dwTType][dwEnd].dwRateAry[m_TickInfo[dwPair][dwTType][dwEnd].Tc - 1] > dwRate) {
					m_TickInfo[dwPair][dwTType][dwEnd].dwRateDirect = RATE_DIRECT_DOWN;
					m_TickInfo[dwPair][dwTType][dwEnd].dwDirectChangeCnt++;
					m_TickInfo[dwPair][dwTType][dwEnd].dwDnCnt++;
					m_TickInfo[dwPair][dwTType][dwEnd].dwDnVal += m_TickInfo[dwPair][dwTType][dwEnd].dwRateAry[m_TickInfo[dwPair][dwTType][dwEnd].Tc - 1] - dwRate;
				}
				else if(m_TickInfo[dwPair][dwTType][dwEnd].dwRateAry[m_TickInfo[dwPair][dwTType][dwEnd].Tc - 1] < dwRate) {
					m_TickInfo[dwPair][dwTType][dwEnd].dwRateDirect = RATE_DIRECT_UP;
					m_TickInfo[dwPair][dwTType][dwEnd].dwDirectChangeCnt++;
					m_TickInfo[dwPair][dwTType][dwEnd].dwUpCnt++;
					m_TickInfo[dwPair][dwTType][dwEnd].dwUpVal += dwRate - m_TickInfo[dwPair][dwTType][dwEnd].dwRateAry[m_TickInfo[dwPair][dwTType][dwEnd].Tc - 1];
				}
			}
		}

		m_TickInfo[dwPair][dwTType][dwEnd].dwRateAry[m_TickInfo[dwPair][dwTType][dwEnd].Tc] = dwRate;
		m_TickInfo[dwPair][dwTType][dwEnd].Tc++;
		if(bUpdateView)	UpdateTickInfoData(dwPair, TI_COL_TYPE_CNT);

		if(m_TickInfo[dwPair][dwTType][dwEnd].Tc >= 70) {
			memcpy(&m_TickInfo[dwPair][dwTType][dwEnd].eDt, pRegTime, sizeof(SYSTEMTIME));

			m_TickInfo[dwPair][dwTType][dwEnd].bCl = TRUE;

			//	更新時にティック情報をファイルに書き込む
			//	とりあえずAskだけ
			TickInfoWrite(dwPair, dwTType, dwEnd);

			GetIchimokuData(dwPair, dwEnd, &m_TickInfo[dwPair][dwTType][dwEnd].ichi_info);

			SigmaCheck(dwPair);

			dwPrevTick = GetTickCount();
		}
	}
}

void CAgentWorkerDlg::RegisterRate_1Min( DWORD dwPair, DWORD dwRate, SYSTEMTIME* pRegTime )
{
	DWORD dwTType = TKI_1MIN;
	DWORD dwTID = 0;
	DWORD dwEnd = m_TickInfo[dwPair][dwTType].size() - 1;
	TICK_INFO AddData;

	//	map<DWORD,map<DWORD,vector<TICK_INFO>>>

		//	Tickを切り替える(初回もこっちに入れる
	if(m_TickInfo[dwPair][dwTType].size() == 0 || m_TickInfo[dwPair][dwTType][dwEnd].bCl != FALSE) {
		if(m_TickInfo[dwPair][dwTType].size() > 0) {
			//	最大数に達していたら、先頭を削除する
			if(m_TickInfo[dwPair][dwTType].size() >= MAX_TICK_INFOCNT) {
				m_TickInfo[dwPair][dwTType].erase(m_TickInfo[dwPair][dwTType].begin());

				dwEnd = m_TickInfo[dwPair][dwTType].size() - 1;
			}
			dwTID = m_TickInfo[dwPair][dwTType][dwEnd].dwTID + 1;
		}

		memset(&AddData, 0x00, sizeof(TICK_INFO));

		AddData.Ver = TICK_LATEST_VER;
		AddData.dwTickType = dwTType;
		AddData.Hi = dwRate;
		AddData.Op = dwRate;
		AddData.Cl = dwRate;
		AddData.Lo = dwRate;
		memcpy(&AddData.sDt, pRegTime, sizeof(SYSTEMTIME));
		AddData.ema = 0;
		AddData.Tc = 1;
		AddData.dwTID = dwTID;
		AddData.dwRateAry[0] = dwRate;

		////if(m_TickInfo[dwPair][dwTType].size() != 0) {
		////	if(m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].dwRateDirect == RATE_DIRECT_UP) {
		////		if(m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].dwRateAry[m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].Tc - 1] > dwRate) {
		////			AddData.dwRateDirect = RATE_DIRECT_DOWN;
		////			AddData.dwDirectChangeCnt++;
		////		}
		////		else {
		////			AddData.dwRateDirect = RATE_DIRECT_UP;
		////		}
		////	}
		////	else if(m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].dwRateDirect == RATE_DIRECT_DOWN) {
		////		if(m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].dwRateAry[m_TickInfo[dwPair][dwTType][m_TickInfo[dwPair][dwTType].size() - 1].Tc - 1] < dwRate) {
		////			AddData.dwRateDirect = RATE_DIRECT_UP;
		////			AddData.dwDirectChangeCnt++;
		////		}
		////		else {
		////			AddData.dwRateDirect = RATE_DIRECT_DOWN;
		////		}
		////	}
		////}
		m_TickInfo[dwPair][dwTType].push_back(AddData);

		//	リストデータを更新する
		OutputTickInfo(dwPair);
	}
	//	最後のTickデータを更新する
	else {
		//	最高または最低を必要があれば更新
		if(m_TickInfo[dwPair][dwTType][dwEnd].Hi < dwRate) {
			m_TickInfo[dwPair][dwTType][dwEnd].Hi = dwRate;
		}
		else if(m_TickInfo[dwPair][dwTType][dwEnd].Lo > dwRate) {
			m_TickInfo[dwPair][dwTType][dwEnd].Lo = dwRate;
		}

		//	最終は常に更新する
		if(m_TickInfo[dwPair][dwTType][dwEnd].Cl != dwRate) {
			m_TickInfo[dwPair][dwTType][dwEnd].Cl = dwRate;
		}

		///m_TickInfo[dwPair][dwTType][dwEnd].dwRateAry[m_TickInfo[dwPair][dwTType][dwEnd].Tc] = dwRate;
		m_TickInfo[dwPair][dwTType][dwEnd].Tc++;

		if(m_TickInfo[dwPair][dwTType][dwEnd].sDt.wMinute != pRegTime->wMinute ) {
			memcpy(&m_TickInfo[dwPair][dwTType][dwEnd].eDt, pRegTime, sizeof(SYSTEMTIME));
			m_TickInfo[dwPair][dwTType][dwEnd].bCl = TRUE;

			//	更新時にティック情報をファイルに書き込む
			//	とりあえずAskだけ
			TickInfoWrite(dwPair, dwTType, dwEnd);

			///GetIchimokuData(dwPair, dwEnd, &m_TickInfo[dwPair][dwTType][dwEnd].ichi_info);

			///SigmaCheck(dwPair);
		}
	}
}

typedef struct _RATECHK_SETTING
{
	struct _SUDDEN_UP{
		BOOL bEnable;
		DWORD dwSpan;
		DWORD dwVal;
	}su;
	struct _OVER_VAL{
		BOOL bEnable;
	}ov;
	struct _RANGECHK{
		BOOL bEnable;
		DWORD dwSpan;
		DWORD dwWidth;
		DWORD dwVagueVal;
	}rc;
	struct OVER_MAX{
		BOOL bEnable;
	}om;
	struct OVER_3SIGMA{
		BOOL bEnable;
	}os;
	struct ALLTICK_UP{
		BOOL bEnable;
	}au;
}RATECHK_SETTING,*PRATECHK_SETTING;

RATECHK_SETTING m_RateChkSet;

void CAgentWorkerDlg::GetRateCheckSetting( DWORD dwPair )
{
	char cIniPath[1024];
	char* pFileName;
	char cReadBuf[256];


	GetModuleFileName(NULL, cIniPath, sizeof(cIniPath));
	pFileName = (char*)_mbsrchr((unsigned char*)cIniPath, (unsigned int)'\\');

	if( this->m_bTestMode == FALSE ){
		if( dwPair == PAIR_UNKNOWN ){
			lstrcpy(pFileName, "\\AgentWorker.ini");
		}else{
			wsprintf( pFileName, "\\%s_AgentWorker.ini", GetPairName(dwPair) );
		}
	}else{
		lstrcpy(pFileName, "\\AgentWorker_Test.ini");
	}

	memset( &m_RateChkSet, 0x00, sizeof(RATECHK_SETTING));

	//	数Tickで10pipsを超えた
	GetPrivateProfileString("SUDDEN_UP", "ENABLE", "1", cReadBuf, sizeof(cReadBuf), cIniPath);
	m_RateChkSet.su.bEnable = (atoi(cReadBuf)==0)?FALSE:TRUE;
	GetPrivateProfileString("SUDDEN_UP", "SPANTICK", "5", cReadBuf, sizeof(cReadBuf), cIniPath);
	m_RateChkSet.su.dwSpan = atoi(cReadBuf);
	GetPrivateProfileString("SUDDEN_UP", "UPVALUE", "100", cReadBuf, sizeof(cReadBuf), cIniPath);
	m_RateChkSet.su.dwVal = atoi(cReadBuf);

	//	キリのいいpipsを超えたら
	GetPrivateProfileString("OVER_VAL", "ENABLE", "1", cReadBuf, sizeof(cReadBuf), cIniPath);
	m_RateChkSet.ov.bEnable = (atoi(cReadBuf)==0)?FALSE:TRUE;

	//	一定期間10pipsの幅に収まっている
	GetPrivateProfileString("RANGECHK", "ENABLE", "1", cReadBuf, sizeof(cReadBuf), cIniPath);
	m_RateChkSet.rc.bEnable = (atoi(cReadBuf)==0)?FALSE:TRUE;
	GetPrivateProfileString("RANGECHK", "SPANTICK", "30", cReadBuf, sizeof(cReadBuf), cIniPath);
	m_RateChkSet.rc.dwSpan = atoi(cReadBuf);
	GetPrivateProfileString("RANGECHK", "RANGEWIDTH", "100", cReadBuf, sizeof(cReadBuf), cIniPath);
	m_RateChkSet.rc.dwWidth = atoi(cReadBuf);
	GetPrivateProfileString("RANGECHK", "VAGUEVAL", "20", cReadBuf, sizeof(cReadBuf), cIniPath);
	m_RateChkSet.rc.dwVagueVal = atoi(cReadBuf);

	//	一定期間内の最大値を超えた場合
	GetPrivateProfileString("OVER_MAX", "ENABLE", "1", cReadBuf, sizeof(cReadBuf), cIniPath);
	m_RateChkSet.om.bEnable = (atoi(cReadBuf)==0)?FALSE:TRUE;

	//	δ3を突破
	GetPrivateProfileString("OVER_3SIGMA", "ENABLE", "1", cReadBuf, sizeof(cReadBuf), cIniPath);
	m_RateChkSet.os.bEnable = (atoi(cReadBuf)==0)?FALSE:TRUE;

	//	70Tick、1M、5M、15M、30、1Hがすべて上昇だったら
	GetPrivateProfileString("ALLTICK_UP", "ENABLE", "1", cReadBuf, sizeof(cReadBuf), cIniPath);
	m_RateChkSet.au.bEnable = (atoi(cReadBuf)==0)?FALSE:TRUE;

	

	return;
}

bool CAgentWorkerDlg::RateCheckWrite( DWORD dwPair, RATE_CHECK_INFO* pRci ) {
	char cLogFilePath[1024];
	HANDLE hFile;
	DWORD dwWrite;
	SYSTEMTIME systime;

	if (DEBUG_FILEOUT < 1)	return false;

	GetLocalTime(&systime);
	if(m_bTestMode==FALSE){
		wsprintf(cLogFilePath, "%s\\RCI\\%s_%02d%02d%02d.rci", _LOG_DIR_, GetPairName(dwPair), systime.wYear, systime.wMonth, systime.wDay);

		hFile = CreateFile(cLogFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		SetFilePointer(hFile, 0, 0, FILE_END);
	}
	//	\TestMode_Result\20161029_142530\usdjpy.tki
	else{
		wsprintf(cLogFilePath, "%s\\TestMode_Result\\%s", _LOG_DIR_, m_Setting[dwPair].cTestName );

		CreateDirectory(cLogFilePath, NULL);

		if(m_bAutoMode != FALSE && m_bAnaTKIWrite == FALSE ) {
			return false;
		}

		wsprintf(cLogFilePath, "%s\\TestMode_Result\\%s\\%s_%s.rci", _LOG_DIR_, m_Setting[dwPair].cTestName, GetPairName(dwPair), GetTickName(TKI_70TICK) );

		hFile = CreateFile(cLogFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		SetFilePointer(hFile, 0, 0, FILE_END);
	}

	if ( hFile != INVALID_HANDLE_VALUE) {
		WriteFile(hFile, pRci, sizeof(RATE_CHECK_INFO), &dwWrite, NULL);

		CloseHandle(hFile);
	}

	return true;
}


DWORD CAgentWorkerDlg::GetTickDataFromTime( DWORD dwPair, SYSTEMTIME* pRegTime, DWORD dwBefTime )
{
	DWORD dwIdx = -1;
	CTime CurTime;// = CTime(*pRegTime);
	CTime time;
	DWORD dwTType = TKI_70TICK;
	DWORD dwEnd = m_TickInfo[dwPair][dwTType].size() - 1;

	if( pRegTime != NULL ){
		CurTime = CTime(*pRegTime);
	}else{
		CurTime = CTime::GetCurrentTime();
	}

	if( dwBefTime == TIME_BEFORE_1MIN ){
		time = CurTime - CTimeSpan( 0, 0, 1, 0);
	}else if( dwBefTime == TIME_BEFORE_5MIN ){
		time = CurTime - CTimeSpan( 0, 0, 5, 0);
	}else if( dwBefTime == TIME_BEFORE_15MIN ){
		time = CurTime - CTimeSpan( 0, 0, 15, 0);
	}else if( dwBefTime == TIME_BEFORE_30MIN ){
		time = CurTime - CTimeSpan( 0, 0, 30, 0);
	}else if( dwBefTime == TIME_BEFORE_1HOUR ){
		time = CurTime - CTimeSpan( 0, 1, 0, 0);
	}

	for( int i = (int)dwEnd; i >= 0; i-- ){
		if( m_TickInfo[dwPair][dwTType][i].sDt.wYear <= time.GetYear()
		 && m_TickInfo[dwPair][dwTType][i].sDt.wMonth <= time.GetMonth()
		 && m_TickInfo[dwPair][dwTType][i].sDt.wDay <= time.GetDay()
		 && m_TickInfo[dwPair][dwTType][i].sDt.wHour <= time.GetHour()
		 && m_TickInfo[dwPair][dwTType][i].sDt.wMinute <= time.GetMinute()
		){
			dwIdx = i;
			break;
		}
	}

	return dwIdx;
}



//	取引ってよりはRateの流れをチェックして場合によっては記録する
DWORD CAgentWorkerDlg::RateCheck_Main( DWORD dwPair, SYSTEMTIME* pRegTime )
{
	DWORD dwRet = 0;
	DWORD dwTType = TKI_70TICK;
	DWORD dwEnd = m_TickInfo[dwPair][dwTType].size() - 1;
	RATE_CHECK_INFO rci;
	DWORD dwCurTime = GetTickCount();
	CTime ChkTime;
	CTime CurTime = CTime(*pRegTime);
	static BOOL bFirst = TRUE;

	if( dwEnd < MAX_TICK_INFOCNT - 1 )	return 0;

	if( bFirst != FALSE ){
		GetRateCheckSetting(dwPair);
		bFirst = FALSE;
	}

	//	数Tickで10pipsを超えた
	if( m_RateChkSet.su.bEnable != FALSE ){
		ChkTime = m_LastChkSUH + CTimeSpan( 0, 0, 30, 0);
		if( ChkTime < CurTime ){
			if( dwEnd + 1  >= m_RateChkSet.su.dwSpan ){
				DWORD dwMax = 0;
				DWORD dwMin = 77777777;
				for( DWORD i = dwEnd-m_RateChkSet.su.dwSpan; i <= dwEnd; i++ ){
					if( m_TickInfo[dwPair][dwTType][i].Hi > dwMax ){
						dwMax = m_TickInfo[dwPair][dwTType][i].Hi;
					}
					if( m_TickInfo[dwPair][dwTType][i].Lo < dwMin ){
						dwMin = m_TickInfo[dwPair][dwTType][i].Lo;
					}
				}
				if( dwMin < m_TickInfo[dwPair][dwTType][dwEnd].Cl ){
					if( m_TickInfo[dwPair][dwTType][dwEnd].Cl - dwMin > m_RateChkSet.su.dwVal ){
						memset( &rci, 0x00, sizeof(RATE_CHECK_INFO));
						rci.dwChkType = RATECHK_SUDDEN_UP;
						dwRet |= RATECHK_SUDDEN_UP;
						rci.dwChkRate = m_TickInfo[dwPair][dwTType][dwEnd].Cl;
						rci.dwUpOrDn = 1;
						rci.dwMaxVal = m_TickInfo[dwPair][dwTType][dwEnd].Cl;
						rci.dwMinVal = dwMin;
						memcpy( &rci.sDt, &m_TickInfo[dwPair][dwTType][dwEnd - m_RateChkSet.su.dwSpan].sDt, sizeof(SYSTEMTIME) );
						memcpy( &rci.eDt, pRegTime, sizeof(SYSTEMTIME) );

						RateCheckWrite( dwPair, &rci );

						m_LastChkSUH = CurTime;
					}
				}			
			}
		}
		ChkTime = m_LastChkSUL + CTimeSpan( 0, 0, 30, 0);
		if( ChkTime < CurTime ){
			if( dwEnd + 1  >= m_RateChkSet.su.dwSpan ){
				DWORD dwMax = 0;
				DWORD dwMin = 77777777;
				for( DWORD i = dwEnd-m_RateChkSet.su.dwSpan; i <= dwEnd; i++ ){
					if( m_TickInfo[dwPair][dwTType][i].Hi > dwMax ){
						dwMax = m_TickInfo[dwPair][dwTType][i].Hi;
					}
					if( m_TickInfo[dwPair][dwTType][i].Lo < dwMin ){
						dwMin = m_TickInfo[dwPair][dwTType][i].Lo;
					}
				}

				if( dwMax > m_TickInfo[dwPair][dwTType][dwEnd].Cl ){
					if( dwMax - m_TickInfo[dwPair][dwTType][dwEnd].Cl > m_RateChkSet.su.dwVal ){
						memset( &rci, 0x00, sizeof(RATE_CHECK_INFO));
						rci.dwChkType = RATECHK_SUDDEN_UP;
						dwRet |= RATECHK_SUDDEN_UP;
						rci.dwChkRate = m_TickInfo[dwPair][dwTType][dwEnd].Cl;
						rci.dwUpOrDn = 2;
						rci.dwMaxVal = dwMax;
						rci.dwMinVal = m_TickInfo[dwPair][dwTType][dwEnd].Cl;
						memcpy( &rci.sDt, &m_TickInfo[dwPair][dwTType][dwEnd - m_RateChkSet.su.dwSpan].sDt, sizeof(SYSTEMTIME) );
						memcpy( &rci.eDt, pRegTime, sizeof(SYSTEMTIME) );

						RateCheckWrite( dwPair, &rci );

						m_LastChkSUL = CurTime;
					}
				}
			}
		}
	}

	//	キリのいいpipsを超えたら
	if( m_RateChkSet.ov.bEnable != FALSE ){
		ChkTime = m_LastChkOV + CTimeSpan( 0, 0, 10, 0);
		if( ChkTime < CurTime ){
			DWORD dwLine;
			if( m_TickInfo[dwPair][dwTType][dwEnd].Cl > m_TickInfo[dwPair][dwTType][dwEnd].Op ){
				dwLine = ((m_TickInfo[dwPair][dwTType][dwEnd].Op + 1000 )/1000)*1000;
				if( dwLine < m_TickInfo[dwPair][dwTType][dwEnd].Cl ){
					memset( &rci, 0x00, sizeof(RATE_CHECK_INFO));
					rci.dwChkType = RATECHK_OVER_VAL;
					dwRet |= RATECHK_OVER_VAL;
					rci.dwChkRate = m_TickInfo[dwPair][dwTType][dwEnd].Cl;
					rci.dwUpOrDn = 1;
					memcpy( &rci.sDt, &m_TickInfo[dwPair][dwTType][dwEnd].sDt, sizeof(SYSTEMTIME) );
					memcpy( &rci.eDt, pRegTime, sizeof(SYSTEMTIME) );

					RateCheckWrite( dwPair, &rci );

					m_LastChkOV = CurTime;
				}
			}else if( m_TickInfo[dwPair][dwTType][dwEnd].Cl < m_TickInfo[dwPair][dwTType][dwEnd].Op ){
				dwLine = ((m_TickInfo[dwPair][dwTType][dwEnd].Op )/1000)*1000;
				if( dwLine > m_TickInfo[dwPair][dwTType][dwEnd].Cl ){
					memset( &rci, 0x00, sizeof(RATE_CHECK_INFO));
					rci.dwChkType = RATECHK_OVER_VAL;
					dwRet |= RATECHK_OVER_VAL;
					rci.dwChkRate = m_TickInfo[dwPair][dwTType][dwEnd].Cl;
					rci.dwUpOrDn = 2;
					memcpy( &rci.sDt, &m_TickInfo[dwPair][dwTType][dwEnd].sDt, sizeof(SYSTEMTIME) );
					memcpy( &rci.eDt, pRegTime, sizeof(SYSTEMTIME) );

					RateCheckWrite( dwPair, &rci );

					m_LastChkOV = CurTime;
				}
			}
		}
	}
	//	一定期間10pipsの幅に収まっている
	if( m_RateChkSet.rc.bEnable != FALSE ){
static BOOL bStockRC = FALSE;
static RATE_CHECK_INFO stock_rci;
BOOL bWrite = FALSE;

		ChkTime = m_LastChkRC + CTimeSpan( 0, 0, 15, 0);
		if( ChkTime < CurTime ){
			if( bStockRC != FALSE ){
				if( m_TickInfo[dwPair][dwTType][dwEnd].Cl > stock_rci.dwMaxVal + m_RateChkSet.rc.dwVagueVal){
					//if( m_TickInfo[dwPair][dwTType][dwEnd].Cl - stock_rci.dwMinVal >= m_RateChkSet.rc.dwWidth )
					{
						//stock_rci.dwMaxVal = m_TickInfo[dwPair][dwTType][dwEnd].Cl;
						bWrite = TRUE;
					}
				}else if( m_TickInfo[dwPair][dwTType][dwEnd].Cl < stock_rci.dwMinVal - m_RateChkSet.rc.dwVagueVal ){
					//if( stock_rci.dwMaxVal- m_TickInfo[dwPair][dwTType][dwEnd].Cl >= m_RateChkSet.rc.dwWidth )
					{
						//stock_rci.dwMinVal = m_TickInfo[dwPair][dwTType][dwEnd].Cl;
						bWrite = TRUE;
					}
				}

				if( bWrite != FALSE ){
					memcpy( &rci, &stock_rci, sizeof(RATE_CHECK_INFO));
					memcpy( &rci.eDt, pRegTime, sizeof(SYSTEMTIME) );
					bStockRC = FALSE;

					RateCheckWrite( dwPair, &rci );

					m_LastChkRC = CurTime;
				}
			}else{
				if( dwEnd + 1  >= m_RateChkSet.rc.dwSpan ){
					DWORD dwMax = 0;
					DWORD dwMin = 77777777;
					for( DWORD i = dwEnd-m_RateChkSet.rc.dwSpan; i <= dwEnd; i++ ){
						if( m_TickInfo[dwPair][dwTType][i].Hi > dwMax ){
							dwMax = m_TickInfo[dwPair][dwTType][i].Hi;
						}
						if( m_TickInfo[dwPair][dwTType][i].Lo < dwMin ){
							dwMin = m_TickInfo[dwPair][dwTType][i].Lo;
						}
					}

					if( dwMax - dwMin < m_RateChkSet.rc.dwWidth ){
						memset( &stock_rci, 0x00, sizeof(RATE_CHECK_INFO));
						stock_rci.dwChkType = RATECHK_RANGECHK;
						//dwRet |= RATECHK_RANGECHK;
						stock_rci.dwUpOrDn = 0;
						stock_rci.dwMaxVal = dwMax;
						stock_rci.dwMinVal = dwMin;
						memcpy( &stock_rci.sDt, &m_TickInfo[dwPair][dwTType][dwEnd-m_RateChkSet.rc.dwSpan].sDt, sizeof(SYSTEMTIME) );
						//memcpy( &stock_rci.eDt, pRegTime, sizeof(SYSTEMTIME) );
						bStockRC = TRUE;

						//RateCheckWrite( dwPair, &rci );

						//m_LastChkRC = CurTime;
					}
				}
			}
		}
	}
	//	一定期間内の最大値を超えた場合
	if( m_RateChkSet.om.bEnable != FALSE ){
		if( dwEnd > 100 ){
			ChkTime = m_LastChkOM + CTimeSpan( 0, 0, 10, 0);
			if( ChkTime < CurTime ){
				DWORD dwMax = 0;
				DWORD dwMin = 77777777;
				for(DWORD i = 0; i < dwEnd; i++) {
					if(m_TickInfo[dwPair][dwTType][i].Hi > dwMax) {
						dwMax = m_TickInfo[dwPair][dwTType][i].Hi;
					}
					if(m_TickInfo[dwPair][dwTType][i].Lo < dwMin) {
						dwMin = m_TickInfo[dwPair][dwTType][i].Lo;
					}
				}
				if( m_TickInfo[dwPair][dwTType][dwEnd].Cl == m_TickInfo[dwPair][dwTType][dwEnd].Hi 
				 && m_TickInfo[dwPair][dwTType][dwEnd].Cl > m_TickInfo[dwPair][dwTType][dwEnd].Lo 
				){
					if( dwMax < m_TickInfo[dwPair][dwTType][dwEnd].Cl ){
						memset( &rci, 0x00, sizeof(RATE_CHECK_INFO));
						rci.dwChkType = RATECHK_OVER_MAX;
						dwRet |= RATECHK_OVER_MAX;
						rci.dwChkRate = m_TickInfo[dwPair][dwTType][dwEnd].Cl;
						rci.dwUpOrDn = 1;
						memcpy( &rci.sDt, &m_TickInfo[dwPair][dwTType][0].sDt, sizeof(SYSTEMTIME) );
						memcpy( &rci.eDt, pRegTime, sizeof(SYSTEMTIME) );

						RateCheckWrite( dwPair, &rci );

						m_LastChkOM = CurTime;
					}
				}else if( m_TickInfo[dwPair][dwTType][dwEnd].Cl == m_TickInfo[dwPair][dwTType][dwEnd].Lo 
				 && m_TickInfo[dwPair][dwTType][dwEnd].Cl < m_TickInfo[dwPair][dwTType][dwEnd].Hi 
				){
					if( dwMin > m_TickInfo[dwPair][dwTType][dwEnd].Cl ){
						memset( &rci, 0x00, sizeof(RATE_CHECK_INFO));
						rci.dwChkType = RATECHK_OVER_MAX;
						dwRet |= RATECHK_OVER_MAX;
						rci.dwChkRate = m_TickInfo[dwPair][dwTType][dwEnd].Cl;
						rci.dwUpOrDn = 2;
						memcpy( &rci.sDt, &m_TickInfo[dwPair][dwTType][0].sDt, sizeof(SYSTEMTIME) );
						memcpy( &rci.eDt, pRegTime, sizeof(SYSTEMTIME) );

						RateCheckWrite( dwPair, &rci );

						m_LastChkOM = CurTime;
					}
				}
			}
		}
	}

	//	δ3を突破
	if( m_RateChkSet.os.bEnable != FALSE ){
		ChkTime = m_LastChkOS + CTimeSpan( 0, 0, 10, 0);
		if( ChkTime < CurTime ){
			if( m_TickInfo[dwPair][dwTType][dwEnd].Cl == m_TickInfo[dwPair][dwTType][dwEnd].Hi 
			 && m_TickInfo[dwPair][dwTType][dwEnd].Cl > m_TickInfo[dwPair][dwTType][dwEnd].Lo 
			){
				if( CalcBollingerBandsSigma( dwPair, dwEnd, 21, 3 ) < m_TickInfo[dwPair][dwTType][dwEnd].Cl ){
					memset( &rci, 0x00, sizeof(RATE_CHECK_INFO));
					rci.dwChkType = RATECHK_OVER_3SIGMA;
					dwRet |= RATECHK_OVER_3SIGMA;
					rci.dwChkRate = m_TickInfo[dwPair][dwTType][dwEnd].Cl;
					rci.dwUpOrDn = 1;
					memcpy( &rci.sDt, &m_TickInfo[dwPair][dwTType][dwEnd].sDt, sizeof(SYSTEMTIME) );
					memcpy( &rci.eDt, pRegTime, sizeof(SYSTEMTIME) );

					RateCheckWrite( dwPair, &rci );

					m_LastChkOS = CurTime;
				}
			}else if( m_TickInfo[dwPair][dwTType][dwEnd].Cl == m_TickInfo[dwPair][dwTType][dwEnd].Lo 
			 && m_TickInfo[dwPair][dwTType][dwEnd].Cl < m_TickInfo[dwPair][dwTType][dwEnd].Hi 
			){
				if( CalcBollingerBandsSigma( dwPair, dwEnd, 21, -3 ) > m_TickInfo[dwPair][dwTType][dwEnd].Cl ){
					memset( &rci, 0x00, sizeof(RATE_CHECK_INFO));
					rci.dwChkType = RATECHK_OVER_3SIGMA;
					dwRet |= RATECHK_OVER_3SIGMA;
					rci.dwChkRate = m_TickInfo[dwPair][dwTType][dwEnd].Cl;
					rci.dwUpOrDn = 2;
					memcpy( &rci.sDt, &m_TickInfo[dwPair][dwTType][dwEnd].sDt, sizeof(SYSTEMTIME) );
					memcpy( &rci.eDt, pRegTime, sizeof(SYSTEMTIME) );

					RateCheckWrite( dwPair, &rci );

					m_LastChkOS = CurTime;
				}
			}
		}
	}

	//	70Tick、1M、5M、15M、30、1Hがすべて上昇だったら
	if( m_RateChkSet.au.bEnable != FALSE ){
		ChkTime = m_LastChkAU + CTimeSpan( 0, 0, 5, 0);
		if( ChkTime < CurTime ){
			BOOL bSameDir = TRUE;
			DWORD dwIdx;

			if(m_TickInfo[dwPair][dwTType][dwEnd].Cl == m_TickInfo[dwPair][dwTType][dwEnd].Hi
				&& m_TickInfo[dwPair][dwTType][dwEnd].Cl > m_TickInfo[dwPair][dwTType][dwEnd].Lo
				) {
				for(DWORD i = TIME_BEFORE_1MIN; i < TIME_BEFORE_LAST; i++) {
					dwIdx = GetTickDataFromTime(dwPair, pRegTime, i);
					if(dwIdx == (DWORD)-1) {
						bSameDir = FALSE;
						break;
					}

					if(m_TickInfo[dwPair][dwTType][dwIdx].Op >= m_TickInfo[dwPair][dwTType][dwEnd].Cl) {
						bSameDir = FALSE;
						break;
					}
				}

				if( bSameDir != FALSE ){
					memset( &rci, 0x00, sizeof(RATE_CHECK_INFO));
					rci.dwChkType = RATECHK_ALLTICK_UP;
					dwRet |= RATECHK_ALLTICK_UP;
					rci.dwChkRate = m_TickInfo[dwPair][dwTType][dwEnd].Cl;
					rci.dwUpOrDn = 1;
					memcpy( &rci.sDt, &m_TickInfo[dwPair][dwTType][dwEnd].sDt, sizeof(SYSTEMTIME) );
					memcpy( &rci.eDt, pRegTime, sizeof(SYSTEMTIME) );

					RateCheckWrite( dwPair, &rci );

					m_LastChkAU = CurTime;
				}
			}
			else if(m_TickInfo[dwPair][dwTType][dwEnd].Cl == m_TickInfo[dwPair][dwTType][dwEnd].Lo
				&& m_TickInfo[dwPair][dwTType][dwEnd].Cl < m_TickInfo[dwPair][dwTType][dwEnd].Hi
				) {
				for(DWORD i = TIME_BEFORE_1MIN; i < TIME_BEFORE_LAST; i++) {
					dwIdx = GetTickDataFromTime(dwPair, pRegTime, i);
					if(dwIdx == (DWORD)-1) {
						bSameDir = FALSE;
						break;
					}

					if(m_TickInfo[dwPair][dwTType][dwIdx].Op <= m_TickInfo[dwPair][dwTType][dwEnd].Cl) {
						bSameDir = FALSE;
						break;
					}
				}

				if( bSameDir != FALSE ){
					memset( &rci, 0x00, sizeof(RATE_CHECK_INFO));
					rci.dwChkType = RATECHK_ALLTICK_UP;
					dwRet |= RATECHK_ALLTICK_UP;
					rci.dwChkRate = m_TickInfo[dwPair][dwTType][dwEnd].Cl;
					rci.dwUpOrDn = 2;
					memcpy( &rci.sDt, &m_TickInfo[dwPair][dwTType][dwEnd].sDt, sizeof(SYSTEMTIME) );
					memcpy( &rci.eDt, pRegTime, sizeof(SYSTEMTIME) );

					RateCheckWrite( dwPair, &rci );

					m_LastChkAU = CurTime;
				}
			}
		}
	}

	return dwRet;
}

DWORD CAgentWorkerDlg::RegisterRate_Main( DWORD dwPair, DWORD dwRate, SYSTEMTIME* pRegTime )
{
	DWORD dwRet = 0;
	//DWORD i = dwPair;
//	TICK_INFO AddData;
	DWORD dwTID = 0;
	static DWORD dwPrevRate[PAIR_COUNT] = {0,0,0};
	static SYSTEMTIME prev_time;

	if( dwRate == 0 )	return 0;

	if( dwPair < PAIR_COUNT) {
		SYSTEMTIME systime;
		if( pRegTime != NULL ){
			memcpy( &systime, pRegTime, sizeof(SYSTEMTIME) );
		}else{
			GetLocalTime(&systime);
		}

		if( dwPrevRate[dwPair] != 0 ){
			if( dwPrevRate[dwPair] > dwRate ){
				if( dwPrevRate[dwPair] - dwRate > 300 ){
					if( prev_time.wMinute == systime.wMinute ){
						return 0;
					}
				}
			}else{
				if( dwRate - dwPrevRate[dwPair] > 300 ){
					if( prev_time.wMinute == systime.wMinute ){
						return 0;
					}
				}
			}
		}
		dwPrevRate[dwPair] = dwRate;
		memcpy( &prev_time, &systime, sizeof(SYSTEMTIME) );

		//	70Tick登録
		RegisterRate_70Tick(dwPair, dwRate, &systime );
		//	1分足登録
		//RegisterRate_1Min(dwPair, dwRate, &systime );

		RateCheck_Main( dwPair,  pRegTime );

		if( g_BBBreakInfo.dwHiRate < dwRate )	g_BBBreakInfo.dwHiRate = dwRate;
		if( g_BBBreakInfo.dwLowRate > dwRate )	g_BBBreakInfo.dwLowRate = dwRate;

		dwRet = 1;
	}


	return dwRet;
}

//	Ask値の登録
//	ファイルへの記録もここで行う
void CAgentWorkerDlg::MT4_RegisterRate(DWORD dwMapId) {
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];


	PMT4_REGISTERRATE_DATA pMapData;

	wsprintf(cMapName, "MT4_RegisterRate_%d", dwMapId);

	if ((hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cMapName)) != INVALID_HANDLE_VALUE) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_REGISTERRATE_DATA)pMap;


			pMapData->dwRet = RegisterRate_Main( pMapData->dwPair, pMapData->dwRate, NULL );

			//	Viewで現在情報を表示している場合は
			//	おまけでViewにRate情報を伝える
			if( this->m_ViewHWnd[pMapData->dwPair] != NULL ){
				::PostMessage( m_ViewHWnd[pMapData->dwPair], WM_APP+1001, pMapData->dwPair, pMapData->dwRate );
			}
			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	return;
}

void CAgentWorkerDlg::MT4_DbgRegisterTickInfo(DWORD dwMapId) {
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];
	TICK_INFO AddData;


	PMT4_DBGREGISTERTICKINFO_DATA pMapData;

	wsprintf(cMapName, "MT4_DbgRegisterTickInfo_%d", dwMapId);

	if ((hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cMapName)) != INVALID_HANDLE_VALUE) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_DBGREGISTERTICKINFO_DATA)pMap;

			DWORD i = pMapData->dwPair;

			if (i < PAIR_COUNT) {
				DWORD dwEnd = m_TickInfo[i][TKI_70TICK].size() - 1;
				SYSTEMTIME systime;
				DWORD dwTID = 0;

				GetLocalTime(&systime);

				//	現在の最後のデータを無理やり完了する
				if (m_TickInfo[i][TKI_70TICK].size() > 0) {
					if (m_TickInfo[i][TKI_70TICK][dwEnd].Tc < m_Setting[pMapData->dwPair].dwTickCount ) {
						m_TickInfo[i][TKI_70TICK][dwEnd].Tc = m_Setting[pMapData->dwPair].dwTickCount;
						dwTID = m_TickInfo[i][TKI_70TICK][dwEnd].dwTID + 1;

						memcpy(&(m_TickInfo[i][TKI_70TICK][dwEnd].eDt), &systime, sizeof(SYSTEMTIME));

						if (m_TickInfo[i][TKI_70TICK].size() >= MAX_TICK_INFOCNT) {
							m_TickInfo[i][TKI_70TICK].erase(m_TickInfo[i][TKI_70TICK].begin());
						}
					}
				}

				memset(&AddData, 0x00, sizeof(TICK_INFO));

				AddData.Hi = pMapData->dwHi;
				AddData.Op = pMapData->dwOp;
				AddData.Cl = pMapData->dwCl;
				AddData.Lo = pMapData->dwLo;
				memcpy(&AddData.sDt, &systime, sizeof(SYSTEMTIME));
				memcpy(&AddData.eDt, &systime, sizeof(SYSTEMTIME));
				AddData.ema = 0;
				AddData.Tc = m_Setting[pMapData->dwPair].dwTickCount;
				AddData.dwTID = dwTID;

				m_TickInfo[i][TKI_70TICK].push_back(AddData);

				OutputTickInfo(i);
				
				pMapData->dwRet = 1;
			}
			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	return;
}

void CAgentWorkerDlg::MakeSendBuffer(char* pSendBuf, DWORD dwPair, double dVal, BOOL bBuy)
{
	if ((int)bBuy == -1) {
		if (dwPair == PAIR_EURUSD || dwPair == PAIR_GBPUSD || dwPair == PAIR_USDCHF ){
			_stprintf_s(pSendBuf, 1024, "%s:%.4f", GetPairName(dwPair), dVal);
		}
		else {
			_stprintf_s(pSendBuf, 1024, "%s:%.2f", GetPairName(dwPair), dVal);
		}
	}
	else if( bBuy != FALSE ){
		if (dwPair == PAIR_EURUSD || dwPair == PAIR_GBPUSD || dwPair == PAIR_USDCHF ){
			_stprintf_s(pSendBuf, 1024, "%s:[BUY]:%.4f", GetPairName(dwPair), dVal);
		}
		else {
			_stprintf_s(pSendBuf, 1024, "%s:[BUY]:%.2f", GetPairName(dwPair), dVal);
		}
	}
	else{
		if (dwPair == PAIR_EURUSD || dwPair == PAIR_GBPUSD || dwPair == PAIR_USDCHF ){
			_stprintf_s(pSendBuf, 1024, "%s:[SELL]:%.4f", GetPairName(dwPair), dVal);
		}
		else {
			_stprintf_s(pSendBuf, 1024, "%s:[SELL]:%.2f", GetPairName(dwPair), dVal);
		}
	}

	return;
}


DWORD CAgentWorkerDlg::CalcEMAValue(DWORD idx, DWORD dwEmaVal, DWORD dwPair )
{
	DWORD dwRet = 0;
	bool bFirst = false;
	int nCurIdx = idx;
	DWORD sumCl = 0;
	int i;

	if(idx < dwEmaVal - 1) {
		return 0;
	}

	if(idx <= dwEmaVal - 1) {
		bFirst = true;
	}

	sumCl = 0;
	for(i = idx; i >(int)idx - (int)dwEmaVal; i--) {
		sumCl += m_TickInfo[dwPair][TKI_70TICK][i].Cl;
	}

	if(bFirst == false) {
		sumCl += m_TickInfo[dwPair][TKI_70TICK][idx].Cl;
		dwRet = sumCl / (dwEmaVal + 1);
	}
	else {
		dwRet = sumCl / dwEmaVal;
	}

	return dwRet;
}

DWORD CAgentWorkerDlg::TradeCheck_EMABreak(DWORD dwPair)
{
	DWORD dwRet = 0;

	DWORD dwCurIdx = m_TickInfo[dwPair][TKI_70TICK].size()-1;
	DWORD dwEMA1;
	DWORD dwEMA2;
	DWORD dwPrevEMA1;
	DWORD dwPrevEMA2;

	if( dwCurIdx > 0 ){
		dwEMA1 = CalcEMAValue(dwCurIdx, m_Setting[dwPair].mab.dwMA1Val, dwPair );
		dwEMA2 = CalcEMAValue(dwCurIdx, m_Setting[dwPair].mab.dwMA2Val, dwPair);
		dwPrevEMA1 = CalcEMAValue(dwCurIdx-1, m_Setting[dwPair].mab.dwMA1Val, dwPair);
		dwPrevEMA2 = CalcEMAValue(dwCurIdx-1, m_Setting[dwPair].mab.dwMA2Val, dwPair);

		if( dwEMA1 > 0 && dwEMA2 > 0 && dwPrevEMA1 > 0 && dwPrevEMA2 > 0 ){
			if( dwPrevEMA1 < dwPrevEMA2 && dwEMA1 > dwEMA2) {
				if(dwEMA1- dwEMA2 > m_Setting[dwPair].mab.dwSigTrig ){
					dwRet = 1;
				}
			}
			else if( dwPrevEMA1 > dwPrevEMA2 && dwEMA1 < dwEMA2) {
				if( dwEMA2 - dwEMA1 > m_Setting[dwPair].mab.dwSigTrig ) {
					dwRet = 2;
				}
			}
		}
	}

	return dwRet;
}

DWORD CAgentWorkerDlg::TradeCheck_DoujiBreak(DWORD dwPair )
{
	DWORD dwRet = 0;

	vector<TICK_INFO>::iterator itr_e = m_TickInfo[dwPair][TKI_70TICK].end() - 1;

	DWORD dwCurRate = (*itr_e).Cl;

	//	一個前からさかのぼっていく
	DWORD dwPrevDojiVal = 0;
	DWORD dojiCnt = 0;
	DWORD dwABSOpCl;
	DWORD dwABSHiLo;

	for(itr_e--; itr_e != m_TickInfo[dwPair][TKI_70TICK].begin(); itr_e--) {
		dwABSOpCl = ABS((*itr_e).Op, (*itr_e).Cl);
		dwABSHiLo = ABS((*itr_e).Hi, (*itr_e).Lo);
		if(dwPrevDojiVal == 0 
		 && dwABSOpCl <= m_Setting[dwPair].db.dRgOpCl
		 && dwABSHiLo <= m_Setting[dwPair].db.dRgHiLo
		) {
			dwPrevDojiVal = ((*itr_e).Op < (*itr_e).Cl) ? ((*itr_e).Op + ((*itr_e).Cl - (*itr_e).Op) / 2) : ((*itr_e).Cl + ((*itr_e).Op - (*itr_e).Cl) / 2);
			dojiCnt++;
		}
		else if(dwPrevDojiVal>0) {
			if(dwABSHiLo <= m_Setting[dwPair].db.dRgHiLo
			 && ( dwPrevDojiVal - (DWORD)m_Setting[dwPair].db.dRgOpCl <= (*itr_e).Op && (*itr_e).Op <= dwPrevDojiVal + (DWORD)m_Setting[dwPair].db.dRgOpCl )
			 && ( dwPrevDojiVal - (DWORD)m_Setting[dwPair].db.dRgOpCl <= (*itr_e).Cl && (*itr_e).Cl <= dwPrevDojiVal + (DWORD)m_Setting[dwPair].db.dRgOpCl )
			) {
				dojiCnt++;
			}
			else {
				if(dojiCnt >= m_Setting[dwPair].db.dwContNum ) {
					//	0:注文しない 1:買い注文 2:売り注文
					if( dwPrevDojiVal + m_Setting[dwPair].db.dSigTrig < dwCurRate ) {
						dwRet = 1;
					}
					else if( dwPrevDojiVal - m_Setting[dwPair].db.dSigTrig > dwCurRate ) {
						dwRet = 2;
					}

				}
				break;
			}
		}
		else {
			break;
		}
	}

	return dwRet;
}


BOOL CalcDM( DWORD* pdwPDM, DWORD* pdwMDM, DWORD dwNowHi, DWORD dwPrevHi, DWORD dwNowLo, DWORD dwPrevLo )
{
	BOOL bRet = FALSE;

	DWORD dwPDM = -1;
	DWORD dwMDM = -1;

	if( dwNowHi == 0
	 || dwPrevHi == 0
	 || dwNowLo == 0
	 || dwPrevLo == 0
	){
		return FALSE;
	}

	if( dwNowHi > dwPrevHi ){
		dwPDM = dwNowHi - dwPrevHi;
	}else{
		dwPDM = 0;
	}
	if( dwPrevLo > dwNowLo ){
		dwMDM = dwPrevLo - dwNowLo;
	}else{
		dwMDM = 0;
	}

	if( dwPDM > dwMDM ){
		dwMDM = 0;
	}else if( dwMDM > dwPDM ){
		dwPDM = 0;
	}else{
		dwPDM = 0;
		dwMDM = 0;
	}

	if( pdwPDM != NULL )	*pdwPDM = dwPDM;
	if( pdwMDM != NULL )	*pdwMDM = dwMDM;

	return bRet;
}

double CAgentWorkerDlg::CalcDX( DWORD dwPair, DWORD i, DWORD dwSpan )
{
	DWORD dwAdx = 0;

	double dPDI = 0;
	double dMDI = 0;
	DWORD dwPDM = 0;
	DWORD dwMDM = 0;
	DWORD dwSumPDM = 0;
	DWORD dwSumMDM = 0;
	double dSumTR = 0;
	double dDX;

	DWORD l;
	for( l = i - (dwSpan - 1); l <= i; l++ ){
		dwPDM = 0;
		dwMDM = 0;
		CalcDM( &dwPDM, &dwMDM, m_TickInfo[dwPair][TKI_70TICK][l].Hi, m_TickInfo[dwPair][TKI_70TICK][l-1].Hi, m_TickInfo[dwPair][TKI_70TICK][l].Lo, m_TickInfo[dwPair][TKI_70TICK][l-1].Lo );

		if( dwPDM == -1 || dwMDM == -1 ){
			return -1;
		}

		dwSumPDM += dwPDM;
		dwSumMDM += dwMDM;

		dSumTR += ((m_TickInfo[dwPair][TKI_70TICK][l].Hi>m_TickInfo[dwPair][TKI_70TICK][l-1].Cl)?(m_TickInfo[dwPair][TKI_70TICK][l].Hi):(m_TickInfo[dwPair][TKI_70TICK][l-1].Cl)) - ((m_TickInfo[dwPair][TKI_70TICK][l].Lo<m_TickInfo[dwPair][TKI_70TICK][l-1].Cl)?(m_TickInfo[dwPair][TKI_70TICK][l].Lo):(m_TickInfo[dwPair][TKI_70TICK][l-1].Cl));
	}

	dPDI = (double)dwSumPDM*100/dSumTR;
	dMDI = (double)dwSumMDM*100/dSumTR;

	dDX = ((dPDI>dMDI)?(dPDI-dMDI):(dMDI-dPDI))*100/(dPDI+dMDI);

	return dDX;
}

double CAgentWorkerDlg::CalcADX( DWORD dwPair, DWORD i, DWORD dwSpan )
{
	double dTemp;
	double dSumDX = 0;
	DWORD l;
	for( l = i - (dwSpan - 1); l <= i; l++ ){
		dTemp = CalcDX( dwPair, l, dwSpan );
		if( dTemp == (DWORD)-1 ){
			return 0;
		}

		dSumDX += dTemp;
	}

	dSumDX /= (double)dwSpan;

	return dSumDX;
}

/////////////////////////////////////////////////////
//σ=√(((X1-Xm)^2+(X2-Xm)^2・・・(Xn-Xm)^2)/N-1)
//N・・・任意の期間(25日を使うことが多いです)
//X1～Xn・・・1～N日までの各終値
//Xm・・・X1～Xnの平均値 
DWORD CAgentWorkerDlg::CalcBollingerBandsSigma( DWORD dwPair, DWORD i , DWORD dwSpan, double nSigma )
{
	if ( i + 1 < dwSpan )	return 0;

	DWORD l;
	double nAve = 0;
	double nMole = 0;

	for( l = i - dwSpan + 1; l <= i; l++ ){
		nAve += (double)m_TickInfo[dwPair][TKI_70TICK][l].Cl;
	}
	nAve /= (double)dwSpan;

	for( l = i - dwSpan + 1; l <= i; l++ ){
		nMole += ((double)m_TickInfo[dwPair][TKI_70TICK][l].Cl-nAve)*((double)m_TickInfo[dwPair][TKI_70TICK][l].Cl-nAve);
	}

	nMole /= (double)dwSpan-1;

	return (DWORD)(nAve + (sqrt(nMole)*nSigma));
}

double CAgentWorkerDlg::CalcSigma( DWORD dwPair, DWORD i , DWORD dwSpan )
{
	if ( i + 1 < dwSpan )	return 0;

	DWORD l;
	double nAve = 0;
	double nMole = 0;

	for( l = i - dwSpan + 1; l <= i; l++ ){
		nAve += (double)m_TickInfo[dwPair][TKI_70TICK][l].Cl;
	}
	nAve /= (double)dwSpan;

	for( l = i - dwSpan + 1; l <= i; l++ ){
		nMole += ((double)m_TickInfo[dwPair][TKI_70TICK][l].Cl-nAve)*((double)m_TickInfo[dwPair][TKI_70TICK][l].Cl-nAve);
	}

	nMole /= (double)dwSpan-1;

	return sqrt(nMole);
}


BOOL CAgentWorkerDlg::IsBandWalkStart( DWORD dwPair )
{
#if 1
	BOOL bIsBand = FALSE;
	DWORD dwIdx = m_TickInfo[dwPair][TKI_70TICK].size()-1;

	DWORD dwHiSigma1 = CalcBollingerBandsSigma( dwPair, dwIdx, 21, 1 );
	DWORD dwLoSigma1 = CalcBollingerBandsSigma( dwPair, dwIdx, 21, -1 );

	//	上突き抜け
	if( dwHiSigma1 < m_TickInfo[dwPair][TKI_70TICK][dwIdx].Cl ){
		for( DWORD i = dwIdx-1; i >= 0; i-- ){
			dwLoSigma1 = CalcBollingerBandsSigma( dwPair, i, 21, -1 );

			if( dwLoSigma1 > m_TickInfo[dwPair][TKI_70TICK][i].Lo ){
				bIsBand = TRUE;
				break;
			}else if( m_TickInfo[dwPair][TKI_70TICK][i+1].Op > m_TickInfo[dwPair][TKI_70TICK][i].Op ){
				;;
			}else{
				break;
			}
		}
	}
	//	下突き抜け
	else if( dwLoSigma1 >  m_TickInfo[dwPair][TKI_70TICK][dwIdx].Cl ){
		for( DWORD i = dwIdx-1; i >= 0; i-- ){
			dwHiSigma1 = CalcBollingerBandsSigma( dwPair, i, 21, 1 );

			if( dwHiSigma1 < m_TickInfo[dwPair][TKI_70TICK][i].Hi ){
				bIsBand = TRUE;
				break;
			}else if( m_TickInfo[dwPair][TKI_70TICK][i+1].Op < m_TickInfo[dwPair][TKI_70TICK][i].Op ){
				;;
			}else{
				break;
			}
		}
	}
#else
	BOOL bIsBand = FALSE;
	DWORD dwIdx = m_TickInfo[dwPair][TKI_70TICK].size()-1;
	double dMinSigma = 777777;
	double dTempSigma;
	DWORD dwSqHi;
	DWORD dwSqLo;

	for( DWORD i = dwIdx-1; i > dwIdx - 11; i-- ){
		dTempSigma = CalcSigma( dwPair, i, 21 );

		if( dMinSigma > dTempSigma ){
			dMinSigma = dTempSigma;
			dwSqHi = CalcBollingerBandsSigma( dwPair, i, 21, +2 );
			dwSqLo = CalcBollingerBandsSigma( dwPair, i, 21, -2 );
		}
	}

	DWORD dwHiSigma2 = CalcBollingerBandsSigma( dwPair, dwIdx, 21, 2 );
	DWORD dwLoSigma2 = CalcBollingerBandsSigma( dwPair, dwIdx, 21, -2 );
	DWORD dwSpan;

	if( dwHiSigma2 < m_TickInfo[dwPair][TKI_70TICK][dwIdx].Cl ){
		if( dwLoSigma2 < dwSqLo ){
			dwSpan = dwSqLo- dwLoSigma2;
		}
	}else{
		if( dwHiSigma2 > dwSqHi ){
			dwSpan = dwHiSigma2- dwSqHi;
		}
	}
#endif
	return bIsBand;
}


BOOL CAgentWorkerDlg::IsRangeNow( DWORD dwPair )
{
#if 1
	BOOL bIsRange = FALSE;
	double dPrevSigma;
	double dPrevPrevSigma;
	DWORD dwIdx = m_TickInfo[dwPair][TKI_70TICK].size()-1;

	dPrevSigma = CalcSigma(dwPair, dwIdx-1, 21 );
	dPrevPrevSigma = CalcSigma(dwPair, dwIdx-2, 21 );

	if( dPrevSigma <= dPrevPrevSigma ){
		bIsRange = TRUE;
	}
	return bIsRange;
#else
	BOOL bTrand = FALSE;
	DWORD dwIdx = m_TickInfo[dwPair][TKI_70TICK].size()-1;

	if( dwIdx < 21*2 ){
		return FALSE;
	}

	double dADX = CalcADX( dwPair, dwIdx, 14 );
	double dPrevADX = CalcADX( dwPair, dwIdx-1, 14 );

	if( dADX == 0 || dPrevADX == 0 ){
		return FALSE;
	}

	if( dPrevADX < dADX ){
		bTrand = TRUE;
	}

	return bTrand;
#endif
}

DWORD dwMinSigma;
struct{
	DWORD dwPeakRate;
	DWORD dwNoChangeCnt;
}g_BBChk = { 0, 0};

struct{
	DWORD dwTrandStartIdx;
	DWORD dwTrandEndIdx;
	DWORD dwTrandStartTID;
	DWORD dwTrandEndTID;
	DWORD dwBBWalkCnt;
	DWORD dwMaxGain;
	DWORD dwPeakVal;
}g_BBWalkTradeInfo = {0,0,0};

BOOL CAgentWorkerDlg::IsConvergeSigma(DWORD dwPair, BOOL bHiCheck )
{
	BOOL bConverge = FALSE;
	DWORD dwIdx = m_TickInfo[dwPair][TKI_70TICK].size()-1;

	DWORD dwHiSigma2 = CalcBollingerBandsSigma( dwPair,  dwIdx, 21, 2 );
	DWORD dwLoSigma2 = CalcBollingerBandsSigma( dwPair,  dwIdx, 21, -2 );
	DWORD dwPrevHiSigma2 = CalcBollingerBandsSigma( dwPair,  dwIdx-1, 21, 2 );
	DWORD dwPrevLoSigma2 = CalcBollingerBandsSigma( dwPair,  dwIdx-1, 21, -2 );
	DWORD i;

	if( bHiCheck != FALSE ){
		if( dwPrevHiSigma2 > dwHiSigma2 ){
			dwHiSigma2 = dwPrevHiSigma2;
			for( i = dwIdx -1; i >= 0; i-- ){
				dwPrevHiSigma2 = CalcBollingerBandsSigma( dwPair,  i-1, 21, 2 );

				if( dwHiSigma2 == dwPrevHiSigma2 ){
					;
				}else if( dwHiSigma2 > dwPrevHiSigma2 ){
					bConverge = TRUE;
					break;
				}else{
					break;
				}
			}
		}
	}else{
		if( dwPrevLoSigma2 < dwLoSigma2 ){
			dwLoSigma2 = dwPrevLoSigma2;
			for( i = dwIdx -1; i >= 0; i-- ){
				dwPrevLoSigma2 = CalcBollingerBandsSigma( dwPair,  i-1, 21, -2 );

				if( dwLoSigma2 == dwPrevLoSigma2 ){
					;
				}else if( dwLoSigma2 < dwPrevLoSigma2 ){
					bConverge = TRUE;
					break;
				}else{
					break;
				}
			}
		}
	}

	return bConverge;
}


double CAgentWorkerDlg::CalcRSI( DWORD dwPair, DWORD dwSpan, double dPrevUpAve, double dPrevDownAve )
{

	double dRet = 0;
	double dUp = 0;
	double dDown = 0;
	DWORD i = m_TickInfo[dwPair][TKI_70TICK].size()-1;
	if( dwSpan > i )	return 0;

	if( dPrevUpAve == 0 && dPrevDownAve == 0 ){
		for( DWORD j = i; (int)j > (int)i - (int)dwSpan; j-- ){
			if( m_TickInfo[dwPair][TKI_70TICK][j-1].Cl < m_TickInfo[dwPair][TKI_70TICK][j].Cl ){
				dUp += m_TickInfo[dwPair][TKI_70TICK][j].Cl - m_TickInfo[dwPair][TKI_70TICK][j-1].Cl;
			}else{
				dDown += m_TickInfo[dwPair][TKI_70TICK][j-1].Cl - m_TickInfo[dwPair][TKI_70TICK][j].Cl;
			}
		}
	}else{
		dUp = dPrevUpAve*(dwSpan-1);
		dDown = dPrevDownAve*(dwSpan-1);

		if( m_TickInfo[dwPair][TKI_70TICK][i-1].Cl < m_TickInfo[dwPair][TKI_70TICK][i].Cl ){
			dUp += m_TickInfo[dwPair][TKI_70TICK][i].Cl - m_TickInfo[dwPair][TKI_70TICK][i-1].Cl;
		}else{
			dDown += m_TickInfo[dwPair][TKI_70TICK][i-1].Cl - m_TickInfo[dwPair][TKI_70TICK][i].Cl;
		}
	}

	dUp /= dwSpan;
	dDown /= dwSpan;

	dRet = dUp*100/(dUp+dDown);
	
	return dRet;
}


bool CAgentWorkerDlg::CalcKLKH( DWORD dwPair, DWORD dwStart, DWORD dwKSpan, DWORD* pdwHi, DWORD* pdwLo )
{
	bool bRet = false;
	DWORD i;
	DWORD dwEnd;
	DWORD dwRet = 0;
	DWORD dwHi = 0;
	DWORD dwLo = 777777;

	if (pdwHi == NULL || pdwLo == NULL)	return false;

	dwEnd = dwStart - dwKSpan;

	for (i = dwStart; i > dwEnd; i--) {
		//DebugPrint( "%.3f=%.3f,", m_TickInfo[i].Hi, m_TickInfo[i].Lo);
		if (m_TickInfo[dwPair][TKI_70TICK][i].Hi > dwHi)	dwHi = m_TickInfo[dwPair][TKI_70TICK][i].Hi;
		if (m_TickInfo[dwPair][TKI_70TICK][i].Lo < dwLo)	dwLo = m_TickInfo[dwPair][TKI_70TICK][i].Lo;
	}

	//DebugPrint("[Hi]%.3f=[Lo]%.3f,", dHi, dLo);
	*pdwHi = dwHi;
	*pdwLo = dwLo;

	if( dwHi == 0 || dwLo == 0 ){
		int a;
		a = 3;
	}
	return true;
}

DWORD CAgentWorkerDlg::CalcD( DWORD dwPair, DWORD dwStart, DWORD dwKSpan, DWORD dwDSpan, DWORD dwSDSpan )
{
	DWORD i;
	DWORD dwEnd;
	DWORD dwDL = 0;
	DWORD dwDH = 0;
	DWORD dwHi;
	DWORD dwLo;

	dwEnd = dwStart + 1 - dwDSpan;
	for (i = dwStart; i >= dwEnd; i--) {
		CalcKLKH( dwPair, i, dwDSpan, &dwHi, &dwLo);

		dwDH += m_TickInfo[dwPair][TKI_70TICK][i].Cl - dwLo;
		dwDL += dwHi - dwLo;
	}

	if(dwDL>0){
		return (dwDH * 100) / dwDL;
	}else{
		return 0;
	}
}

BOOL CAgentWorkerDlg::CalcStochasticOscillator( DWORD dwPair, DWORD i, DWORD dwKSpan, DWORD dwDSpan, DWORD dwSDSpan, DWORD* pdwSO_K, DWORD* pdwSO_D, DWORD* pdwSO_Slow )
{
	//if( i >= (m_TickInfo[dwPair][TKI_70TICK].size()-1 ) ){
	//	return FALSE;
	//}

	BOOL bRet = FALSE;

	DWORD dwHi = 0;
	DWORD dwLo = 0;
	DWORD dwSO_K;
	DWORD dwSO_D;
	DWORD dwSO_SlowD;
	DWORD dwEnd;
	DWORD dwSlowDSum = 0;
	
	if ( i < dwKSpan + (dwDSpan-1))	return FALSE;

	//	%Kの計算
	CalcKLKH( dwPair, i, dwKSpan, &dwHi, &dwLo);
	dwSO_K = ( m_TickInfo[dwPair][TKI_70TICK][i].Cl - dwLo)*100/(dwHi-dwLo);
	//	%Dの計算
	dwSO_D = CalcD( dwPair, i, dwKSpan, dwDSpan, dwSDSpan );

	//	Slow%Dの計算
	dwEnd = i + 1 - dwSDSpan;
	for ( DWORD j = i; j >= dwEnd; j--) {
		dwSlowDSum += CalcD( dwPair, j, dwKSpan, dwDSpan, dwSDSpan);
	}
	dwSO_SlowD = dwSlowDSum / dwSDSpan;

	if( pdwSO_K != NULL )		*pdwSO_K = dwSO_K;
	if( pdwSO_D != NULL )		*pdwSO_D = dwSO_D;
	if( pdwSO_Slow != NULL )	*pdwSO_Slow = dwSO_SlowD;

	return TRUE;
}



BOOL CAgentWorkerDlg::GetIchimokuData( DWORD dwPair, DWORD i, ICHIMOKU_INFO* pRetInfo )
{
	BOOL bRet = FALSE;

	//	転換値：(当日含む過去9日間の高値+当日含む過去9日間の安値)÷2
	if( i >= 9 - 1 ){
		DWORD dwHi = 0;
		DWORD dwLo = 7777777;

		for( DWORD j = i; (int)j >= (int)(i - ( 9 - 1 )); j-- ){
			if( dwHi < m_TickInfo[dwPair][TKI_70TICK][j].Hi )	dwHi = m_TickInfo[dwPair][TKI_70TICK][j].Hi;
			if( dwLo > m_TickInfo[dwPair][TKI_70TICK][j].Lo )	dwLo = m_TickInfo[dwPair][TKI_70TICK][j].Lo;
		}

		pRetInfo->dwConvVal = ( dwHi + dwLo )/2;

		bRet = TRUE;
	}
	
	//	基準値：(当日含む過去26日間の高値+当日含む過去26日間の安値)÷2
	if( i >= 26 - 1 ){
		DWORD dwHi = 0;
		DWORD dwLo = 7777777;

		for( DWORD j = i; (int)j >= (int)(i - ( 26 - 1 )); j-- ){
			if( dwHi < m_TickInfo[dwPair][TKI_70TICK][j].Hi )	dwHi = m_TickInfo[dwPair][TKI_70TICK][j].Hi;
			if( dwLo > m_TickInfo[dwPair][TKI_70TICK][j].Lo )	dwLo = m_TickInfo[dwPair][TKI_70TICK][j].Lo;
		}

		pRetInfo->dwBaseVal = ( dwHi + dwLo )/2;

		bRet = TRUE;
	}

	//	遅行スパン：当日の終値を26日後ろにずらしたもの
	if( i + 26 < m_TickInfo[dwPair][TKI_70TICK].size() ){
		pRetInfo->dwLaggingSpan = m_TickInfo[dwPair][TKI_70TICK][i+26].Cl;

		bRet = TRUE;
	}

	//	先行スパン１：(転換値の値+基準値の値)÷2・・・26日先にずらして書く
	if( i >= 26 - 1 + 26 ){
		DWORD dwHi = 0;
		DWORD dwLo = 7777777;
		DWORD dwConvVal;
		DWORD dwBaseVal;
		DWORD dwStart = i - 26;

		for( DWORD j = dwStart; (int)j >= (int)(dwStart - ( 9 - 1 )); j-- ){
			if( dwHi < m_TickInfo[dwPair][TKI_70TICK][j].Hi )	dwHi = m_TickInfo[dwPair][TKI_70TICK][j].Hi;
			if( dwLo > m_TickInfo[dwPair][TKI_70TICK][j].Lo )	dwLo = m_TickInfo[dwPair][TKI_70TICK][j].Lo;
		}

		dwConvVal = ( dwHi + dwLo )/2;

		dwHi = 0;
		dwLo = 7777777;

		for( DWORD j = dwStart; (int)j >= (int)(dwStart - ( 26 - 1 )); j-- ){
			if( dwHi < m_TickInfo[dwPair][TKI_70TICK][j].Hi )	dwHi = m_TickInfo[dwPair][TKI_70TICK][j].Hi;
			if( dwLo > m_TickInfo[dwPair][TKI_70TICK][j].Lo )	dwLo = m_TickInfo[dwPair][TKI_70TICK][j].Lo;
		}

		dwBaseVal = ( dwHi + dwLo )/2;

		pRetInfo->dwLeadingSpan1 = (dwConvVal+dwBaseVal)/2;

		bRet = TRUE;
	}


	//	先行スパン２：(過去52日間の高値+過去52日間の安値)÷2・・・26日先にずらして書く
	if( i >= 52 - 1 + 26 ){
		DWORD dwHi = 0;
		DWORD dwLo = 7777777;
		DWORD dwStart = i - 26;

		for( DWORD j = dwStart; (int)j >= (int)(dwStart - ( 52 - 1 )); j-- ){
			if( dwHi < m_TickInfo[dwPair][TKI_70TICK][j].Hi )	dwHi = m_TickInfo[dwPair][TKI_70TICK][j].Hi;
			if( dwLo > m_TickInfo[dwPair][TKI_70TICK][j].Lo )	dwLo = m_TickInfo[dwPair][TKI_70TICK][j].Lo;
		}

		pRetInfo->dwLeadingSpan2 = ( dwHi + dwLo )/2;

		bRet = TRUE;
	}
	return bRet;
}

BOOL CAgentWorkerDlg::IsCrossIchimokuClound(DWORD dwPair, int idx, DWORD dwTradeType )
{
	BOOL bIsCross = FALSE;
	int i;
	DWORD Hi,Lo;

	if( dwTradeType == 1 )
	{
		bIsCross = TRUE;
		for( i = idx-5; i > 0 && i > idx-5-3 ; i-- ){
			if( m_TickInfo[dwPair][TKI_70TICK][i].ichi_info.dwLeadingSpan1 < m_TickInfo[dwPair][TKI_70TICK][i].ichi_info.dwLeadingSpan2 ){
				Lo = m_TickInfo[dwPair][TKI_70TICK][i].ichi_info.dwLeadingSpan1;
			}else{
				Lo = m_TickInfo[dwPair][TKI_70TICK][i].ichi_info.dwLeadingSpan2;
			}

			if( Lo < m_TickInfo[dwPair][TKI_70TICK][i].Op 
			 || Lo < m_TickInfo[dwPair][TKI_70TICK][i].Cl
			){
				bIsCross = FALSE;
				break;
			}
		}
	}else if( dwTradeType == 2 )
	{
		bIsCross = TRUE;
		for( i = idx-5; i > 0 && i > idx-5-3 ; i-- ){
			if( m_TickInfo[dwPair][TKI_70TICK][i].ichi_info.dwLeadingSpan1 < m_TickInfo[dwPair][TKI_70TICK][i].ichi_info.dwLeadingSpan2 ){
				Hi = m_TickInfo[dwPair][TKI_70TICK][i].ichi_info.dwLeadingSpan2;
			}else{
				Hi = m_TickInfo[dwPair][TKI_70TICK][i].ichi_info.dwLeadingSpan1;
			}

			if( Hi > m_TickInfo[dwPair][TKI_70TICK][i].Op 
			 || Hi > m_TickInfo[dwPair][TKI_70TICK][i].Cl
			){
				bIsCross = FALSE;
				break;
			}
		}
	}

	return bIsCross;
}
DWORD CAgentWorkerDlg::RangeBarrierCheck( DWORD dwPair, DWORD dwHiVal, DWORD dwLoVal ){
	DWORD dwRet = 0;
	DWORD dwTType = TKI_70TICK;
	DWORD dwEnd = m_TickInfo[dwPair][dwTType].size()-1;

	DWORD dwDoujiCnt = 0;
	DWORD i;
	DWORD dwBorder=0;

	if( max( m_TickInfo[dwPair][dwTType][dwEnd-1].Op, m_TickInfo[dwPair][dwTType][dwEnd-1].Cl ) > (dwLoVal+(dwHiVal-dwLoVal)/2) ){
		for( i = dwEnd-1; ; i-- ){
			DWORD dwChkVal = max( m_TickInfo[dwPair][dwTType][i].Op, m_TickInfo[dwPair][dwTType][i].Cl );
			if( dwChkVal > dwHiVal ){
				if( dwBorder == 0 ){
					dwBorder = min( m_TickInfo[dwPair][dwTType][i].Op, m_TickInfo[dwPair][dwTType][i].Cl );
				}else{
					dwBorder = min( dwBorder, min( m_TickInfo[dwPair][dwTType][i].Op, m_TickInfo[dwPair][dwTType][i].Cl ));
				}
				dwDoujiCnt++;
			}else{
				break;
			}
		}

		if( dwDoujiCnt > 2 ){
			if( m_TickInfo[dwPair][dwTType][dwEnd].Cl < dwBorder ){
				dwRet = 2;
			}
		}
	}else{
		for( i = dwEnd-1; ; i-- ){
			DWORD dwChkVal = min( m_TickInfo[dwPair][dwTType][i].Op, m_TickInfo[dwPair][dwTType][i].Cl );
			if( dwChkVal < dwLoVal ){
				if( dwBorder == 0 ){
					dwBorder = max( m_TickInfo[dwPair][dwTType][i].Op, m_TickInfo[dwPair][dwTType][i].Cl );
				}else{
					dwBorder = max( dwBorder, max( m_TickInfo[dwPair][dwTType][i].Op, m_TickInfo[dwPair][dwTType][i].Cl ));
				}
				dwDoujiCnt++;
			}else{
				break;
			}
		}

		if( dwDoujiCnt > 2 ){
			if( m_TickInfo[dwPair][dwTType][dwEnd].Cl > dwBorder ){
				dwRet = 1;
			}
		}
	}
	return dwRet;
}


BOOL CAgentWorkerDlg::CloseCheck_RangeBreak( TRADE_INFO& ti)
{
	BOOL bClose = FALSE;
	DWORD dwCurIdx = m_TickInfo[ti.dwPair][TKI_70TICK].size() - 1;

	if(dwCurIdx > 0) {
		DWORD dwCurRate = m_TickInfo[ti.dwPair][TKI_70TICK][m_TickInfo[ti.dwPair][TKI_70TICK].size() - 1].Cl;
		DWORD dwOpenPrice = Rate2EVal(ti.dOpenPrice, ti.dwPair);
		DWORD dwSpread = Rate2EVal(GetSpread(ti.dwPair), ti.dwPair);
		DWORD dwClosePrice;
		DWORD dwPeakPrice;

		//double dRSI = CalcRSI( ti.dwPair, m_Setting[ti.dwPair].rsi.dwSpan , 0, 0 );


		if(ti.dwOrderType == 1) {
			dwClosePrice = dwCurRate - dwSpread / 2;
			dwPeakPrice = ti.dwPeakRate - dwSpread / 2;

			if(ti.dwPeakRate < dwCurRate) {
				ti.dwPeakRate = dwCurRate;
			}

			if(dwClosePrice <= dwOpenPrice - 20 ) {
				ti.dwCloseSign = CLOSESIGN_LOSSCUT;
				bClose = TRUE;
			}else if( dwPeakPrice > dwOpenPrice + 20 && dwClosePrice <= dwOpenPrice ){
				ti.dwCloseSign = CLOSESIGN_ZEROBACK;
				bClose = TRUE;
			}else if(dwClosePrice >= dwOpenPrice + 100 ) {
				ti.dwCloseSign = CLOSESIGN_PROFIT;
				bClose = TRUE;
			}

		}
		else if(ti.dwOrderType == 2) {
			dwClosePrice = dwCurRate + dwSpread / 2;
			dwPeakPrice = ti.dwPeakRate + dwSpread / 2;

			if(ti.dwPeakRate > dwCurRate) {
				ti.dwPeakRate = dwCurRate;
			}

			if(dwClosePrice >= dwOpenPrice + 20 ) {
				ti.dwCloseSign = CLOSESIGN_LOSSCUT;
				bClose = TRUE;
			}else if( dwPeakPrice < dwOpenPrice - 20 && dwClosePrice >= dwOpenPrice ){
				ti.dwCloseSign = CLOSESIGN_ZEROBACK;
				bClose = TRUE;
			}else if(dwClosePrice <= dwOpenPrice - 100 ) {
				ti.dwCloseSign = CLOSESIGN_PROFIT;
				bClose = TRUE;
			}

		}
	}
	return bClose;
}

DWORD CAgentWorkerDlg::TradeCheck_RangeBreak(DWORD dwPair )
{
	DWORD dwRet = 0;
	DWORD dwTType = TKI_70TICK;
	DWORD dwEnd = m_TickInfo[dwPair][dwTType].size()-1;

	static BOOL bStockRC[PAIR_COUNT];
	static RATE_CHECK_INFO stock_rci[PAIR_COUNT];
	BOOL bWrite = FALSE;

	if( bStockRC[dwPair] == FALSE ){
		if( dwEnd + 1  >= m_Setting[dwPair].rb.dwSpan ){
			DWORD dwMax = 0;
			DWORD dwMin = 77777777;
			for( DWORD i = dwEnd-m_Setting[dwPair].rb.dwSpan; i <= dwEnd; i++ ){
				if( m_TickInfo[dwPair][dwTType][i].Hi > dwMax ){
					dwMax = m_TickInfo[dwPair][dwTType][i].Hi;
				}
				if( m_TickInfo[dwPair][dwTType][i].Lo < dwMin ){
					dwMin = m_TickInfo[dwPair][dwTType][i].Lo;
				}
			}

			if( dwMax - dwMin < m_Setting[dwPair].rb.dwWidth ){
				memset( &stock_rci[dwPair], 0x00, sizeof(RATE_CHECK_INFO));
				stock_rci[dwPair].dwChkType = RATECHK_RANGECHK;
				//dwRet |= RATECHK_RANGECHK;
				stock_rci[dwPair].dwUpOrDn = 0;
				stock_rci[dwPair].dwMaxVal = dwMax;
				stock_rci[dwPair].dwMinVal = dwMin;
				//memcpy( &stock_rci[dwPair].sDt, &m_TickInfo[dwPair][dwTType][dwEnd-m_RateChkSet.rc.dwSpan].sDt, sizeof(SYSTEMTIME) );
				//memcpy( &stock_rci[dwPair].eDt, pRegTime, sizeof(SYSTEMTIME) );
				bStockRC[dwPair] = TRUE;

				//RateCheckWrite( dwPair, &rci );

				//m_LastChkRC = CurTime;
			}
		}
	}

	if( bStockRC[dwPair] != FALSE ){
		if( m_TickInfo[dwPair][dwTType][dwEnd].Cl > stock_rci[dwPair].dwMaxVal + m_Setting[dwPair].rb.dwVagueVal ){
			bStockRC[dwPair] = FALSE;
		}else if( m_TickInfo[dwPair][dwTType][dwEnd].Cl < stock_rci[dwPair].dwMinVal - m_Setting[dwPair].rb.dwVagueVal ){
			bStockRC[dwPair] = FALSE;
		}else{
			dwRet = RangeBarrierCheck( dwPair, stock_rci[dwPair].dwMaxVal, stock_rci[dwPair].dwMinVal );
		}

		if( dwRet != 0 ){
			bStockRC[dwPair] = FALSE;
		}
	}	

	return dwRet;
}


DWORD CAgentWorkerDlg::TradeCheck_ICHIMOKUBreak(DWORD dwPair )
{
	DWORD dwRet = 0;
	DWORD idx = m_TickInfo[dwPair][TKI_70TICK].size()-1;

	ICHIMOKU_INFO ichi_info;

	if( m_TickInfo[dwPair][TKI_70TICK][idx].bCl == FALSE )	return 0;

	memset( &ichi_info, 0x00, sizeof(ICHIMOKU_INFO) );
	if( GetIchimokuData( dwPair, idx, &ichi_info )	== FALSE )	return 0;

	if(ichi_info.dwLeadingSpan1 != 0 && ichi_info.dwLeadingSpan2 != 0
	 && ichi_info.dwBaseVal != 0 && ichi_info.dwConvVal != 0
	) {
		DWORD dwIchimokuArray[4];

		dwIchimokuArray[0] = ichi_info.dwBaseVal;
		dwIchimokuArray[1] = ichi_info.dwConvVal;
		dwIchimokuArray[2] = ichi_info.dwLeadingSpan1;
		dwIchimokuArray[3] = ichi_info.dwLeadingSpan2;
		//dwIchimokuArray[4] = ichi_info.dwLaggingSpan;

		DWORD dwTemp;
		for( DWORD l = 0; l < 3; l++ ){
			for( DWORD m = l+1; m < 4; m++ ){
				if( dwIchimokuArray[l] > dwIchimokuArray[m] ){
					dwTemp = dwIchimokuArray[l];
					dwIchimokuArray[l] = dwIchimokuArray[m];
					dwIchimokuArray[m] = dwTemp;
				}
			}
		}
		
		if( dwIchimokuArray[3] - dwIchimokuArray[0] < 50 )	return 0;

		//if(ABS_CMP(ichi_info.dwLeadingSpan1, ichi_info.dwLeadingSpan2, 10) < 0) 
		{
			//	実体が雲を挟む
			DWORD dwHi;
			DWORD dwLo;
			//DWORD dwPrevHi;
			//DWORD dwPrevLo;
			//ICHIMOKU_INFO ichi_prev;

			//memset(&ichi_prev, 0x00, sizeof(ICHIMOKU_INFO));

			//if(GetIchimokuData( dwPair, idx - 1, &ichi_prev) != FALSE) 
			{
				if(ichi_info.dwLeadingSpan1 < ichi_info.dwLeadingSpan2) {
					dwHi = ichi_info.dwLeadingSpan2;
					dwLo = ichi_info.dwLeadingSpan1;
				}
				else {
					dwHi = ichi_info.dwLeadingSpan1;
					dwLo = ichi_info.dwLeadingSpan2;
				}
				//if(ichi_prev.dwLeadingSpan1 < ichi_prev.dwLeadingSpan2) {
				//	dwPrevHi = ichi_prev.dwLeadingSpan2;
				//	dwPrevLo = ichi_prev.dwLeadingSpan1;
				//}
				//else {
				//	dwPrevHi = ichi_prev.dwLeadingSpan2;
				//	dwPrevLo = ichi_prev.dwLeadingSpan1;
				//}

				if( m_TickInfo[dwPair][TKI_70TICK][idx].Op < m_TickInfo[dwPair][TKI_70TICK][idx].Cl) {
					if( dwHi < m_TickInfo[dwPair][TKI_70TICK][idx].Cl
					 && dwHi > m_TickInfo[dwPair][TKI_70TICK][idx].Op
					) {
						//if( dwLo < ichi_info.dwBaseVal )
						if( IsCrossIchimokuClound( dwPair, idx, 1 ) != FALSE ){
							if( ichi_info.dwBaseVal < m_TickInfo[dwPair][TKI_70TICK][idx].Op ){
								dwRet = 1;
							}
						}
					}
				}
				else{
					if( dwLo > m_TickInfo[dwPair][TKI_70TICK][idx].Cl
					 && dwLo < m_TickInfo[dwPair][TKI_70TICK][idx].Op
					) {
						//if( dwPrevLo < m_TickInfo[dwPair][TKI_70TICK][idx-1].Cl
						// && dwPrevLo < m_TickInfo[dwPair][TKI_70TICK][idx-1].Op
						//) 
						//if( dwHi > ichi_info.dwBaseVal )
						if( IsCrossIchimokuClound( dwPair, idx, 2 ) != FALSE ){
							if( ichi_info.dwBaseVal > m_TickInfo[dwPair][TKI_70TICK][idx].Op ){
								dwRet = 2;
							}
						}
					}
				}
			}
		}
	}

	return dwRet;
}

struct{
	double dPeakRSI;
	DWORD dwTrandType;	//	1:上　２：下
//	BOOL bRSICheck;
}g_RSIBreakChk[PAIR_COUNT] = { 0, 0 };

DWORD CAgentWorkerDlg::TradeCheck_RSIBreak(DWORD dwPair )
{
	DWORD dwRet = 0;
	DWORD idx = m_TickInfo[dwPair][TKI_70TICK].size()-1;
	DWORD dwSO_K = 0;
	DWORD dwSO_D = 0;
	DWORD dwSO_SlowD = 0;

	
	double dRSI = CalcRSI( dwPair, m_Setting[dwPair].rsi.dwSpan , 0, 0 );

	if( dRSI == 0 ) return 0;

	if( g_RSIBreakChk[dwPair].dPeakRSI != 0 ){
		if( g_RSIBreakChk[dwPair].dwTrandType == 1 ){
			if( g_RSIBreakChk[dwPair].dPeakRSI <= dRSI ){
				g_RSIBreakChk[dwPair].dPeakRSI = dRSI;
			}else{
/*				if( dwSO_D < 80 ){
					g_RSIBreakChk[dwPair].dPeakRSI = 0;
					g_RSIBreakChk[dwPair].dwTrandType = 0;
				}else*/{
					//if( g_RSIBreakChk[dwPair].bRSICheck == FALSE ){
					//	if( CalcRSI( dwPair, 21, 0, 0 ) >= 80 ){
					//		g_RSIBreakChk[dwPair].bRSICheck = TRUE;
					//	}
					//}
					//if( g_RSIBreakChk[dwPair].bRSICheck != FALSE )
					{
						//if( m_TickInfo[dwPair][TKI_70TICK][idx].bCl == FALSE )	return 0;

						if( m_TickInfo[dwPair][TKI_70TICK][idx].Cl < m_TickInfo[dwPair][TKI_70TICK][idx].Op ){
							if( g_RSIBreakChk[dwPair].dPeakRSI >= dRSI + 2 /*&& dwSO_SlowD >= dwSO_D*/ ){
								dwRet = 2;
								g_RSIBreakChk[dwPair].dPeakRSI = 0;
								g_RSIBreakChk[dwPair].dwTrandType = 0;
							}
						}
					}
				}
			}
		}else if( g_RSIBreakChk[dwPair].dwTrandType == 2 ){
			if( g_RSIBreakChk[dwPair].dPeakRSI >= dRSI ){
				g_RSIBreakChk[dwPair].dPeakRSI = dRSI;
			}else{
/*				if( dwSO_D > 20 ){
					g_RSIBreakChk[dwPair].dwPeakSO = 0;
					g_RSIBreakChk[dwPair].dwTrandType = 0;
					g_RSIBreakChk[dwPair].bRSICheck = FALSE;
				}else*/{
					//if( g_RSIBreakChk[dwPair].bRSICheck == FALSE ){
					//	if( CalcRSI( dwPair, 21, 0, 0 ) <= 20 ){
					//		g_RSIBreakChk[dwPair].bRSICheck = TRUE;
					//	}
					//}
					//if( g_RSIBreakChk[dwPair].bRSICheck != FALSE )
					{
						//if( m_TickInfo[dwPair][TKI_70TICK][idx].bCl == FALSE )	return 0;

						if( m_TickInfo[dwPair][TKI_70TICK][idx].Cl > m_TickInfo[dwPair][TKI_70TICK][idx].Op ){				
							if( g_RSIBreakChk[dwPair].dPeakRSI <= dRSI - 2 /*&& dwSO_SlowD <= dwSO_D*/ ){
								dwRet = 1;	
		
								g_RSIBreakChk[dwPair].dPeakRSI = 0;
								g_RSIBreakChk[dwPair].dwTrandType = 0;
							}
						}
					}
				}
			}
		}
	}else{
		//if( m_TickInfo[dwPair][TKI_70TICK][idx].bCl == FALSE )	return 0;


		if( 80 <= dRSI ){
			g_RSIBreakChk[dwPair].dPeakRSI = dRSI;
			g_RSIBreakChk[dwPair].dwTrandType = 1;
		}else if( dRSI <= 20 ){
			g_RSIBreakChk[dwPair].dPeakRSI = dRSI;
			g_RSIBreakChk[dwPair].dwTrandType = 2;
		}
	}

	return dwRet;
}

struct{
	DWORD dwPeakSO;
	DWORD dwTrandType;	//	1:上　２：下
	BOOL bRSICheck;
}g_SOBreakChk[PAIR_COUNT] = { 0, 0, 0 };

DWORD CAgentWorkerDlg::TradeCheck_SOBreak(DWORD dwPair )
{
	DWORD dwRet = 0;
	DWORD idx = m_TickInfo[dwPair][TKI_70TICK].size()-1;
	DWORD dwSO_K = 0;
	DWORD dwSO_D = 0;
	DWORD dwSO_SlowD = 0;

	if( m_TickInfo[dwPair][TKI_70TICK][idx].bCl == FALSE )	return 0;

	if( CalcStochasticOscillator( dwPair, idx, m_Setting[dwPair].so.dwKSpan, m_Setting[dwPair].so.dwDSpan, m_Setting[dwPair].so.dwSlowDSpan, &dwSO_K, &dwSO_D, &dwSO_SlowD ) == FALSE )	return 0;

	if( dwSO_D == 0 )	return 0;

	if( g_SOBreakChk[dwPair].dwPeakSO != 0 ){
		if( g_SOBreakChk[dwPair].dwTrandType == 1 ){
			if( g_SOBreakChk[dwPair].dwPeakSO <= dwSO_D ){
				g_SOBreakChk[dwPair].dwPeakSO = dwSO_D;
			}else{
				if( dwSO_D < 80 ){
					g_SOBreakChk[dwPair].dwPeakSO = 0;
					g_SOBreakChk[dwPair].dwTrandType = 0;
					g_SOBreakChk[dwPair].bRSICheck = FALSE;
				}else{
					//if( g_SOBreakChk[dwPair].bRSICheck == FALSE ){
					//	if( CalcRSI( dwPair, 21, 0, 0 ) >= 80 ){
					//		g_SOBreakChk[dwPair].bRSICheck = TRUE;
					//	}
					//}
					//if( g_SOBreakChk[dwPair].bRSICheck != FALSE )
					{
						if( m_TickInfo[dwPair][TKI_70TICK][idx].Cl < m_TickInfo[dwPair][TKI_70TICK][idx].Op ){
							if( g_SOBreakChk[dwPair].dwPeakSO >= dwSO_D + 2 /*&& dwSO_SlowD >= dwSO_D*/ ){
								dwRet = 2;
								g_SOBreakChk[dwPair].dwPeakSO = 0;
								g_SOBreakChk[dwPair].dwTrandType = 0;
								g_SOBreakChk[dwPair].bRSICheck = FALSE;
							}
						}
					}
				}
			}
		}else if( g_SOBreakChk[dwPair].dwTrandType == 2 ){
			if( g_SOBreakChk[dwPair].dwPeakSO >= dwSO_D ){
				g_SOBreakChk[dwPair].dwPeakSO = dwSO_D;
			}else{
				if( dwSO_D > 20 ){
					g_SOBreakChk[dwPair].dwPeakSO = 0;
					g_SOBreakChk[dwPair].dwTrandType = 0;
					g_SOBreakChk[dwPair].bRSICheck = FALSE;
				}else{
					//if( g_SOBreakChk[dwPair].bRSICheck == FALSE ){
					//	if( CalcRSI( dwPair, 21, 0, 0 ) <= 20 ){
					//		g_SOBreakChk[dwPair].bRSICheck = TRUE;
					//	}
					//}
					//if( g_SOBreakChk[dwPair].bRSICheck != FALSE )
					{
						if( m_TickInfo[dwPair][TKI_70TICK][idx].Cl > m_TickInfo[dwPair][TKI_70TICK][idx].Op ){				
							if( g_SOBreakChk[dwPair].dwPeakSO <= dwSO_D - 2 /*&& dwSO_SlowD <= dwSO_D*/ ){
								dwRet = 1;	
		
								g_SOBreakChk[dwPair].dwPeakSO = 0;
								g_SOBreakChk[dwPair].dwTrandType = 0;	
								g_SOBreakChk[dwPair].bRSICheck = FALSE;
							}
						}
					}
				}
			}
		}
	}else{
		if( 80 < dwSO_D ){
			g_SOBreakChk[dwPair].dwPeakSO = dwSO_D;
			g_SOBreakChk[dwPair].dwTrandType = 1;
		}else if( dwSO_D < 20 ){
			g_SOBreakChk[dwPair].dwPeakSO = dwSO_D;
			g_SOBreakChk[dwPair].dwTrandType = 2;
		}
	}

	return dwRet;
}

struct{
	DWORD dwPeakRate;
	DWORD dwTrandType;	//	1:上　２：下
	double dRSI;
}g_3SigmaBreakChk[PAIR_COUNT] = { 0, 0, 0 };

DWORD CAgentWorkerDlg::TradeCheck_BB3Sigma(DWORD dwPair )
{
	DWORD dwRet = 0;
	DWORD idx = m_TickInfo[dwPair][TKI_70TICK].size()-1;
	DWORD dwHiSigma3 = CalcBollingerBandsSigma( dwPair,  idx, m_Setting[dwPair].brsi.dwRSISpan, 2);
	DWORD dwLoSigma3 = CalcBollingerBandsSigma( dwPair, idx, m_Setting[dwPair].brsi.dwRSISpan, -2 );
	double dRSI = CalcRSI( dwPair, m_Setting[dwPair].brsi.dwRSISpan, 0, 0 );
	DWORD dwSO_K = 0;
	DWORD dwNextSO_K = 0;
	DWORD dwSO_D = 0;
	DWORD dwNextSO_D = 0;
	DWORD dwSO_SlowD = 0;
	DWORD dwNextSO_SlowD = 0;

	if( CalcStochasticOscillator( dwPair, idx, m_Setting[dwPair].so.dwKSpan, m_Setting[dwPair].so.dwDSpan, m_Setting[dwPair].so.dwSlowDSpan, &dwSO_K, &dwSO_D, &dwSO_SlowD ) == FALSE )	return 0;

	if( dwSO_D < 20 ){
		dwRet = 0;
	}else if( dwSO_D > 80 ){
		dwRet = 0;
	}

	//if( CalcStochasticOscillator( i+1, 9, dwDSpan, dwSDSpan, &dwNextSO_K, &dwNextSO_D, &dwNextSO_SlowD ) == FALSE )	return;

	if( dwHiSigma3==0 || dwLoSigma3==0 )	return 0;

	if( g_3SigmaBreakChk[dwPair].dwPeakRate != 0 ){
		if( g_3SigmaBreakChk[dwPair].dwTrandType == 1 ){
#if 1
			if( g_3SigmaBreakChk[dwPair].dwPeakRate - 10 > m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
				dwRet = 2;
				g_3SigmaBreakChk[dwPair].dwTrandType = 0;
				g_3SigmaBreakChk[dwPair].dwPeakRate = 0;
			}else if( g_3SigmaBreakChk[dwPair].dwPeakRate < m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
				g_3SigmaBreakChk[dwPair].dwPeakRate = m_TickInfo[dwPair][TKI_70TICK][idx].Cl;
			}
#else
			if( dRSI < 100 - m_Setting[dwPair].brsi.dwRSI && dwHiSigma3 > m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
				dwRet = 2;
				g_3SigmaBreakChk[dwPair].dwTrandType = 0;
				g_3SigmaBreakChk[dwPair].dwPeakRate = 0;
			}else if( g_3SigmaBreakChk[dwPair].dwPeakRate < m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
				g_3SigmaBreakChk[dwPair].dwPeakRate = m_TickInfo[dwPair][TKI_70TICK][idx].Cl;
			}
#endif
		}else if( g_3SigmaBreakChk[dwPair].dwTrandType == 2 ){
#if 1
			if( g_3SigmaBreakChk[dwPair].dwPeakRate + 10 < m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
				dwRet = 1;
				g_3SigmaBreakChk[dwPair].dwTrandType = 0;
				g_3SigmaBreakChk[dwPair].dwPeakRate = 0;
			}else if( g_3SigmaBreakChk[dwPair].dwPeakRate > m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
				g_3SigmaBreakChk[dwPair].dwPeakRate = m_TickInfo[dwPair][TKI_70TICK][idx].Cl;
			}
#else
			if( dRSI > m_Setting[dwPair].brsi.dwRSI && dwLoSigma3 < m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
				dwRet = 1;
				g_3SigmaBreakChk[dwPair].dwTrandType = 0;
				g_3SigmaBreakChk[dwPair].dwPeakRate = 0;
			}else if( g_3SigmaBreakChk[dwPair].dwPeakRate > m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
				g_3SigmaBreakChk[dwPair].dwPeakRate = m_TickInfo[dwPair][TKI_70TICK][idx].Cl;
			}
#endif
		}
	}else{
		if( dwHiSigma3 <= m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
			if( dRSI >= 100 - m_Setting[dwPair].brsi.dwRSI && dwSO_D > 80 ){
				if( g_3SigmaBreakChk[dwPair].dwTrandType == 1 && g_3SigmaBreakChk[dwPair].dwPeakRate - 5 > m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
					dwRet = 2;
					g_3SigmaBreakChk[dwPair].dwTrandType = 0;
					g_3SigmaBreakChk[dwPair].dwPeakRate = 0;
				}else{
					g_3SigmaBreakChk[dwPair].dwTrandType = 1;
					if( g_3SigmaBreakChk[dwPair].dwPeakRate == 0 || g_3SigmaBreakChk[dwPair].dwPeakRate < m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
						g_3SigmaBreakChk[dwPair].dwPeakRate = m_TickInfo[dwPair][TKI_70TICK][idx].Cl;
					}
				}
			}
		}else if( dwLoSigma3 >= m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
			if( dRSI <= m_Setting[dwPair].brsi.dwRSI && dwSO_D < 20 ){
				if( g_3SigmaBreakChk[dwPair].dwTrandType == 2 && g_3SigmaBreakChk[dwPair].dwPeakRate + 5 < m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
					dwRet = 1;
					g_3SigmaBreakChk[dwPair].dwTrandType = 0;
					g_3SigmaBreakChk[dwPair].dwPeakRate = 0;
				}else{
					g_3SigmaBreakChk[dwPair].dwTrandType = 2;
					if( g_3SigmaBreakChk[dwPair].dwPeakRate == 0 || g_3SigmaBreakChk[dwPair].dwPeakRate > m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
						g_3SigmaBreakChk[dwPair].dwPeakRate = m_TickInfo[dwPair][TKI_70TICK][idx].Cl;
					}
				}
			}
		}
	}

	return dwRet;
}

#define USE_BBBACK		(0)
#define USE_BANDWALK	(1)

//	ボリジャーバンドを使った判定
DWORD CAgentWorkerDlg::TradeCheck_BBBackBreak(DWORD dwPair, DWORD* pdwBreakSign )
{
	DWORD dwRet = 0;
	DWORD dwPeakEVal = 0;
	DWORD dwEndIdx = 0;
	static DWORD dwPrevChkTID = 0;
	DWORD idx = m_TickInfo[dwPair][TKI_70TICK].size()-1;
	DWORD dwTID = m_TickInfo[dwPair][TKI_70TICK][idx].dwTID;

	if( idx < 50 )	return 0;
#if USE_BBBACK
	if( m_TickInfo[dwPair][TKI_70TICK][idx].bCl != FALSE ){
		//	σ2が縮小傾向にあるか？を超えているか？
		DWORD dwHiSigma2 = CalcBollingerBandsSigma( dwPair, idx, 21, 2 );
		DWORD dwLoSigma2 = CalcBollingerBandsSigma( dwPair, idx, 21, -2 );
#define NEED_SIGMA_PIPS	(3)

		if( dwHiSigma2 - dwLoSigma2 > Pips2EVal(NEED_SIGMA_PIPS)*2 ){
			DWORD dwPrevHiSigma2 = CalcBollingerBandsSigma( dwPair, idx-1, 21, 2 );
			DWORD dwPrevLoSigma2 = CalcBollingerBandsSigma( dwPair, idx-1, 21, -2 );
			DWORD dwPrevPrevHiSigma2 = CalcBollingerBandsSigma( dwPair, idx-2, 21, 2 );
			DWORD dwPrevPrevLoSigma2 = CalcBollingerBandsSigma( dwPair, idx-2, 21, -2 );
			DWORD dwWalkCnt = 0;

			DWORD i = 0;

			//	σの縮まりはじめを検知
			if( IsConvergeSigma( dwPair, FALSE ) != FALSE ){
				if( dwPrevChkTID == dwTID )	return 0;
				if( m_TickInfo[dwPair][TKI_70TICK][idx-1].Cl >=  dwPrevHiSigma2
				 || m_TickInfo[dwPair][TKI_70TICK][idx-2].Cl >=  dwPrevPrevHiSigma2
				){
					dwPeakEVal = 0;
					dwHiSigma2 = dwPrevHiSigma2;
					//	遡っていき、スクイーズしている場所を探す
					for( i = idx -1; i >= 0; i-- ){
						dwPrevHiSigma2 = CalcBollingerBandsSigma( dwPair, i-1, 21, 2 );
						if( dwPrevHiSigma2 > dwHiSigma2 ){
							break;
						}

						if( m_TickInfo[dwPair][TKI_70TICK][i].Cl >=  dwHiSigma2 ){
							dwWalkCnt++;
						}
						if( dwEndIdx == 0 ){
							if( m_TickInfo[dwPair][TKI_70TICK][i].Op < m_TickInfo[dwPair][TKI_70TICK][i].Cl ){
								dwEndIdx = i;
							}
						}else{
							/*if( m_TickInfo[dwPair][TKI_70TICK][i].Op > m_TickInfo[dwPair][TKI_70TICK][i].Cl ){
								break;
							}*/
						}
						if( dwPeakEVal < m_TickInfo[dwPair][TKI_70TICK][i].Cl ){
							dwPeakEVal = m_TickInfo[dwPair][TKI_70TICK][i].Cl;
						}
						dwHiSigma2 = dwPrevHiSigma2;
					}
				}
			}else if( IsConvergeSigma( dwPair, TRUE ) != FALSE ){
				if( dwPrevChkTID == dwTID )	return 0;

				if( m_TickInfo[dwPair][TKI_70TICK][idx-1].Cl <=  dwPrevLoSigma2
				 || m_TickInfo[dwPair][TKI_70TICK][idx-2].Cl <=  dwPrevPrevLoSigma2
				){
					dwPeakEVal = 77777777;
					dwLoSigma2 = dwPrevLoSigma2;
					//	遡っていき、スクイーズしている場所を探す
					for( i = idx -1; i >= 0; i-- ){
						dwPrevLoSigma2 = CalcBollingerBandsSigma(dwPair,  i-1, 21, -2 );
						if( dwPrevLoSigma2 < dwLoSigma2 ){
							break;
						}

						if( m_TickInfo[dwPair][TKI_70TICK][i].Cl <=  dwLoSigma2 ){
							dwWalkCnt++;
						}

						if( dwEndIdx == 0 ){
							if( m_TickInfo[dwPair][TKI_70TICK][i].Cl < m_TickInfo[dwPair][TKI_70TICK][i].Op ){
								dwEndIdx = i;
							}
						}else{
							/*if( m_TickInfo[dwPair][TKI_70TICK][i].Cl > m_TickInfo[dwPair][TKI_70TICK][i].Op ){
								break;
							}*/
						}

						if( dwPeakEVal > m_TickInfo[dwPair][TKI_70TICK][i].Cl ){
							dwPeakEVal = m_TickInfo[dwPair][TKI_70TICK][i].Cl;
						}

						dwLoSigma2 = dwPrevLoSigma2;
					}
				}
			
			}

			if( dwWalkCnt >= 1 ){
				for( ; i<=dwEndIdx; i++ ){
					dwHiSigma2 = CalcBollingerBandsSigma( dwPair, i, 21, 2 );
					dwLoSigma2 = CalcBollingerBandsSigma( dwPair, i, 21, -2 );
					DWORD dwNextHiSigma2 = CalcBollingerBandsSigma( dwPair, i+1, 21, 2 );
					DWORD dwNextLoSigma2 = CalcBollingerBandsSigma( dwPair, i+1, 21, -2 );

					//	エクスパンション
					if( dwHiSigma2 < dwNextHiSigma2 
						&& dwLoSigma2 > dwNextLoSigma2 
					){
						break;
					}
				}
				//if( dwWalkRange > Pips2EVal(5) )
				{
					g_BBWalkTradeInfo.dwTrandStartIdx = i;
					g_BBWalkTradeInfo.dwTrandEndIdx  = dwEndIdx;
					g_BBWalkTradeInfo.dwTrandStartTID = m_TickInfo[dwPair][TKI_70TICK][i].dwTID;
					g_BBWalkTradeInfo.dwTrandEndTID  = m_TickInfo[dwPair][TKI_70TICK][dwEndIdx].dwTID;
					g_BBWalkTradeInfo.dwBBWalkCnt = dwWalkCnt;
					g_BBWalkTradeInfo.dwPeakVal = dwPeakEVal;

					if(  m_TickInfo[dwPair][TKI_70TICK][dwEndIdx].Cl > m_TickInfo[dwPair][TKI_70TICK][i].Op ){
						g_BBWalkTradeInfo.dwMaxGain = m_TickInfo[dwPair][TKI_70TICK][dwEndIdx].Cl - m_TickInfo[dwPair][TKI_70TICK][i].Op;
						dwRet = 2;
					}else{
						g_BBWalkTradeInfo.dwMaxGain = m_TickInfo[dwPair][TKI_70TICK][i].Op - m_TickInfo[dwPair][TKI_70TICK][dwEndIdx].Cl;
						dwRet = 1;
					}

					if( g_BBWalkTradeInfo.dwMaxGain > Pips2EVal(5) ){
						*pdwBreakSign = BREAKSIGN_BY_BBBREAK_BWBACK;
						dwPrevChkTID = dwTID;
					}else{
						dwRet = 0;
					}
				}
			}
		}
	}
#endif



	return dwRet;
}


DWORD g_dwExpandCnt = 0;

DWORD CAgentWorkerDlg::TradeCheck_BBBreak(DWORD dwPair, DWORD* pdwBreakSign )
{
	DWORD dwRet = 0;
	DWORD dwPeakEVal = 0;
	DWORD dwEndIdx = 0;
	static DWORD dwPrevChkTID = 0;
	DWORD idx = m_TickInfo[dwPair][TKI_70TICK].size()-1;
	DWORD dwTID = m_TickInfo[dwPair][TKI_70TICK][idx].dwTID;

	if( idx < 50 )	return 0;


#if USE_BANDWALK
	
	DWORD dwSMA = CalcBollingerBandsSigma(dwPair, idx, 21, 0);
	DWORD dwHiSigma2 = CalcBollingerBandsSigma(dwPair, idx, 21, 2);
	DWORD dwLoSigma2 = CalcBollingerBandsSigma(dwPair, idx, 21, -2);
	DWORD dwPrevHiSigma2 = CalcBollingerBandsSigma(dwPair, idx - 1, 21, 2);
	DWORD dwPrevLoSigma2 = CalcBollingerBandsSigma(dwPair, idx - 1, 21, -2);
	DWORD dwPrevPrevHiSigma2 = CalcBollingerBandsSigma(dwPair, idx - 2, 21, 2);
	DWORD dwPrevPrevLoSigma2 = CalcBollingerBandsSigma(dwPair, idx - 2, 21, -2);

	//	エクスパンション
	if( dwPrevHiSigma2 < dwHiSigma2
	 && dwPrevLoSigma2 > dwLoSigma2
//	 && m_dwExpansionTID[dwPair] == m_TickInfo[dwPair][TKI_70TICK][idx].dwTID
	 && m_dwExpansionTID[dwPair] < m_dwSqueezeTID[dwPair]
	) {
		DWORD dwSMA = CalcBollingerBandsSigma( dwPair, idx, 21, 0 );
		DWORD dwSigma3;

		if( m_TickInfo[dwPair][TKI_70TICK][idx].Op < m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
			if( m_TickInfo[dwPair][TKI_70TICK][idx-1].Op < m_TickInfo[dwPair][TKI_70TICK][idx-1].Cl 
			// && m_TickInfo[dwPair][TKI_70TICK][idx-2].Op < m_TickInfo[dwPair][TKI_70TICK][idx-2].Cl 
			// && m_TickInfo[dwPair][TKI_70TICK][idx-3].Op < m_TickInfo[dwPair][TKI_70TICK][idx-3].Cl 
			){
				if( m_TickInfo[dwPair][TKI_70TICK][idx].Cl > dwSMA ){
					dwSigma3 = CalcBollingerBandsSigma( dwPair, idx, 21, 3 );
					if( m_TickInfo[dwPair][TKI_70TICK][idx].Cl < dwSigma3 ){
						if( g_BBBreakInfo.dwStartSMA < dwSMA && g_BBBreakInfo.dwStartMSigma < dwLoSigma2 )
						//if( m_dwExpansionTID[dwPair] < m_dwSqueezeTID[dwPair] )
						{
							dwRet = 1;;
							*pdwBreakSign =BREAKSIGN_BY_BBBREAK_BW;
							g_dwExpandCnt++;
						}
					}
				}
			}
		}else if( m_TickInfo[dwPair][TKI_70TICK][idx].Op > m_TickInfo[dwPair][TKI_70TICK][idx].Cl ){
			if( m_TickInfo[dwPair][TKI_70TICK][idx-1].Op > m_TickInfo[dwPair][TKI_70TICK][idx-1].Cl 
			// && m_TickInfo[dwPair][TKI_70TICK][idx-2].Op > m_TickInfo[dwPair][TKI_70TICK][idx-2].Cl 
			// && m_TickInfo[dwPair][TKI_70TICK][idx-3].Op > m_TickInfo[dwPair][TKI_70TICK][idx-3].Cl 
			){
				if( m_TickInfo[dwPair][TKI_70TICK][idx].Cl < dwSMA ){
					dwSigma3 = CalcBollingerBandsSigma( dwPair, idx, 21, -3 );
					if( m_TickInfo[dwPair][TKI_70TICK][idx].Cl > dwSigma3 ){
						if( g_BBBreakInfo.dwStartSMA > dwSMA && g_BBBreakInfo.dwStartPSigma > dwHiSigma2 )
						//if( m_dwExpansionTID[dwPair] < m_dwSqueezeTID[dwPair] )
						{
							dwRet = 2;;
							*pdwBreakSign =BREAKSIGN_BY_BBBREAK_BW;
							g_dwExpandCnt++;
						}
					}
				}
			}
		}

		if( g_dwExpandCnt >= 1 ){
			g_dwExpandCnt = 0;
			m_dwExpansionTID[dwPair] = m_TickInfo[dwPair][TKI_70TICK][idx].dwTID;

			g_BBBreakInfo.dwExpansionTID = m_TickInfo[dwPair][TKI_70TICK][idx].dwTID;
			g_BBBreakInfo.dwEndPSigma = CalcBollingerBandsSigma(dwPair, idx, 21, 2);
			g_BBBreakInfo.dwEndSMA = CalcBollingerBandsSigma(dwPair, idx, 21, 0);
			g_BBBreakInfo.dwEndMSigma = CalcBollingerBandsSigma(dwPair, idx, 21, -2);
		}else{
			if( dwRet > 0 ){
				dwRet = 0;;
				*pdwBreakSign = 0;
			}else{
				g_dwExpandCnt = 0;
			}
		}
	}
#endif


#if 0
	if( LastTradeInfo[dwPair].dwBreakTrigger& BREAKSIGN_BY_BBBREAK_BW ){
		if( LastTradeInfo[dwPair].dwOrderType == 1 && ( LastTradeInfo[dwPair].dClosePrice-LastTradeInfo[dwPair].dOpenPrice ) > 0 ){
			dwRet = 2;;
			*pdwBreakSign = BREAKSIGN_BY_BBBREAK_BWBACK;
		}else if( LastTradeInfo[dwPair].dwOrderType == 2 && ( LastTradeInfo[dwPair].dOpenPrice-LastTradeInfo[dwPair].dClosePrice ) > 0 ){
			dwRet = 1;;
			*pdwBreakSign = BREAKSIGN_BY_BBBREAK_BWBACK;
		}
		if( dwRet != 0 )	return dwRet;
	}

	dwHiSigma2 = CalcBollingerBandsSigma( dwPair, dwIdx, 21, 2 );
	dwLoSigma2 = CalcBollingerBandsSigma( dwPair, dwIdx, 21, -2 );

	if( dwHiSigma2 == 0 || dwLoSigma2 == 0 )	return 0;

	if( dwHiSigma2 < m_TickInfo[dwPair][TKI_70TICK][dwIdx].Cl ){
#if 0
		if( IsRangeNow(dwPair) == FALSE ){
			if( IsBandWalkStart(dwPair) != FALSE ){
				dwRet = 1;
			}else{
				g_BBChk.dwNoChangeCnt = 0;
				g_BBChk.dwPeakRate = 0;
			}
		}
#endif
		if( IsBandWalkStart(dwPair) != FALSE ){
			dwRet = 1;
			*pdwBreakSign = BREAKSIGN_BY_BBBREAK_BW;
		}else if( IsRangeNow(dwPair) == FALSE ){
			g_BBChk.dwNoChangeCnt = 0;
			g_BBChk.dwPeakRate = 0;
		}else if( g_BBChk.dwPeakRate > 0 ){
			if( g_BBChk.dwPeakRate < m_TickInfo[dwPair][TKI_70TICK][dwIdx].Cl ){
				g_BBChk.dwPeakRate = m_TickInfo[dwPair][TKI_70TICK][dwIdx].Cl;
				g_BBChk.dwNoChangeCnt = 0;
			}else if( g_BBChk.dwPeakRate - m_TickInfo[dwPair][TKI_70TICK][dwIdx].Cl > Pips2EVal(1) ){
				dwRet = 2;
				*pdwBreakSign = BREAKSIGN_BY_BBBREAK;
			}else if( g_BBChk.dwNoChangeCnt > 5 ){
				dwRet = 2;;
				*pdwBreakSign = BREAKSIGN_BY_BBBREAK;
			}else{
				g_BBChk.dwNoChangeCnt++;
			}
		}else{
			g_BBChk.dwPeakRate = m_TickInfo[dwPair][TKI_70TICK][dwIdx].Cl;
			g_BBChk.dwNoChangeCnt = 0;
		}
	}else if( dwLoSigma2 > m_TickInfo[dwPair][TKI_70TICK][dwIdx].Cl ){
#if 0
		if( IsRangeNow(dwPair) == FALSE ){
			if( IsBandWalkStart(dwPair) != FALSE ){
				dwRet = 2;
			}else{
				g_BBChk.dwNoChangeCnt = 0;
				g_BBChk.dwPeakRate = 0;
			}
#endif
		if( IsBandWalkStart(dwPair) != FALSE ){
			dwRet = 2;
			*pdwBreakSign = BREAKSIGN_BY_BBBREAK_BW;
		}else if( IsRangeNow(dwPair) == FALSE ){
			g_BBChk.dwNoChangeCnt = 0;
			g_BBChk.dwPeakRate = 0;
		}else if( g_BBChk.dwPeakRate > 0 ){
			if( g_BBChk.dwPeakRate > m_TickInfo[dwPair][TKI_70TICK][dwIdx].Cl ){
				g_BBChk.dwPeakRate = m_TickInfo[dwPair][TKI_70TICK][dwIdx].Cl;
				g_BBChk.dwNoChangeCnt = 0;
			}else if( m_TickInfo[dwPair][TKI_70TICK][dwIdx].Cl - g_BBChk.dwPeakRate > Pips2EVal(1) ){
				dwRet = 1;
				*pdwBreakSign = BREAKSIGN_BY_BBBREAK;
			}else if( g_BBChk.dwNoChangeCnt > 5 ){
				dwRet = 1;;
				*pdwBreakSign = BREAKSIGN_BY_BBBREAK;
			}else{
				g_BBChk.dwNoChangeCnt++;
			}
		}else{
			g_BBChk.dwPeakRate = m_TickInfo[dwPair][TKI_70TICK][dwIdx].Cl;
			g_BBChk.dwNoChangeCnt = 0;
		}
	}else{
		g_BBChk.dwNoChangeCnt = 0;
		g_BBChk.dwPeakRate = 0;
	}

	if( *pdwBreakSign == BREAKSIGN_BY_BBBREAK ){
		dwRet = 0;
	}
#endif
	return dwRet;
}
DWORD CAgentWorkerDlg::TradeCheck_Main( DWORD dwPair, DWORD* pdwRetBreakSign )
{
	DWORD dwRet = 0;
	DWORD dwDBRet = 0;
	DWORD dwMABRet = 0;
	DWORD dwBBRet = 0;

	*pdwRetBreakSign = 0;

	if(m_TickInfo[dwPair][TKI_70TICK].size() > 1) {
		//	同時選ブレイクのチェック
		if(m_Setting[dwPair].db.bEnable != FALSE) {
			if((dwDBRet = TradeCheck_DoujiBreak(dwPair)) > 0) {
				*pdwRetBreakSign |= BREAKSIGN_BY_DOUJIBREAK;

				dwRet = dwDBRet;
			}
		}
		if(m_Setting[dwPair].mab.bEnable != FALSE) {
			if((dwMABRet = TradeCheck_EMABreak(dwPair)) > 0) {
				*pdwRetBreakSign |= BREAKSIGN_BY_EMABREAK;

				dwRet = dwMABRet;
			}
		}
		if(m_Setting[dwPair].brsi.bEnable != FALSE) {
			if((dwBBRet = TradeCheck_BB3Sigma( dwPair )) > 0) {
				*pdwRetBreakSign |= BREAKSIGN_BY_3SIGMABREAK;

				dwRet = dwBBRet;
			}
		}

		if(m_Setting[dwPair].so.bEnable != FALSE){
			if((dwBBRet = TradeCheck_SOBreak( dwPair )) > 0 ){
				*pdwRetBreakSign |= BREAKSIGN_BY_SOBREAK;

				dwRet = dwBBRet;
			}
		}
		
		if(m_Setting[dwPair].rsi.bEnable != FALSE){
			if((dwBBRet = TradeCheck_RSIBreak( dwPair )) > 0 ){
				*pdwRetBreakSign |= BREAKSIGN_BY_RSIBREAK;

				dwRet = dwBBRet;
			}
		}

		if(m_Setting[dwPair].ichimoku.bEnable != FALSE){
			if((dwBBRet = TradeCheck_ICHIMOKUBreak( dwPair )) > 0 ){
				*pdwRetBreakSign |= BREAKSIGN_BY_ICHIMOKUBREAK;

				dwRet = dwBBRet;
			}
		}

		if(m_Setting[dwPair].bb.bEnable != FALSE) {
			//DWORD dwBreadkSign = 0;
			//if((dwBBRet = TradeCheck_BBBackBreak(dwPair, &dwBreadkSign )) > 0) {
			//	*pdwRetBreakSign |= dwBreadkSign;

			//	dwRet = dwBBRet;
			//}else if((dwBBRet = TradeCheck_BBBreak(dwPair, &dwBreadkSign )) > 0) {
			//	*pdwRetBreakSign |= dwBreadkSign;

			//	dwRet = dwBBRet;
			//}
		}

		if(m_Setting[dwPair].rb.bEnable != FALSE) {
			if((dwBBRet = TradeCheck_RangeBreak( dwPair )) > 0 ){
				*pdwRetBreakSign |= BREAKSIGN_BY_RANGEBREAK;

				dwRet = dwBBRet;
			}
		}
	}

	return dwRet;
}

void CAgentWorkerDlg::MT4_TradeCheck(DWORD dwMapId)
{
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];
	DWORD dwDBRet = 0;


	PMT4_TRADECHECK_DATA pMapData;

	wsprintf(cMapName, "MT4_TradeCheck_%d", dwMapId);

	if ((hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cMapName)) != INVALID_HANDLE_VALUE) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_TRADECHECK_DATA)pMap;

			pMapData->dwRet = 0;
			pMapData->dwRetBreakSign = 0;

			pMapData->dwRet = TradeCheck_Main( pMapData->dwPair, &(pMapData->dwRetBreakSign) );

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	return;
}
DWORD CAgentWorkerDlg::RegisterTradeInfo_Main( DWORD dwPair, DWORD dwTicket, DWORD dwOrderType, double dOpenPrice, double dStopLoss, DWORD dwBreakTrigger )
{
	DWORD dwRet = 0;
	TRADE_INFO AddData;

	memset(&AddData, 0x00, sizeof(TRADE_INFO));

	AddData.dwVer = TRADE_LATEST_VER;
	AddData.dwPair = dwPair;
	AddData.dwTicket = dwTicket;
	AddData.dwOrderType = dwOrderType;
	AddData.dOpenPrice = dOpenPrice;
	AddData.dClosePrice = 0;
	AddData.dStopLoss = dStopLoss;
	AddData.dwMaxEval = Rate2EVal(dOpenPrice,dwPair);
	AddData.dwTID = m_TickInfo[dwPair][TKI_70TICK][m_TickInfo[dwPair][TKI_70TICK].size()-1].dwTID;
	AddData.dwPeakRate = m_TickInfo[dwPair][TKI_70TICK][m_TickInfo[dwPair][TKI_70TICK].size()-1].Cl;
	//GetLocalTime(&AddData.openDt);
	if( m_bTestMode != FALSE ){
		memcpy( &AddData.openDt, &m_TestCurTime, sizeof(SYSTEMTIME) );
	}else{
		GetLocalTime(&AddData.openDt);
	}

	AddData.dwBreakTrigger = dwBreakTrigger;

	if( dwBreakTrigger == BREAKSIGN_BY_BBBREAK_BW ){
		memcpy( &AddData.bbi, &g_BBBreakInfo, sizeof(BBBREAK_INFO) );
	}else if( dwBreakTrigger == BREAKSIGN_BY_RSIBREAK ){
		if( dwOrderType == 1 ){
			AddData.rsi.dRSI = 20;
		}else{
			AddData.rsi.dRSI = 80;
		}
	}

	m_tradeInfo.push_back(AddData);

	memcpy( &LastTradeInfo[dwPair], &AddData, sizeof(TRADE_INFO) );

	WriteTradeInfo(&AddData, TRUE);

	OutputTradeInfo();

	//	現在のTick情報にトレード開始を記録する
	DWORD dwCurIdx = m_TickInfo[dwPair][TKI_70TICK].size() - 1;
	m_TickInfo[dwPair][TKI_70TICK][dwCurIdx].dwStrtTicket = dwTicket;

	dwRet = 1;

	return dwRet;
}

void CAgentWorkerDlg::MT4_RegisterTradeInfo(DWORD dwMapId)
{
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];


	PMT4_REGISTERTRADEINFO_DATA pMapData;

	wsprintf(cMapName, "MT4_RegisterTradeInfo_%d", dwMapId);

	if ((hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cMapName)) != INVALID_HANDLE_VALUE) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_REGISTERTRADEINFO_DATA)pMap;

			pMapData->dwRet = RegisterTradeInfo_Main( pMapData->dwPair, pMapData->dwTicket, pMapData->dwOrderType, pMapData->dOpenPrice, pMapData->dStopLoss, pMapData->dwBreakTrigger );

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	return;
}

DWORD CAgentWorkerDlg::GetPassedTradeTick(DWORD dwPair, DWORD dwTicket)
{
	DWORD dwRet = 0;

	for( DWORD i = m_TickInfo[dwPair][TKI_70TICK].size() - 1; i > 0; i--  ){
		if( m_TickInfo[dwPair][TKI_70TICK][i].dwStrtTicket == dwTicket ){
			dwRet = m_TickInfo[dwPair][TKI_70TICK].size() - 1 - i;
			break;
		}
	}

	return dwRet;
}

BOOL CAgentWorkerDlg::CloseCheck_EMABreak( TRADE_INFO& ti)
{
	BOOL bClose = FALSE;
	DWORD dwCurIdx = m_TickInfo[ti.dwPair][TKI_70TICK].size() - 1;
	DWORD dwEMA1;
	DWORD dwEMA2;

	if(dwCurIdx > 0) {
		dwEMA1 = CalcEMAValue(dwCurIdx, m_Setting[ti.dwPair].mab.dwMA1Val, ti.dwPair);
		dwEMA2 = CalcEMAValue(dwCurIdx, m_Setting[ti.dwPair].mab.dwMA2Val, ti.dwPair);

		if( ti.dwOrderType == 1 ){
			if(dwEMA1 <= dwEMA2){
				bClose = TRUE;
			}
		}else if(ti.dwOrderType == 2) {
			if(dwEMA1 >= dwEMA2) {
				bClose = TRUE;
			}
		}
	}

	return bClose;
}

#if 0
struct{
	DWORD dwTrandStartTID;
	DWORD dwTrandEndTID;
	DWORD dwBBWalkCnt;
}g_BBWalkTradeInfo = {0,0,0};
#endif

//	閉じる条件
//	条件1:順張り側に進みバンドウォーク最大値お上回った（失敗）
//	条件2:
//	条件3:
BOOL CAgentWorkerDlg::CloseCheck_BBBreak_BWBack( TRADE_INFO& ti)
{
	BOOL bClose = FALSE;
	DWORD dwCurIdx = m_TickInfo[ti.dwPair][TKI_70TICK].size() - 1;
return FALSE;
	if(dwCurIdx > 0) {
#if 0
		DWORD dwCurRate = m_TickInfo[ti.dwPair][TKI_70TICK][m_TickInfo[ti.dwPair][TKI_70TICK].size() - 1].Cl;
		DWORD dwOpenPrice = Rate2EVal(ti.dOpenPrice, ti.dwPair);
		DWORD dwSpread = Rate2EVal(GetSpread(ti.dwPair), ti.dwPair);
		DWORD dwLossCutEVal = Pips2EVal(m_Setting[ti.dwPair].dwLosCutPips);
		DWORD dwIKEIKEModeEndEVal = Pips2EVal(m_Setting[ti.dwPair].dwIKEIKEEndPips);
		DWORD dwGainEVal = Pips2EVal(m_Setting[ti.dwPair].dwGainPips);
		DWORD dwClosePrice;

		if(ti.dwOrderType == 1) {
			dwClosePrice = dwCurRate - dwSpread / 2;
			//dwOpenPrice・・・買ったときの値段
			//dwClosePrice・・・今売った場合の値段
			if( dwOpenPrice > dwClosePrice ) {
				//	とりあえず儲けは出てる
				if( dwOpenPrice - dwClosePrice  >= Pips2EVal(2) ) {
					bClose = TRUE;
				}
			}
		}else{
			dwClosePrice = dwCurRate + dwSpread / 2;
			//dwOpenPrice・・・売ったときの値段
			//dwClosePrice・・・今売った場合の値段
			if( dwOpenPrice < dwClosePrice ) {
				//	とりあえず儲けは出てる
				if( dwClosePrice -dwOpenPrice >= Pips2EVal(2) ) {
					bClose = TRUE;
				}
			}
		}
#endif
#if 1
		DWORD dwWalkSpan = g_BBWalkTradeInfo.dwTrandEndTID - g_BBWalkTradeInfo.dwTrandStartTID;

		if( ( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].dwTID - (g_BBWalkTradeInfo.dwTrandEndTID+1) ) > dwWalkSpan ){

			DWORD dwOpenPrice = Rate2EVal(ti.dOpenPrice, ti.dwPair);
			DWORD dwSpread = Rate2EVal(GetSpread(ti.dwPair), ti.dwPair);
			DWORD dwLossCutEVal = Pips2EVal(m_Setting[ti.dwPair].dwLosCutPips);
			DWORD dwIKEIKEModeEndEVal = Pips2EVal(m_Setting[ti.dwPair].dwIKEIKEEndPips);
			DWORD dwGainEVal = Pips2EVal(m_Setting[ti.dwPair].dwGainPips);

			bClose = TRUE;

			if( ti.dwOrderType == 1 ){
				//	期待した通りの儲けがでなかったらClose
				//if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl <= dwOpenPrice + dwGainEVal )
				{
					bClose = TRUE;
				}
			}else if(ti.dwOrderType == 2) {
				//	期待した通りの儲けがでなかったらClose
				//if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl >= dwOpenPrice + dwGainEVal )
				{
					bClose = TRUE;
				}
			}
		}else{
			if( ti.dwOrderType == 1 ){
				if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl > m_TickInfo[ti.dwPair][TKI_70TICK][g_BBWalkTradeInfo.dwTrandEndIdx+1].Op ){
					if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl - m_TickInfo[ti.dwPair][TKI_70TICK][g_BBWalkTradeInfo.dwTrandEndIdx+1].Op > g_BBWalkTradeInfo.dwMaxGain ){
						bClose = TRUE;
					}
				}
				if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl <= g_BBWalkTradeInfo.dwPeakVal ){
					bClose = TRUE;
				}
			}else if(ti.dwOrderType == 2) {
				if( m_TickInfo[ti.dwPair][TKI_70TICK][g_BBWalkTradeInfo.dwTrandEndIdx+1].Op > m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl ){
					if( m_TickInfo[ti.dwPair][TKI_70TICK][g_BBWalkTradeInfo.dwTrandEndIdx+1].Op - m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl > g_BBWalkTradeInfo.dwMaxGain ){
						bClose = TRUE;
					}
				}
				if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl >= g_BBWalkTradeInfo.dwPeakVal ){
					bClose = TRUE;
				}
			}
		}
#endif
	}

	return bClose;
}


BOOL CAgentWorkerDlg::CloseCheck_IchimokuBreak( TRADE_INFO& ti)
{
	BOOL bClose = FALSE;
	DWORD dwCurIdx = m_TickInfo[ti.dwPair][TKI_70TICK].size() - 1;

	if(dwCurIdx > 0) {
		DWORD dwCurRate = m_TickInfo[ti.dwPair][TKI_70TICK][m_TickInfo[ti.dwPair][TKI_70TICK].size() - 1].Cl;
		DWORD dwOpenPrice = Rate2EVal(ti.dOpenPrice, ti.dwPair);
		DWORD dwSpread = Rate2EVal(GetSpread(ti.dwPair), ti.dwPair);
		DWORD dwClosePrice;
		DWORD dwPeakPrice;

		ICHIMOKU_INFO ichi_info;

		memset( &ichi_info, 0x00, sizeof(ICHIMOKU_INFO) );

		if( GetIchimokuData( ti.dwPair, dwCurIdx, &ichi_info )	== FALSE )	return 0;
		DWORD dwSO_K = 0;
		DWORD dwSO_D = 0;
		DWORD dwSO_SlowD = 0;

		//if( m_TickInfo[dwPair][TKI_70TICK][idx].bCl == FALSE )	return 0;

		if( CalcStochasticOscillator( ti.dwPair, dwCurIdx, 26, 26, 3, &dwSO_K, &dwSO_D, &dwSO_SlowD ) == FALSE )	return 0;

		if(ti.dwOrderType == 1) {
			dwClosePrice = dwCurRate - dwSpread / 2;
			dwPeakPrice = ti.dwPeakRate - dwSpread / 2;

			if(ti.dwPeakRate < dwCurRate) {
				ti.dwPeakRate = dwCurRate;
			}

			//if(dwClosePrice <= dwOpenPrice - 100 ) {
			//	ti.dwCloseSign = CLOSESIGN_LOSSCUT;
			//	bClose = TRUE;
			//}else 
			if(dwClosePrice >= dwOpenPrice + 200 ) {
				ti.dwCloseSign = CLOSESIGN_PROFIT;
				bClose = TRUE;
			}else if( ichi_info.dwBaseVal > dwCurRate || dwSO_D > 80
				//|| ichi_info.dwLeadingSpan1 > dwCurRate 
				//|| ichi_info.dwLeadingSpan2 > dwCurRate 
			){
				ti.dwCloseSign = CLOSESIGN_LOSSCUT;
				bClose = TRUE;
			}else if( dwPeakPrice > dwOpenPrice + 5 && dwClosePrice <= dwOpenPrice ){
				ti.dwCloseSign = CLOSESIGN_ZEROBACK;
				bClose = TRUE;
			}
		}
		else if(ti.dwOrderType == 2) {
			dwClosePrice = dwCurRate + dwSpread / 2;
			dwPeakPrice = ti.dwPeakRate + dwSpread / 2;

			if(ti.dwPeakRate > dwCurRate) {
				ti.dwPeakRate = dwCurRate;
			}

			//if(dwClosePrice >= dwOpenPrice + 100 ) {
			//	ti.dwCloseSign = CLOSESIGN_LOSSCUT;
			//	bClose = TRUE;
			//}else 
			if(dwClosePrice <= dwOpenPrice - 200 ) {
				ti.dwCloseSign = CLOSESIGN_PROFIT;
				bClose = TRUE;
			}else if( ichi_info.dwBaseVal < dwCurRate  || dwSO_D < 20
				//|| ichi_info.dwLeadingSpan1 < dwCurRate 
				//|| ichi_info.dwLeadingSpan2 < dwCurRate 
			){
				ti.dwCloseSign = CLOSESIGN_LOSSCUT;
				bClose = TRUE;
			}else if( dwPeakPrice < dwOpenPrice - 5 && dwClosePrice >= dwOpenPrice ){
				ti.dwCloseSign = CLOSESIGN_ZEROBACK;
				bClose = TRUE;
			}
		}
	}
	return bClose;
}

BOOL CAgentWorkerDlg::CloseCheck_RSIBreak( TRADE_INFO& ti)
{
	BOOL bClose = FALSE;
	DWORD dwCurIdx = m_TickInfo[ti.dwPair][TKI_70TICK].size() - 1;

	if(dwCurIdx > 0) {
		DWORD dwCurRate = m_TickInfo[ti.dwPair][TKI_70TICK][m_TickInfo[ti.dwPair][TKI_70TICK].size() - 1].Cl;
		DWORD dwOpenPrice = Rate2EVal(ti.dOpenPrice, ti.dwPair);
		DWORD dwSpread = Rate2EVal(GetSpread(ti.dwPair), ti.dwPair);
		DWORD dwClosePrice;
		DWORD dwPeakPrice;

		double dRSI = CalcRSI( ti.dwPair, m_Setting[ti.dwPair].rsi.dwSpan , 0, 0 );


		if(ti.dwOrderType == 1) {
			dwClosePrice = dwCurRate - dwSpread / 2;
			dwPeakPrice = ti.dwPeakRate - dwSpread / 2;

			if(ti.dwPeakRate < dwCurRate) {
				ti.dwPeakRate = dwCurRate;
			}

			if(dwClosePrice <= dwOpenPrice - 50 ) {
				ti.dwCloseSign = CLOSESIGN_LOSSCUT;
				bClose = TRUE;
			}else if( dRSI > 70 ){
				ti.dwCloseSign = CLOSESIGN_PROFIT;
				bClose = TRUE;
			}else if( ti.rsi.dRSI == 20 ){
				if( dRSI > 60 ){
					ti.rsi.dRSI = 60;
				}else if( dRSI > 40 ){
					ti.rsi.dRSI = 40;
				}
			}else if( ti.rsi.dRSI == 40 ){
				if( dRSI < 40 ){
					if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].bCl != FALSE ){
						ti.dwCloseSign = CLOSESIGN_LOSSCUT;
						bClose = TRUE;
					}
				}else if( dRSI > 60 ){
					ti.rsi.dRSI = 60;
				}
			}else if( ti.rsi.dRSI == 60 ){
				if( dRSI < 40 ){
					if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].bCl != FALSE ){
						ti.dwCloseSign = CLOSESIGN_LOSSCUT;
						bClose = TRUE;
					}
				}
			}


		}
		else if(ti.dwOrderType == 2) {
			dwClosePrice = dwCurRate + dwSpread / 2;
			dwPeakPrice = ti.dwPeakRate + dwSpread / 2;

			if(ti.dwPeakRate > dwCurRate) {
				ti.dwPeakRate = dwCurRate;
			}

			if(dwClosePrice >= dwOpenPrice + 50 ) {
				ti.dwCloseSign = CLOSESIGN_LOSSCUT;
				bClose = TRUE;
			}else if( dRSI < 30 ){
				ti.dwCloseSign = CLOSESIGN_PROFIT;
				bClose = TRUE;
			}else if( ti.rsi.dRSI == 80 ){
				if( dRSI < 40 ){
					ti.rsi.dRSI = 40;
				}else if( dRSI < 60 ){
					ti.rsi.dRSI = 60;
				}
			}else if( ti.rsi.dRSI == 60 ){
				if( dRSI > 60 ){
					if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].bCl != FALSE ){
						ti.dwCloseSign = CLOSESIGN_LOSSCUT;
						bClose = TRUE;
					}
				}else if( dRSI < 40 ){
					ti.rsi.dRSI = 40;
				}
			}else if( ti.rsi.dRSI == 40 ){
				if( dRSI > 40 ){
					if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].bCl != FALSE ){
						ti.dwCloseSign = CLOSESIGN_LOSSCUT;
						bClose = TRUE;
					}
				}
			}

		}
	}
	return bClose;
}

BOOL CAgentWorkerDlg::CloseCheck_SOBreak( TRADE_INFO& ti)
{
	BOOL bClose = FALSE;
	DWORD dwCurIdx = m_TickInfo[ti.dwPair][TKI_70TICK].size() - 1;

	if(dwCurIdx > 0) {
		DWORD dwCurRate = m_TickInfo[ti.dwPair][TKI_70TICK][m_TickInfo[ti.dwPair][TKI_70TICK].size() - 1].Cl;
		DWORD dwOpenPrice = Rate2EVal(ti.dOpenPrice, ti.dwPair);
		DWORD dwSpread = Rate2EVal(GetSpread(ti.dwPair), ti.dwPair);
		DWORD dwClosePrice;
		DWORD dwPeakPrice;

		DWORD dwSO_K = 0;
		DWORD dwSO_D = 0;
		DWORD dwSO_SlowD = 0;

		//if( m_TickInfo[dwPair][TKI_70TICK][idx].bCl == FALSE )	return 0;

		if( CalcStochasticOscillator( ti.dwPair, dwCurIdx, m_Setting[ti.dwPair].so.dwKSpan, m_Setting[ti.dwPair].so.dwDSpan, m_Setting[ti.dwPair].so.dwSlowDSpan, &dwSO_K, &dwSO_D, &dwSO_SlowD ) == FALSE )	return 0;

		if(ti.dwOrderType == 1) {
			dwClosePrice = dwCurRate - dwSpread / 2;
			dwPeakPrice = ti.dwPeakRate - dwSpread / 2;

			if(ti.dwPeakRate < dwCurRate) {
				ti.dwPeakRate = dwCurRate;
			}

			if(dwClosePrice <= dwOpenPrice - 100 ) {
				ti.dwCloseSign = CLOSESIGN_LOSSCUT;
				bClose = TRUE;
			}else if( dwSO_D > 80 ){
				ti.dwCloseSign = CLOSESIGN_PROFIT;
				bClose = TRUE;
			}
			//if(dwClosePrice <= dwOpenPrice - m_Setting[ti.dwPair].brsi.dwLossCut) {
			//	ti.dwCloseSign = CLOSESIGN_LOSSCUT;
			//	bClose = TRUE;
			//}else if( dwPeakPrice > dwOpenPrice + 5 && dwClosePrice <= dwOpenPrice ){
			//	ti.dwCloseSign = CLOSESIGN_ZEROBACK;
			//	bClose = TRUE;
			//}else if( dwSO_D > 80 ){
			//	ti.dwCloseSign = CLOSESIGN_PROFIT;
			//	bClose = TRUE;
			//}

		}
		else if(ti.dwOrderType == 2) {
			dwClosePrice = dwCurRate + dwSpread / 2;
			dwPeakPrice = ti.dwPeakRate + dwSpread / 2;

			if(ti.dwPeakRate > dwCurRate) {
				ti.dwPeakRate = dwCurRate;
			}

			if(dwClosePrice >= dwOpenPrice + 100 ) {
				ti.dwCloseSign = CLOSESIGN_LOSSCUT;
				bClose = TRUE;
			}else /*if( dwPeakPrice < dwOpenPrice - 5 && dwClosePrice >= dwOpenPrice ){
				ti.dwCloseSign = CLOSESIGN_ZEROBACK;
				bClose = TRUE;
			}else */if( dwSO_D < 20 ){
				ti.dwCloseSign = CLOSESIGN_PROFIT;
				bClose = TRUE;
			}

		}
	}
	return bClose;
}

//	閉じる条件
//	条件1:順張り側に進みバンドウォーク最大値お上回った（失敗）
//	条件2:
//	条件3:
BOOL CAgentWorkerDlg::CloseCheck_BBBreak_BW( TRADE_INFO& ti)
{
	BOOL bClose = FALSE;
	DWORD dwCurIdx = m_TickInfo[ti.dwPair][TKI_70TICK].size() - 1;

	if( ti.dwTID == m_TickInfo[ti.dwPair][TKI_70TICK][m_TickInfo[ti.dwPair][TKI_70TICK].size() - 1].dwTID ){
		return FALSE;
	}

	if(dwCurIdx > 0) {
		DWORD dwCurSigma;
		DWORD dwCurSigma3;
		DWORD dwPrevSigma;
		
		DWORD dwCurRate = m_TickInfo[ti.dwPair][TKI_70TICK][m_TickInfo[ti.dwPair][TKI_70TICK].size() - 1].Cl;
		DWORD dwOpenPrice = Rate2EVal(ti.dOpenPrice, ti.dwPair);
		DWORD dwSpread = Rate2EVal(GetSpread(ti.dwPair), ti.dwPair);
		//DWORD dwLossCutEVal = Pips2EVal(m_Setting[ti.dwPair].dwLosCutPips);
		//DWORD dwIKEIKEModeEndEVal = Pips2EVal(m_Setting[ti.dwPair].dwIKEIKEEndPips);
		//DWORD dwGainEVal = Pips2EVal(m_Setting[ti.dwPair].dwGainPips);
		DWORD dwClosePrice;
		DWORD dwPeakPrice;

		if(ti.dwOrderType == 1) {
			dwCurSigma3 = CalcBollingerBandsSigma( ti.dwPair, dwCurIdx, 21,3 );
			dwCurSigma = CalcBollingerBandsSigma( ti.dwPair, dwCurIdx, 21,-2 );
			dwPrevSigma = CalcBollingerBandsSigma( ti.dwPair, dwCurIdx-1, 21, -2 );

			dwClosePrice = dwCurRate - dwSpread / 2;
			dwPeakPrice = ti.dwPeakRate - dwSpread / 2;

			if( ti.dwPeakRate < dwCurRate ){
				ti.dwPeakRate = dwCurRate;
			}

/*			if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl >= dwCurSigma3 ){
				ti.dwCloseSign = CLOSESIGN_BY_BBBREAK_BW_OVERSIGMA3;
				bClose = TRUE;
			}else if( dwPrevSigma < dwCurSigma ){
				ti.dwCloseSign = CLOSESIGN_BY_BBBREAK_BW_CHANGESIGMA;
				bClose = TRUE;
			}
			else */if( dwClosePrice <= dwOpenPrice - m_Setting[ti.dwPair].brsi.dwLossCut ){
				ti.dwCloseSign = CLOSESIGN_LOSSCUT;
				bClose = TRUE;
			}
			else if( dwPeakPrice > dwOpenPrice + 5 && dwClosePrice <= dwOpenPrice ){
				ti.dwCloseSign = CLOSESIGN_ZEROBACK;
				bClose = TRUE;
			}
			else if( ti.bIKEIKE_Mode != FALSE && ti.dwPeakRate - dwCurRate > 10  ){
				ti.dwCloseSign = CLOSESIGN_PROFIT;
				bClose = TRUE;
			}
			else if( ti.bIKEIKE_Mode == FALSE && dwClosePrice >= dwOpenPrice + m_Setting[ti.dwPair].brsi.dwProfit ){
				//ti.dwCloseSign = CLOSESIGN_PROFIT;
				//bClose = TRUE;
				ti.bIKEIKE_Mode = TRUE;
			}
			/*else{
				if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].bCl != FALSE ){
					if( dwCurIdx - ti.dwTID <= 1 ){
						if(  ABS_CMP( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl, m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Op, 10 ) < 0 ){
							bClose = TRUE;
						}
					}else{
						if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl < m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Op ){
							bClose = TRUE;
						}
					}
				}
			}*/


			/*
			else{
				if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].bCl != FALSE ){
					if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Op > m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl ){
						bClose = TRUE;
			}
		}
			}*/

		}
		else if(ti.dwOrderType == 2) {
			dwCurSigma3 = CalcBollingerBandsSigma( ti.dwPair, dwCurIdx, 21,-3 );
			dwCurSigma = CalcBollingerBandsSigma( ti.dwPair, dwCurIdx, 21, 2 );
			dwPrevSigma = CalcBollingerBandsSigma( ti.dwPair, dwCurIdx-1, 21, 2 );
			dwClosePrice = dwCurRate + dwSpread / 2;
			dwPeakPrice = ti.dwPeakRate + dwSpread / 2;

			if( ti.dwPeakRate > dwCurRate ){
				ti.dwPeakRate = dwCurRate;
			}

/*			if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl <= dwCurSigma3 ){
				ti.dwCloseSign = CLOSESIGN_BY_BBBREAK_BW_OVERSIGMA3;
				bClose = TRUE;
			}else if( dwPrevSigma > dwCurSigma ){
				ti.dwCloseSign = CLOSESIGN_BY_BBBREAK_BW_CHANGESIGMA;
				bClose = TRUE;
			}
			else*/ if( dwClosePrice >= dwOpenPrice + m_Setting[ti.dwPair].brsi.dwLossCut ){
				ti.dwCloseSign = CLOSESIGN_LOSSCUT;
				bClose = TRUE;
			}
			else if( dwPeakPrice < dwOpenPrice - 5 && dwClosePrice >= dwOpenPrice ){
				ti.dwCloseSign = CLOSESIGN_ZEROBACK;
				bClose = TRUE;
			}else if( ti.bIKEIKE_Mode != FALSE && ti.dwPeakRate - dwCurRate > 10  ){
				ti.dwCloseSign = CLOSESIGN_PROFIT;
				bClose = TRUE;
			}else if( ti.bIKEIKE_Mode == FALSE && dwClosePrice <= dwOpenPrice - m_Setting[ti.dwPair].brsi.dwProfit ){
			//	ti.dwCloseSign = CLOSESIGN_PROFIT;
			//	bClose = TRUE;
				ti.bIKEIKE_Mode = TRUE;
			}
			/*
			else{
				if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].bCl != FALSE ){
					if( dwCurIdx - ti.dwTID <= 1 ){
						if(  ABS_CMP( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl, m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Op, 10 ) < 0 ){
							bClose = TRUE;
						}
					}else{
						if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl > m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Op ){
							bClose = TRUE;
						}
					}
				}
			}
*/
			/*
			else{
				if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].bCl != FALSE ){
					if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Op < m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl ){
						bClose = TRUE;
					}
				}
			}*/
		}
	}
#if 0
		DWORD dwCurRate = m_TickInfo[ti.dwPair][TKI_70TICK][m_TickInfo[ti.dwPair][TKI_70TICK].size() - 1].Cl;
		DWORD dwOpenPrice = Rate2EVal(ti.dOpenPrice, ti.dwPair);
		DWORD dwSpread = Rate2EVal(GetSpread(ti.dwPair), ti.dwPair);
		DWORD dwLossCutEVal = Pips2EVal(m_Setting[ti.dwPair].dwLosCutPips);
		DWORD dwIKEIKEModeEndEVal = Pips2EVal(m_Setting[ti.dwPair].dwIKEIKEEndPips);
		DWORD dwGainEVal = Pips2EVal(m_Setting[ti.dwPair].dwGainPips);
		DWORD dwClosePrice;

		if(ti.dwOrderType == 1) {
			dwClosePrice = dwCurRate - dwSpread / 2;
			//dwOpenPrice・・・買ったときの値段
			//dwClosePrice・・・今売った場合の値段
			if( dwOpenPrice > dwClosePrice ) {
				//	とりあえず儲けは出てる
				if( dwOpenPrice - dwClosePrice  >= Pips2EVal(2) ) {
					bClose = TRUE;
				}
			}
		}else{
			dwClosePrice = dwCurRate + dwSpread / 2;
			//dwOpenPrice・・・売ったときの値段
			//dwClosePrice・・・今売った場合の値段
			if( dwOpenPrice < dwClosePrice ) {
				//	とりあえず儲けは出てる
				if( dwClosePrice -dwOpenPrice >= Pips2EVal(2) ) {
					bClose = TRUE;
				}
			}
		}
#endif
#if 0
		DWORD dwWalkSpan = g_BBWalkTradeInfo.dwTrandEndTID - g_BBWalkTradeInfo.dwTrandStartTID;

		if( ( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].dwTID - (g_BBWalkTradeInfo.dwTrandEndTID+1) ) > dwWalkSpan ){

			DWORD dwOpenPrice = Rate2EVal(ti.dOpenPrice, ti.dwPair);
			DWORD dwSpread = Rate2EVal(GetSpread(ti.dwPair), ti.dwPair);
			DWORD dwLossCutEVal = Pips2EVal(m_Setting[ti.dwPair].dwLosCutPips);
			DWORD dwIKEIKEModeEndEVal = Pips2EVal(m_Setting[ti.dwPair].dwIKEIKEEndPips);
			DWORD dwGainEVal = Pips2EVal(m_Setting[ti.dwPair].dwGainPips);

			bClose = TRUE;

			if( ti.dwOrderType == 1 ){
				//	期待した通りの儲けがでなかったらClose
				//if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl <= dwOpenPrice + dwGainEVal )
				{
					bClose = TRUE;
				}
			}else if(ti.dwOrderType == 2) {
				//	期待した通りの儲けがでなかったらClose
				//if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl >= dwOpenPrice + dwGainEVal )
				{
					bClose = TRUE;
				}
			}
		}/*else{
			if( ti.dwOrderType == 1 ){
				if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl > m_TickInfo[ti.dwPair][TKI_70TICK][g_BBWalkTradeInfo.dwTrandEndIdx+1].Op ){
					if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl - m_TickInfo[ti.dwPair][TKI_70TICK][g_BBWalkTradeInfo.dwTrandEndIdx+1].Op > g_BBWalkTradeInfo.dwMaxGain ){
						bClose = TRUE;
					}
				}
				if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl <= g_BBWalkTradeInfo.dwPeakVal ){
					bClose = TRUE;
				}
			}else if(ti.dwOrderType == 2) {
				if( m_TickInfo[ti.dwPair][TKI_70TICK][g_BBWalkTradeInfo.dwTrandEndIdx+1].Op > m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl ){
					if( m_TickInfo[ti.dwPair][TKI_70TICK][g_BBWalkTradeInfo.dwTrandEndIdx+1].Op - m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl > g_BBWalkTradeInfo.dwMaxGain ){
						bClose = TRUE;
					}
				}
				if( m_TickInfo[ti.dwPair][TKI_70TICK][dwCurIdx].Cl >= g_BBWalkTradeInfo.dwPeakVal ){
					bClose = TRUE;
				}
			}
		}*/
#endif
	//}

	return bClose;
}

DWORD CAgentWorkerDlg::CheckCloseTrade_Main( DWORD dwPair, DWORD dwTicket)
{
	DWORD dwRet = 0;

	if(m_TickInfo[dwPair][TKI_70TICK].size() > 0) {
		vector<TRADE_INFO>::iterator itr = m_tradeInfo.begin();

		DWORD dwCurRate = m_TickInfo[dwPair][TKI_70TICK][m_TickInfo[dwPair][TKI_70TICK].size() - 1].Cl;

		for(; itr != m_tradeInfo.end(); itr++) {
			if((*itr).dwTicket == dwTicket) {
				DWORD dwOpenPrice = Rate2EVal((*itr).dOpenPrice, dwPair);
				DWORD dwSpread = Rate2EVal(GetSpread(dwPair), dwPair);
				DWORD dwLossCutEVal = Pips2EVal(m_Setting[dwPair].dwLosCutPips);
				DWORD dwIKEIKEModeEndEVal = Pips2EVal(m_Setting[dwPair].dwIKEIKEEndPips);
				DWORD dwGainEVal = Pips2EVal(m_Setting[dwPair].dwGainPips);
				DWORD dwClosePrice;

				//	買い注文->売り決算
				if((*itr).dwOrderType == 1) {
					if( (*itr).dwPeakRate < dwCurRate )	(*itr).dwPeakRate = dwCurRate;

					dwClosePrice = dwCurRate - dwSpread / 2;
					//dwOpenPrice・・・買ったときの値段
					//dwClosePrice・・・今売った場合の値段
					if((*itr).bIKEIKE_Mode != FALSE) {
						if((*itr).dwIKEIKE_EVal - dwIKEIKEModeEndEVal >= dwClosePrice) {
							dwRet = 1;
						}
						else {
							if((*itr).dwIKEIKE_EVal < dwClosePrice) {
								(*itr).dwIKEIKE_EVal = dwClosePrice;
							}
						}
					}
					else {
						if(dwClosePrice > dwOpenPrice) {
							//	とりあえず儲けは出てる
							if(dwClosePrice - dwOpenPrice >= dwGainEVal) {
								//	とりあえず、期待利益を超えた。
								//	行けるところまで頑張ってみる
								//(*itr).bIKEIKE_Mode = TRUE;
								(*itr).dwIKEIKE_EVal = dwClosePrice;
								//dwRet = 1;
							}
						}
						else {
							//	現状は損してる状態
							if(dwOpenPrice - dwClosePrice >= dwLossCutEVal) {
								//	損がロスカット値を超過したら取引を決算する
								dwRet = 1;
							}else if(GetPassedTradeTick( dwPair, (*itr).dwTicket ) >= m_Setting[dwPair].dwForceStopTickCnt ){
								dwRet = 1;								
							}
						}
					}
					//	TradeCheckして売り状態だったら、一度決算する
					if( dwRet == 0 ){
						if( (*itr).dwBreakTrigger & (BREAKSIGN_BY_BBBREAK|BREAKSIGN_BY_BBBREAK_BW|BREAKSIGN_BY_BBBREAK_BWBACK) ){
							;
						}else{
							DWORD  dwBreakSign = 0;
							if( TradeCheck_Main(dwPair, &dwBreakSign) == 2 ){
								dwRet = 1;
							}
						}
					}
					////	最大値を更新する
					//if((*itr).dwMaxEval<dwCurRate) {
					//	(*itr).dwMaxEval = dwCurRate;
					//}else if( (*itr).dwMaxEval - dwClosePrice > dwLossCutEVal ){
					//	dwRet = 1;
					//}

					if( (*itr).dwBreakTrigger & BREAKSIGN_BY_EMABREAK ){
						if(CloseCheck_EMABreak( *itr ) != FALSE ){
							dwRet = 1;
						}
					}

					if( (*itr).dwBreakTrigger & BREAKSIGN_BY_BBBREAK_BWBACK ){
						dwRet = 0;
						if(CloseCheck_BBBreak_BWBack( *itr ) != FALSE ){
							dwRet = 1;
						}
					}

					if( (*itr).dwBreakTrigger & BREAKSIGN_BY_BBBREAK_BW || (*itr).dwBreakTrigger & BREAKSIGN_BY_3SIGMABREAK ){
						dwRet = 0;
						if(CloseCheck_BBBreak_BW( *itr ) != FALSE ){
							dwRet = 1;
						}
					}

					if( (*itr).dwBreakTrigger & BREAKSIGN_BY_SOBREAK ){
						dwRet = 0;
						if(CloseCheck_SOBreak( *itr ) != FALSE ){
							dwRet = 1;
						}
					}

					if( (*itr).dwBreakTrigger & BREAKSIGN_BY_RSIBREAK ){
						dwRet = 0;
						if(CloseCheck_RSIBreak( *itr ) != FALSE ){
							dwRet = 1;
						}
					}


					if( (*itr).dwBreakTrigger & BREAKSIGN_BY_RANGEBREAK ){
						dwRet = 0;
						if(CloseCheck_RangeBreak( *itr ) != FALSE ){
							dwRet = 1;
						}
					}

					
					if( (*itr).dwBreakTrigger & BREAKSIGN_BY_ICHIMOKUBREAK ){
						DWORD dwBreakSign;
						dwRet = 0;
						if( TradeCheck_Main(dwPair, &dwBreakSign) == 2 ){
							dwRet = 1;
						}else if(CloseCheck_IchimokuBreak( *itr ) != FALSE ){
							dwRet = 1;
						}
					}
				}
				//	売り注文->買い決算
				else {
					if( (*itr).dwPeakRate > dwCurRate )	(*itr).dwPeakRate = dwCurRate;

					dwClosePrice = dwCurRate + dwSpread / 2;
					//dwOpenPrice・・・売ったときの値段
					//dwClosePrice・・・今売った場合の値段
					if((*itr).bIKEIKE_Mode != FALSE) {
						if((*itr).dwIKEIKE_EVal + dwIKEIKEModeEndEVal <= dwClosePrice) {
							dwRet = 1;
						}
						else {
							if((*itr).dwIKEIKE_EVal > dwClosePrice) {
								(*itr).dwIKEIKE_EVal = dwClosePrice;
							}
						}
					}
					else {
						if(dwClosePrice < dwOpenPrice) {
							//	とりあえず儲けは出てる
							if(dwOpenPrice - dwClosePrice >= dwGainEVal) {
								//	とりあえず、期待利益を超えた。
								//	行けるところまで頑張ってみる
								//(*itr).bIKEIKE_Mode = TRUE;
								(*itr).dwIKEIKE_EVal = dwClosePrice;
								//dwRet = 1;
							}
						}
						else {
							//	現状は損してる状態
							if(dwClosePrice - dwOpenPrice >= dwLossCutEVal) {
								//	損がロスカット値を超過したら取引を決算する
								dwRet = 1;
							}
							else if(GetPassedTradeTick(dwPair, (*itr).dwTicket) >= m_Setting[dwPair].dwForceStopTickCnt){
								dwRet = 1;
							}
						}
					}
					//	TradeCheckして買い状態だったら、一度決算する
					if(dwRet == 0) {
						if( (*itr).dwBreakTrigger & (BREAKSIGN_BY_BBBREAK|BREAKSIGN_BY_BBBREAK_BW|BREAKSIGN_BY_BBBREAK_BWBACK) ){
							;
						}else{
							DWORD  dwBreakSign = 0;
							if(TradeCheck_Main(dwPair, &dwBreakSign) == 1)
							{
								dwRet = 1;
							}
						}
					}
					////	最大値を更新する
					//if((*itr).dwMaxEval>dwCurRate) {
					//	(*itr).dwMaxEval = dwCurRate;
					//}
					//else if( dwClosePrice - (*itr).dwMaxEval  > dwLossCutEVal) {
					//	dwRet = 1;
					//}

					if((*itr).dwBreakTrigger & BREAKSIGN_BY_EMABREAK) {
						if(CloseCheck_EMABreak(*itr) != FALSE) {
							dwRet = 1;
						}
					}

					if( (*itr).dwBreakTrigger & BREAKSIGN_BY_BBBREAK_BWBACK ){
						dwRet = 0;
						if(CloseCheck_BBBreak_BW( *itr ) != FALSE ){
							dwRet = 1;
						}
					}

					if( (*itr).dwBreakTrigger & BREAKSIGN_BY_BBBREAK_BW || (*itr).dwBreakTrigger & BREAKSIGN_BY_3SIGMABREAK ){
						dwRet = 0;
						if(CloseCheck_BBBreak_BW( *itr ) != FALSE ){
							dwRet = 1;
						}
					}

					if( (*itr).dwBreakTrigger & BREAKSIGN_BY_SOBREAK ){
						dwRet = 0;
						if(CloseCheck_SOBreak( *itr ) != FALSE ){
							dwRet = 1;
						}
					}

					if( (*itr).dwBreakTrigger & BREAKSIGN_BY_RSIBREAK ){
						dwRet = 0;
						if(CloseCheck_RSIBreak( *itr ) != FALSE ){
							dwRet = 1;
						}
					}
					if( (*itr).dwBreakTrigger & BREAKSIGN_BY_RANGEBREAK ){
						dwRet = 0;
						if(CloseCheck_RangeBreak( *itr ) != FALSE ){
							dwRet = 1;
						}
					}
					if( (*itr).dwBreakTrigger & BREAKSIGN_BY_ICHIMOKUBREAK ){
						DWORD dwBreakSign;
						dwRet = 0;
						if( TradeCheck_Main(dwPair, &dwBreakSign) == 1 ){
							dwRet = 1;
						}else if(CloseCheck_IchimokuBreak( *itr ) != FALSE ){
							dwRet = 1;
						}
					}
				}
				break;
			}
		}
	}

	return dwRet;
}
void CAgentWorkerDlg::MT4_CheckCloseTrade(DWORD dwMapId)
{
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];


	PMT4_CHECKCLOSETRADE_DATA pMapData;

	wsprintf(cMapName, "MT4_CheckCloseTrade_%d", dwMapId);

	if ((hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cMapName)) != INVALID_HANDLE_VALUE) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_CHECKCLOSETRADE_DATA)pMap;

			pMapData->dwRet = CheckCloseTrade_Main( pMapData->dwPair, pMapData->dwTicket );

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	return;
}

DWORD CAgentWorkerDlg::RegisterCloseTrade_Main( DWORD dwPair, DWORD dwTicket, double dClosePrice )
{
	DWORD dwRet = 0;
	SYSTEMTIME stCurTime;

	//GetLocalTime(&stCurTime);
	if( m_bTestMode != FALSE ){
		memcpy( &stCurTime, &m_TestCurTime, sizeof(SYSTEMTIME) );
	}else{
		GetLocalTime(&stCurTime);
	}
	vector<TRADE_INFO>::iterator itr = m_tradeInfo.begin();

	for(; itr != m_tradeInfo.end(); itr++) {
		if((*itr).dwTicket == dwTicket) {
			(*itr).dClosePrice = dClosePrice;
			if( m_bTestMode != FALSE ){
				memcpy( &(*itr).closeDt, &m_TestCurTime, sizeof(SYSTEMTIME) );
			}else{
				memcpy(&(*itr).closeDt, &stCurTime, sizeof(SYSTEMTIME));
			}

			memcpy( &LastTradeInfo[(*itr).dwPair], &(*itr), sizeof(TRADE_INFO) );

			WriteTradeInfo(&(*itr), FALSE);

			OutputTradeInfo();
			//	現在のTick情報にトレード開始を記録する
			DWORD dwCurIdx = m_TickInfo[dwPair][TKI_70TICK].size() - 1;
			m_TickInfo[dwPair][TKI_70TICK][dwCurIdx].dwEndTicket = dwTicket;

			if(m_bTestMode == FALSE) {
				char cSendBuf[245];
				char cRslt[32];
				DWORD PipsA, PipsB;

				PipsA = EVal2Pips(Rate2EVal((*itr).dOpenPrice, (*itr).dwPair));
				PipsB = EVal2Pips(Rate2EVal((*itr).dClosePrice, (*itr).dwPair));

				if((*itr).dwOrderType == 1) {
					if(PipsA <= PipsB) {
						_stprintf_s(cRslt, "+%d pips", PipsB - PipsA);
					}
					else
					{
						_stprintf_s(cRslt, "- %d pips ", PipsA - PipsB);
					}
				}
				else {
					if(PipsA <= PipsB) {
						_stprintf_s(cRslt, "- %d pips", PipsB - PipsA);
					}
					else
					{
						_stprintf_s(cRslt, "+%d pips ", PipsA - PipsB);
					}
				}

				wsprintf(cSendBuf, "%s:%s", GetPairName(dwPair), cRslt);

				SendPushBullet_Finish(cSendBuf);
			}

			dwRet = 1;

			break;
		}
	}

	return dwRet;
}

void CAgentWorkerDlg::MT4_RegisterCloseTrade(DWORD dwMapId)
{
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];
	
	PMT4_REGISTERCLOSETRADE_DATA pMapData;

	wsprintf(cMapName, "MT4_RegisterCloseTrade_%d", dwMapId);


	if ((hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cMapName)) != INVALID_HANDLE_VALUE) {
		if ((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_REGISTERCLOSETRADE_DATA)pMap;

			pMapData->dwRet = RegisterCloseTrade_Main( pMapData->dwPair, pMapData->dwTicket, pMapData->dClosePrice );			

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	return;
}

DWORD CAgentWorkerDlg::FinishProcess_Main( DWORD dwPair )
{
	DWORD dwRet = 0;

	if( m_bTestMode != FALSE ){
		DWORD i = 0;
		DWORD dwTradeCnt = 0;
		DWORD dwObtainCnt = 0;
		DWORD dwLossCnt = 0;
		DWORD dwObtainPips = 0;
		DWORD dwLossPips = 0;

		//	結果をファイルに出力する
		for( ; i < m_tradeInfo.size(); i++ ){
			if(m_tradeInfo[i].dClosePrice > 0 ){
				DWORD PipsA, PipsB;

				//PipsA = EVal2Pips(Rate2EVal(m_tradeInfo[i].dOpenPrice, m_tradeInfo[i].dwPair));
				//PipsB = EVal2Pips(Rate2EVal(m_tradeInfo[i].dClosePrice, m_tradeInfo[i].dwPair));
				PipsA = Rate2EVal(m_tradeInfo[i].dOpenPrice, m_tradeInfo[i].dwPair);
				PipsB = Rate2EVal(m_tradeInfo[i].dClosePrice, m_tradeInfo[i].dwPair);

				if(m_tradeInfo[i].dwOrderType == 1) {
					if(PipsA <= PipsB) {
						dwObtainPips += PipsB - PipsA;
						dwObtainCnt++;
					}else{
						dwLossPips += PipsA - PipsB;
						dwLossCnt++;
					}
				}
				else {
					if(PipsA <= PipsB) {
						dwLossPips += PipsB - PipsA;
						dwLossCnt++;
					}else {
						dwObtainPips += PipsA - PipsB;
						dwObtainCnt++;
					}
				}
				dwTradeCnt++;
			}
		}

		//通貨ペア,設定組み合わせ,取引合計,得回数、損回数、取引結果、得合計、損合計
		WriteTestResult( (m_tradeInfo.size()>0)?(m_tradeInfo[0].dwPair):PAIR_UNKNOWN, dwTradeCnt, dwObtainCnt, dwLossCnt, (int)dwObtainPips -(int)dwLossPips, dwObtainPips, dwLossPips );

		for( DWORD i = 0; i < TKI_COUNT; i++ ){
			if( m_hTickFile[i] != INVALID_HANDLE_VALUE ){
				CloseHandle(m_hTickFile[i]);
				m_hTickFile[i] = INVALID_HANDLE_VALUE;
			}
		}
		
		if(m_hTradeFile != INVALID_HANDLE_VALUE) {
			CloseHandle(m_hTradeFile);
			m_hTradeFile = INVALID_HANDLE_VALUE;
		}

		//	取引情報をクリア
		m_tradeInfo.clear();

		dwRet = 1;
	}

	if(dwPair == PAIR_USDJPY) {
		m_ListUJ.DeleteAllItems();
	}
	else if(dwPair == PAIR_EURJPY) {
		m_ListEJ.DeleteAllItems();
	}
	else if(dwPair == PAIR_EURUSD) {
		m_ListEU.DeleteAllItems();
	}
	//m_ListTrade.DeleteAllItems();


	return dwRet;
}

void CAgentWorkerDlg::MT4_FinishProcess(DWORD dwMapId)
{
	HANDLE hMap = INVALID_HANDLE_VALUE;
	PBYTE pMap = NULL;
	char cMapName[256];


	PMT4_FINISHPROCESS_DATA pMapData;

	wsprintf(cMapName, "MT4_FinishProcess_%d", dwMapId);

	if((hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, cMapName)) != INVALID_HANDLE_VALUE) {
		if((pMap = (PBYTE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0)) != NULL) {
			pMapData = (PMT4_FINISHPROCESS_DATA)pMap;

			pMapData->dwRet = FinishProcess_Main( pMapData->dwPair );

			if(m_hCmdOpeThread != NULL)	CloseHandle(m_hCmdOpeThread);

			UnmapViewOfFile(pMap);
		}
		CloseHandle(hMap);
	}

	ExitProcess(1);

	return;
}

//	CsvView側からウィンドウハンドルを受け取り覚える
void CAgentWorkerDlg::SetViewWindowHandle( DWORD dwPair, HWND hWnd )
{
	m_ViewHWnd[dwPair] = hWnd;
}

LRESULT CAgentWorkerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	switch (message) {
		case WM_APP + 1000:
		{
			DWORD dwRet = 1;
			DWORD dwFuncNo = (DWORD)wParam;
			DWORD dwMapId = (DWORD)lParam;

			switch (dwFuncNo) {
				//	初期化
			case MT4_INITILIZE:
				MT4_Initilize(dwMapId);
				break;
			case MT4_REGISTERRATE:
				MT4_RegisterRate(dwMapId);
				break;
			case MT4_DBGREGISTERTICKINFO:
				MT4_DbgRegisterTickInfo(dwMapId);
				break;
			case MT4_TRADECHECK:
				MT4_TradeCheck(dwMapId);
				break;
			case MT4_REGISTERTRADEINFO:
				MT4_RegisterTradeInfo(dwMapId);
				break;
			case MT4_CHECKCLOSETRADE:
				MT4_CheckCloseTrade(dwMapId);
				break;
			case MT4_REGISTERCLOSETRADE:
				MT4_RegisterCloseTrade(dwMapId);
				break;
			case MT4_FINISHPROCESS:
				MT4_FinishProcess(dwMapId);
				break;
			default:
				dwRet = 0;
				break;
			}
			return dwRet;
		}
		case WM_APP + 1001:
		{
			SetViewWindowHandle( (DWORD)wParam,  (HWND)lParam );
			return 0;
		}
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}


HBRUSH CAgentWorkerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_LISTBOX){
		//if (*pWnd == m_xcList)
		//{
		//	// 文字色
		//	pDC->SetTextColor(m_color);
		//	// 背景色
		//	hbr = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		//	pDC->SetBkColor(RGB(0, 0, 0));
		//}
	}


	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


void CAgentWorkerDlg::OnNMCustomdrawListTickuj(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMCD;

	// 描画サイクル開始メッセージをキャッチ
	if (lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT) {
		// カスタムドローを行うことを通知
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;
	}

	// アイテムを描画するメッセージをキャッチ
	if (lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT) {
		// ３行目の場合（任意）
		if (lplvcd->nmcd.dwItemSpec == 3) {
			//// 行の背景を黄色にする
			//lplvcd->clrTextBk = RGB(255, 255, 0);

			//// 行の文字色を赤色にする
			//lplvcd->clrText = RGB(255, 0, 255);
		}
	}

	*pResult = 0;
}


void CAgentWorkerDlg::ReadSettingData(SETTING_PRAMETER* pSetting, DWORD dwPair )
{
	char cIniPath[1024];
	char* pFileName;
	char cReadBuf[256];


	GetModuleFileName(NULL, cIniPath, sizeof(cIniPath));
	pFileName = (char*)_mbsrchr((unsigned char*)cIniPath, (unsigned int)'\\');

	if( this->m_bTestMode == FALSE ){
		if( dwPair == PAIR_UNKNOWN ){
			lstrcpy(pFileName, "\\AgentWorker.ini");
		}else{
			wsprintf( pFileName, "\\%s_AgentWorker.ini", GetPairName(dwPair) );
		}
	}else{
		lstrcpy(pFileName, "\\AgentWorker_Test.ini");
	}

	memset( pSetting, 0x00, sizeof(SETTING_PRAMETER));

	GetPrivateProfileString("GENERAL", "TICKCOUNT", "70", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->dwTickCount = atoi(cReadBuf);

	GetPrivateProfileString("GENERAL", "LOSSCUTPIPS", "2", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->dwLosCutPips = atoi(cReadBuf);

	GetPrivateProfileString("GENERAL", "IKEIKEMODEENDPIPS", "2", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->dwIKEIKEEndPips = atoi(cReadBuf);

	GetPrivateProfileString("GENERAL", "GAINPIPS", "8", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->dwGainPips = atoi(cReadBuf);

	GetPrivateProfileString("GENERAL", "FORCESTOPTICKCNT", "20", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->dwForceStopTickCnt = atoi(cReadBuf);

	GetPrivateProfileString("GENERAL", "TESTNAME", "", pSetting->cTestName, sizeof(pSetting->cTestName), cIniPath);
	
	GetPrivateProfileString("DOUJIBREAK", "ENABLE", "1", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->db.bEnable = (atoi(cReadBuf)==0)?FALSE:TRUE;

	GetPrivateProfileString("DOUJIBREAK", "DB_OPCLRANGE", "20", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->db.dRgOpCl = atof(cReadBuf);

	GetPrivateProfileString("DOUJIBREAK", "DB_HILORANGE", "30", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->db.dRgHiLo = atof(cReadBuf);

	GetPrivateProfileString("DOUJIBREAK", "CONTINUTENUM", "5", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->db.dwContNum = atoi(cReadBuf);

	GetPrivateProfileString("DOUJIBREAK", "SIGNALTRIGGER", "20", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->db.dSigTrig = atof(cReadBuf);


	GetPrivateProfileString("EMABREAK", "ENABLE", "1", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->mab.bEnable = (atoi(cReadBuf) == 0) ? FALSE : TRUE;

	GetPrivateProfileString("EMABREAK", "MAB_EMA1", "13", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->mab.dwMA1Val = atoi(cReadBuf);

	GetPrivateProfileString("EMABREAK", "MAB_EMA2", "21", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->mab.dwMA2Val = atoi(cReadBuf);

	GetPrivateProfileString("EMABREAK", "SIGNALTRIGGER", "3", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->mab.dwSigTrig = atoi(cReadBuf);

	GetPrivateProfileString("BBBREAK", "ENABLE", "1", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->bb.bEnable = (atoi(cReadBuf) == 0) ? FALSE : TRUE;

	GetPrivateProfileString("BRSIBREAK", "ENABLE", "1", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->brsi.bEnable = (atoi(cReadBuf) == 0) ? FALSE : TRUE;

	GetPrivateProfileString("BRSIBREAK", "RSI", "30", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->brsi.dwRSI = atoi(cReadBuf);

	GetPrivateProfileString("BRSIBREAK", "RSISPAN", "14", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->brsi.dwRSISpan = atoi(cReadBuf);

	GetPrivateProfileString("BRSIBREAK", "PROFIT", "60", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->brsi.dwProfit = atoi(cReadBuf);

	GetPrivateProfileString("BRSIBREAK", "LOSSCUT", "20", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->brsi.dwLossCut = atoi(cReadBuf);

	GetPrivateProfileString("SOBREAK", "ENABLE", "1", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->so.bEnable = (atoi(cReadBuf) == 0) ? FALSE : TRUE;

	GetPrivateProfileString("SOBREAK", "KSPAN", "9", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->so.dwKSpan = atoi(cReadBuf);

	GetPrivateProfileString("SOBREAK", "DSPAN", "3", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->so.dwDSpan = atoi(cReadBuf);

	GetPrivateProfileString("SOBREAK", "SLOWDSPAN", "3", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->so.dwSlowDSpan = atoi(cReadBuf);

	GetPrivateProfileString("RSIBREAK", "ENABLE", "1", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->rsi.bEnable = (atoi(cReadBuf) == 0) ? FALSE : TRUE;

	GetPrivateProfileString("RSIBREAK", "SPAN", "21", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->rsi.dwSpan = atoi(cReadBuf);

	GetPrivateProfileString("ICHIMOKUBREAK", "ENABLE", "1", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->ichimoku.bEnable = (atoi(cReadBuf) == 0) ? FALSE : TRUE;

	GetPrivateProfileString("RANGEBREAK", "ENABLE", "1", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->rb.bEnable = (atoi(cReadBuf) == 0) ? FALSE : TRUE;

	GetPrivateProfileString("RANGEBREAK", "SPANTICK", "40", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->rb.dwSpan = atoi(cReadBuf);

	GetPrivateProfileString("RANGEBREAK", "RANGEWIDTH", "80", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->rb.dwWidth = atoi(cReadBuf);

	GetPrivateProfileString("RANGEBREAK", "VAGUEVAL", "20", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->rb.dwVagueVal = atoi(cReadBuf);

	return;
}

void DeleteDirectory( const std::string& dir_path )
{
	WIN32_FIND_DATAA fd;
	std::string ff;
	std::string ss = dir_path + "*";
	HANDLE hFind = FindFirstFileA(ss.c_str(), &fd);

	// 検索失敗
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// フォルダは除く
			if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if(fd.cFileName[0] != '.' ){
					
					ff = dir_path + fd.cFileName + "\\";

					DeleteDirectory( ff );
				}
			}else{
				ff = dir_path + fd.cFileName;

				DeleteFile( ff.c_str() );
			}
		} while(FindNextFileA(hFind, &fd));

		FindClose(hFind);
	}
}

void CAgentWorkerDlg::StartTest_Main( SETTING_PRAMETER*	pParam, vector<ADF_DATA>& RateFileTable, char* pDataPath )
{
	BOOL bTradeNow = FALSE;
	DWORD dwTicket = 0;
	DWORD dwOrderType = 0;
	char* pFileName;
	DWORD dwYear;
	DWORD dwMonth;
	DWORD dwDay;
	DWORD dwPair = PAIR_UNKNOWN;

	char cTemp[1024];
	lstrcpy( cTemp, pDataPath );

	pFileName = (char*)_mbsrchr((unsigned char*)cTemp, (unsigned int)'.');
	*pFileName = 0x00;
	pFileName = (char*)_mbsrchr((unsigned char*)cTemp, (unsigned int)'\\');
	dwDay = atoi(pFileName + 1);
	*pFileName = 0x00;	pFileName--;
	pFileName = (char*)_mbsrchr((unsigned char*)cTemp, (unsigned int)'\\');
	dwMonth = atoi(pFileName + 1);
	*pFileName = 0x00;	pFileName--;
	pFileName = (char*)_mbsrchr((unsigned char*)cTemp, (unsigned int)'\\');
	dwYear = atoi(pFileName + 1);

	//	データファイルの読み込み
	dwPair = GetPairIdFromStrI(pDataPath);

	//	設定情報をiniに書き込む
	WriteSettingData(pParam);

	//	データの初期化
	Initilize_Main(dwPair);

	vector<ADF_DATA>::iterator itr;

	for(itr = RateFileTable.begin(); itr != RateFileTable.end(); itr++) {
		//vector<string>& str_vec = (*itr);

		if(RateFileTable.size() >= 2) {
			double dRate = (*itr).dRate;
			DWORD dwTicket;

			if(dRate > 0) {	//	原因は不明だけどadfに0が書かれていることがあるため
							//	データファイル内のAsk情報を登録する
				memcpy(&m_TestCurTime, &(*itr).time, sizeof(SYSTEMTIME));

				if(RegisterRate_Main(dwPair, Rate2EVal(dRate, dwPair), &m_TestCurTime ) == 0)	continue;

				if(bTradeNow != FALSE) {
					if(CheckCloseTrade_Main(dwPair, dwTicket) != FALSE) {
						double dClosePrice;

						//	買い注文->決算
						if(dwOrderType == 1) {
							dClosePrice = dRate - (GetSpread(dwPair) / 2);
						}
						//	売り注文->決算
						else if(dwOrderType == 2) {
							dClosePrice = dRate + (GetSpread(dwPair) / 2);
						}
						//	0.1pipsを四捨五入する
						//dClosePrice = PriceFractionCut(dClosePrice, dwPair);


						RegisterCloseTrade_Main(dwPair, dwTicket, dClosePrice);

						bTradeNow = FALSE;
						dwTicket = 0;
						dwOrderType = 0;
					}
				}

				if(bTradeNow == FALSE) {
					DWORD  dwBreakSign = 0;
					double dTradePrice = 0;
					int nTradeChk = TradeCheck_Main(dwPair, &dwBreakSign);

					if(nTradeChk > 0) {
						//	買い注文
						if(nTradeChk == 1) {
							dTradePrice = dRate + (GetSpread(dwPair) / 2);
						}
						//	売り注文
						else if(nTradeChk == 2) {
							dTradePrice = dRate - (GetSpread(dwPair) / 2);
						}
						//	0.1pipsを四捨五入する
						//dTradePrice = PriceFractionCut(dTradePrice, dwPair);

						LARGE_INTEGER li;
						QueryPerformanceCounter(&li);
						//dwTicket = MakeMapId(Rate2EVal(dRate, dwPair), li.LowPart);
						dwTicket = li.LowPart;

						//	トレードした体で開始情報を登録する
						RegisterTradeInfo_Main(dwPair, dwTicket, nTradeChk, dTradePrice, 0, dwBreakSign);

						bTradeNow = TRUE;
						//dwTicket = dwTicket;
						dwOrderType = nTradeChk;
					}
				}
			}
		}
	}

	FinishProcess_Main(dwPair);
}

map<DWORD,vector<string>> vsDataFilePath;
vector<DWORD> vdwGainPips;
vector<DWORD> vdwLosCutPips;
vector<DWORD> vdwIKEIKEEndPips;
vector<DWORD> vdwForceStopTickCnt;

vector<double> vdRgOpCl;
vector<double> vdRgHiLo;
vector<DWORD> vdwContNum;
vector<double> vdSigTrig;
vector<DWORD> vdwEma1;
vector<DWORD> vdwEma2;
vector<DWORD> vdwSigTrig;

vector<DWORD> vdwbrsiRSI;
vector<DWORD> vdwbrsiRSISpan;
vector<DWORD> vdwbrsiProfit;
vector<DWORD> vdwbrsiLossCut;

vector<DWORD> vdwsoKSpan;
vector<DWORD> vdwsoDSpan;
vector<DWORD> vdwsoSlowDSpan;

vector<DWORD> vdwRSISpan;

vector<DWORD> vdwRBVagueVal;
vector<DWORD> vdwRBRangeWidth;
vector<DWORD> vdwRBSpanTick;

void SetAutoSetting_General(SETTING_PRAMETER*	pParam )
{
	char cReadBuf[256];
	DWORD i;
	char cAutoIniPath[1024];
	char* pFileName;

	GetModuleFileName(NULL, cAutoIniPath, sizeof(cAutoIniPath));
	pFileName = (char*)_mbsrchr((unsigned char*)cAutoIniPath, (unsigned int)'\\');

	lstrcpy(pFileName, "\\AutoSetting.ini");

	pParam->dwTickCount = 70;

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("TAKEPROFIT", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwGainPips.push_back((DWORD)nSet);
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("STOPLOSS", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwLosCutPips.push_back((DWORD)nSet);
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("IKEIKEEND", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwIKEIKEEndPips.push_back((DWORD)nSet);
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("FORCESTOPTICKCNT", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwForceStopTickCnt.push_back((DWORD)nSet);
	}
}

void SetAutoSetting_DB( SETTING_PRAMETER*	pParam )
{
	char cReadBuf[256];
	DWORD i;
	char cAutoIniPath[1024];
	char* pFileName;

	GetModuleFileName(NULL, cAutoIniPath, sizeof(cAutoIniPath));
	pFileName = (char*)_mbsrchr((unsigned char*)cAutoIniPath, (unsigned int)'\\');

	lstrcpy(pFileName, "\\AutoSetting.ini");


	if(GetPrivateProfileInt("DOUJIBREAK", "ENABLE", 1, cAutoIniPath) == 0) {
		pParam->db.bEnable = FALSE;
	}
	else {
		pParam->db.bEnable = TRUE;
	}

	for(i = 1;;i++) {
		char cNo[32];
		double nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("DB1", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atof(cReadBuf);
		if(nSet == -1)	break;
		vdRgOpCl.push_back((DWORD)nSet);
	}

	for(i = 1;;i++) {
		char cNo[32];
		double nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("DB2", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atof(cReadBuf);
		if(nSet == -1)	break;
		vdRgHiLo.push_back((DWORD)nSet);
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("DB3", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwContNum.push_back((DWORD)nSet);
	}

	for(i = 1;;i++) {
		char cNo[32];
		double nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("DB4", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atof(cReadBuf);
		if(nSet == -1)	break;
		vdSigTrig.push_back((DWORD)nSet);
	}
}

void SetAutoSetting_EMA( SETTING_PRAMETER*	pParam )
{
	char cReadBuf[256];
	DWORD i;
	char cAutoIniPath[1024];
	char* pFileName;

	GetModuleFileName(NULL, cAutoIniPath, sizeof(cAutoIniPath));
	pFileName = (char*)_mbsrchr((unsigned char*)cAutoIniPath, (unsigned int)'\\');

	lstrcpy(pFileName, "\\AutoSetting.ini");

	if(GetPrivateProfileInt("EMABREAK", "ENABLE", 1, cAutoIniPath) == 0) {
		pParam->mab.bEnable = FALSE;
	}
	else {
		pParam->mab.bEnable = TRUE;
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("MA1", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwEma1.push_back((DWORD)nSet);
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("MA2", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwEma2.push_back((DWORD)nSet);
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("MA3", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwSigTrig.push_back((DWORD)nSet);
	}
}

void SetAutoSetting_BRSI( SETTING_PRAMETER*	pParam )
{
	char cReadBuf[256];
	DWORD i;
	char cAutoIniPath[1024];
	char* pFileName;

	GetModuleFileName(NULL, cAutoIniPath, sizeof(cAutoIniPath));
	pFileName = (char*)_mbsrchr((unsigned char*)cAutoIniPath, (unsigned int)'\\');

	lstrcpy(pFileName, "\\AutoSetting.ini");

	//	ボリジャー+RSI
	if(GetPrivateProfileInt("BRSIBREAK", "ENABLE", 1, cAutoIniPath) == 0) {
		pParam->brsi.bEnable = FALSE;
	}
	else {
		pParam->brsi.bEnable = TRUE;
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("BRSI_RSI", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwbrsiRSI.push_back((DWORD)nSet);
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("BRSI_RSISPAN", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwbrsiRSISpan.push_back((DWORD)nSet);
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("BRSI_PROFIT", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwbrsiProfit.push_back((DWORD)nSet);
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("BRSI_LOSSCUT", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwbrsiLossCut.push_back((DWORD)nSet);
	}
}

//vector<DWORD> vdwsoKSpan;
//vector<DWORD> vdwsoDSpan;
//vector<DWORD> vdwsoSlowDSpan;
void SetAutoSetting_SO( SETTING_PRAMETER*	pParam )
{
	char cReadBuf[256];
	DWORD i;
	char cAutoIniPath[1024];
	char* pFileName;

	GetModuleFileName(NULL, cAutoIniPath, sizeof(cAutoIniPath));
	pFileName = (char*)_mbsrchr((unsigned char*)cAutoIniPath, (unsigned int)'\\');

	lstrcpy(pFileName, "\\AutoSetting.ini");

	//	ボリジャー+RSI
	if(GetPrivateProfileInt("SOBREAK", "ENABLE", 1, cAutoIniPath) == 0) {
		pParam->so.bEnable = FALSE;
	}
	else {
		pParam->so.bEnable = TRUE;
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("SO_KSPAN", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwsoKSpan.push_back((DWORD)nSet);
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("SO_DSPAN", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwsoDSpan.push_back((DWORD)nSet);
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("SO_SLOWDSPAN", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwsoSlowDSpan.push_back((DWORD)nSet);
	}
}

void SetAutoSetting_RSI( SETTING_PRAMETER*	pParam )
{
	char cReadBuf[256];
	DWORD i;
	char cAutoIniPath[1024];
	char* pFileName;

	GetModuleFileName(NULL, cAutoIniPath, sizeof(cAutoIniPath));
	pFileName = (char*)_mbsrchr((unsigned char*)cAutoIniPath, (unsigned int)'\\');

	lstrcpy(pFileName, "\\AutoSetting.ini");

	if(GetPrivateProfileInt("RSIBREAK", "ENABLE", 1, cAutoIniPath) == 0) {
		pParam->rsi.bEnable = FALSE;
	}
	else {
		pParam->rsi.bEnable = TRUE;
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("RSI_SPAN", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwRSISpan.push_back((DWORD)nSet);
	}
}
void SetAutoSetting_ICHIMOKU( SETTING_PRAMETER*	pParam )
{
//	char cReadBuf[256];
//	DWORD i;
	char cAutoIniPath[1024];
	char* pFileName;

	GetModuleFileName(NULL, cAutoIniPath, sizeof(cAutoIniPath));
	pFileName = (char*)_mbsrchr((unsigned char*)cAutoIniPath, (unsigned int)'\\');

	lstrcpy(pFileName, "\\AutoSetting.ini");

	if(GetPrivateProfileInt("ICHIMOKUBREAK", "ENABLE", 1, cAutoIniPath) == 0) {
		pParam->ichimoku.bEnable = FALSE;
	}
	else {
		pParam->ichimoku.bEnable = TRUE;
	}
}
void SetAutoSetting_RB( SETTING_PRAMETER*	pParam )
{
	char cReadBuf[256];
	DWORD i;
	char cAutoIniPath[1024];
	char* pFileName;

	GetModuleFileName(NULL, cAutoIniPath, sizeof(cAutoIniPath));
	pFileName = (char*)_mbsrchr((unsigned char*)cAutoIniPath, (unsigned int)'\\');

	lstrcpy(pFileName, "\\AutoSetting.ini");

	if(GetPrivateProfileInt("RANGEBREAK", "ENABLE", 1, cAutoIniPath) == 0) {
		pParam->rb.bEnable = FALSE;
	}
	else {
		pParam->rb.bEnable = TRUE;
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("RB_SPANTICK", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwRBSpanTick.push_back((DWORD)nSet);
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("RB_RANGEWIDTH", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwRBRangeWidth.push_back((DWORD)nSet);
	}

	for(i = 1;;i++) {
		char cNo[32];
		int nSet;
		wsprintf(cNo, "%d", i);
		GetPrivateProfileString("RB_VAGUEVAL", cNo, "-1", cReadBuf, sizeof(cReadBuf), cAutoIniPath);
		nSet = atoi(cReadBuf);
		if(nSet == -1)	break;
		vdwRBVagueVal.push_back((DWORD)nSet);
	}
}

void CAgentWorkerDlg::StartTest()
{
	SETTING_PRAMETER	Param;

	vsDataFilePath.clear();
	vdwGainPips.clear();
	vdwLosCutPips.clear();
	vdwIKEIKEEndPips.clear();
	vdwForceStopTickCnt.clear();

	vdRgOpCl.clear();
	vdRgHiLo.clear();
	vdwContNum.clear();
	vdSigTrig.clear();
	vdwEma1.clear();
	vdwEma2.clear();
	vdwSigTrig.clear();

	vdwbrsiRSI.clear();
	vdwbrsiRSISpan.clear();
	vdwbrsiProfit.clear();
	vdwbrsiLossCut.clear();


	char cDataPath[1024];
	char cIniPath[1024];
	char* pFileName;

	GetModuleFileName(NULL, cIniPath, sizeof(cIniPath));
	pFileName = (char*)_mbsrchr((unsigned char*)cIniPath, (unsigned int)'\\');

	lstrcpy(pFileName, "\\AgentWorker_Test.ini");

	if(m_bTaskMode == FALSE) {
		char cTempDir[1024];

		GetTempPath( sizeof(cTempDir), cTempDir );

		lstrcat( cTempDir, "AWTemp\\" );

		std::string stTempDir(cTempDir);
		//	一時フォルダを削除する
		//DeleteDirectory( stTempDir );

		for( DWORD f = 1; ; f++ ){
			char cKey[32];
			wsprintf( cKey, "%d", f );
			GetPrivateProfileString("DATAFILE", cKey, "", cDataPath, sizeof(cDataPath), cIniPath);

			if( cDataPath[0] == 0x00 )	break;

			if( GetFileAttributes( cDataPath ) & FILE_ATTRIBUTE_DIRECTORY ){
				WIN32_FIND_DATAA fd;
				char cFindPath[1024];

				if( cDataPath[lstrlen(cDataPath)-1] != '\\' ){
					wsprintf( cFindPath, "%s\\*.adf", cDataPath );
				}else{
					wsprintf( cFindPath, "%s*.adf", cDataPath );
				}

				HANDLE hFind = FindFirstFileA( cFindPath, &fd);

				do
				{
					// フォルダは除く
					if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
						;;
					}
					else {
						if( cDataPath[lstrlen(cDataPath)-1] != '\\' ){
							wsprintf( cFindPath, "%s\\%s", cDataPath, fd.cFileName );
						}else{
							wsprintf( cFindPath, "%s%s", cDataPath, fd.cFileName );
						}
						string stDataFile = cFindPath;
						DWORD dwPairId = GetPairIdFromStrI(cFindPath);
						
						if( vsDataFilePath.find(dwPairId ) == vsDataFilePath.end() ){
							vector<string> vDataPath;

							vsDataFilePath[dwPairId] = vDataPath;
						}
						vsDataFilePath[dwPairId].push_back(stDataFile);
					}
				} while(FindNextFileA(hFind, &fd));

				FindClose(hFind);
			}else{
				string stDataFile = cDataPath;
				DWORD dwPairId = GetPairIdFromStrI(cDataPath);
						
				if( vsDataFilePath.find(dwPairId ) == vsDataFilePath.end() ){
					vector<string> vDataPath;

					vsDataFilePath[dwPairId] = vDataPath;
				}
				vsDataFilePath[dwPairId].push_back(cDataPath);
				//vsDataFilePath.push_back(stDataFile);
			}
		}
	}
	else {
		SYSTEMTIME CurTime;
		string stDataFile;
		vector<string> vDataPath;

		GetLocalTime(&CurTime);
		wsprintf(cDataPath, "%s\\%s_%02d%02d%02d.adf", _LOG_DIR_, GetPairName(PAIR_USDJPY), CurTime.wYear, CurTime.wMonth, CurTime.wDay - 1);
		stDataFile = cDataPath;

		vsDataFilePath[PAIR_USDJPY] = vDataPath;
		vsDataFilePath[PAIR_USDJPY].push_back(stDataFile);

		wsprintf(cDataPath, "%s\\%s_%02d%02d%02d.adf", _LOG_DIR_, GetPairName(PAIR_EURJPY), CurTime.wYear, CurTime.wMonth, CurTime.wDay - 1);
		stDataFile = cDataPath;

		vsDataFilePath[PAIR_EURJPY] = vDataPath;
		vsDataFilePath[PAIR_EURJPY].push_back(stDataFile);

		wsprintf(cDataPath, "%s\\%s_%02d%02d%02d.adf", _LOG_DIR_, GetPairName(PAIR_EURUSD), CurTime.wYear, CurTime.wMonth, CurTime.wDay - 1);
		stDataFile = cDataPath;

		vsDataFilePath[PAIR_EURUSD] = vDataPath;
		vsDataFilePath[PAIR_EURUSD].push_back(stDataFile);
	}

	ReadSettingData(&Param,PAIR_UNKNOWN );

	if(m_bAutoMode != FALSE) {
		SetAutoSetting_General( &Param );

		SetAutoSetting_DB( &Param );

		SetAutoSetting_EMA( &Param );
		
		SetAutoSetting_BRSI( &Param );

		SetAutoSetting_SO( &Param );

		SetAutoSetting_RSI( &Param );

		SetAutoSetting_ICHIMOKU( &Param );

		SetAutoSetting_RB( &Param );

		if(m_bTaskMode != FALSE) {
			SYSTEMTIME	CTime;
			GetLocalTime(&CTime);
			//	テスト名を書き込む
			wsprintf(Param.cTestName, "%d%02d%02d_AnalyzeData", CTime.wYear, CTime.wMonth, CTime.wDay );
			WritePrivateProfileString("GENERAL", "TESTNAME", Param.cTestName, cIniPath);

		}

	}
	else {
		vdwGainPips.push_back((DWORD)Param.dwGainPips);
		vdwLosCutPips.push_back((DWORD)Param.dwLosCutPips);
		vdwIKEIKEEndPips.push_back((DWORD)Param.dwIKEIKEEndPips);
		vdwForceStopTickCnt.push_back((DWORD)Param.dwForceStopTickCnt);
		vdRgOpCl.push_back((DWORD)Param.db.dRgOpCl);
		vdRgHiLo.push_back((DWORD)Param.db.dRgHiLo);
		vdwContNum.push_back((DWORD)Param.db.dwContNum);
		vdSigTrig.push_back((DWORD)Param.db.dSigTrig);
		vdwEma1.push_back((DWORD)Param.mab.dwMA1Val);
		vdwEma2.push_back((DWORD)Param.mab.dwMA2Val);
		vdwSigTrig.push_back((DWORD)Param.mab.dwSigTrig);

		vdwbrsiRSI.push_back((DWORD)Param.brsi.dwRSI);
		vdwbrsiRSISpan.push_back((DWORD)Param.brsi.dwRSISpan);
		vdwbrsiProfit.push_back((DWORD)Param.brsi.dwProfit);
		vdwbrsiLossCut.push_back((DWORD)Param.brsi.dwLossCut);

		vdwsoKSpan.push_back((DWORD)Param.so.dwKSpan);
		vdwsoDSpan.push_back((DWORD)Param.so.dwDSpan);
		vdwsoSlowDSpan.push_back((DWORD)Param.so.dwSlowDSpan);

		vdwRSISpan.push_back((DWORD)Param.rsi.dwSpan);


		vdwRBVagueVal.push_back((DWORD)Param.rb.dwVagueVal);
		vdwRBRangeWidth.push_back((DWORD)Param.rb.dwWidth);
		vdwRBSpanTick.push_back((DWORD)Param.rb.dwSpan);


	}

	vector<ADF_DATA> RateFileTable;


	//map<DWORD,vector<string>>::iterator itr_mappath = vsDataFilePath.begin();

    for(auto itr = vsDataFilePath.begin(); itr != vsDataFilePath.end(); ++itr) {
        //std::cout << "key = " << itr->first           // キーを表示
        //                << ", val = " << itr->second << "\n";    // 値を表示

		//vector<string>::iterator itr_vsDataFilePath = vsDataFilePath.begin();

		vector<string>::iterator itr_vsDataFilePath = itr->second.begin();
		for(;itr_vsDataFilePath != itr->second.end(); itr_vsDataFilePath++) {
			lstrcpy(cDataPath, (*itr_vsDataFilePath).c_str());

			ReadADFFile(cDataPath, RateFileTable);
		}		

		if( RateFileTable.size() > 0 ){
			BOOL bWriteFirst = TRUE;
			if(bWriteFirst != FALSE) {
				m_bAnaTKIWrite = TRUE;
				bWriteFirst = FALSE;
			}

			
			vector<DWORD>::iterator itr_vdwGainPips = vdwGainPips.begin();
			for(;itr_vdwGainPips != vdwGainPips.end(); itr_vdwGainPips++) {
				Param.dwGainPips = *itr_vdwGainPips;
				vector<DWORD>::iterator itr_vdwLosCutPips = vdwLosCutPips.begin();
				for(;itr_vdwLosCutPips != vdwLosCutPips.end(); itr_vdwLosCutPips++) {
					Param.dwLosCutPips = *itr_vdwLosCutPips;
					vector<DWORD>::iterator itr_vdwIKEIKEEndPips = vdwIKEIKEEndPips.begin();
					for(;itr_vdwIKEIKEEndPips != vdwIKEIKEEndPips.end(); itr_vdwIKEIKEEndPips++) {
						Param.dwIKEIKEEndPips = *itr_vdwIKEIKEEndPips;
						vector<DWORD>::iterator itr_vdwForceStopTickCnt = vdwForceStopTickCnt.begin();
						for(;itr_vdwForceStopTickCnt != vdwForceStopTickCnt.end(); itr_vdwForceStopTickCnt++) {
							Param.dwForceStopTickCnt = *itr_vdwForceStopTickCnt;
							
							//	ICHIMOKU	////////////////////////////////////////////////////////////
							if(Param.ichimoku.bEnable != FALSE) {
								StartTest_Main(&Param, RateFileTable, cDataPath);
							}

							//	RSI	////////////////////////////////////////////////////////////
							if(Param.rsi.bEnable != FALSE) {
								vector<DWORD>::iterator itr_vdwRSISpan = vdwRSISpan.begin();
								for(;itr_vdwRSISpan != vdwRSISpan.end(); itr_vdwRSISpan++) {
									Param.rsi.dwSpan = *itr_vdwRSISpan;

									StartTest_Main(&Param, RateFileTable, cDataPath);
								}
							}

							//	SO	////////////////////////////////////////////////////////////
							if(Param.so.bEnable != FALSE) {
								vector<DWORD>::iterator itr_vdwsoKSpan = vdwsoKSpan.begin();
								for(;itr_vdwsoKSpan != vdwsoKSpan.end(); itr_vdwsoKSpan++) {
									Param.so.dwKSpan = *itr_vdwsoKSpan;

									vector<DWORD>::iterator itr_vdwsoDSpan = vdwsoDSpan.begin();
									for(;itr_vdwsoDSpan != vdwsoDSpan.end(); itr_vdwsoDSpan++) {
										Param.so.dwDSpan = *itr_vdwsoDSpan;

										if( Param.so.dwKSpan <= Param.so.dwDSpan )	continue;

										vector<DWORD>::iterator itr_vdwsoSlowDSpan = vdwsoSlowDSpan.begin();
										for(;itr_vdwsoSlowDSpan != vdwsoSlowDSpan.end(); itr_vdwsoSlowDSpan++) {
											Param.so.dwSlowDSpan = *itr_vdwsoSlowDSpan;

											if( Param.so.dwDSpan < Param.so.dwSlowDSpan )	continue;

											StartTest_Main(&Param, RateFileTable, cDataPath);
										}
									}
								}
							}

							//	BRSI	////////////////////////////////////////////////////////////
							if(Param.brsi.bEnable != FALSE){
								vector<DWORD>::iterator itr_vdwbrsiRSI = vdwbrsiRSI.begin();
								for(;itr_vdwbrsiRSI != vdwbrsiRSI.end(); itr_vdwbrsiRSI++) {
									Param.brsi.dwRSI = *itr_vdwbrsiRSI;

									vector<DWORD>::iterator itr_vdwbrsiRSISpan = vdwbrsiRSISpan.begin();
									for(;itr_vdwbrsiRSISpan != vdwbrsiRSISpan.end(); itr_vdwbrsiRSISpan++) {
										Param.brsi.dwRSISpan = *itr_vdwbrsiRSISpan;

										vector<DWORD>::iterator itr_vdwbrsiProfit = vdwbrsiProfit.begin();
										for(;itr_vdwbrsiProfit != vdwbrsiProfit.end(); itr_vdwbrsiProfit++) {
											Param.brsi.dwProfit = *itr_vdwbrsiProfit;

											vector<DWORD>::iterator itr_vdwbrsiLossCut = vdwbrsiLossCut.begin();
											for(;itr_vdwbrsiLossCut != vdwbrsiLossCut.end(); itr_vdwbrsiLossCut++) {
												Param.brsi.dwLossCut = *itr_vdwbrsiLossCut;

												StartTest_Main( &Param, RateFileTable, cDataPath );

											}
										}
									}
								}
							}

							//	DB	////////////////////////////////////////////////////////////
							if(Param.db.bEnable != FALSE){
								vector<double>::iterator itr_vdRgOpCl = vdRgOpCl.begin();
								for(;itr_vdRgOpCl != vdRgOpCl.end(); itr_vdRgOpCl++) {
									Param.db.dRgOpCl = *itr_vdRgOpCl;
									vector<double>::iterator itr_vdRgHiLo = vdRgHiLo.begin();
									for(;itr_vdRgHiLo != vdRgHiLo.end(); itr_vdRgHiLo++) {
										Param.db.dRgHiLo = *itr_vdRgHiLo;
										vector<DWORD>::iterator itr_vdwContNum = vdwContNum.begin();
										for(;itr_vdwContNum != vdwContNum.end(); itr_vdwContNum++) {
											Param.db.dwContNum = *itr_vdwContNum;

											vector<double>::iterator itr_vdSigTrig = vdSigTrig.begin();
											for(;itr_vdSigTrig != vdSigTrig.end(); itr_vdSigTrig++) {
												Param.db.dSigTrig = *itr_vdSigTrig;

												StartTest_Main( &Param, RateFileTable, cDataPath );
								
											}
										}
									}
								}
							}

							//	MAB	////////////////////////////////////////////////////////////
							if(Param.mab.bEnable != FALSE) {
								vector<DWORD>::iterator itr_vdwEma1 = vdwEma1.begin();
								for(;itr_vdwEma1 != vdwEma1.end(); itr_vdwEma1++) {
									Param.mab.dwMA1Val = *itr_vdwEma1;

									vector<DWORD>::iterator itr_vdwEma2 = vdwEma2.begin();
									for(;itr_vdwEma2 != vdwEma2.end(); itr_vdwEma2++) {
										Param.mab.dwMA2Val = *itr_vdwEma2;

										vector<DWORD>::iterator itr_vdwSigTrig = vdwSigTrig.begin();
										for(;itr_vdwSigTrig != vdwSigTrig.end(); itr_vdwSigTrig++) {
											Param.mab.dwSigTrig = *itr_vdwSigTrig;

											StartTest_Main(&Param, RateFileTable, cDataPath);
										}
									}
								}
							}



							//	RB	////////////////////////////////////////////////////////////
							if(Param.rb.bEnable != FALSE){
								vector<DWORD>::iterator itr_vdwRBVagueVal = vdwRBVagueVal.begin();
								for(;itr_vdwRBVagueVal != vdwRBVagueVal.end(); itr_vdwRBVagueVal++) {
									Param.rb.dwVagueVal = *itr_vdwRBVagueVal;
									vector<DWORD>::iterator itr_vdwRBRangeWidth = vdwRBRangeWidth.begin();
									for(;itr_vdwRBRangeWidth != vdwRBRangeWidth.end(); itr_vdwRBRangeWidth++) {
										Param.rb.dwWidth = *itr_vdwRBRangeWidth;
										vector<DWORD>::iterator itr_vdwRBSpanTick = vdwRBSpanTick.begin();
										for(;itr_vdwRBSpanTick != vdwRBSpanTick.end(); itr_vdwRBSpanTick++) {
											Param.rb.dwSpan = *itr_vdwRBSpanTick;

											StartTest_Main( &Param, RateFileTable, cDataPath );
										}
									}
								}
							}

							if( Param.db.bEnable == FALSE 
							 && Param.mab.bEnable == FALSE
							 && Param.brsi.bEnable == FALSE
							 && Param.so.bEnable == FALSE
							 && Param.rsi.bEnable == FALSE
							 && Param.ichimoku.bEnable == FALSE
							 && Param.rb.bEnable == FALSE 
							) {
								StartTest_Main(&Param, RateFileTable, cDataPath);
							}
						}
					}
				}
			}
			RateFileTable.clear();

			m_bAnaTKIWrite = FALSE;
		}
	}
}
