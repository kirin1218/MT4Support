#pragma once

typedef enum{
	MT4_INITILIZE = 0,
	MT4_REGISTERRATE,
	MT4_TRADECHECK,
	MT4_REGISTERTRADEINFO,
	MT4_CHECKCLOSETRADE,
	MT4_REGISTERCLOSETRADE,
	MT4_DBGREGISTERTICKINFO,
	MT4_FINISHPROCESS
}FUNCTION_NUMBER;

typedef enum{
	TKI_70TICK = 0,
	TKI_1MIN,
	TKI_5MIN,
	TKI_15MIN,
	TKI_30MIN,
	TKI_1HOUR,

	TKI_COUNT,
}TICK_INFO_TYPE;

static char* GetTickName(DWORD i)
{
	if(i == TKI_70TICK) {
		return "70tick";
	}else if(i == TKI_1MIN) {
		return "1min";
	}else if(i == TKI_5MIN) {
		return "5min";
	}else if(i == TKI_15MIN) {
		return "15min";
	}else if(i == TKI_1HOUR) {
		return "1hour";
	}
	return "";
}

typedef enum {
	PAIR_UNKNOWN = -1,
	PAIR_USDJPY = 0,
	PAIR_EURJPY,
	PAIR_EURUSD,
	PAIR_AUDJPY,
	PAIR_CHFJPY,
	PAIR_GBPJPY,
	PAIR_GBPUSD,
	PAIR_NZDJPY,
	PAIR_USDCHF,
	PAIR_ZARJPY,
	//
	PAIR_COUNT,
}PAIR_TYPE;

static char* GetPairName(DWORD i)
{
	if(i == PAIR_USDJPY) {
		return "usdjpy";
	}
	else if(i == PAIR_EURJPY) {
		return "eurjpy";
	}
	else if(i == PAIR_EURUSD) {
		return "eurusd";
	}
	else if(i == PAIR_AUDJPY) {
		return "audjpy";
	}
	else if(i == PAIR_CHFJPY) {
		return "chfjpy";
	}
	else if(i == PAIR_GBPJPY) {
		return "gbpjpy";
	}
	else if(i == PAIR_GBPUSD) {
		return "gbpusd";
	}
	else if(i == PAIR_NZDJPY) {
		return "nzdjpy";
	}
	else if(i == PAIR_USDCHF) {
		return "usdchf";
	}
	else if(i == PAIR_ZARJPY) {
		return "zarjpy";
	}

	return "";
}

# pragma pack (1)

typedef struct _TICK_INFO_VER1 {
	double	Op;
	double	Hi;
	double	Lo;
	double	Cl;
	DWORD	 Tc;	//	保持ティック数
	double	ema;
	SYSTEMTIME	sDt;
	SYSTEMTIME	eDt;
}TICK_INFO_VER1, *PTICK_INFO_VER1;

typedef struct _TICK_INFO_VER2 {
	DWORD	Ver;
	double	Op;
	double	Hi;
	double	Lo;
	double	Cl;
	DWORD	 Tc;	//	保持ティック数
	double	ema;
	SYSTEMTIME	sDt;
	SYSTEMTIME	eDt;
	BYTE	Reserv[1024];
}TICK_INFO_VER2, *PTICK_INFO_VER2;

//	2->3で保持する値を変更
//	各ペアのAsk->Eval(小数をやめて実数した値）かつAskはやめて実際のRateの値（スプレッドを含めない値）
//	よってdoubleからDWORDにする
//	ついでにemaもDWORDにしたけど、この値は使わないかも
typedef struct _TICK_INFO_VER3 {
	DWORD	Ver;
	DWORD	Op;
	DWORD	Hi;
	DWORD	Lo;
	DWORD	Cl;
	DWORD	 Tc;	//	保持ティック数
	DWORD	ema;
	SYSTEMTIME	sDt;
	SYSTEMTIME	eDt;
	DWORD	dwStrtTicket;
	DWORD	dwEndTicket;
	BYTE	Reserv[1024-sizeof(DWORD)*2];
}TICK_INFO_VER3, *PTICK_INFO_VER3;

