
// CsvDataViewer.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "CsvDataViewer.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "CsvDataViewerDoc.h"
#include "CsvDataViewerView.h"
#include "TRFViewerDoc.h"
#include "TRFViewerView.h"
#include "ZeroGraphDoc.h"
#include "ZeroGraphViewer.h"
#include "CompTickChartDoc.h"
#include "CompTickChartView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCsvDataViewerApp

BEGIN_MESSAGE_MAP(CCsvDataViewerApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CCsvDataViewerApp::OnAppAbout)
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CCsvDataViewerApp コンストラクション

CCsvDataViewerApp::CCsvDataViewerApp()
{
	m_bHiColorIcons = TRUE;

	// 再起動マネージャーをサポートします
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// アプリケーションが共通言語ランタイム サポート (/clr) を使用して作成されている場合:
	//     1) この追加設定は、再起動マネージャー サポートが正常に機能するために必要です。
	//     2) 作成するには、プロジェクトに System.Windows.Forms への参照を追加する必要があります。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 下のアプリケーション ID 文字列を一意の ID 文字列で置換します。推奨される
	// 文字列の形式は CompanyName.ProductName.SubProduct.VersionInformation です
	SetAppID(_T("CsvDataViewer.AppID.NoVersion"));

	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

// 唯一の CCsvDataViewerApp オブジェクトです。

CCsvDataViewerApp theApp;


// CCsvDataViewerApp 初期化

BOOL CCsvDataViewerApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// OLE ライブラリを初期化します。
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// RichEdit コントロールを使用するには AfxInitRichEdit2() が必要です	
	// AfxInitRichEdit2();

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));
	LoadStdProfileSettings(4);  // 標準の INI ファイルのオプションをロードします (MRU を含む)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。
	//CMultiDocTemplate* pDocTemplate;
	m_pDocTKITemplate = new CMultiDocTemplate(IDR_CsvDataViewerTYPE,
		RUNTIME_CLASS(CCsvDataViewerDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CCsvDataViewerView));
	if (!m_pDocTKITemplate)
		return FALSE;
	AddDocTemplate(m_pDocTKITemplate);

	m_pDocTRFTemplate = new CMultiDocTemplate(IDR_CsvDataViewerTYPE,
		RUNTIME_CLASS(CTRFViewerDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CTRFViewerView));
	if (!m_pDocTRFTemplate)
		return FALSE;
	AddDocTemplate(m_pDocTRFTemplate);

	m_pDocADFTemplate = new CMultiDocTemplate(IDR_CsvDataViewerTYPE,
		RUNTIME_CLASS(CZeroGraphDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CZeroGraphViewer));
	if (!m_pDocADFTemplate)
		return FALSE;
	AddDocTemplate(m_pDocADFTemplate);

	m_pDocCTCTemplate = new CMultiDocTemplate(IDR_CsvDataViewerTYPE,
		RUNTIME_CLASS(CCompTickChartDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CCompTickChartView));
	if (!m_pDocCTCTemplate)
		return FALSE;
	AddDocTemplate(m_pDocCTCTemplate);

	
	// メイン MDI フレーム ウィンドウを作成します。
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// 接尾辞が存在する場合にのみ DragAcceptFiles を呼び出します
	//  MDI アプリケーションでは、この呼び出しは、m_pMainWnd を設定した直後に発生しなければなりません。
	// ドラッグ/ドロップ オープンを許可します。
	m_pMainWnd->DragAcceptFiles();

	// DDE、file open など標準のシェル コマンドのコマンド ラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// DDE Execute open を使用可能にします。
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// コマンド ラインで指定されたディスパッチ コマンドです。アプリケーションが
	// /RegServer、/Register、/Unregserver または /Unregister で起動された場合、False を返します。
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;

	
	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	
	//OnFileOpen();

	return TRUE;
}

int CCsvDataViewerApp::ExitInstance()
{
	//TODO: 追加したリソースがある場合にはそれらも処理してください
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CCsvDataViewerApp メッセージ ハンドラー


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

// ダイアログを実行するためのアプリケーション コマンド
void CCsvDataViewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CCsvDataViewerApp のカスタマイズされた読み込みメソッドと保存メソッド

void CCsvDataViewerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CCsvDataViewerApp::LoadCustomState()
{
}

void CCsvDataViewerApp::SaveCustomState()
{
}

// CCsvDataViewerApp メッセージ ハンドラー





CDocument* CCsvDataViewerApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	return CWinAppEx::OpenDocumentFile(lpszFileName);
}

