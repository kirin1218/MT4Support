
// TickData2ImgDataDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "TickData2ImgData.h"
#include "TickData2ImgDataDlg.h"
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


// CTickData2ImgDataDlg ダイアログ



CTickData2ImgDataDlg::CTickData2ImgDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TICKDATA2IMGDATA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTickData2ImgDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_SRC, m_EdSrc);
}

BEGIN_MESSAGE_MAP(CTickData2ImgDataDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTickData2ImgDataDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTickData2ImgDataDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTickData2ImgDataDlg メッセージ ハンドラー

BOOL CTickData2ImgDataDlg::OnInitDialog()
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

void CTickData2ImgDataDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTickData2ImgDataDlg::OnPaint()
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
HCURSOR CTickData2ImgDataDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LONG MakeTickPathList( std::string &RootPath, std::vector<std::string> &PathList )
{
	std::string FindPath;
	WIN32_FIND_DATAA fd;

	if(*RootPath.rbegin() != '\\') {
		RootPath.push_back('\\');
	}

	FindPath = RootPath;
	FindPath.push_back('*');


	HANDLE hFind = FindFirstFileA( FindPath.c_str(), &fd);

	if( INVALID_HANDLE_VALUE == hFind ){
		return -1;
	}

	do{
		// フォルダは除く
		if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if( fd.cFileName[0] != '.'){
				std::string SearchPath = RootPath + fd.cFileName + "\\";

				MakeTickPathList( SearchPath, PathList );
			}
		}
		else {
			std::string FindPath = RootPath + fd.cFileName;

			PathList.push_back( FindPath );
		}
	} while(FindNextFileA(hFind, &fd));

	FindClose(hFind);

	return (LONG)PathList.size();
}