typedef struct _TICK_INFO_VER4{
	DWORD	Ver;
	DWORD	Op;
	DWORD	Hi;
	DWORD	Lo;
	DWORD	Cl;
	DWORD	 Tc;	//	保持ティック数
	DWORD	ema;
	SYSTEMTIME	sDt;
	SYSTEMTIME	eDt;
	DWORD	dwStrtTicket;
	DWORD	dwEndTicket;
	DWORD	dwTID;
	BYTE	Reserv[1024-sizeof(DWORD)*3];
}TICK_INFO_VER4, *PTICK_INFO_VER4;

typedef struct _TICK_INFO_VER5{
	DWORD	Ver;
	DWORD	Op;
	DWORD	Hi;
	DWORD	Lo;
	DWORD	Cl;
	DWORD	 Tc;	//	保持ティック数
	DWORD	ema;
	SYSTEMTIME	sDt;
	SYSTEMTIME	eDt;
	DWORD	dwStrtTicket;
	DWORD	dwEndTicket;
	DWORD	dwTID;
	DWORD	dwRateAry[70];
	BYTE	Reserv[1024-sizeof(DWORD)*3-sizeof(DWORD)*70];
}TICK_INFO_VER5, *PTICK_INFO_VER5;

enum{
	RATE_DIRECT_STAY = 0,
	RATE_DIRECT_UP,
	RATE_DIRECT_DOWN
};

typedef struct _TICK_INFO_VER6{
	DWORD	Ver;
	DWORD	Op;
	DWORD	Hi;
	DWORD	Lo;
	DWORD	Cl;
	DWORD	 Tc;	//	保持ティック数
	DWORD	ema;
	SYSTEMTIME	sDt;
	SYSTEMTIME	eDt;
	DWORD	dwStrtTicket;
	DWORD	dwEndTicket;
	DWORD	dwTID;
	DWORD	dwRateAry[70];
	DWORD	dwRateDirect;
	DWORD	dwDirectChangeCnt;
	BYTE	Reserv[1024-sizeof(DWORD)*5-sizeof(DWORD)*70];
}TICK_INFO_VER6, *PTICK_INFO_VER6;

typedef struct _ICHIMOKU_INFO{
	DWORD dwConvVal;
	DWORD dwBaseVal;
	DWORD dwLaggingSpan;
	DWORD dwLeadingSpan1;
	DWORD dwLeadingSpan2;
}ICHIMOKU_INFO,*PICHIMOKU_INFO;

typedef struct _TICK_INFO_VER7{
	DWORD	Ver;
	DWORD	Op;
	DWORD	Hi;
	DWORD	Lo;
	DWORD	Cl;
	DWORD	 Tc;	//	保持ティック数
	DWORD	ema;
	SYSTEMTIME	sDt;
	SYSTEMTIME	eDt;
	DWORD	dwStrtTicket;
	DWORD	dwEndTicket;
	DWORD	dwTID;
	DWORD	dwRateAry[70];
	DWORD	dwRateDirect;
	DWORD	dwDirectChangeCnt;
	ICHIMOKU_INFO ichi_info;
	BYTE	Reserv[1024-sizeof(DWORD)*5-sizeof(DWORD)*70-sizeof(ICHIMOKU_INFO)];
}TICK_INFO_VER7, *PTICK_INFO_VER7;

typedef struct _TICK_INFO_VER8{
	DWORD	Ver;
	DWORD	Op;
	DWORD	Hi;
	DWORD	Lo;
	DWORD	Cl;
	DWORD	 Tc;	//	保持ティック数
	DWORD	ema;
	SYSTEMTIME	sDt;
	SYSTEMTIME	eDt;
	DWORD	dwStrtTicket;
	DWORD	dwEndTicket;
	DWORD	dwTID;
	DWORD	dwRateAry[70];
	DWORD	dwRateDirect;
	DWORD	dwDirectChangeCnt;
	ICHIMOKU_INFO ichi_info;
	BOOL	bCl;
	DWORD	dwTickType;
	DWORD	dwUpCnt;
	DWORD	dwDnCnt;
	DWORD	dwUpVal;
	DWORD	dwDnVal;
	BYTE	Reserv[1024-sizeof(DWORD)*6-sizeof(DWORD)*70-sizeof(ICHIMOKU_INFO)-sizeof(BOOL)];
}TICK_INFO_VER8, *PTICK_INFO_VER8;

