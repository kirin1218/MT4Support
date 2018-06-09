
// CsvDataViewer.h : CsvDataViewer アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CCsvDataViewerApp:
// このクラスの実装については、CsvDataViewer.cpp を参照してください。
//

class CCsvDataViewerApp : public CWinAppEx
{
public:
	CCsvDataViewerApp();
	void AddDocTemplate(CDocTemplate* pTemplate);

// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
public:
	void OpenTickFile();
	void OpenTestResultFile(char* pTestName, DWORD dwPair );
	void OpenCurrentChart(char* pPairName);
	void OpenTickRateGraph( char* pPath );
	void OpenCompTickChartFile( const char* pPath );

private:
	CMultiDocTemplate* m_pDocTKITemplate;
	CMultiDocTemplate* m_pDocTRFTemplate;
	CMultiDocTemplate* m_pDocADFTemplate;
	CMultiDocTemplate* m_pDocCTCTemplate;
	
	
};

extern CCsvDataViewerApp theApp;
