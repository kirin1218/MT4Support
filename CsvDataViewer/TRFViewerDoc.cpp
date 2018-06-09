// TRFViewerDoc.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "TRFViewerDoc.h"


// CTRFViewerDoc

IMPLEMENT_DYNCREATE(CTRFViewerDoc, CDocument)

CTRFViewerDoc::CTRFViewerDoc()
{
}

BOOL CTRFViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CTRFViewerDoc::~CTRFViewerDoc()
{
}


BEGIN_MESSAGE_MAP(CTRFViewerDoc, CDocument)
END_MESSAGE_MAP()


// CTRFViewerDoc 診断

#ifdef _DEBUG
void CTRFViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CTRFViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CTRFViewerDoc シリアル化

void CTRFViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 格納するコードをここに追加してください。
	}
	else
	{
		char cFilePath[1024];

		lstrcpy( cFilePath, ar.m_strFileName );
		ReadTRF( cFilePath );
	}
}
#endif


// CTRFViewerDoc コマンド


void CTRFViewerDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDocument::SetTitle(lpszTitle);
	CDocument::SetTitle("RateGraph");
}


void CTRFViewerDoc::ReadTRF( char* pFilePath )
{
	if(GetFileAttributes(pFilePath) != INVALID_FILE_ATTRIBUTES) {
		HANDLE hFile;

		hFile = CreateFile(pFilePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE) {
			DWORD dwFileSize = GetFileSize(hFile, NULL);
			DWORD dwRead;
			char* pData;
			char* pCur;
			char* pPoint;
			char cRate[1024];

			m_vRateList.clear();

			if((pData = (char*)LocalAlloc(LMEM_FIXED, dwFileSize + 1)) != NULL) {
				ReadFile(hFile, pData, dwFileSize, &dwRead, NULL);
				pData[dwFileSize - 1] = 0x00;

				pCur = pData;

				while(*pCur != 0x00) {
					pPoint = strchr(pCur, '\r');
					if(pPoint != NULL) {
						memcpy(cRate, pCur, (DWORD)(pPoint - pCur));
						cRate[(DWORD)(pPoint - pCur)] = 0x00;
						pCur = pPoint + 1;

						if(*pCur == '\n') {
							pCur++;
						}

					}
					else {
						lstrcpy(cRate, pCur);
						pCur += lstrlen(cRate);
					}

					if(cRate[0] != 0x00) {
						DWORD dwRate = atoi(cRate);

						if(dwRate > 0) {
							m_vRateList.push_back(dwRate);
						}
					}
				}

				//	一行ごとに
				LocalFree(pData);
			}

			CloseHandle(hFile);
		}
	}
}