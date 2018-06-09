
// ChildFrm.cpp : CChildFrame �N���X�̎���
//

#include "stdafx.h"
#include "CsvDataViewer.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "MainFrm.h"

extern CMainFrame* g_pMainFrame;

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()

// CChildFrame �R���X�g���N�V����/�f�X�g���N�V����

CChildFrame::CChildFrame()
{
	// TODO: �����o�[�������R�[�h�������ɒǉ����Ă��������B
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs ��ύX���āAWindow �N���X�܂��̓X�^�C����ύX���܂��B
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame �f�f

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame ���b�Z�[�W �n���h���[


void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	if( bActivate != FALSE ){
		char cWndName[256];

		pActivateWnd->GetWindowTextA( cWndName, sizeof(cWndName) );

		if( strcmp( cWndName, "RateGraph" ) != 0 ){
			CView* pView = this->GetActiveView();

			g_pMainFrame->ResetIndicatorTree();
		}
	}
}