#define TICK_INFO	TICK_INFO_VER8
#define TICK_LATEST_VER	(8)

typedef struct _TICK_INDICATOR_INFO {
	DWORD dw13ema;
	DWORD dw21ema;
	DWORD dw20ema;			//	20	EMA
	DWORD dw50ema;			//	50	EMA
	DWORD dw200ema;			//	200	EMA
	DWORD dwSO_K;			//	Stochastic Oscillator(K)
	DWORD dwSO_D;			//	Stochastic Oscillator(D)
	DWORD dwSO_SlowD;		//	Stochastic Oscillator(Slow D)
}TICK_INDICATOR_INFO, *PTICK_INDICATOR_INFO;

typedef struct _BBBREAK_INFO{
	DWORD dwStartTID;
	DWORD dwExpansionTID;
	DWORD dwStartRate;
	DWORD dwHiRate;
	DWORD dwLowRate;
	DWORD dwS2ESpan;
	DWORD dwStartPSigma;
	DWORD dwStartMSigma;
	DWORD dwStartSMA;
	DWORD dwEndPSigma;
	DWORD dwEndMSigma;
	DWORD dwEndSMA;
}BBBREAK_INFO,*PBBBREAK_INFO;

typedef struct _RSIBREAK_INFO{
	double dRSI;
}RSIBREAK_INFO,*PRSIBREAK_INFO;

typedef struct _RANGEBREAK_INFO{
	DWORD dwPeekVal;
}RANGEBREAK_INFO,*PRANGEBREAK_INFO;

typedef struct _TRADE_INFO_VER1 {
	DWORD dwPair;
	DWORD dwTicket;
	DWORD dwOrderType;
	double dOpenPrice;
	double dClosePrice;
	double dStopLoss;
	BOOL bIKEIKE_Mode;
	double dIKEIKE_Val;
	SYSTEMTIME	openDt;
	SYSTEMTIME	closeDt;
}TRADE_INFO_VER1, *PTRADE_INFO_VER1;

typedef struct _TRADE_INFO_VER2 {
	DWORD dwVer;
	DWORD dwPair;
	DWORD dwTicket;
	DWORD dwOrderType;
	double dOpenPrice;
	double dClosePrice;
	double dStopLoss;
	BOOL bIKEIKE_Mode;
	DWORD dwIKEIKE_EVal;
	SYSTEMTIME	openDt;
	SYSTEMTIME	closeDt;
	BYTE	Reserv[1024];
}TRADE_INFO_VER2, *PTRADE_INFO_VER2;

typedef struct _TRADE_INFO_VER3 {
	DWORD dwVer;
	DWORD dwPair;
	DWORD dwTicket;
	DWORD dwOrderType;
	double dOpenPrice;
	double dClosePrice;
	double dStopLoss;
	BOOL bIKEIKE_Mode;
	DWORD dwIKEIKE_EVal;
	SYSTEMTIME	openDt;
	SYSTEMTIME	closeDt;
	DWORD dwBreakTrigger;
	BYTE	Reserv[1024-sizeof(DWORD)];
}TRADE_INFO_VER3, *PTRADE_INFO_VER3;

typedef struct _TRADE_INFO_VER4 {
	DWORD dwVer;
	DWORD dwPair;
	DWORD dwTicket;
	DWORD dwOrderType;
	double dOpenPrice;
	double dClosePrice;
	double dStopLoss;
	BOOL bIKEIKE_Mode;
	DWORD dwIKEIKE_EVal;
	SYSTEMTIME	openDt;
	SYSTEMTIME	closeDt;
	DWORD dwBreakTrigger;
	DWORD dwMaxEval;
	BYTE	Reserv[1024 - sizeof(DWORD)*2];
}TRADE_INFO_VER4, *PTRADE_INFO_VER4;

