
// CsvDataViewerDoc.h : CCsvDataViewerDoc �N���X�̃C���^�[�t�F�C�X
//


#pragma once
#include "MT4_Supoort.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>  // ��istringstream���g�����߂ɕK�v

// ���O���
using namespace std;


class CCsvDataViewerDoc : public CDocument
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CCsvDataViewerDoc();
	DECLARE_DYNCREATE(CCsvDataViewerDoc)

// ����
public:

// ����
public:

// �I�[�o�[���C�h
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ����
public:
	virtual ~CCsvDataViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �����n���h���[�̌����R���e���c��ݒ肷��w���p�[�֐�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	void ClearTickData();
	bool ReadTickFile( char* pFilePath = NULL );
	bool ReadTradeFile();
	bool ReadRateChkFile();

	void CalculateIndicatorValue();
	DWORD CalcEMAValue(DWORD i, DWORD dwEmaVal);
	DWORD CalcSMAValue(DWORD i, DWORD dwSmaVal );
	void CalcStochasticOscillatorValue(TICK_INDICATOR_INFO* pTickIndicate, DWORD i, DWORD dwKSpan, DWORD dwDSpan, DWORD dwSDSpan);
	bool CalcKLKH(DWORD dwStart, DWORD dwKSpan, DWORD* pdwHi, DWORD* pdwLo);
	DWORD CalcD(DWORD dwStart, DWORD dwKSpan, DWORD dwDSpan, DWORD dwSDSpan);
	bool ReadCsvData(const char* filename, vector<vector<string>>& vec_table);
	vector<TICK_INFO> m_TickInfo;
	vector<TICK_INDICATOR_INFO> m_TickIndicteInfo;
	vector<TRADE_INFO> m_TradeInfo;
	vector<RATE_CHECK_INFO> m_RateCheckInfo;

	DWORD m_dwPair;
	DWORD m_dwTickType;
	double m_dSpread;

	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	BOOL UpdateTKIFile(char* pTKIPath);
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	virtual void SetTitle(LPCTSTR lpszTitle);

	BOOL m_bCurRateMode;
	char m_PrevTradeFile[1024];
	char m_PrevRCIFile[1024];

	typedef struct _TICK_MINI_DATA{
		DWORD dwType;
		DWORD dwDiff;
	}TICK_MINI_DATA,*PTICK_MINI_DATA;

	BOOL MatchTickData( DWORD d1, DWORD d2 ) ;
	void GetTickMiniData( DWORD i, TICK_MINI_DATA* pData );

};
