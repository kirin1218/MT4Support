// ZeroGraphDoc.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "ZeroGraphDoc.h"


// CZeroGraphDoc

IMPLEMENT_DYNCREATE(CZeroGraphDoc, CDocument)

CZeroGraphDoc::CZeroGraphDoc()
{
}

BOOL CZeroGraphDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CZeroGraphDoc::~CZeroGraphDoc()
{
}


BEGIN_MESSAGE_MAP(CZeroGraphDoc, CDocument)
END_MESSAGE_MAP()


// CZeroGraphDoc 診断

#ifdef _DEBUG
void CZeroGraphDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CZeroGraphDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CZeroGraphDoc シリアル化

void CZeroGraphDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 格納するコードをここに追加してください。
	}
	else
	{
		char cFilePath[1024];

		lstrcpy( cFilePath, ar.m_strFileName );
		ReadADF( cFilePath );
	}
}
#endif


// CTRFViewerDoc コマンド


void CZeroGraphDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	//CDocument::SetTitle(lpszTitle);
	CDocument::SetTitle("RateGraph");
}

DWORD CZeroGraphDoc::CalcSMAValue( DWORD dwSpan, DWORD idx )
{
	DWORD dwRet = 0;

	DWORD dwSum = 0;
	DWORD dwCnt = 0;
	DWORD dwStart = (idx >= (dwSpan - 1) )?(idx - (dwSpan - 1)):0;

	for( DWORD i = dwStart; i <= idx ; i++ ){
		dwSum += m_vRateList[i];

		dwCnt++;
	}
	dwRet = dwSum / dwCnt;

	return dwRet;
}

void CZeroGraphDoc::ReadADF( char* pFilePath )
{
	if(GetFileAttributes(pFilePath) != INVALID_FILE_ATTRIBUTES) {
		HANDLE hFile;

		m_dwPair = GetPairIdFromStrI( pFilePath );
		m_dSpread = GetSpread( m_dwPair )/2;

		hFile = CreateFile(pFilePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE) {
			DWORD dwFileSize = GetFileSize(hFile, NULL);
			DWORD dwRead;
			char* pData;
			char* pCur;
			char* pPoint;
			char cRate[1024];
			char cTime[64];

			m_vRateList.clear();

			if((pData = (char*)LocalAlloc(LMEM_FIXED, dwFileSize + 1)) != NULL) {
				ReadFile(hFile, pData, dwFileSize, &dwRead, NULL);
				pData[dwFileSize - 1] = 0x00;

				pCur = pData;

				while(*pCur != 0x00) {
					pPoint = strchr(pCur, ',');
					memcpy(cTime, pCur, (DWORD)(pPoint - pCur));
					cTime[(DWORD)(pPoint - pCur)] = 0x00;
					pCur = pPoint + 1;

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
						DWORD dwRate = Rate2EVal(atof(cRate), m_dwPair );

						if(dwRate > 0) {
							m_vRateList.push_back(dwRate);

							m_vSMAList.push_back( CalcSMAValue( 21, m_vRateList.size()-1 ) );
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