typedef struct _TRADE_INFO_VER5 {
	DWORD dwVer;
	DWORD dwPair;
	DWORD dwTicket;
	DWORD dwOrderType;
	double dOpenPrice;
	double dClosePrice;
	double dStopLoss;
	BOOL bIKEIKE_Mode;
	DWORD dwIKEIKE_EVal;
	SYSTEMTIME	openDt;
	SYSTEMTIME	closeDt;
	DWORD dwBreakTrigger;
	DWORD dwMaxEval;
	DWORD dwTID;
	BYTE	Reserv[1024 - sizeof(DWORD)*3];
}TRADE_INFO_VER5, *PTRADE_INFO_VER5;


typedef struct _TRADE_INFO_VER6 {
	DWORD dwVer;
	DWORD dwPair;
	DWORD dwTicket;
	DWORD dwOrderType;
	double dOpenPrice;
	double dClosePrice;
	double dStopLoss;
	BOOL bIKEIKE_Mode;
	DWORD dwIKEIKE_EVal;
	SYSTEMTIME	openDt;
	SYSTEMTIME	closeDt;
	DWORD dwBreakTrigger;
	DWORD dwMaxEval;
	DWORD dwTID;
	DWORD dwPeakRate;
	DWORD dwCloseSign;
	union{
		BBBREAK_INFO bbi;
		RSIBREAK_INFO rsi;
		RANGEBREAK_INFO rbi;
	};
	BYTE	Reserv[1024 - sizeof(DWORD)*5 - sizeof(BBBREAK_INFO) ];
}TRADE_INFO_VER6, *PTRADE_INFO_VER6;


#define TRADE_INFO	TRADE_INFO_VER6
#define TRADE_LATEST_VER	(6)

typedef struct _VIEW_TICK_DATA{
	TICK_INFO ti;
	RECT rc;
	DWORD dwIdx;
}VIEW_TICK_DATA,*PVIEW_TICK_DATA;

typedef struct _MT4_INITILIZE_DATA {
	DWORD	dwRet;
	DWORD	dwPair;
	BYTE	reserv[1024 - sizeof(DWORD) - sizeof(DWORD)];
}MT4_INITILIZE_DATA, *PMT4_INITILIZE_DATA;

typedef struct _MT4_REGISTERRATE_DATA {
	DWORD	dwRet;
	DWORD	dwPair;
	DWORD	dwRate;
	BYTE	reserv[1024 - sizeof(DWORD)*3];
}MT4_REGISTERRATE_DATA, *PMT4_REGISTERRATE_DATA;

typedef struct _MT4_DBGREGISTERTICKINFO_DATA {
	DWORD	dwRet;
	DWORD	dwPair;
	DWORD	dwHi;
	DWORD	dwLo;
	DWORD	dwOp;
	DWORD	dwCl;
	BYTE	reserv[1024 - sizeof(DWORD)*6];
}MT4_DBGREGISTERTICKINFO_DATA, *PMT4_DBGREGISTERTICKINFO_DATA;

typedef struct _SIMPLE_TICK_INFO {
	DWORD dwId;
	int nDirectType;	//	1:上　0:同時　-1:下
	DWORD dwTickCnt;
	int nVal;
	DWORD sId;
	DWORD eId;
	SYSTEMTIME	sDt;
	SYSTEMTIME	eDt;
}SIMPLE_TICK_INFO, *PSIMPLE_TICK_INFO;

typedef struct _COMP_STICK_INFO{
	DWORD dwSId;
	DWORD dwEId;
	SYSTEMTIME	sDt;
	SYSTEMTIME	eDt;
}COMP_STICK_INFO,*PCOMP_STICK_INFO;

