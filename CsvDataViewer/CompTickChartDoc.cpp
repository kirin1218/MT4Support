// CompTickChartDoc.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CsvDataViewer.h"
#include "CompTickChartDoc.h"

#include "MainFrm.h"
extern CMainFrame* g_pMainFrame;

// CCompTickChartDoc

IMPLEMENT_DYNCREATE(CCompTickChartDoc, CDocument)

CCompTickChartDoc::CCompTickChartDoc()
{
}

BOOL CCompTickChartDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CCompTickChartDoc::~CCompTickChartDoc()
{
}


BEGIN_MESSAGE_MAP(CCompTickChartDoc, CDocument)
END_MESSAGE_MAP()


// CCompTickChartDoc 診断

#ifdef _DEBUG
void CCompTickChartDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CCompTickChartDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CCompTickChartDoc シリアル化

void CCompTickChartDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 格納するコードをここに追加してください。
	}
	else
	{
		//	データの読み込み
		if( ReadTickFile( ar.m_strFileName ) != false ){
			if( MakeSimpleTickInfo() != false ){
				if( CompareSimgleTickInfo() > 0 ){
					g_pMainFrame->UpdateCompSTickInfoListCtrl( &m_CmpSTick );
				}
			}
		}
	}
}
#endif


/////////////////////////////////////////////////////
//σ=√(((X1-Xm)^2+(X2-Xm)^2・・・(Xn-Xm)^2)/N-1)
//N・・・任意の期間(25日を使うことが多いです)
//X1～Xn・・・1～N日までの各終値
//Xm・・・X1～Xnの平均値 
double CCompTickChartDoc::CalcSigma( DWORD i , DWORD dwSpan )
{
	if ( i + 1 < dwSpan )	return 0;

	DWORD l;
	double nAve = 0;
	double nMole = 0;

	for( l = i - dwSpan + 1; l <= i; l++ ){
		nAve += (double)m_TickInfo[l].Cl;
	}
	nAve /= (double)dwSpan;

	for( l = i - dwSpan + 1; l <= i; l++ ){
		nMole += ((double)m_TickInfo[l].Cl-nAve)*((double)m_TickInfo[l].Cl-nAve);
	}

	nMole /= (double)dwSpan-1;

	return sqrt(nMole);
}

DWORD CCompTickChartDoc::CalcBollingerBandsSigma( DWORD i , DWORD dwSpan, int nSigma )
{
	if ( i + 1 < dwSpan )	return 0;

	DWORD l;
	double nAve = 0;
	double nMole = 0;

	for( l = i - dwSpan + 1; l <= i; l++ ){
		nAve += (double)m_TickInfo[l].Cl;
	}
	nAve /= (double)dwSpan;

	return (DWORD)(nAve + (CalcSigma(i, dwSpan)*nSigma));
}

void DebugPrint(char* pszFormat, ...);
#define DIFF_VAL( a, b )	((a>b)?(a-b):(b-a))

