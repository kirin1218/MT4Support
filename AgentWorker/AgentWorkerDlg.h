
// AgentWorkerDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxcmn.h"
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

// CAgentWorkerDlg ダイアログ
class CAgentWorkerDlg : public CDialogEx
{
// コンストラクション
public:
	CAgentWorkerDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AGENTWORKER_DIALOG };
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
	HANDLE m_hCmdOpeThread;
	CListCtrl m_ListUJ;
	CListCtrl m_ListEJ;
	CListCtrl m_ListEU;
	CListCtrl m_ListTrade;
	afx_msg void OnBnClickedOk();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void MT4_Initilize(DWORD dwMapId);
	void MT4_RegisterRate(DWORD dwMapId);
	void MT4_TradeCheck(DWORD dwMapId);
	void MT4_RegisterTradeInfo(DWORD dwMapId);
	void MT4_CheckCloseTrade(DWORD dwMapId);
	void MT4_RegisterCloseTrade(DWORD dwMapId);
	void MT4_DbgRegisterTickInfo(DWORD dwMapId);
	void MT4_FinishProcess(DWORD dwMapId);

	DWORD TradeCheck_Main(DWORD dwPair, DWORD* pdwRetBreakSign);
	DWORD RegisterRate_Main(DWORD dwPair, DWORD dwRate, SYSTEMTIME* pRegTime );
	DWORD Initilize_Main(DWORD dwPair);
	DWORD RegisterTradeInfo_Main(DWORD dwPair, DWORD dwTicket, DWORD dwOrderType, double dOpenPrice, double dStopLoss, DWORD dwBreakTrigger);
	DWORD CheckCloseTrade_Main(DWORD dwPair, DWORD dwTicket);
	DWORD RegisterCloseTrade_Main(DWORD dwPair, DWORD dwTicket, double dClosePrice);
	DWORD FinishProcess_Main(DWORD dwPair);

	void WriteTestResult(DWORD dwPair, DWORD dwTradeCnt, DWORD dwObtainCnt, DWORD dwLossCnt, int nRlstPips, DWORD dwObtainPips, DWORD dwLossPips);
	BOOL WriteTradeInfo(TRADE_INFO* ti, BOOL bOpen);
	bool RateDataWrite(DWORD dwPairIdx, DWORD dwRate );
	bool TickInfoWrite(DWORD dwPairIdx, DWORD dwTickIdx, DWORD dwWriteIdx) ;
	DWORD CalcEMAValue(DWORD dwPairIdx, DWORD dwCalcIdx);
	void MakeSendBuffer(char* pSendBuf, DWORD dwPair, double dVal, BOOL bBuy);
	void OutputTickInfo(DWORD dwPair);
	void OutputTradeInfo();
	void SetTickInfoData(CListCtrl* pListCtrl, DWORD dwPair);
	void UpdateTickInfoData( DWORD dwPair, DWORD dwCol);
	void ReadSettingData(SETTING_PRAMETER*,DWORD);
	DWORD GetPassedTradeTick(DWORD dwPair, DWORD dwTicket);

	DWORD CalcEMAValue(DWORD idx, DWORD dwEmaVal, DWORD dwPair);
	DWORD TradeCheck_DoujiBreak(DWORD dwPair);
	DWORD TradeCheck_EMABreak(DWORD dwPair);
	BOOL CloseCheck_EMABreak( TRADE_INFO& ti );
	DWORD TradeCheck_BBBreak(DWORD dwPair , DWORD* pdwBreakSign );
	DWORD TradeCheck_BBBackBreak(DWORD dwPair , DWORD* pdwBreakSign );
	BOOL CloseCheck_BBBreak_BW( TRADE_INFO& ti );
	BOOL CloseCheck_BBBreak_BWBack( TRADE_INFO& ti);
	DWORD CalcBollingerBandsSigma( DWORD dwPair, DWORD i , DWORD dwSpan, double nSigma );
	DWORD TradeCheck_BB3Sigma(DWORD dwPair );
	
	map<DWORD,map<DWORD,vector<TICK_INFO>>>	m_TickInfo;
	//vector<TICK_INFO>	m_TickInfo_usdjpy;
	//vector<TICK_INFO>	m_TickInfo_eurjpy;
	//vector<TICK_INFO>	m_TickInfo_eurusd;
	//vector<TICK_INFO>	m_TickInfo_audjpy;
	//vector<TICK_INFO>	m_TickInfo_chfjpy;
	//vector<TICK_INFO>	m_TickInfo_gbpjpy;
	//vector<TICK_INFO>	m_TickInfo_gbpusd;
	//vector<TICK_INFO>	m_TickInfo_nzdjpy;
	//vector<TICK_INFO>	m_TickInfo_usdchf;
	//vector<TICK_INFO>	m_TickInfo_zarjpy;

	vector<TRADE_INFO> m_tradeInfo;

	map<DWORD,HWND>	m_ViewHWnd;

	SETTING_PRAMETER m_Setting[PAIR_COUNT];
	BOOL m_bTestMode;
	BOOL m_bAutoMode;
	BOOL m_bTaskMode;
	void StartTest();
	void InitilizeTickInfo(DWORD dwPair );
	void InitilizeTickData( DWORD dwPair );
	

	enum {
		TI_COL_TYPE_DATE = 0,
		TI_COL_TYPE_OPEN,
		TI_COL_TYPE_CLOSE,
		TI_COL_TYPE_HIGH,
		TI_COL_TYPE_LOW,
		TI_COL_TYPE_CNT
	};
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNMCustomdrawListTickuj(NMHDR *pNMHDR, LRESULT *pResult);

	//bool ReadCsvData(const char* filename, vector<vector<string>>& vec_table);
	bool ReadADFFile(const char* filename, vector<ADF_DATA>& vec_table);
	void WriteSettingData(SETTING_PRAMETER* pTP);

	HANDLE m_hTickFile[TKI_COUNT];
	HANDLE m_hTradeFile;
	BOOL m_bAnaTKIWrite;
	SYSTEMTIME m_TestCurTime;

	void SetViewWindowHandle( DWORD dwPair, HWND hWnd );
	double CalcDX( DWORD dwPair, DWORD i, DWORD dwSpan );
	double CalcADX( DWORD dwPair, DWORD i, DWORD dwSpan );
	BOOL IsRangeNow( DWORD dwPair );
	double CalcSigma( DWORD dwPair, DWORD i , DWORD dwSpan );
	BOOL IsBandWalkStart( DWORD dwPair );
	BOOL IsConvergeSigma(DWORD dwPair, BOOL bHiCheck );
	TRADE_INFO LastTradeInfo[PAIR_COUNT];

	DWORD m_dwExpansionTID[PAIR_COUNT];
	DWORD m_dwSqueezeTID[PAIR_COUNT];
	void SigmaCheck( DWORD dwPair );
	double CalcRSI( DWORD dwPair, DWORD dwSpan, double dPrevUpAve, double dPrevDownAve );

	BOOL CalcStochasticOscillator( DWORD dwPair, DWORD i, DWORD dwKSpan, DWORD dwDSpan, DWORD dwSDSpan, DWORD* pdwSO_K, DWORD* pdwSO_D, DWORD* pdwSO_Slow );
	DWORD CalcD( DWORD dwPair, DWORD dwStart, DWORD dwKSpan, DWORD dwDSpan, DWORD dwSDSpan );
	bool CalcKLKH( DWORD dwPair, DWORD dwStart, DWORD dwKSpan, DWORD* pdwHi, DWORD* pdwLo );

	DWORD TradeCheck_SOBreak(DWORD dwPair );
	BOOL CloseCheck_SOBreak( TRADE_INFO& ti);
	void StartTest_Main(SETTING_PRAMETER*	pParam, vector<ADF_DATA>& RateFileTable, char* pDataPath );

	DWORD TradeCheck_RSIBreak(DWORD dwPair );
	BOOL CloseCheck_RSIBreak( TRADE_INFO& ti);

	BOOL GetIchimokuData( DWORD dwPair, DWORD i, ICHIMOKU_INFO* pRetInfo );
	DWORD TradeCheck_ICHIMOKUBreak(DWORD dwPair );
	BOOL CloseCheck_IchimokuBreak( TRADE_INFO& ti);

	DWORD TradeCheck_RangeBreak(DWORD dwPair );
	BOOL CloseCheck_RangeBreak( TRADE_INFO& ti);
	DWORD RangeBarrierCheck( DWORD dwPair, DWORD dwHiVal, DWORD dwLoVal );

	BOOL IsCrossIchimokuClound(DWORD dwPair, int idx, DWORD dwTradeType );


	void RegisterRate_70Tick( DWORD dwPair, DWORD dwRate, SYSTEMTIME* pRegTime );
	void RegisterRate_1Min( DWORD dwPair, DWORD dwRate, SYSTEMTIME* pRegTime );

	DWORD RateCheck_Main( DWORD dwPair, SYSTEMTIME* pRegTime );
	void GetRateCheckSetting( DWORD dwPair );

	DWORD GetTickDataFromTime( DWORD dwPair, SYSTEMTIME* pRegTime, DWORD dwBefTime );
	bool RateCheckWrite( DWORD dwPair, RATE_CHECK_INFO* pRci );

	CTime m_LastChkSUH;// = 0;
	CTime m_LastChkSUL;// = 0;
	CTime m_LastChkOV;// = 0;
	CTime m_LastChkRC;// = 0;
	CTime m_LastChkOM;// = 0;
	CTime m_LastChkAU;// = 0;
	CTime m_LastChkOS;// = 0;
};
