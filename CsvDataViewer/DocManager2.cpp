// DocManager2.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DocManager2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDocManager2

IMPLEMENT_DYNAMIC(CDocManager2, CDocManager)

CDocManager2::CDocManager2()
{
}

CDocManager2::~CDocManager2()
{
}


//BEGIN_MESSAGE_MAP(CDocManager2, CDocManager)
//{{AFX_MSG_MAP(CDocManager2)
// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
//}}AFX_MSG_MAP
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDocManager2 診断

#ifdef _DEBUG
void CDocManager2::AssertValid() const
{
	CDocManager::AssertValid();
}

void CDocManager2::Dump(CDumpContext& dc) const
{
	CDocManager::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDocManager2 コマンド

CDocument* CDocManager2::OpenDocumentFile(LPCTSTR lpszFileName)
{
	return OpenDocumentFile(lpszFileName, TRUE);
}

//#define _countof(array) (sizeof(array)/sizeof(array[0]))
BOOL AFXAPI AfxFullPath(LPTSTR lpszPathOut, LPCTSTR lpszFileIn);
BOOL AFXAPI AfxComparePath(LPCTSTR lpszPath1, LPCTSTR lpszPath2);
#ifndef _MAC
BOOL AFXAPI AfxResolveShortcut(CWnd* pWnd, LPCTSTR pszShortcutFile, LPTSTR pszPath, int cchPath);
#endif

CDocument* CDocManager2::OpenDocumentFile(LPCTSTR lpszFileName, BOOL bAddToMRU)
{
	if(lpszFileName == NULL)
	{
		AfxThrowInvalidArgException();
	}
	// find the highest confidence
	POSITION pos = m_templateList.GetHeadPosition();
	CDocTemplate::Confidence bestMatch = CDocTemplate::noAttempt;
	CDocTemplate* pBestTemplate = NULL;
	CDocument* pOpenDocument = NULL;

	TCHAR szPath[_MAX_PATH];
	ASSERT(AtlStrLen(lpszFileName) < _countof(szPath));
	TCHAR szTemp[_MAX_PATH];
	if(lpszFileName[0] == '\"')
		++lpszFileName;
	Checked::tcsncpy_s(szTemp, _countof(szTemp), lpszFileName, _TRUNCATE);
	LPTSTR lpszLast = _tcsrchr(szTemp, '\"');
	if(lpszLast != NULL)
		*lpszLast = 0;

	if(AfxFullPath(szPath, szTemp) == FALSE)
	{
		ASSERT(FALSE);
		return NULL; // We won't open the file. MFC requires paths with
					 // length < _MAX_PATH
	}

	TCHAR szLinkName[_MAX_PATH];
	if(AfxResolveShortcut(AfxGetMainWnd(), szPath, szLinkName, _MAX_PATH))
		Checked::tcscpy_s(szPath, _countof(szPath), szLinkName);

	while(pos != NULL)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
		ASSERT_KINDOF(CDocTemplate, pTemplate);

		CDocTemplate::Confidence match;
		ASSERT(pOpenDocument == NULL);
		match = pTemplate->MatchDocType(szPath, pOpenDocument);
		if(match > bestMatch)
		{
			bestMatch = match;
			pBestTemplate = pTemplate;
		}
		if(match == CDocTemplate::yesAlreadyOpen){
			pOpenDocument = NULL;
			break;      // stop here
		}
	}

	if(pOpenDocument != NULL)
	{
		POSITION posOpenDoc = pOpenDocument->GetFirstViewPosition();
		if(posOpenDoc != NULL)
		{
			CView* pView = pOpenDocument->GetNextView(posOpenDoc); // get first one
			ASSERT_VALID(pView);
			CFrameWnd* pFrame = pView->GetParentFrame();

			if(pFrame == NULL)
				TRACE(traceAppMsg, 0, "Error: Can not find a frame for document to activate.\n");
			else
			{
				pFrame->ActivateFrame();

				if(pFrame->GetParent() != NULL)
				{
					CFrameWnd* pAppFrame;
					if(pFrame != (pAppFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd))
					{
						ASSERT_KINDOF(CFrameWnd, pAppFrame);
						pAppFrame->ActivateFrame();
					}
				}
			}
		}
		else
			TRACE(traceAppMsg, 0, "Error: Can not find a view for document to activate.\n");

		return pOpenDocument;
	}

	if(pBestTemplate == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_OPEN_DOC);
		return NULL;
	}

	return pBestTemplate->OpenDocumentFile(szPath, bAddToMRU, TRUE);
}
