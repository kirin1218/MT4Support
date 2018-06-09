
// CsvDataViewer.h : CsvDataViewer �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��


// CCsvDataViewerApp:
// ���̃N���X�̎����ɂ��ẮACsvDataViewer.cpp ���Q�Ƃ��Ă��������B
//

class CCsvDataViewerApp : public CWinAppEx
{
public:
	CCsvDataViewerApp();
	void AddDocTemplate(CDocTemplate* pTemplate);

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����
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
