
// TickData2ImgData.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CTickData2ImgDataApp:
// このクラスの実装については、TickData2ImgData.cpp を参照してください。
//

class CTickData2ImgDataApp : public CWinApp
{
public:
	CTickData2ImgDataApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CTickData2ImgDataApp theApp;