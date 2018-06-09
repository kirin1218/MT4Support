#include "stdafx.h"
#include "Bmp2Jpeg.h"


CBmp2Jpeg::CBmp2Jpeg()
{
}


CBmp2Jpeg::~CBmp2Jpeg()
{
}

//GDI+�ŕK�v
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

	//GDI+�̏�����
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	CLSID	encoderClsid;
	//�P�����p�����[�^���w�肵�Ȃ��ꍇ�͂�����
	//EncoderParameters encoderParameters;
	//�����̃p�����[�^���w�肷��ꍇ�͂�����
	EncoderParameters* encoderParameters = (EncoderParameters*)malloc(sizeof(EncoderParameters) + 4 * sizeof(EncoderParameter));
	ULONG	colordepth;
	ULONG	quality;
	ULONG	transformation;
	Status	stat;

#ifdef  UNICODE 
	// �t�@�C�����w�肵�ăI�u�W�F�N�g���쐬.
	Image* image = new Image( pBmp );
#else
	WCHAR wBmp[1024];
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pBmp, -1, wBmp, sizeof(wBmp) );
	// �t�@�C�����w�肵�ăI�u�W�F�N�g���쐬.
	Image* image = new Image( wBmp );
#endif

	//�摜�̃t�H�[�}�b�g
	GUID guid;
	image->GetRawFormat(&guid);

	//�T�C�Y
	UINT width = image->GetWidth();
	UINT height = image->GetHeight();

	//�𑜓x
	REAL res_hori = image->GetHorizontalResolution();
	REAL res_ver = image->GetVerticalResolution();

	//�t�@�C���^�C�v
	ImageType type = image->GetType();

	//�J���[�t�H�[�}�b�g
	PixelFormat pixelFormat = image->GetPixelFormat();

	//�ۑ�����Ƃ��̃G���R�[�_��JPG�ɐݒ�
	GetEncoderClsid(L"image/jpeg", &encoderClsid);

	//�p�����[�^�̐����w��
	encoderParameters->Count = 3;
	//�F�[�x
	encoderParameters->Parameter[2].Guid = EncoderColorDepth;
	encoderParameters->Parameter[2].Type = EncoderParameterValueTypeLong;
	encoderParameters->Parameter[2].NumberOfValues = 1;
	colordepth = 24;
	encoderParameters->Parameter[2].Value = &colordepth;
	//�N�I���e�B�̐ݒ�
	encoderParameters->Parameter[1].Guid = EncoderQuality;
	encoderParameters->Parameter[1].Type = EncoderParameterValueTypeLong;
	encoderParameters->Parameter[1].NumberOfValues = 1;
	quality = 80;	//0�`100
	encoderParameters->Parameter[1].Value = &quality;
	//��]�̐ݒ�
	encoderParameters->Parameter[0].Guid = EncoderTransformation;
	encoderParameters->Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters->Parameter[0].NumberOfValues = 1;
	transformation = EncoderValueTransformRotate90;
	encoderParameters->Parameter[0].Value = &transformation;


	//�e�폈�������ĕۑ�
#ifdef  UNICODE 
	stat = image->Save( pJpg, &encoderClsid, encoderParameters);
#else
	WCHAR wJpg[1024];
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pJpg, -1, wJpg, sizeof(wJpg) );
	stat = image->Save( wJpg, &encoderClsid, encoderParameters);
#endif
	

	//����
	if(stat == Ok) {
		bRet = TRUE;
	}
	//�I�u�W�F�N�g�̔j��
	delete image;

	free(encoderParameters);

	//GDI+�̏I������
	GdiplusShutdown(gdiplusToken);

	return bRet;
}