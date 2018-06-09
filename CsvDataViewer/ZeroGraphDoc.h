#pragma once

// CZeroGraphDoc �h�L�������g
#include "MT4_Supoort.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>  // ��istringstream���g�����߂ɕK�v
// ���O���
using namespace std;

class CZeroGraphDoc : public CDocument
{
	DECLARE_DYNCREATE(CZeroGraphDoc)

public:
	CZeroGraphDoc();
	virtual ~CZeroGraphDoc();
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
	vector<DWORD> m_vRateList;
	vector<DWORD> m_vSMAList;

	virtual void SetTitle(LPCTSTR lpszTitle);
	void ReadADF( char* pFilePath );
	DWORD CalcSMAValue( DWORD dwSpan, DWORD idx );

	DWORD m_dwPair;
	double m_dSpread;


};