DWORD CCompTickChartDoc::CompareSimgleTickInfo( ){
	DWORD dwRet = 0;
	m_CmpSTick.clear();

	DWORD dwEnd = m_STickInfo.size()-1;


	for( int i = dwEnd; i > 0; i-- ){
		if( m_TickInfo[i].Cl > m_TickInfo[i].Op ){
			if( CalcBollingerBandsSigma( i, 21, 2 ) > m_TickInfo[i].Cl ){
				continue;
			}
		}else{
			if( CalcBollingerBandsSigma( i, 21, -2 ) < m_TickInfo[i].Cl ){
				continue;
			}
		}
		for( int j = i-10; j > 0; j-- ){
			int nMatch;
			int nIMinusPnt = 0;
			int nJMinusPnt = 0;
			int nIIdx;
			int nJIdx;

			nMatch=0;
			nIIdx = i;// - nMatch;
			nJIdx = j;// - nMatch;

			while(1){
IDX_CHANGE_RETRY:
				if(  nJIdx > 0 ){
					if( m_STickInfo[nJIdx].nDirectType == 0 && m_STickInfo[nJIdx].dwTickCnt <= 1
					 && m_STickInfo[nIIdx].nDirectType == 0 && m_STickInfo[nIIdx].dwTickCnt <= 1
					){
						nMatch++;
					}else{
						if( m_STickInfo[nJIdx].nDirectType == 0 && m_STickInfo[nJIdx].dwTickCnt <= 1){
							nJIdx--;
							nJMinusPnt++;

							goto IDX_CHANGE_RETRY;
						}else if( m_STickInfo[nIIdx].nDirectType == 0 && m_STickInfo[nIIdx].dwTickCnt <= 1){
							nIIdx--;
							nIMinusPnt++;

							goto IDX_CHANGE_RETRY;
						}

						if( m_STickInfo[nJIdx].nDirectType != m_STickInfo[nIIdx].nDirectType){
							break;
						}else if( DIFF_VAL( m_STickInfo[nJIdx].nVal, m_STickInfo[nIIdx].nVal ) > 15
							   || DIFF_VAL( m_STickInfo[nJIdx].dwTickCnt, m_STickInfo[nIIdx].dwTickCnt ) > 1
						){
							break;
						}else{
							int a;
							a = 3;
							nMatch++;

						}
					}
					nIIdx--;
					nJIdx--;
				}else{
					break;
				}
			}

			if( nMatch >= 12 && (nJMinusPnt + nIMinusPnt) < 2 ){
				COMP_STICK_LIST newData;

				memset( &newData, 0x00, sizeof(COMP_STICK_LIST) );

				newData.csi1.dwSId = m_STickInfo[nIIdx+1].eId;
				newData.csi1.dwEId = m_STickInfo[i].sId;
				memcpy( &newData.csi1.sDt, &m_STickInfo[nIIdx+1].sDt, sizeof(SYSTEMTIME) );
				memcpy( &newData.csi1.eDt, &m_STickInfo[i].eDt, sizeof(SYSTEMTIME) );

				newData.csi2.dwSId = m_STickInfo[nJIdx+1].eId;
				newData.csi2.dwEId = m_STickInfo[j].sId;
				memcpy( &newData.csi2.sDt, &m_STickInfo[nJIdx+1].sDt, sizeof(SYSTEMTIME) );
				memcpy( &newData.csi2.eDt, &m_STickInfo[j].eDt, sizeof(SYSTEMTIME) );
				newData.dwMinusPnt = nJMinusPnt + nIMinusPnt;
				newData.dwMatch = nMatch;
				m_CmpSTick.push_back(newData);

				//DebugPrint( "%d,%02d/%02d %02d:%02d,%02d/%02d %02d:%02d,", dwMatch, 
				//	m_STickInfo[i].sDt.wMonth, m_STickInfo[i].sDt.wDay, m_STickInfo[i].sDt.wHour, m_STickInfo[i].sDt.wMinute,
				//	m_STickInfo[j].sDt.wMonth, m_STickInfo[j].sDt.wDay, m_STickInfo[j].sDt.wHour, m_STickInfo[j].sDt.wMinute
				//);
				//DWORD k;
				//for( k = i; k >= i-dwMatch; k-- ){
				//	TICK_MINI_DATA tData;

				//	GetTickMiniData( k, &tData);
				//	DebugPrint( "%d,", tData.dwDiff );
				//}

				//for( k = j; k >= j-dwMatch; k-- ){
				//	TICK_MINI_DATA tData;

				//	GetTickMiniData( k, &tData);
				//	DebugPrint( "%d,", tData.dwDiff );
				//}

				//DebugPrint( "\r\n" );

				dwRet++;
if( dwRet>100){
	return dwRet;
}
			}
		}
	}

	return dwRet;
}
// CCompTickChartDoc コマンド
bool CCompTickChartDoc::MakeSimpleTickInfo( ){
	bool bRet = false;

	//vector<TICK_INFO>::iterator itr = m_TickInfo.begin();

	m_STickInfo.clear();

	//for( ;itr != m_TickInfo.end(); itr++ )
	for( DWORD i = 0; i < m_TickInfo.size(); i++ ){
		int nVal;
		int nDirType;

		nVal = m_TickInfo[i].Op - m_TickInfo[i].Cl;

		if( m_TickInfo[i].Op > m_TickInfo[i].Cl ){
			nDirType = -1;
		}else{
			nDirType = 1;
		}

		if( -10 < nVal && nVal < 10 ){
			nDirType = 0;
			nVal = 0;
		}

		DWORD dwEnd = m_STickInfo.size()-1;
		if( m_STickInfo.size() == 0 || m_STickInfo[dwEnd].nDirectType != nDirType ){
			SIMPLE_TICK_INFO newData;
			memset( &newData, 0x00, sizeof(SIMPLE_TICK_INFO));
			newData.dwId = (dwEnd==(DWORD)-1)?0:dwEnd+1;
			newData.nDirectType = nDirType;
			newData.dwTickCnt = 1;
			newData.nVal = nVal;
			newData.sId = i;
			newData.eId = i;
			memcpy( &newData.sDt, &m_TickInfo[i].sDt, sizeof(SYSTEMTIME) );
			memcpy( &newData.eDt, &m_TickInfo[i].eDt, sizeof(SYSTEMTIME) );

			m_STickInfo.push_back( newData );
		}else{
			m_STickInfo[dwEnd].dwTickCnt++;
			m_STickInfo[dwEnd].nVal += nVal;
			m_STickInfo[dwEnd].eId = i;
			memcpy( &m_STickInfo[dwEnd].eDt, &m_TickInfo[i].eDt, sizeof(SYSTEMTIME) );
		}
	}

	if (m_STickInfo.size() > 0) {
		bRet = true;
	}

	return bRet;
}