typedef struct _COMP_STICK_LIST{
	COMP_STICK_INFO csi1;
	COMP_STICK_INFO csi2;
	DWORD dwMinusPnt;
	DWORD dwMatch;
}COMP_STICK_LIST,*PCOMP_STICK_LIST;

#define BREAKSIGN_BY_DOUJIBREAK		0x00000001
#define BREAKSIGN_BY_EMABREAK		0x00000002
#define BREAKSIGN_BY_BBBREAK		0x00000004
#define BREAKSIGN_BY_BBBREAK_BW		0x00000008
#define BREAKSIGN_BY_BBBREAK_BWBACK 0x00000010
#define BREAKSIGN_BY_3SIGMABREAK	 0x00000020
#define BREAKSIGN_BY_SOBREAK		0x00000040
#define BREAKSIGN_BY_RSIBREAK		0x00000080
#define BREAKSIGN_BY_ICHIMOKUBREAK	0x00000100
#define BREAKSIGN_BY_RANGEBREAK		0x00000200

#define CLOSESIGN_BY_BBBREAK_BW_OVERSIGMA3		0x00010000
#define CLOSESIGN_BY_BBBREAK_BW_CHANGESIGMA		0x00020000
#define CLOSESIGN_LOSSCUT						0x00000001
#define CLOSESIGN_ZEROBACK						0x00000002
#define CLOSESIGN_PROFIT						0x00000004

typedef struct _MT4_TRADECHECK_DATA {
	DWORD	dwRet;
	DWORD	dwRetBreakSign;
	DWORD	dwPair;
	BYTE	reserv[1024 - sizeof(DWORD)*3];
}MT4_TRADECHECK_DATA, *PMT4_TRADECHECK_DATA;


typedef struct _MT4_REGISTERTRADEINFO_DATA {
	DWORD	dwRet;
	DWORD	dwPair;
	DWORD	dwTicket;
	DWORD	dwOrderType;
	double	dOpenPrice;
	double	dStopLoss;
	SYSTEMTIME stOpenDate;
	DWORD	dwBreakTrigger;
	BYTE	reserv[1024 - sizeof(DWORD)*4 - sizeof(double)*2 - sizeof(SYSTEMTIME)];
}MT4_REGISTERTRADEINFO_DATA, *PMT4_REGISTERTRADEINFO_DATA;

typedef struct _MT4_CHECKCLOSETRADE_DATA {
	DWORD	dwRet;
	DWORD	dwPair;
	DWORD	dwTicket;
	BYTE	reserv[1024 - sizeof(DWORD)*3];
}MT4_CHECKCLOSETRADE_DATA, *PMT4_CHECKCLOSETRADE_DATA;

typedef struct _MT4_REGISTERCLOSETRADE_DATA {
	DWORD	dwRet;
	DWORD	dwPair;
	DWORD	dwTicket;
	double	dClosePrice;
	BYTE	reserv[1024 - sizeof(DWORD) * 3 - sizeof(double)];
}MT4_REGISTERCLOSETRADE_DATA, *PMT4_REGISTERCLOSETRADE_DATA;

typedef struct _MT4_FINISHPROCESS_DATA {
	DWORD	dwRet;
	DWORD	dwPair;
	BYTE	reserv[1024 - sizeof(DWORD)*2];
}MT4_FINISHPROCESS_DATA, *PMT4_FINISHPROCESS_DATA;

# pragma pack ()

typedef struct __MqlDateTime
{
	int year;           // 年
	int mon;            // 月
	int day;            // 日
	int hour;           // 時間
	int min;            // 分
	int sec;            // 秒
	int day_of_week;    // 曜日(0:日曜, 1:月曜, … ,6-土曜)
	int day_of_year;    // 年の日数(1月1日は0の値が割り当てられます)
}_MqlDateTime;

