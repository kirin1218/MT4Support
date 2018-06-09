#pragma once
// �C���N���[�h
#include "MT4_Supoort.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>  // ��istringstream���g�����߂ɕK�v
// ���O���
using namespace std;

// CCompTickChartDoc �h�L�������g

class CCompTickChartDoc : public CDocument
{
	DECLARE_DYNCREATE(CCompTickChartDoc)

public:
	CCompTickChartDoc();
	virtual ~CCompTickChartDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // �h�L�������g I/O �ɑ΂��ăI�[�o�[���C�h����܂����B
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()

public:
	bool ReadTickFile( const char* pTickFilePath );
	bool MakeSimpleTickInfo();
	DWORD CompareSimgleTickInfo( );
	double CalcSigma( DWORD i , DWORD dwSpan );
	DWORD CalcBollingerBandsSigma( DWORD i , DWORD dwSpan, int nSigma );

	vector<TICK_INFO> m_TickInfo;
	vector<SIMPLE_TICK_INFO> m_STickInfo;
	vector<COMP_STICK_LIST> m_CmpSTick;
	DWORD m_dwPair;
	double m_dSpread;
};