bool CCompTickChartDoc::ReadTickFile( const char* pTickFilePath ) {
	bool bRet = false;

	HANDLE hFile;
	TICK_INFO readata;
	DWORD dwRead;
	char cPath[1024];
	DWORD i=0;

	if( pTickFilePath != NULL ){
		lstrcpy( cPath, pTickFilePath );
	}else{
		lstrcpy( cPath, GetPathName() );
	}

	char* pExt;

	pExt = (char*)_mbsrchr((unsigned char*)cPath, (unsigned int)'.');

	
	m_TickInfo.clear();

	//m_dwTickType = GetTickTypeFromStrI( cPath );
	m_dwPair = GetPairIdFromStrI( cPath );
	m_dSpread = GetSpread( m_dwPair )/2;


	if ((hFile = CreateFile(cPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL))
		!= INVALID_HANDLE_VALUE
		) {
		SetFilePointer(hFile, 0, 0, FILE_BEGIN);

		while (ReadFile(hFile, &readata, sizeof(TICK_INFO), &dwRead, NULL) != FALSE && sizeof(TICK_INFO) == dwRead) {
			if( readata.Ver <= 2 ){
				readata.Op = Rate2EVal((*(double*)(&readata.Op)), m_dwPair);
				readata.Cl = Rate2EVal((*(double*)(&readata.Cl)), m_dwPair);
				readata.Hi = Rate2EVal((*(double*)(&readata.Hi)), m_dwPair);
				readata.Lo = Rate2EVal((*(double*)(&readata.Lo)), m_dwPair);
				m_TickInfo.push_back(readata);
			}else{
				if( ABS_CMP(readata.Hi, readata.Lo, 100) < 0
					&& ABS_CMP(readata.Op, readata.Op, 100) < 0
				){
					m_TickInfo.push_back(readata);
				}
			}
		}
		CloseHandle(hFile);
	}

	//DWORD dwEnd = m_TickInfo.size()-1;


	//for( DWORD i = dwEnd; i > 0; i-- ){
	//	for( DWORD j = i-1; j > 0; j-- ){
	//		DWORD dwMatch;
	//		for( dwMatch=0;; dwMatch++ ){
	//			if(  j-dwMatch > 0 ){
	//				if( MatchTickData( i-dwMatch, j-dwMatch ) == FALSE ){
	//					break;
	//				}
	//			}else{
	//				break;
	//			}
	//		}

	//		if( dwMatch >= 12 ){
	//			DebugPrint( "%d,%02d/%02d %02d:%02d,%02d/%02d %02d:%02d,", dwMatch, 
	//				m_TickInfo[i].sDt.wMonth, m_TickInfo[i].sDt.wDay, m_TickInfo[i].sDt.wHour, m_TickInfo[i].sDt.wMinute,
	//				m_TickInfo[j].sDt.wMonth, m_TickInfo[j].sDt.wDay, m_TickInfo[j].sDt.wHour, m_TickInfo[j].sDt.wMinute
	//			);
	//			DWORD k;
	//			for( k = i; k >= i-dwMatch; k-- ){
	//				TICK_MINI_DATA tData;

	//				GetTickMiniData( k, &tData);
	//				DebugPrint( "%d,", tData.dwDiff );
	//			}

	//			//for( k = j; k >= j-dwMatch; k-- ){
	//			//	TICK_MINI_DATA tData;

	//			//	GetTickMiniData( k, &tData);
	//			//	DebugPrint( "%d,", tData.dwDiff );
	//			//}

	//			DebugPrint( "\r\n" );
	//		}
	//	}
	//}

	if (m_TickInfo.size() > 0) {
		bRet = true;
	}

	return bRet;
}