typedef struct _SETTING_PRAMETER {
	//char cDataFilePath[1024];
	DWORD dwTickCount;	//	70:70Tick 1:1Min 5:5Min 15:15Min
	char cTestName[128];
	DWORD dwLosCutPips;		//	ロスカットする目安(pips指定)
	DWORD dwGainPips;		//	利益確定する目安(pips指定)
	DWORD dwIKEIKEEndPips;
	DWORD dwForceStopTickCnt;	//	指定されたTick数を超過した場合は+-0で決算する
	struct {
		BOOL bEnable;
		double	dRgOpCl;
		double	dRgHiLo;
		DWORD	dwContNum;
		double	dSigTrig;
	}db;
	struct {
		BOOL bEnable;
		DWORD dwMA1Val;
		DWORD dwMA2Val;
		DWORD dwSigTrig; 
	}mab;
	struct {
		BOOL bEnable;
	}bb;
	struct {
		BOOL bEnable;
		DWORD dwRSI;
		DWORD dwRSISpan;
		DWORD dwProfit;
		DWORD dwLossCut; 
	}brsi;
	struct {
		BOOL bEnable;
		DWORD dwKSpan;
		DWORD dwDSpan;
		DWORD dwSlowDSpan;
	}so;
	struct {
		BOOL bEnable;
		DWORD dwSpan;
	}rsi;
	struct {
		BOOL bEnable;
	}ichimoku;
	struct {
		BOOL bEnable;
		DWORD dwSpan;
		DWORD dwWidth;
		DWORD dwVagueVal;
	}rb;
}SETTING_PRAMETER, *PSETTING_PRAMETER;

typedef struct _ADF_DATA
{
	SYSTEMTIME time;
	double		dRate;
}ADF_DATA,*PADF_DATA;

//#define INDICATOR_OSCILLATOR_FLAG	0x10000000
#define INDICATOR_OSCILLATOR_FLAG	0x10000000
enum{
	INDICATOR_TYPE_UNKNOWN = -1,
	INDICATOR_TYPE_BB = 0,
	INDICATOR_TYPE_MA,
	INDICATOR_TYPE_ICHIMOKU,
	//	以下オシレーター系
	INDICATOR_TYPE_ADX = INDICATOR_OSCILLATOR_FLAG,	//	(0x10000000)
	INDICATOR_TYPE_RSI,
	INDICATOR_TYPE_SO,
	INDICATOR_TYPE_MACD,
	INDICATOR_TYPE_RCI,
};

#define IS_OSCILLATOR(a)	((a&INDICATOR_OSCILLATOR_FLAG)?TRUE:FALSE)

typedef struct _INDICATOR_SETTING{
	DWORD dwIndiType;
	DWORD dwSettingId;
	char DisplayName[128];

	union{
		struct _IS_BB{
			DWORD dwSpan;
			DWORD dwSigma;
		}bb;
		struct _IS_MA{
			DWORD dwSpan;
			COLORREF color;
		}ma;
		struct _IS_ADX{
			DWORD dwSpan;
		}adx;
		struct _IS_RSI{
			DWORD dwSpan;
		}rsi;
		struct _IS_SO{
			DWORD dwKSpan;
			DWORD dwDSpan;
			DWORD dwSSpan;
		}so;
		struct _IS_MACD{
			DWORD dwSSpan;
			DWORD dwLSpan;
			DWORD dwSignal;
		}macd;
		struct _IS_RCI{
			DWORD dwSpan1;
			DWORD dwSpan2;
			DWORD dwSpan3;
		}rci;		
	};
}INDICATOR_SETTING,*PINDICATOR_SETTING;

enum{
	TIME_BEFORE_1MIN = 1,
	TIME_BEFORE_5MIN,
	TIME_BEFORE_15MIN,
	TIME_BEFORE_30MIN,
	TIME_BEFORE_1HOUR,
	TIME_BEFORE_LAST
};


#define	RATECHK_SUDDEN_UP	0x00000001
#define	RATECHK_OVER_VAL	0x00000002
#define	RATECHK_RANGECHK	0x00000004
#define	RATECHK_OVER_MAX	0x00000008
#define	RATECHK_OVER_3SIGMA 0x00000010
#define	RATECHK_ALLTICK_UP	0x00000020

