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

// CTRFViewerDoc �h�L�������g

class CTRFViewerDoc : public CDocument
{
	DECLARE_DYNCREATE(CTRFViewerDoc)

public:
	CTRFViewerDoc();
	virtual ~CTRFViewerDoc();
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
	virtual void SetTitle(LPCTSTR lpszTitle);
	void ReadTRF( char* pFilePath );
};
