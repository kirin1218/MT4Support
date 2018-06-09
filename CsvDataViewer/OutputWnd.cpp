
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �^�u�t���E�B���h�E�̍쐬:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("�^�u�t���o�̓E�B���h�E���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}

	// �o�̓y�C���̍쐬:
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_VSCROLL | LVS_REPORT;
	CRect rect;
	GetClientRect(&rect);

	m_TradeInfoList.Create(dwStyle, rect, &m_wndTabs, 77777);
	LVCOLUMN    lvc;
	char		cCapTrade[][32] = { "�J�n����", "�y�A", "�������@", "�J�n", "�I��", "����", "�I������", "�J�n���R", "�I�����R", "" };
	DWORD		dwWdthTrade[] = { 120, 80, 80, 100, 100, 100, 120, 200, 200, 0 };
	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_FMT;    // �L���t���O

	for( DWORD i = 0; cCapTrade[i][0] != 0x00; i++) {
		lvc.iSubItem = i;            // �T�u�A�C�e���ԍ�
		lvc.pszText = cCapTrade[i];   // ���o���e�L�X�g
		lvc.cx = dwWdthTrade[i];          // ����
		lvc.fmt = LVCFMT_RIGHT;

		m_TradeInfoList.InsertColumn(i, &lvc);
	}

	DWORD dwExStyle = m_TradeInfoList.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;

	m_TradeInfoList.SetExtendedStyle(dwExStyle);

	m_CmpGraphList.Create(dwStyle, rect, &m_wndTabs, 8888);
	char		cCapCmpG[][32] = { "No", "�J�n�@", "�I���@", "�J�n�A", "�I���B", "��v��", "�s��v��" };
	DWORD		dwWdthCmpG[] = { 80, 200, 200, 200, 200, 50, 50, 0 };
	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_FMT;    // �L���t���O

	for( DWORD i = 0; cCapCmpG[i][0] != 0x00; i++) {
		lvc.iSubItem = i;            // �T�u�A�C�e���ԍ�
		lvc.pszText = cCapCmpG[i];   // ���o���e�L�X�g
		lvc.cx = dwWdthCmpG[i];          // ����
		lvc.fmt = LVCFMT_RIGHT;

		m_CmpGraphList.InsertColumn(i, &lvc);
	}

	dwExStyle = m_CmpGraphList.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;

	m_CmpGraphList.SetExtendedStyle(dwExStyle);

	m_RateChkInfoList.Create(dwStyle, rect, &m_wndTabs, 8888);
	char		cCapRCI[][32] = { "�^�C�v", "Rate", "�J�n", "�I��", "" };
	DWORD		dwWdthRCI[] = { 200, 100, 200, 200, 0 };
	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_FMT;    // �L���t���O

	for( DWORD i = 0; cCapRCI[i][0] != 0x00; i++) {
		lvc.iSubItem = i;            // �T�u�A�C�e���ԍ�
		lvc.pszText = cCapRCI[i];   // ���o���e�L�X�g
		lvc.cx = dwWdthRCI[i];          // ����
		lvc.fmt = LVCFMT_RIGHT;

		m_RateChkInfoList.InsertColumn(i, &lvc);
	}

	dwExStyle = m_RateChkInfoList.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;

	m_RateChkInfoList.SetExtendedStyle(dwExStyle);

	//if (!m_wndOutputBuild.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
	//	!m_wndOutputDebug.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
	//	!m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs, 4))
	//{
	//	TRACE0("�o�̓E�B���h�E���쐬�ł��܂���ł���\n");
	//	return -1;      // �쐬�ł��Ȃ��ꍇ
	//}

	UpdateFonts();

	CString strTabName;
	BOOL bNameValid;

	//// �ꗗ�E�B���h�E���^�u�Ɋ��蓖�Ă܂�:
	bNameValid = strTabName.LoadString(IDS_TRADE_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_TradeInfoList, strTabName, (UINT)0);
	bNameValid = strTabName.LoadString(IDS_COMPGRAPH_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_CmpGraphList, strTabName, (UINT)1);
	bNameValid = strTabName.LoadString(IDS_RATECHK_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_RateChkInfoList, strTabName, (UINT)2);
	//bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	//ASSERT(bNameValid);
	//m_wndTabs.AddTab(&m_wndOutputFind, strTabName, (UINT)2);

	// �o�̓^�u�Ƀ_�~�[ �e�L�X�g����͂��܂�
	//FillBuildWindow();
	//FillDebugWindow();
	//FillFindWindow();

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// �^�u �R���g���[���́A�N���C�A���g�̈�S�̂��J�o�[����K�v������܂�:
	//m_TradeInfoList.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

//void COutputWnd::FillBuildWindow()
//{
//	m_wndOutputBuild.AddString(_T("�r���h�o�̓f�[�^�������ɕ\������܂��B"));
//	m_wndOutputBuild.AddString(_T("�o�̓f�[�^�̓��X�g �r���[�̊e�s�ɕ\������܂�"));
//	m_wndOutputBuild.AddString(_T("�\�����@��ύX���邱�Ƃ��ł��܂�..."));
//}
//
//void COutputWnd::FillDebugWindow()
//{
//	m_wndOutputDebug.AddString(_T("�f�o�b�O�o�̓f�[�^�������ɕ\������܂��B"));
//	m_wndOutputDebug.AddString(_T("�o�̓f�[�^�̓��X�g �r���[�̊e�s�ɕ\������܂�"));
//	m_wndOutputDebug.AddString(_T("�\�����@��ύX���邱�Ƃ��ł��܂�..."));
//}
//
//void COutputWnd::FillFindWindow()
//{
//	m_wndOutputFind.AddString(_T("�����o�̓f�[�^�������ɕ\������܂��B"));
//	m_wndOutputFind.AddString(_T("�o�̓f�[�^�̓��X�g �r���[�̊e�s�ɕ\������܂�"));
//	m_wndOutputFind.AddString(_T("�\�����@��ύX���邱�Ƃ��ł��܂�..."));
//}
//
void COutputWnd::UpdateFonts()
{
	m_TradeInfoList.SetFont(&afxGlobalData.fontRegular);
	//m_wndOutputDebug.SetFont(&afxGlobalData.fontRegular);
	//m_wndOutputFind.SetFont(&afxGlobalData.fontRegular);
}

void COutputWnd::UpdateTradeInfoListCtrl(vector<TRADE_INFO>* pTrInfoList )
{
	
	m_TradeInfoList.DeleteAllItems();
	m_TradeInfoList.vTradeInfo.clear();

	int nMax;
	DWORD i,j;
	int idx;
	LVITEM       lvi;
	char cData[1024];

	if( pTrInfoList->size() > 0) {
		nMax = pTrInfoList->size() - 1;
		//	char		cCapTrade[][32] = { "�J�n����", "�y�A", "�J�n", "�I��", "����", "�I������", "" };

		for( i = 0, j = 0; i < pTrInfoList->size(); i++, j++ ) {
			m_TradeInfoList.vTradeInfo.push_back( pTrInfoList->at(i) );
			j = 0;

			lvi.mask = LVIF_TEXT;
			lvi.iItem = i;
			lvi.iSubItem = j++;
			wsprintf(cData, "%02d/%02d %02d:%02d:%02d", pTrInfoList->at(i).openDt.wMonth, pTrInfoList->at(i).openDt.wDay, pTrInfoList->at(i).openDt.wHour, pTrInfoList->at(i).openDt.wMinute, pTrInfoList->at(i).openDt.wSecond);
			lvi.pszText = cData;
			idx = m_TradeInfoList.InsertItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			lvi.pszText = GetPairName(pTrInfoList->at(i).dwPair);
			m_TradeInfoList.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			lvi.pszText = (pTrInfoList->at(i).dwOrderType==1)?"����":"����";
			m_TradeInfoList.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			if(pTrInfoList->at(i).dwPair == PAIR_EURUSD || pTrInfoList->at(i).dwPair == PAIR_GBPUSD || pTrInfoList->at(i).dwPair == PAIR_USDCHF ){
				_stprintf_s(cData, "%.4f", pTrInfoList->at(i).dOpenPrice);
			}
			else
			{
				_stprintf_s(cData, "%.2f", pTrInfoList->at(i).dOpenPrice);
			}
			lvi.pszText = cData;
			m_TradeInfoList.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			if(pTrInfoList->at(i).dClosePrice > 0) {
				if(pTrInfoList->at(i).dwPair == PAIR_EURUSD || pTrInfoList->at(i).dwPair == PAIR_GBPUSD || pTrInfoList->at(i).dwPair == PAIR_USDCHF ) {
					_stprintf_s(cData, "%.4f", pTrInfoList->at(i).dClosePrice);
				}
				else
				{
					_stprintf_s(cData, "%.2f", pTrInfoList->at(i).dClosePrice);
				}
			}
			else {
				cData[0] = 0x00;
			}
			lvi.pszText = cData;
			m_TradeInfoList.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			if(pTrInfoList->at(i).dClosePrice > 0) {
				DWORD PipsA, PipsB;

				//PipsA = EVal2Pips(Rate2EVal(pTrInfoList->at(i).dOpenPrice, pTrInfoList->at(i).dwPair));
				//PipsB = EVal2Pips(Rate2EVal(pTrInfoList->at(i).dClosePrice, pTrInfoList->at(i).dwPair));
				PipsA = Rate2EVal(pTrInfoList->at(i).dOpenPrice, pTrInfoList->at(i).dwPair);
				PipsB = Rate2EVal(pTrInfoList->at(i).dClosePrice, pTrInfoList->at(i).dwPair);

				if(pTrInfoList->at(i).dwOrderType == 1 ){
					if( PipsA <= PipsB ) {
						_stprintf_s(cData, "%d", PipsB - PipsA );
					}
					else
					{
						_stprintf_s(cData, "- %d", PipsA - PipsB );
					}
				}else {
					if(PipsA >= PipsB) {
						_stprintf_s(cData, "%d", PipsA - PipsB);
					}
					else
					{
						_stprintf_s(cData, "- %d", PipsB - PipsA);
					}
				}
			}
			else {
				cData[0] = 0x00;
			}
			lvi.pszText = cData;
			m_TradeInfoList.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			if(pTrInfoList->at(i).dClosePrice > 0) {
				wsprintf(cData, "%02d/%02d %02d:%02d:%02d", pTrInfoList->at(i).closeDt.wMonth, pTrInfoList->at(i).closeDt.wDay, pTrInfoList->at(i).closeDt.wHour, pTrInfoList->at(i).closeDt.wMinute, pTrInfoList->at(i).closeDt.wSecond);
				//wsprintf(cData, "%02d:%02d:%02d", pTrInfoList->at(i).closeDt.wHour, pTrInfoList->at(i).closeDt.wMinute, pTrInfoList->at(i).closeDt.wSecond);
			}
			else {
				cData[0] = 0x00;
			}
			lvi.pszText = cData;
			m_TradeInfoList.SetItem(&lvi);

		
			lvi.iItem = idx;
			lvi.iSubItem = j++;
			cData[0] = 0x00;
			if( pTrInfoList->at(i).dwBreakTrigger & BREAKSIGN_BY_DOUJIBREAK ){
				lstrcat( cData, "�������u���C�N " );
			}
			if( pTrInfoList->at(i).dwBreakTrigger & BREAKSIGN_BY_EMABREAK ){
				lstrcat( cData, "EMA�u���C�N" );
			}
			if( pTrInfoList->at(i).dwBreakTrigger & BREAKSIGN_BY_BBBREAK ){
				lstrcat( cData, "�{���W���[�u���C�N" );
			}
			if( pTrInfoList->at(i).dwBreakTrigger & BREAKSIGN_BY_BBBREAK_BW ){
				lstrcat( cData, "BW�u���C�N" );
			}
			if( pTrInfoList->at(i).dwBreakTrigger & BREAKSIGN_BY_BBBREAK_BWBACK ){
				lstrcat( cData, "BW�o�b�N�u���C�N" );
			}
			if( pTrInfoList->at(i).dwBreakTrigger & BREAKSIGN_BY_3SIGMABREAK ){
				lstrcat( cData, "3Sigma�u���C�N" );
			}
			if( pTrInfoList->at(i).dwBreakTrigger & BREAKSIGN_BY_SOBREAK ){
				lstrcat( cData, "SO�u���C�N" );
			}
			if( pTrInfoList->at(i).dwBreakTrigger & BREAKSIGN_BY_RSIBREAK ){
				lstrcat( cData, "RSI�u���C�N" );
			}
			if( pTrInfoList->at(i).dwBreakTrigger & BREAKSIGN_BY_ICHIMOKUBREAK ){
				lstrcat( cData, "��ڃu���C�N" );
			}
			lvi.pszText = cData;
			m_TradeInfoList.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			cData[0] = 0x00;
			if( pTrInfoList->at(i).dwCloseSign & CLOSESIGN_BY_BBBREAK_BW_OVERSIGMA3 ){
				lstrcat( cData, "Sigma3�𒴂������炻�낻�뗎����͂� " );
			}
			if( pTrInfoList->at(i).dwCloseSign & CLOSESIGN_BY_BBBREAK_BW_CHANGESIGMA ){
				lstrcat( cData, "�t��Sigma���������փV�t�g" );
			}
			if( pTrInfoList->at(i).dwCloseSign & CLOSESIGN_LOSSCUT ){
				lstrcat( cData, "2pips���X�J�b�g" );
			}
			if( pTrInfoList->at(i).dwCloseSign & CLOSESIGN_ZEROBACK ){
				lstrcat( cData, "+-0�n�_�։�A" );
			}
			if( pTrInfoList->at(i).dwCloseSign & CLOSESIGN_PROFIT ){
				lstrcat( cData, "���v�m��" );
			}
			lvi.pszText = cData;
			m_TradeInfoList.SetItem(&lvi);

			m_TradeInfoList.SetItemData( idx , i );

		}
	}
	return;
}


void COutputWnd::UpdateRateChkInfoListCtrl(vector<RATE_CHECK_INFO>* pRateChkInfoList )
{
	
	m_RateChkInfoList.DeleteAllItems();
	m_RateChkInfoList.vRateChkInfo.clear();

	int nMax;
	DWORD i,j;
	int idx;
	LVITEM       lvi;
	char cData[1024];

	if( pRateChkInfoList->size() > 0) {
		nMax = pRateChkInfoList->size() - 1;
		//	char		cCapTrade[][32] = { "�J�n����", "�y�A", "�J�n", "�I��", "����", "�I������", "" };

		for( i = 0, j = 0; i < pRateChkInfoList->size(); i++, j++ ) {
			m_RateChkInfoList.vRateChkInfo.push_back( pRateChkInfoList->at(i) );
			j = 0;

			//	�^�C�v
			lvi.mask = LVIF_TEXT;
			lvi.iItem = i;
			lvi.iSubItem = j++;
			cData[0] = 0x00;
			if( pRateChkInfoList->at(i).dwChkType & RATECHK_SUDDEN_UP ){
				lstrcat( cData, "RATECHK_SUDDEN_UP" );
			}
			if( pRateChkInfoList->at(i).dwChkType & RATECHK_OVER_VAL ){
				lstrcat( cData, "RATECHK_OVER_VAL" );
			}
			if( pRateChkInfoList->at(i).dwChkType & RATECHK_RANGECHK ){
				lstrcat( cData, "RATECHK_RANGECHK" );
			}
			if( pRateChkInfoList->at(i).dwChkType & RATECHK_OVER_MAX ){
				lstrcat( cData, "RATECHK_OVER_MAX" );
			}
			if( pRateChkInfoList->at(i).dwChkType & RATECHK_OVER_3SIGMA ){
				lstrcat( cData, "BRATECHK_OVER_3SIGMA" );
			}
			if( pRateChkInfoList->at(i).dwChkType & RATECHK_ALLTICK_UP ){
				lstrcat( cData, "RATECHK_ALLTICK_UP" );
			}
			lvi.pszText = cData;
			idx = m_RateChkInfoList.InsertItem(&lvi);

			//	Rate
			lvi.iItem = idx;
			lvi.iSubItem = j++;
			wsprintf(cData, "%d", pRateChkInfoList->at(i).dwChkRate );
			lvi.pszText = cData;
			m_RateChkInfoList.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			wsprintf(cData, "%02d/%02d %02d:%02d:%02d", pRateChkInfoList->at(i).sDt.wMonth, pRateChkInfoList->at(i).sDt.wDay, pRateChkInfoList->at(i).sDt.wHour, pRateChkInfoList->at(i).sDt.wMinute, pRateChkInfoList->at(i).sDt.wSecond);
			lvi.pszText = cData;
			m_RateChkInfoList.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			wsprintf(cData, "%02d/%02d %02d:%02d:%02d", pRateChkInfoList->at(i).eDt.wMonth, pRateChkInfoList->at(i).eDt.wDay, pRateChkInfoList->at(i).eDt.wHour, pRateChkInfoList->at(i).eDt.wMinute, pRateChkInfoList->at(i).eDt.wSecond);
			lvi.pszText = cData;
			m_RateChkInfoList.SetItem(&lvi);
			
			m_RateChkInfoList.SetItemData( idx , i );

		}
	}
	return;
}

void COutputWnd::UpdateCompSTickInfoListCtrl(vector<COMP_STICK_LIST>* pSTickInfoList )
{
	
	m_CmpGraphList.DeleteAllItems();
	m_CmpGraphList.vCmpSTickInfo.clear();

	int nMax;
	DWORD i,j;
	int idx;
	LVITEM       lvi;
	char cData[1024];

	if( pSTickInfoList->size() > 0) {
		nMax = pSTickInfoList->size() - 1;
		//	char		cCapTrade[][32] = { "�J�n����", "�y�A", "�J�n", "�I��", "����", "�I������", "" };

		for( i = 0, j = 0; i < pSTickInfoList->size(); i++, j++ ) {
			m_CmpGraphList.vCmpSTickInfo.push_back( pSTickInfoList->at(i) );
			j = 0;

			lvi.mask = LVIF_TEXT;
			lvi.iItem = i;
			lvi.iSubItem = j++;
			wsprintf(cData, "%d", i );
			lvi.pszText = cData;
			idx = m_CmpGraphList.InsertItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			wsprintf(cData, "%02d/%02d %02d:%02d:%02d", pSTickInfoList->at(i).csi1.sDt.wMonth, pSTickInfoList->at(i).csi1.sDt.wDay, pSTickInfoList->at(i).csi1.sDt.wHour, pSTickInfoList->at(i).csi1.sDt.wMinute, pSTickInfoList->at(i).csi1.sDt.wSecond);
			lvi.pszText = cData;
			m_CmpGraphList.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			wsprintf(cData, "%02d/%02d %02d:%02d:%02d", pSTickInfoList->at(i).csi1.eDt.wMonth, pSTickInfoList->at(i).csi1.eDt.wDay, pSTickInfoList->at(i).csi1.eDt.wHour, pSTickInfoList->at(i).csi1.eDt.wMinute, pSTickInfoList->at(i).csi1.eDt.wSecond);
			lvi.pszText = cData;
			m_CmpGraphList.SetItem(&lvi);


			lvi.iItem = idx;
			lvi.iSubItem = j++;
			wsprintf(cData, "%02d/%02d %02d:%02d:%02d", pSTickInfoList->at(i).csi2.sDt.wMonth, pSTickInfoList->at(i).csi2.sDt.wDay, pSTickInfoList->at(i).csi2.sDt.wHour, pSTickInfoList->at(i).csi2.sDt.wMinute, pSTickInfoList->at(i).csi2.sDt.wSecond);
			lvi.pszText = cData;
			m_CmpGraphList.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			wsprintf(cData, "%02d/%02d %02d:%02d:%02d", pSTickInfoList->at(i).csi2.eDt.wMonth, pSTickInfoList->at(i).csi2.eDt.wDay, pSTickInfoList->at(i).csi2.eDt.wHour, pSTickInfoList->at(i).csi2.eDt.wMinute, pSTickInfoList->at(i).csi2.eDt.wSecond);
			lvi.pszText = cData;
			m_CmpGraphList.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			wsprintf(cData, "%d", pSTickInfoList->at(i).dwMatch );
			lvi.pszText = cData;
			m_CmpGraphList.SetItem(&lvi);

			lvi.iItem = idx;
			lvi.iSubItem = j++;
			wsprintf(cData, "%d", pSTickInfoList->at(i).dwMinusPnt );
			lvi.pszText = cData;
			m_CmpGraphList.SetItem(&lvi);

			m_CmpGraphList.SetItemData( idx , i );

		}
	}
	return;
}
/////////////////////////////////////////////////////////////////////////////
// COutputList1

CTradeListCtrl::CTradeListCtrl()
{
}

CTradeListCtrl::~CTradeListCtrl()
{
}

BEGIN_MESSAGE_MAP(CTradeListCtrl, CListCtrl)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CTradeListCtrl::OnLvnItemchanged)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CTradeListCtrl ���b�Z�[�W �n���h���[

void CTradeListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void CTradeListCtrl::OnEditCopy()
{
	MessageBox(_T("�o�̓f�[�^���R�s�[���܂�"));
}

void CTradeListCtrl::OnEditClear()
{
	MessageBox(_T("�o�̓f�[�^���N���A���܂�"));
}

void CTradeListCtrl::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}

void CTradeListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVIS_SELECTED))
    {
		DWORD dwItemData = GetItemData( pNMListView->iItem );
		

		CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

		if(pMainFrame != NULL )
		{
			((CMainFrame*)pMainFrame)->ViewShiftSelectTrade( &vTradeInfo[dwItemData].openDt, &vTradeInfo[dwItemData].closeDt );

		}
    }

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CRateChkListCtrl