typedef struct _RATE_CHECK_INFO{
	DWORD dwChkType;
	DWORD dwChkRate;
	DWORD dwUpOrDn;
	SYSTEMTIME sDt;
	SYSTEMTIME eDt;
	DWORD dwMaxVal;
	DWORD dwMinVal;
}RATE_CHECK_INFO,*PRATE_CHECK_INFO;



// 保持するティック値情報の数
#define  MAX_TICK_INFOCNT  (300)
// ティックの単位
//#define REM_TICK_CNT       (70)

#define EMA_VALUE			(20)


#define _LOG_DIR_	"D:\\Source\\Data"

#define DEBUG_FILEOUT	(1)

#define ABS(a,b)	((a>b)?(a-b):(b-a))
#define ABS_CMP(a,b,c)	((a>b)?((a-b>c)?1:-1):((b-a>c)?1:-1))

#define MT4_EXPFUNC __declspec(dllexport)

typedef MT4_EXPFUNC BOOL WINAPI MT4_INITILIZE_FUNC(int);
typedef MT4_EXPFUNC BOOL WINAPI MT4_REGISTERRATE_FUNC (int , double);
typedef MT4_EXPFUNC int WINAPI  MT4_TRADECHECK_FUNC(int , int* );
typedef MT4_EXPFUNC BOOL WINAPI MT4_REGISTERTRADEINFO_FUNC(int  , int , double , int , double , _MqlDateTime*, int );
typedef MT4_EXPFUNC BOOL WINAPI MT4_CHECKCLOSETRADE_FUNC(int , int );
typedef MT4_EXPFUNC BOOL WINAPI MT4_REGISTERCLOSETRADE_FUNC(int , int , double );
//	デバッグ用関数だよ
typedef MT4_EXPFUNC BOOL WINAPI MT4_DBGREGISTERTICKINFO_FUNC(int, double, double, double, double );
typedef MT4_EXPFUNC void WINAPI MT4_SETTESTMODEFLAG_FUNC();
typedef MT4_EXPFUNC void WINAPI MT4_FINISHPROCESS_FUNC(DWORD);

#define LOSSCUT_UJ	(0.02)
#define LOSSCUT_EU	(0.0002)
#define GAINCHK_UJ	(0.06)
#define GAINCHK_EU	(0.0006)

#define AW_WINDOW_NAME		"AgentWorker"
#define AW_TETWINDOW_NAME	"AgentWorker_TESTMODE"

//	MT4起動時の初期化
 
//	各ペアの値（102.333とか1.3456とか）を実数に変換する
//	最小単位を1pipsにするか0.1pipsにするかがかなり悩ましい
//	とりま1EVal=0.1pipsとする。。。。
static DWORD Rate2EVal(double dRate, int nPair) {
	DWORD dwRet = 0;

	if(nPair == PAIR_EURUSD || nPair == PAIR_GBPUSD || nPair == PAIR_USDCHF ){
		dwRet = (DWORD)((dRate * 100000));
	}
	else{
		dwRet = (DWORD)((dRate * 1000));
	}

	return dwRet;
}

static double EVal2Rate(DWORD dwEVal, int nPair) {
	double dRet = 0.0;
	double dVal = (double)dwEVal;

	if(nPair == PAIR_EURUSD || nPair == PAIR_GBPUSD || nPair == PAIR_USDCHF ){
		dRet = dVal / 100000;
	}else{
		dRet = dVal / 1000;
	}
	return dRet;
}

//static double GetSpread(DWORD dwPair)
//{
//	double dRet = 0;
//
//	if(dwPair == PAIR_USDJPY) {
//		dRet = 0.003;
//	}
//	else if(dwPair == PAIR_EURJPY) {
//		dRet = 0.006;
//	}
//	else if(dwPair == PAIR_EURUSD) {
//		dRet = 0.00008;
//	}
//	return dRet;
//}