void CCsvDataViewerApp::OpenTickFile() 
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	CFileDialog fd(
		TRUE,   // TRUE:Open, FAlSE:Close
		".tki",     // デフォルトの拡張子
		"",     // デフォルトのファイル名
		OFN_FILEMUSTEXIST | // 既存のファイルのみ
		OFN_HIDEREADONLY | // ReadOnlyのCheckBoxを表示しない
		OFN_LONGNAMES,  // 長いファイル名を使う
		"TickFile      (*.tki)|*.tki|ADF File      (*.adf)|*.adf|",
		NULL    // 親ウィンドウ
	);
	fd.m_ofn.lpstrInitialDir = (LPCSTR)"D:\\source\\Data\\";

	if (fd.DoModal() == IDOK) {
		if( strcmp( fd.GetFileExt(), "adf" ) == 0 ){
			m_pDocADFTemplate->OpenDocumentFile(fd.GetPathName());
		}else{
			m_pDocTKITemplate->OpenDocumentFile(fd.GetPathName());
		}
	}
}

void CCsvDataViewerApp::OpenTestResultFile( char* pTestName, DWORD dwPair )
{
	char cResultTickFile[1024];

	wsprintf( cResultTickFile, "%s\\TestMode_Result\\%s\\%s.tki", _LOG_DIR_, pTestName, GetPairName(dwPair));

	OpenDocumentFile( cResultTickFile );
}

BOOL CheckDirectoryExist(char* pDir) {
	BOOL bRet = FALSE;

	if(GetFileAttributes(pDir) == INVALID_FILE_ATTRIBUTES) {
		char cDir_Par[1024];
		char* pDirName;
		lstrcpy(cDir_Par, pDir);

		pDirName = (char*)_mbsrchr((unsigned char*)cDir_Par, (unsigned int)'\\');
		*pDirName = 0x00;
		if(CheckDirectoryExist(cDir_Par) != FALSE) {
			bRet = CreateDirectory(pDir, NULL);
		}
	}
	else {
		bRet = TRUE;
	}

	return bRet;
}

void CCsvDataViewerApp::OpenCurrentChart( char* pPairName )
{
	char cOpenFile[1024];
	SYSTEMTIME CurTime;

	GetLocalTime( &CurTime );

	wsprintf(cOpenFile, "%s\\TKI\\%s\\%02d\\%02d\\%02d.tki", _LOG_DIR_, pPairName, CurTime.wYear, CurTime.wMonth, CurTime.wDay);
	if( GetFileAttributes( cOpenFile ) == INVALID_FILE_ATTRIBUTES ){
		wsprintf(cOpenFile, "%s\\ADF\\%s\\%02d\\%02d\\%02d.adf", _LOG_DIR_, pPairName, CurTime.wYear, CurTime.wMonth, CurTime.wDay);
		//if(GetFileAttributes(cOpenFile) != INVALID_FILE_ATTRIBUTES) 
		{
			GetTempPath(sizeof(cOpenFile),cOpenFile);
			//	とりあえず空ファイルをつくっておく
			wsprintf(cOpenFile, "%s%s_current", cOpenFile, pPairName );
			HANDLE hFile;

			if( (hFile = CreateFile( cOpenFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) ) != INVALID_HANDLE_VALUE ){
				CloseHandle(hFile);
			}
		}
	}

	OpenDocumentFile(cOpenFile);
}

void CCsvDataViewerApp::OpenCompTickChartFile( const char* pPath )
{
	//char cOpenFile[1024];
	//SYSTEMTIME CurTime;

	//GetLocalTime( &CurTime );

	//wsprintf(cOpenFile, "%s\\TKI\\%s\\%02d\\%02d\\%02d.tki", _LOG_DIR_, pPairName, CurTime.wYear, CurTime.wMonth, CurTime.wDay);
	//if( GetFileAttributes( cOpenFile ) == INVALID_FILE_ATTRIBUTES ){
	//	wsprintf(cOpenFile, "%s\\ADF\\%s\\%02d\\%02d\\%02d.adf", _LOG_DIR_, pPairName, CurTime.wYear, CurTime.wMonth, CurTime.wDay);
	//	//if(GetFileAttributes(cOpenFile) != INVALID_FILE_ATTRIBUTES) 
	//	{
	//		GetTempPath(sizeof(cOpenFile),cOpenFile);
	//		//	とりあえず空ファイルをつくっておく
	//		wsprintf(cOpenFile, "%s%s_current", cOpenFile, pPairName );
	//		HANDLE hFile;

	//		if( (hFile = CreateFile( cOpenFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL) ) != INVALID_HANDLE_VALUE ){
	//			CloseHandle(hFile);
	//		}
	//	}
	//}

	m_pDocCTCTemplate->OpenDocumentFile(pPath);
}


void CCsvDataViewerApp::OpenTickRateGraph( char* pPath )
{
	m_pDocTRFTemplate->OpenDocumentFile(pPath);
}
#include "DocManager2.h"

void CCsvDataViewerApp::AddDocTemplate(CDocTemplate* pTemplate)
{
	if(m_pDocManager == NULL)
		m_pDocManager = new CDocManager2;
	m_pDocManager->AddDocTemplate(pTemplate);
}