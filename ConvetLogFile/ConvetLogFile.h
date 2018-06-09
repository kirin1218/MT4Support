
// ConvetLogFile.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CConvetLogFileApp:
// このクラスの実装については、ConvetLogFile.cpp を参照してください。
//

class CConvetLogFileApp : public CWinApp
{
public:
	CConvetLogFileApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CConvetLogFileApp theApp;