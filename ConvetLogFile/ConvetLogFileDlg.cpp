
// ConvetLogFileDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "ConvetLogFile.h"
#include "ConvetLogFileDlg.h"
#include "afxdialogex.h"

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


// CConvetLogFileDlg �_�C�A���O



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


// CConvetLogFileDlg ���b�Z�[�W �n���h���[

BOOL CConvetLogFileDlg::OnInitDialog()
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

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CConvetLogFileDlg::OnPaint()
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

			//	�t�@�C����������t���擾
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

			//	CSV�F���ڂ��玞�����擾
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
//  CSV���擾����
//      filename    �ǂݍ��ރt�@�C����
//      table       �ǂݍ���CSV�̓��e
//      delimiter   ��؂蕶��(����̓f�t�H���g�ŃJ���})
//
bool CConvetLogFileDlg::ReadCsvData(const char* filename, vector<vector<string>>& vec_table)
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
		//	Tick�t�@�C��
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
		".csv",     // �f�t�H���g�̊g���q
		"",     // �f�t�H���g�̃t�@�C����
		OFN_FILEMUSTEXIST | // �����̃t�@�C���̂�
		OFN_HIDEREADONLY | // ReadOnly��CheckBox��\�����Ȃ�
		OFN_LONGNAMES,  // �����t�@�C�������g��
		szFilter,
		NULL    // �e�E�B���h�E
	);


	fd.m_ofn.lpstrInitialDir = (LPCSTR)_LOG_DIR_;

	if (fd.DoModal() == IDOK) {
		((CEdit*)GetDlgItem(IDC_ED_PATH))->SetWindowTextA(fd.GetPathName());
	}
}
