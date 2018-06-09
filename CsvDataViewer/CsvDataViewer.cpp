
// CsvDataViewer.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
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
	// �W���̃t�@�C����{�h�L�������g �R�}���h
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// �W���̈���Z�b�g�A�b�v �R�}���h
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CCsvDataViewerApp �R���X�g���N�V����

CCsvDataViewerApp::CCsvDataViewerApp()
{
	m_bHiColorIcons = TRUE;

	// �ċN���}�l�[�W���[���T�|�[�g���܂�
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// �A�v���P�[�V���������ʌ��ꃉ���^�C�� �T�|�[�g (/clr) ���g�p���č쐬����Ă���ꍇ:
	//     1) ���̒ǉ��ݒ�́A�ċN���}�l�[�W���[ �T�|�[�g������ɋ@�\���邽�߂ɕK�v�ł��B
	//     2) �쐬����ɂ́A�v���W�F�N�g�� System.Windows.Forms �ւ̎Q�Ƃ�ǉ�����K�v������܂��B
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ���̃A�v���P�[�V���� ID ���������ӂ� ID ������Œu�����܂��B���������
	// ������̌`���� CompanyName.ProductName.SubProduct.VersionInformation �ł�
	SetAppID(_T("CsvDataViewer.AppID.NoVersion"));

	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

// �B��� CCsvDataViewerApp �I�u�W�F�N�g�ł��B

CCsvDataViewerApp theApp;


// CCsvDataViewerApp ������

BOOL CCsvDataViewerApp::InitInstance()
{
	// �A�v���P�[�V���� �}�j�t�F�X�g�� visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll Version 6 �ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �� InitCommonControlsEx() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �A�v���P�[�V�����Ŏg�p���邷�ׂẴR���� �R���g���[�� �N���X���܂߂�ɂ́A
	// �����ݒ肵�܂��B
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// OLE ���C�u���������������܂��B
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// RichEdit �R���g���[�����g�p����ɂ� AfxInitRichEdit2() ���K�v�ł�	
	// AfxInitRichEdit2();

	// �W��������
	// �����̋@�\���g�킸�ɍŏI�I�Ȏ��s�\�t�@�C����
	// �T�C�Y���k���������ꍇ�́A�ȉ�����s�v�ȏ�����
	// ���[�`�����폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ��Ж��܂��͑g�D���Ȃǂ̓K�؂ȕ������
	// ���̕������ύX���Ă��������B
	SetRegistryKey(_T("�A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ���[�J�� �A�v���P�[�V����"));
	LoadStdProfileSettings(4);  // �W���� INI �t�@�C���̃I�v�V���������[�h���܂� (MRU ���܂�)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// �A�v���P�[�V�����p�̃h�L�������g �e���v���[�g��o�^���܂��B�h�L�������g �e���v���[�g
	//  �̓h�L�������g�A�t���[�� �E�B���h�E�ƃr���[���������邽�߂ɋ@�\���܂��B
	//CMultiDocTemplate* pDocTemplate;
	m_pDocTKITemplate = new CMultiDocTemplate(IDR_CsvDataViewerTYPE,
		RUNTIME_CLASS(CCsvDataViewerDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CCsvDataViewerView));
	if (!m_pDocTKITemplate)
		return FALSE;
	AddDocTemplate(m_pDocTKITemplate);

	m_pDocTRFTemplate = new CMultiDocTemplate(IDR_CsvDataViewerTYPE,
		RUNTIME_CLASS(CTRFViewerDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CTRFViewerView));
	if (!m_pDocTRFTemplate)
		return FALSE;
	AddDocTemplate(m_pDocTRFTemplate);

	m_pDocADFTemplate = new CMultiDocTemplate(IDR_CsvDataViewerTYPE,
		RUNTIME_CLASS(CZeroGraphDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CZeroGraphViewer));
	if (!m_pDocADFTemplate)
		return FALSE;
	AddDocTemplate(m_pDocADFTemplate);

	m_pDocCTCTemplate = new CMultiDocTemplate(IDR_CsvDataViewerTYPE,
		RUNTIME_CLASS(CCompTickChartDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CCompTickChartView));
	if (!m_pDocCTCTemplate)
		return FALSE;
	AddDocTemplate(m_pDocCTCTemplate);

	
	// ���C�� MDI �t���[�� �E�B���h�E���쐬���܂��B
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// �ڔ��������݂���ꍇ�ɂ̂� DragAcceptFiles ���Ăяo���܂�
	//  MDI �A�v���P�[�V�����ł́A���̌Ăяo���́Am_pMainWnd ��ݒ肵������ɔ������Ȃ���΂Ȃ�܂���B
	// �h���b�O/�h���b�v �I�[�v���������܂��B
	m_pMainWnd->DragAcceptFiles();

	// DDE�Afile open �ȂǕW���̃V�F�� �R�}���h�̃R�}���h ���C������͂��܂��B
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// DDE Execute open ���g�p�\�ɂ��܂��B
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// �R�}���h ���C���Ŏw�肳�ꂽ�f�B�X�p�b�` �R�}���h�ł��B�A�v���P�[�V������
	// /RegServer�A/Register�A/Unregserver �܂��� /Unregister �ŋN�����ꂽ�ꍇ�AFalse ��Ԃ��܂��B
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;

	
	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	
	//OnFileOpen();

	return TRUE;
}

int CCsvDataViewerApp::ExitInstance()
{
	//TODO: �ǉ��������\�[�X������ꍇ�ɂ͂������������Ă�������
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CCsvDataViewerApp ���b�Z�[�W �n���h���[


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
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

// �_�C�A���O�����s���邽�߂̃A�v���P�[�V���� �R�}���h
void CCsvDataViewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CCsvDataViewerApp �̃J�X�^�}�C�Y���ꂽ�ǂݍ��݃��\�b�h�ƕۑ����\�b�h

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

// CCsvDataViewerApp ���b�Z�[�W �n���h���[





CDocument* CCsvDataViewerApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	return CWinAppEx::OpenDocumentFile(lpszFileName);
}

void CCsvDataViewerApp::OpenTickFile() 
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
	CFileDialog fd(
		TRUE,   // TRUE:Open, FAlSE:Close
		".tki",     // �f�t�H���g�̊g���q
		"",     // �f�t�H���g�̃t�@�C����
		OFN_FILEMUSTEXIST | // �����̃t�@�C���̂�
		OFN_HIDEREADONLY | // ReadOnly��CheckBox��\�����Ȃ�
		OFN_LONGNAMES,  // �����t�@�C�������g��
		"TickFile      (*.tki)|*.tki|ADF File      (*.adf)|*.adf|",
		NULL    // �e�E�B���h�E
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
			//	�Ƃ肠������t�@�C���������Ă���
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
	//		//	�Ƃ肠������t�@�C���������Ă���
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