static double PriceFractionCut( double dRate, int nPair )
{
	double dRet = 0;

	if(nPair == PAIR_EURUSD || nPair == PAIR_GBPUSD || nPair == PAIR_USDCHF ){
		dRet = ((double)(DWORD)((dRate + 0.00005) * 10000)) / 10000;
	}else{
		dRet = ((double)(DWORD)((dRate + 0.005) * 100)) / 100;
	}
	return dRet;
}

static DWORD EValFractionCut(DWORD dwEVal, int nPair)
{
	return ((dwEVal + 5) / 10) * 10;
}

//	現状はpipsとEvalで10倍の違いがある
static DWORD Pips2EVal( DWORD dwPips ){
	return dwPips*10;
}

//	現状はpipsとEvalで10倍の違いがある
static int EVal2Pips(int Eval) {
	return Eval / 10;
}


#define MakeMapId(e1,e2)	(e1<<16|(e2&0x0000FFFF))
#include <Shlwapi.h>
static DWORD GetPairIdFromStrI( char* pString )
{ 
	DWORD dwRet = PAIR_UNKNOWN;

	if(StrStrI(pString, "USDJPY") != NULL) {
		dwRet = PAIR_USDJPY;
	}else  if(StrStrI(pString, "EURJPY") != NULL) {
		dwRet = PAIR_EURJPY;
	}else  if(StrStrI(pString, "EURUSD") != NULL) {
		dwRet = PAIR_EURUSD;
	}else  if(StrStrI(pString, "AUDJPY") != NULL) {
		dwRet = PAIR_AUDJPY;
	}else  if(StrStrI(pString, "CHFJPY") != NULL) {
		dwRet = PAIR_CHFJPY;
	}else  if(StrStrI(pString, "GBPJPY") != NULL) {
		dwRet = PAIR_GBPJPY;
	}else  if(StrStrI(pString, "GBPUSD") != NULL) {
		dwRet = PAIR_GBPUSD;
	}else  if(StrStrI(pString, "NZDJPY") != NULL) {
		dwRet = PAIR_NZDJPY;
	}else  if(StrStrI(pString, "USDCHF") != NULL) {
		dwRet = PAIR_USDCHF;
	}else  if(StrStrI(pString, "ZARJPY") != NULL) {
		dwRet = PAIR_ZARJPY;
	}

	return dwRet;
}
static DWORD GetTickTypeFromStrI( char* pString )
{
	DWORD dwRet = TKI_70TICK;

	if(StrStrI(pString, "70tick") != NULL) {
		dwRet = TKI_70TICK;
	}else  if(StrStrI(pString, "1min") != NULL) {
		dwRet = TKI_1MIN;
	}else  if(StrStrI(pString, "5min") != NULL) {
		dwRet = TKI_5MIN;
	}else  if(StrStrI(pString, "15min") != NULL) {
		dwRet = TKI_15MIN;
	}else  if(StrStrI(pString, "1hour") != NULL) {
		dwRet = TKI_1HOUR;
	}

	return dwRet;
}
static double GetSpread( DWORD dwPairId )
{ 
	double dRet = 0.0;

	if(dwPairId == PAIR_USDJPY) {
		dRet = 0.003;
	}
	else if(dwPairId == PAIR_EURJPY) {
		dRet = 0.006;
	}
	else if(dwPairId == PAIR_EURUSD) {
		dRet = 0.00008;
	}
	else if(dwPairId == PAIR_GBPUSD) {
		dRet = 0.00011;
	}
	else if(dwPairId == PAIR_USDCHF) {
		dRet = 0.00022;
	}
	else if(dwPairId == PAIR_AUDJPY) {
		dRet = 0.014;
	}
	else if(dwPairId == PAIR_CHFJPY) {
		dRet = 0.029;
	}
	else if(dwPairId == PAIR_GBPJPY) {
		dRet = 0.02;
	}
	else if(dwPairId == PAIR_NZDJPY) {
		dRet = 0.024;
	}

	else if(dwPairId == PAIR_ZARJPY) {
		dRet = 0.1;
	}

	return dRet;
}