
// DebugAW.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CDebugAWApp:
// このクラスの実装については、DebugAW.cpp を参照してください。
//

class CDebugAWApp : public CWinApp
{
public:
	CDebugAWApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CDebugAWApp theApp;