CRateChkListCtrl::CRateChkListCtrl()
{
}

CRateChkListCtrl::~CRateChkListCtrl()
{
}

BEGIN_MESSAGE_MAP(CRateChkListCtrl, CListCtrl)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CRateChkListCtrl::OnLvnItemchanged)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CRateChkListCtrl ���b�Z�[�W �n���h���[

void CRateChkListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void CRateChkListCtrl::OnEditCopy()
{
	MessageBox(_T("�o�̓f�[�^���R�s�[���܂�"));
}

void CRateChkListCtrl::OnEditClear()
{
	MessageBox(_T("�o�̓f�[�^���N���A���܂�"));
}

void CRateChkListCtrl::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}

void CRateChkListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVIS_SELECTED))
    {
		DWORD dwItemData = GetItemData( pNMListView->iItem );
		

		CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

		if(pMainFrame != NULL )
		{
			((CMainFrame*)pMainFrame)->ViewShiftSelectRCI( vRateChkInfo[dwItemData].dwChkType, vRateChkInfo[dwItemData].dwChkRate, &vRateChkInfo[dwItemData].sDt, &vRateChkInfo[dwItemData].eDt );

		}
    }

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCmpGListCtrl

CCmpGListCtrl::CCmpGListCtrl()
{
}

CCmpGListCtrl::~CCmpGListCtrl()
{
}

BEGIN_MESSAGE_MAP(CCmpGListCtrl, CListCtrl)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CCmpGListCtrl::OnLvnItemchanged)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CCmpGListCtrl ���b�Z�[�W �n���h���[

void CCmpGListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void CCmpGListCtrl::OnEditCopy()
{
	MessageBox(_T("�o�̓f�[�^���R�s�[���܂�"));
}

void CCmpGListCtrl::OnEditClear()
{
	MessageBox(_T("�o�̓f�[�^���N���A���܂�"));
}

void CCmpGListCtrl::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}

void CCmpGListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVIS_SELECTED))
    {
		DWORD dwItemData = GetItemData( pNMListView->iItem );
		

		CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

		if(pMainFrame != NULL )
		{
			((CMainFrame*)pMainFrame)->ViewShiftSelectCmpSTick( &vCmpSTickInfo[dwItemData] );

		}
    }

	*pResult = 0;
}