#include "stdafx.h"
#include "Bmp2Jpeg.h"


CBmp2Jpeg::CBmp2Jpeg()
{
}


CBmp2Jpeg::~CBmp2Jpeg()
{
}

//GDI+で必要
#include <shlobj.h>
#include <gdiplus.h>
#include <Gdiplusinit.h>


using namespace Gdiplus;

// helper function
INT GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0; // number of image encoders
	UINT size = 0; // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1; // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1; // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j) {
		if(wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j; // Success
		}
	}

	free(pImageCodecInfo);
	return -1; // Failure
}


BOOL CBmp2Jpeg::ConvBmp2Jpeg( LPTSTR pBmp, LPTSTR pJpg )
{
	BOOL bRet = FALSE;

	//GDI+の初期化
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	CLSID	encoderClsid;
	//１つしかパラメータを指定しない場合はこちら
	//EncoderParameters encoderParameters;
	//複数のパラメータを指定する場合はこちら
	EncoderParameters* encoderParameters = (EncoderParameters*)malloc(sizeof(EncoderParameters) + 4 * sizeof(EncoderParameter));
	ULONG	colordepth;
	ULONG	quality;
	ULONG	transformation;
	Status	stat;

#ifdef  UNICODE 
	// ファイルを指定してオブジェクトを作成.
	Image* image = new Image( pBmp );
#else
	WCHAR wBmp[1024];
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pBmp, -1, wBmp, sizeof(wBmp) );
	// ファイルを指定してオブジェクトを作成.
	Image* image = new Image( wBmp );
#endif

	//画像のフォーマット
	GUID guid;
	image->GetRawFormat(&guid);

	//サイズ
	UINT width = image->GetWidth();
	UINT height = image->GetHeight();

	//解像度
	REAL res_hori = image->GetHorizontalResolution();
	REAL res_ver = image->GetVerticalResolution();

	//ファイルタイプ
	ImageType type = image->GetType();

	//カラーフォーマット
	PixelFormat pixelFormat = image->GetPixelFormat();

	//保存するときのエンコーダをJPGに設定
	GetEncoderClsid(L"image/jpeg", &encoderClsid);

	//パラメータの数を指定
	encoderParameters->Count = 3;
	//色深度
	encoderParameters->Parameter[2].Guid = EncoderColorDepth;
	encoderParameters->Parameter[2].Type = EncoderParameterValueTypeLong;
	encoderParameters->Parameter[2].NumberOfValues = 1;
	colordepth = 24;
	encoderParameters->Parameter[2].Value = &colordepth;
	//クオリティの設定
	encoderParameters->Parameter[1].Guid = EncoderQuality;
	encoderParameters->Parameter[1].Type = EncoderParameterValueTypeLong;
	encoderParameters->Parameter[1].NumberOfValues = 1;
	quality = 80;	//0～100
	encoderParameters->Parameter[1].Value = &quality;
	//回転の設定
	encoderParameters->Parameter[0].Guid = EncoderTransformation;
	encoderParameters->Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters->Parameter[0].NumberOfValues = 1;
	transformation = EncoderValueTransformRotate90;
	encoderParameters->Parameter[0].Value = &transformation;


	//各種処理をして保存
#ifdef  UNICODE 
	stat = image->Save( pJpg, &encoderClsid, encoderParameters);
#else
	WCHAR wJpg[1024];
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pJpg, -1, wJpg, sizeof(wJpg) );
	stat = image->Save( wJpg, &encoderClsid, encoderParameters);
#endif
	

	//結果
	if(stat == Ok) {
		bRet = TRUE;
	}
	//オブジェクトの破棄
	delete image;

	free(encoderParameters);

	//GDI+の終了処理
	GdiplusShutdown(gdiplusToken);

	return bRet;
}