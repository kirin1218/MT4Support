
// DebugAWDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "DebugAW.h"
#include "DebugAWDlg.h"
#include "afxdialogex.h"

#include "MT4_Supoort.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CDebugAWDlg �_�C�A���O



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


// CDebugAWDlg ���b�Z�[�W �n���h���[

BOOL CDebugAWDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
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

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CDebugAWDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CDebugAWDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDebugAWDlg::OnBnClickedOk()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	//CDialogEx::OnOK();
}


void CDebugAWDlg::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
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
//	�f�[�^�̏�����
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

//	�e�B�b�N���𖳗����o�^���Ă��炤�F�F�F���S�Ƀf�o�b�O�����p
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

//	�t�@�C���Q�ƃ_�C�A���O
void CDebugAWDlg::OnBtnRefs()
{
	CFileDialog fd(
		TRUE,   // TRUE:Open, FAlSE:Close
		".aw",     // �f�t�H���g�̊g���q
		"",     // �f�t�H���g�̃t�@�C����
		OFN_FILEMUSTEXIST | // �����̃t�@�C���̂�
		OFN_HIDEREADONLY | // ReadOnly��CheckBox��\�����Ȃ�
		OFN_LONGNAMES,  // �����t�@�C�������g��
		"AskFile      (*.aw)|*.aw|",
		NULL    // �e�E�B���h�E
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

	//	�t�@�C������Ask����ǂݍ���
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

//					std::random_device rnd;     // �񌈒�I�ȗ���������

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
//  CSV���擾����
//      filename    �ǂݍ��ރt�@�C����
//      table       �ǂݍ���CSV�̓��e
//      delimiter   ��؂蕶��(����̓f�t�H���g�ŃJ���})
//
bool CDebugAWDlg::ReadCsvData(const char* filename, vector<vector<string>>& vec_table )
{
	const char delimiter = ',';
	// �t�@�C�����J��
	fstream filestream(filename);
	if (!filestream.is_open())
	{
		// �t�@�C�����J���Ȃ������ꍇ�͏I������
		return false;
	}

	vec_table.clear();

	// �t�@�C����ǂݍ���
	while (!filestream.eof())
	{
		// �P�s�ǂݍ���
		string buffer;
		filestream >> buffer;

		// �t�@�C������ǂݍ��񂾂P�s�̕��������؂蕶���ŕ����ă��X�g�ɒǉ�����
		vector<string> record;              // �P�s���̕�����̃��X�g
		istringstream streambuffer(buffer); // ������X�g���[��
		string token;                       // �P�Z�����̕�����
		while (getline(streambuffer, token, delimiter))
		{
			// �P�Z�����̕���������X�g�ɒǉ�����
			record.push_back(token);
		}

		// �P�s���̕�������o�͈����̃��X�g�ɒǉ�����
		vec_table.push_back(record);
	}

	if (vec_table.size() > 0) {
		return true;
	}
	else {
		return false;
	}
}