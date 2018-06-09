// TestTradeDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "TestTradeDlg.h"
#include "afxdialogex.h"


// CTestTradeDlg ダイアログ

IMPLEMENT_DYNAMIC(CTestTradeDlg, CDialogEx)

CTestTradeDlg::CTestTradeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_TESTMODE, pParent)
{

}

CTestTradeDlg::~CTestTradeDlg()
{
}

void CTestTradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestTradeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTestTradeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTestTradeDlg メッセージ ハンドラー
#include "MT4_Supoort.h"

BOOL StartTest( DWORD dwTestId, BOOL bAuto ){
	BOOL bRet = FALSE;
	char cProcPath[1024];
	char* pFileName;
	char cCmdLine[1024];

	GetModuleFileName(NULL, cProcPath, sizeof(cProcPath));
	pFileName = (char*)_mbsrchr((unsigned char*)cProcPath, (unsigned int)'\\');

	lstrcpy( pFileName, "\\AgentWorker.exe" );

	if( bAuto == FALSE ){
		sprintf_s(cCmdLine, "%s -t %x", cProcPath, dwTestId );
	}else{
		sprintf_s(cCmdLine, "%s -t %x -a", cProcPath, dwTestId );
	}

	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	if( ( bRet = CreateProcess(NULL, (LPTSTR)cCmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS,
		NULL, NULL, &si, &pi) ) != FALSE ){


		CloseHandle(pi.hThread);

		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hProcess);
	}

	return bRet;
}





void CTestTradeDlg::WriteSettingData(SETTING_PRAMETER* pTP, char* pDataPath, char* pTestName )
{
	char cIniPath[1024];
	char* pFileName;
	char cWriteBuf[256];
	//SYSTEMTIME	CTime;

	//GetLocalTime( &CTime );

	GetModuleFileName(NULL, cIniPath, sizeof(cIniPath));
	pFileName = (char*)_mbsrchr((unsigned char*)cIniPath, (unsigned int)'\\');

	lstrcpy(pFileName, "\\AgentWorker_Test.ini");

	//	テスト名を書き込む
	WritePrivateProfileString("GENERAL", "TESTNAME", pTestName, cIniPath);
	//	テストに使うDataFile	
	WritePrivateProfileString("DATAFILE", "1", pDataPath, cIniPath);
	
	if( pTP != NULL ){
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

		if(pTP->db.bEnable){
			WritePrivateProfileString("DOUJIBREAK", "ENABLE", "1", cIniPath);
		}else{
			WritePrivateProfileString("DOUJIBREAK", "ENABLE", "0", cIniPath);
		}

		//	同時線として扱うOpとClの差
		_stprintf_s(cWriteBuf, "%f", pTP->db.dRgOpCl );
		WritePrivateProfileString( "DOUJIBREAK","DB_OPCLRANGE", cWriteBuf, cIniPath );
		//	同時線として扱うHiとLoの差
		_stprintf_s(cWriteBuf, "%f", pTP->db.dRgHiLo );
		WritePrivateProfileString("DOUJIBREAK", "DB_HILORANGE", cWriteBuf, cIniPath);
		//	ブレイクするために何個同時選が続いている必要があるか
		_stprintf_s(cWriteBuf, "%d", pTP->db.dwContNum);
		WritePrivateProfileString("DOUJIBREAK", "CONTINUTENUM", cWriteBuf, cIniPath);
		//	ブレイクに必要となる差分
		_stprintf_s(cWriteBuf, "%f", pTP->db.dSigTrig );
		WritePrivateProfileString("DOUJIBREAK", "SIGNALTRIGGER", cWriteBuf, cIniPath);


		if(pTP->mab.bEnable) {
			WritePrivateProfileString("EMABREAK", "ENABLE", "1", cIniPath);
		}
		else {
			WritePrivateProfileString("EMABREAK", "ENABLE", "0", cIniPath);
		}

		//	同時線として扱うOpとClの差
		_stprintf_s(cWriteBuf, "%d", pTP->mab.dwMA1Val);
		WritePrivateProfileString("EMABREAK", "MAB_EMA1", cWriteBuf, cIniPath);
		//	同時線として扱うHiとLoの差
		_stprintf_s(cWriteBuf, "%d", pTP->mab.dwMA1Val);
		WritePrivateProfileString("EMABREAK", "MAB_EMA2", cWriteBuf, cIniPath);
	}	

	return;
}