bool ReadADFFile( string& FilePath, vector<ADF_DATA>& vec_table )
{
	CTime CurTime = CTime::GetCurrentTime();
	CTime time;
	char cADFPath[1024];
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	
	lstrcpy(cADFPath, FilePath.c_str() );

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

		if(GetFileAttributes(FilePath.c_str()) != INVALID_FILE_ATTRIBUTES) {
			HANDLE hFile;

			hFile = CreateFile(FilePath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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

DWORD CTickData2ImgDataDlg::MakeTickDataList( std::vector<std::string>& PathList )
{
	m_TickDataList.clear();

	for(auto itr = PathList.begin(); itr != PathList.end(); ++itr) {
		ReadADFFile( (*itr), m_TickDataList );
	}

	return m_TickDataList.size();
}

DWORD CTickData2ImgDataDlg::Make70TickDataList( DWORD dwPair )
{
	DWORD dwEnd = 0;// = m_TickInfo[dwPair][dwTType].size() - 1;
	DWORD dwTID = 0;
	m_70TickDataList.clear();

	for(auto itr = m_TickDataList.begin(); itr != m_TickDataList.end(); ++itr) 
	{
		double dRate = (*itr).dRate;
		DWORD dwRate =  Rate2EVal(dRate, dwPair);
		SYSTEMTIME RegTime;

		if(dRate > 0) {	//	原因は不明だけどadfに0が書かれていることがあるため
						//	データファイル内のAsk情報を登録する
			memcpy(&RegTime, &(*itr).time, sizeof(SYSTEMTIME));

			if(m_70TickDataList.size() == 0 || m_70TickDataList[dwEnd].bCl != FALSE) {
				TICK_INFO AddData;

				memset(&AddData, 0x00, sizeof(TICK_INFO));

				AddData.Ver = TICK_LATEST_VER;
				AddData.dwTickType = TKI_70TICK;
				AddData.Hi = dwRate;
				AddData.Op = dwRate;
				AddData.Cl = dwRate;
				AddData.Lo = dwRate;
				memcpy(&AddData.sDt, &RegTime, sizeof(SYSTEMTIME));
				AddData.ema = 0;
				AddData.Tc = 1;
				AddData.dwTID = m_70TickDataList.size();
				AddData.dwRateAry[0] = dwRate;

				m_70TickDataList.push_back(AddData);

				dwEnd = m_70TickDataList.size() - 1;
			}
			//	最後のTickデータを更新する
			else {
				//	最高または最低を必要があれば更新
				if( m_70TickDataList[dwEnd].Hi < dwRate ) {
					m_70TickDataList[dwEnd].Hi = dwRate;
				}
				else if( m_70TickDataList[dwEnd].Lo > dwRate ) {
					m_70TickDataList[dwEnd].Lo = dwRate;
				}

				//	最終は常に更新する
				if(m_70TickDataList[dwEnd].Cl != dwRate) {
					m_70TickDataList[dwEnd].Cl = dwRate;
				}
				m_70TickDataList[dwEnd].dwRateAry[m_70TickDataList[dwEnd].Tc] = dwRate;
				m_70TickDataList[dwEnd].Tc++;

				if( m_70TickDataList[dwEnd].Tc >= 70) {
					memcpy(&m_70TickDataList[dwEnd].eDt, &RegTime, sizeof(SYSTEMTIME));

					m_70TickDataList[dwEnd].bCl = TRUE;
				}
			}
		}
	}

	return m_70TickDataList.size();
}

struct{
	DWORD	LableNo;
	int		Profit;
}g_LabelTable[9] =
{
	{	0,	-60 },
	{	1,	-30 },
	{	2,	0 },
	{	3,	30 },
	{	4,	100050 },
};

BOOL Over5Minute( SYSTEMTIME* pPrevTime, SYSTEMTIME* pCurrentTime ){
	CTime PrevTime = CTime(*pPrevTime);
	CTime CurTime = CTime(*pCurrentTime);

	CTime time = PrevTime + CTimeSpan( 0, 0, 5, 0);

	if( time < CurTime ){
		return TRUE;
	}

	return FALSE;
}

int CTickData2ImgDataDlg::GetLabelNo( DWORD dwStart, DWORD dwStartVal )
{	
	int nRet = -1;
	DWORD* pArray;
	int nPeekRate = 0;
	int nUpOrDn = 0;	//	1:Up -1:Down
	int nProfit;
	BOOL bEnd = FALSE;

	for( DWORD i = dwStart; i < m_70TickDataList.size() && bEnd == FALSE; i++ ){
		//	一個前との時間の差が5分以上あったら、データとして連続性がないので、
		//	ブレイクする
		if( i > 0 ){
			if( Over5Minute( &m_70TickDataList[i-1].eDt, &m_70TickDataList[i].sDt ) != FALSE ){
				return -1;
			}
		}

		if( i - dwStart > 5 ){
			if( nPeekRate == 0 ){
				nProfit = 0;
				break;
			}
		}
		pArray = m_70TickDataList[i].dwRateAry;
		for( DWORD j = 0; j < 70; j++ ){
			if( nPeekRate == 0 ){
				if( dwStartVal < pArray[j] ){
					if( pArray[j] - dwStartVal > 20 ){
						nPeekRate = pArray[j];

						nUpOrDn = 1;
					}
				}else if( dwStartVal > pArray[j] ){
					if( dwStartVal - pArray[j] > 20 ){
						nPeekRate = pArray[j];

						nUpOrDn = -1;
					}
				}
			}else{
				//	上昇中
				if( nUpOrDn > 0 ){
					if( pArray[j] > (DWORD)nPeekRate ){
						nPeekRate = pArray[j];
					}else{
						if( nPeekRate - pArray[j] > 20 ){
							nProfit = nPeekRate - dwStartVal;

							if( nProfit < 0 ){
								nProfit = 0;
							}

							bEnd = TRUE;

							break;
						}
					}
				}
				//	下降中
				else{
					if( (DWORD)nPeekRate > pArray[j]  ){
						nPeekRate = pArray[j];
					}else{
						if( pArray[j] - nPeekRate > 20 ){
							nProfit = nPeekRate - (int)dwStartVal;

							if( nProfit > 0 ){
								nProfit = 0;
							}

							bEnd = TRUE;

							break;
						}
					}
				}
			}
		}
	}

	if( bEnd != FALSE ){
		for( DWORD j = 0; j < _countof(g_LabelTable); j++ ){
			if( g_LabelTable[j].Profit > nProfit ){
				nRet = g_LabelTable[j].LableNo;

				break;
			}
		}
	}

	return nRet;
}

void GetCurrentDir( string& strRet )
{
	char cFilePath[1024];

	GetModuleFileName( NULL, cFilePath, sizeof(cFilePath) );
	//string strFile = cFilePath;
	char* pFile = (char*)_mbsrchr( (unsigned char* const)cFilePath, (unsigned int   const)'\\' ); 
	pFile++;
	*pFile = 0x00;

	strRet = cFilePath;


	return;
}

vector<vector<COLORREF>> CTickData2ImgDataDlg::MakeImg( DWORD dwStart, DWORD dwEnd, DWORD dwOverLine, DWORD dwCenter, DWORD dwUnderLine )
{
	BOOL bRet = FALSE;
	vector< vector<COLORREF> > vImg (IMG_SIZE, vector<COLORREF>(IMG_SIZE) );
	
	DWORD dwDrawLine = (dwOverLine%100) * 2;

	DWORD i,j;

	for( j = dwDrawLine; j < IMG_SIZE; j += 200 ){
		for( i = 0; i < IMG_SIZE; i++ ){
			vImg[j][i] = RGB(0xFF,0xFF,0xFF);
		}
	}

	//	dwUnderLine == 0
	//	dwOverLine == 223
	for(DWORD k = dwStart, i = 0; i < IMG_SIZE; i += TICKBAR_WIDTH + TICK_SPACE, k++) {
		DWORD Bar1, Bar2;
		DWORD Bar3, Bar4;
		BOOL bUp;

		if( k > dwStart) {
			if(Over5Minute(&m_70TickDataList[k - 1].eDt, &m_70TickDataList[k].sDt) != FALSE) {
				return vImg;
			}
		}

		if(m_70TickDataList[k].Op > m_70TickDataList[k].Cl) {
			Bar1 = (dwOverLine - (m_70TickDataList[k].Op / 10)) * 2;
			Bar2 = (dwOverLine - (m_70TickDataList[k].Cl / 10)) * 2;
			bUp = FALSE;
		}
		else {
			Bar1 = (dwOverLine - (m_70TickDataList[k].Cl / 10)) * 2;
			Bar2 = (dwOverLine - (m_70TickDataList[k].Op / 10)) * 2;
			bUp = TRUE;
		}
		Bar3 = (dwOverLine - (m_70TickDataList[k].Hi / 10)) * 2;
		Bar4 = (dwOverLine - (m_70TickDataList[k].Lo / 10)) * 2;
		
		for(j = 0; j < IMG_SIZE; j++) {
			if(Bar1 <= j && j <= Bar2) {
				vImg[j][i] = (bUp)?RGB(0xFF,0,0):RGB(0,0,0xFF);
				if( i + 1 < IMG_SIZE ) vImg[j][i + 1] = (bUp)?RGB(0xFF,0,0):RGB(0,0,0xFF);
				if( i + 1 < IMG_SIZE ) vImg[j][i + 2] = (bUp)?RGB(0xFF,0,0):RGB(0,0,0xFF);
			}else if(Bar3 <= j && j <= Bar4) {
				if( i + 1 < IMG_SIZE )	vImg[j][i + 1] = RGB(0xFF,0xFF,0xFF);;
			}
		}
	}

	return vImg;
}

#include "MakeBitmap.h"
#include "Bmp2Jpeg.h"

BOOL WriteImgData( vector< vector<COLORREF> >& vImg, char* pJpegPath )
{
	BOOL bRet = FALSE;

	char cBitmap[1024];
	wsprintf( cBitmap, "%s.bmp", pJpegPath );

	CMakeBitmap MKBmp( 256,256, 24);
	CBmp2Jpeg Bmp2Jpg;

	MKBmp.CreateBitMap( cBitmap, vImg );

	if( Bmp2Jpg.ConvBmp2Jpeg( cBitmap, pJpegPath ) != FALSE ){
		;;
	}

	DeleteFile( cBitmap );

	return bRet;
}

BOOL WriteLableData( char* pJpgPath, DWORD nLabel, const char* pListFile )
{
	BOOL bRet = FALSE;
	HANDLE hFile;
	DWORD dwWriteSize;
	char cLineBuf[1024];

	wsprintf( cLineBuf, "%s,%d\r\n", pJpgPath, nLabel );

	if((hFile = CreateFile( pListFile, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
		SetFilePointer( hFile, 0, 0, FILE_END );

		WriteFile(hFile, cLineBuf, lstrlen(cLineBuf), &dwWriteSize, NULL);

		CloseHandle(hFile);
	}


	return bRet;
}
#include <random>
#define OUT_DATA_SIZE (1000)

void MakeRandIdx( DWORD dwStart, DWORD dwEnd, DWORD dwDataSize, DWORD* pOutArry )
{
	memset( pOutArry, 0xFF, sizeof(DWORD)*dwDataSize );

	std::random_device rnd;     // 非決定的な乱数生成器を生成
    std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
    std::uniform_int_distribution<> rand_data(dwStart, dwEnd);        // [0, 99] 範囲の一様乱数

	for( DWORD i = 0; i <= dwDataSize; i++ ){
		DWORD dwSetVal;
RETRY:
		dwSetVal = rand_data(mt);
		for( DWORD j = 0; pOutArry[j] != 0xFFFFFFFF && j < dwDataSize; j++ ){
			if( pOutArry[j] == dwSetVal ){
				goto RETRY;
			}
		}

		pOutArry[i] = dwSetVal;
	}
}

DWORD CTickData2ImgDataDlg::ConvertTick2Img()
{
	DWORD dwRet = 0;
	DWORD dwDataSize = m_70TickDataList.size();
	string strFilePath;

	GetCurrentDir(strFilePath);

	string strImgData = strFilePath +  "ImgData\\";
	string strLabelData = strImgData +  "ImgList.txt";

	DWORD dwFileNo = 1;

	CreateDirectory( strImgData.c_str(), NULL );

	DWORD dwRandIdx[OUT_DATA_SIZE+1000];

	MakeRandIdx( 0, dwDataSize-1, OUT_DATA_SIZE+1000, dwRandIdx );

	//	70Tickデータを先頭から回す
	//for( DWORD i=0; i < dwDataSize; i++ )
	for( DWORD l=0; l < OUT_DATA_SIZE+1000; l++ )
	{
		DWORD dwIdx = dwRandIdx[l];
		
		//	最低でも画像に使用する分の残りTick数が必要
		//	結果を求めるためには実際ももっと必要
		if( dwIdx + IMG_DATA_SIZE - 1 < dwDataSize ){
			DWORD Hi = 0;
			DWORD Lo = (DWORD)-1;
			DWORD dwCenter;

			DWORD j = dwIdx;
			DWORD dwMax = dwIdx+IMG_DATA_SIZE;
			for( ; j < dwMax; j++ ){
				if( m_70TickDataList[j].Hi > Hi )	Hi = m_70TickDataList[j].Hi;
				if( m_70TickDataList[j].Lo < Lo )	Lo = m_70TickDataList[j].Lo;
			}

			DWORD dwOverLine;
			DWORD dwUnderLine;

			dwCenter = (DWORD)( ((Hi - Lo)/2) + Lo );

			dwUnderLine =  (dwCenter/10) - (((IMG_SIZE+1)/2)/2);	//	== 0
			dwOverLine = dwUnderLine + (((IMG_SIZE+1)/2));	//	== 224/2 == 112	

			//	グラフ
			int nLabel =  GetLabelNo( dwMax, m_70TickDataList[dwMax].Cl );

			if( nLabel != -1 ){
				vector< vector<COLORREF> >& vImg = MakeImg( dwIdx, dwIdx + IMG_DATA_SIZE - 1 , dwOverLine, dwCenter, dwUnderLine );
				if( vImg.size() >= IMG_SIZE ){
					char cFilePath[1024];

					wsprintf( cFilePath, "%s%03d_%d.jpg", strImgData.c_str(), dwFileNo, nLabel );
					WriteImgData( vImg, cFilePath );

					WriteLableData( cFilePath, nLabel, strLabelData.c_str() );

					dwFileNo++;
				}
			}
		}
	}

	return dwRet;
}
void CTickData2ImgDataDlg::OnBnClickedOk()
{
	//	パスの取得
	CString StrCString;
	std::vector<std::string> PathList;

	m_EdSrc.GetWindowText(StrCString);
	std::string str((LPCSTR)StrCString);

	DWORD dwAttr = GetFileAttributes( str.c_str() );

	if( dwAttr == INVALID_FILE_ATTRIBUTES ){
		//	ファイルが存在しない
	}

	if( dwAttr & FILE_ATTRIBUTE_DIRECTORY ){
		if(*str.rbegin() != '\\') {
			str.push_back('\\');
		}

		MakeTickPathList( str, PathList );
	}else{
		PathList.push_back( str );
	}
	
	if( PathList.size() > 0 ){
		if( MakeTickDataList( PathList ) > 0 ){
			DWORD dwPair = GetPairIdFromStrI( (char*)PathList[0].c_str() );

			if( PAIR_UNKNOWN != dwPair ){
				//	TickDataから70Tickデータリストを作成する
				if( Make70TickDataList( dwPair ) > 0 ) {
					DWORD dwCnt = ConvertTick2Img();

					if( dwCnt > 0 ){
						//ImgFileOut();
					}
				}
			}
		}
	}
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	//CDialogEx::OnOK();
}


void CTickData2ImgDataDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}
