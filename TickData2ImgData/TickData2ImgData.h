
// TickData2ImgData.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// CTickData2ImgDataApp:
// ���̃N���X�̎����ɂ��ẮATickData2ImgData.cpp ���Q�Ƃ��Ă��������B
//

class CTickData2ImgDataApp : public CWinApp
{
public:
	CTickData2ImgDataApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CTickData2ImgDataApp theApp;