//	テストの開始
void CTestTradeDlg::OnBnClickedOk()
{
	//	速度を上げるためになるべくAgetnWorkerに任せる
	DWORD dwTestId = GetTickCount();

	//	GUIから設定を取得
	char cDataFilePath[1024];
	char cTempBuf[256];
	SETTING_PRAMETER	Param;
	BOOL bAuto = FALSE;
	SYSTEMTIME	CTime;

	GetLocalTime(&CTime);


	//	テスト名を書き込む
	wsprintf( this->m_cTestName, "%d%02d%02d_%02d%02d%02d", CTime.wYear, CTime.wMonth, CTime.wDay, CTime.wHour, CTime.wMinute, CTime.wSecond);

	//	全般設定
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_TICK);
	int nCmb = pCmb->GetCurSel();
	if(nCmb == 0) {
		Param.dwTickCount = 70;
	}
	else if(nCmb == 1) {
		Param.dwTickCount = 1;
	}
	else if(nCmb == 2) {
		Param.dwTickCount = 5;
	}
	else if(nCmb == 3) {
		Param.dwTickCount = 15;
	}

	((CEdit*)GetDlgItem(IDC_ED_DATAFILE))->GetWindowTextA(cDataFilePath, sizeof(cDataFilePath));

	//	データファイルの読み込み
	m_dwPair = GetPairIdFromStrI( cDataFilePath );
	if( ((CButton*)GetDlgItem(IDC_CHK_AUTO))->GetCheck() == 0 ){
		((CEdit*)GetDlgItem(IDC_ED_TAKEPROFIT))->GetWindowTextA(cTempBuf, sizeof(cTempBuf));
		Param.dwGainPips = atoi(cTempBuf);
		((CEdit*)GetDlgItem(IDC_ED_STOPLOSS))->GetWindowTextA(cTempBuf, sizeof(cTempBuf));
		Param.dwLosCutPips = atoi(cTempBuf);
		((CEdit*)GetDlgItem(IDC_ED_IKEIKEMODEEND))->GetWindowTextA(cTempBuf, sizeof(cTempBuf));
		Param.dwIKEIKEEndPips = atoi(cTempBuf);
		((CEdit*)GetDlgItem(IDC_ED_FORCESTOPTRADECNT))->GetWindowTextA(cTempBuf, sizeof(cTempBuf));
		Param.dwForceStopTickCnt = atoi(cTempBuf);

		//	多発同時線ブレイク
		(((CButton*)GetDlgItem(IDC_CHK_DB))->GetCheck() != 0) ? (Param.db.bEnable = TRUE) : (Param.db.bEnable = FALSE);
		((CEdit*)GetDlgItem(IDC_ED_DB1))->GetWindowTextA(cTempBuf, sizeof(cTempBuf));
		Param.db.dRgOpCl = atof(cTempBuf);
		((CEdit*)GetDlgItem(IDC_ED_DB2))->GetWindowTextA(cTempBuf, sizeof(cTempBuf));
		Param.db.dRgHiLo = atof(cTempBuf);
		((CEdit*)GetDlgItem(IDC_ED_DB3))->GetWindowTextA(cTempBuf, sizeof(cTempBuf));
		Param.db.dwContNum = atoi(cTempBuf);
		((CEdit*)GetDlgItem(IDC_ED_DB4))->GetWindowTextA(cTempBuf, sizeof(cTempBuf));
		Param.db.dSigTrig = atof(cTempBuf);
		
		//	MAブレイク
		(((CButton*)GetDlgItem(IDC_CHK_MAB))->GetCheck() != 0) ? (Param.mab.bEnable = TRUE) : (Param.mab.bEnable = FALSE);
		((CEdit*)GetDlgItem(IDC_ED_MA1))->GetWindowTextA(cTempBuf, sizeof(cTempBuf));
		Param.mab.dwMA1Val = atoi(cTempBuf);
		((CEdit*)GetDlgItem(IDC_ED_MA2))->GetWindowTextA(cTempBuf, sizeof(cTempBuf));
		Param.mab.dwMA2Val = atoi(cTempBuf);
		((CEdit*)GetDlgItem(IDC_ED_MA3))->GetWindowTextA(cTempBuf, sizeof(cTempBuf));
		Param.mab.dwSigTrig = atoi(cTempBuf);

		//	設定情報をiniに書き込む
		WriteSettingData(&Param, cDataFilePath, this->m_cTestName);

	}else{
		//	設定情報をiniに書き込む
		WriteSettingData(NULL, cDataFilePath, this->m_cTestName);

		bAuto = TRUE;
	}


	//	テストの準備
	//	AgentWorkerの起動
	//	テストIDと自動分析モードの有無をパラメータで渡す
	StartTest( dwTestId, bAuto );
	
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnOK();
}




