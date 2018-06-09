#pragma once

// インクルード
#include "MT4_Supoort.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>  // ※istringstreamを使うために必要
// 名前空間
using namespace std;

// CTRFViewerDoc ドキュメント

class CTRFViewerDoc : public CDocument
{
	DECLARE_DYNCREATE(CTRFViewerDoc)

public:
	CTRFViewerDoc();
	virtual ~CTRFViewerDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // ドキュメント I/O に対してオーバーライドされました。
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