BOOL CTestTradeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_TICK);
	pCmb->InsertString( -1, "70Tick" );
	pCmb->InsertString(-1, "1分間");
	pCmb->InsertString(-1, "5分間");
	pCmb->InsertString(-1, "15分間");
	pCmb->SetCurSel(0);

	char cWriteData[32];
	SETTING_PRAMETER InitSetting;
	ReadSettingData(&InitSetting);

	wsprintf(cWriteData, "%d", InitSetting.dwLosCutPips );
	((CEdit*)GetDlgItem(IDC_ED_STOPLOSS))->SetWindowText(cWriteData);
	wsprintf(cWriteData, "%d", InitSetting.dwIKEIKEEndPips);
	((CEdit*)GetDlgItem(IDC_ED_IKEIKEMODEEND))->SetWindowText(cWriteData);
	wsprintf(cWriteData, "%d", InitSetting.dwGainPips);
	((CEdit*)GetDlgItem(IDC_ED_TAKEPROFIT))->SetWindowText(cWriteData);
	wsprintf(cWriteData, "%d", InitSetting.dwForceStopTickCnt);
	((CEdit*)GetDlgItem(IDC_ED_FORCESTOPTRADECNT))->SetWindowText(cWriteData);

	((CButton*)GetDlgItem(IDC_CHK_DB))->SetCheck(InitSetting.db.bEnable);
	wsprintf(cWriteData, "%d", InitSetting.db.dRgOpCl);
	((CEdit*)GetDlgItem(IDC_ED_DB1))->SetWindowText(cWriteData);
	wsprintf(cWriteData, "%d", InitSetting.db.dRgHiLo);
	((CEdit*)GetDlgItem(IDC_ED_DB2))->SetWindowText(cWriteData);
	wsprintf(cWriteData, "%d", InitSetting.db.dwContNum);
	((CEdit*)GetDlgItem(IDC_ED_DB3))->SetWindowText(cWriteData);
	wsprintf(cWriteData, "%d", InitSetting.db.dSigTrig);
	((CEdit*)GetDlgItem(IDC_ED_DB4))->SetWindowText(cWriteData);

	((CButton*)GetDlgItem(IDC_CHK_MAB))->SetCheck(InitSetting.mab.bEnable);
	wsprintf(cWriteData, "%d", InitSetting.mab.dwMA1Val);
	((CEdit*)GetDlgItem(IDC_ED_MA1))->SetWindowText(cWriteData);
	wsprintf(cWriteData, "%d", InitSetting.mab.dwMA2Val);
	((CEdit*)GetDlgItem(IDC_ED_MA2))->SetWindowText(cWriteData);
	wsprintf(cWriteData, "%d", InitSetting.mab.dwSigTrig);
	((CEdit*)GetDlgItem(IDC_ED_MA3))->SetWindowText(cWriteData);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}



void CTestTradeDlg::ReadSettingData(SETTING_PRAMETER* pSetting)
{
	char cIniPath[1024];
	char* pFileName;
	char cReadBuf[256];


	GetModuleFileName(NULL, cIniPath, sizeof(cIniPath));
	pFileName = (char*)_mbsrchr((unsigned char*)cIniPath, (unsigned int)'\\');

	lstrcpy(pFileName, "\\AgentWorker_Test.ini");

	memset(pSetting, 0x00, sizeof(SETTING_PRAMETER));

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
	pSetting->db.bEnable = (atoi(cReadBuf) == 0) ? FALSE : TRUE;

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

	GetPrivateProfileString("EMABREAK", "SIGNALTRIGGER", "2", cReadBuf, sizeof(cReadBuf), cIniPath);
	pSetting->mab.dwSigTrig = atoi(cReadBuf);

	return;
}