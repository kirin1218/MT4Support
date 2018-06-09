#include "stdafx.h"
#include "MakeBitmap.h"



CMakeBitmap::~CMakeBitmap()
{
	LocalFree(_pPictData);
}

BOOL CMakeBitmap::CreateBitMap( LPTSTR pPath, vector<vector<COLORREF>>& vImg )
{
	BOOL bRet = FALSE;

	if( _pPictData == NULL )	return FALSE;
	//HANDLE hFile;
	DWORD dwWriteSize;

	if(( _hFile = CreateFile( pPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
		/* 画像生成関数 */
		CreateBmpPict( vImg );

		/* ヘッダの書出 */
		WriteBmpHeader();

		/* bitmapの書出 */
		WriteFile( _hFile, _pPictData, _pic_data_size, &dwWriteSize, NULL );

		CloseHandle(_hFile);
	}

	return bRet;
}

/* 画像生成関数 */
//void createPic(unsigned char *b, vector< vector<BYTE>>& vImg )
void CMakeBitmap::CreateBmpPict( vector<vector<COLORREF>>& vImg )
{
	if( _pPictData == NULL ) return;

	PBYTE p = _pPictData;

	/* データを生成 */
	for(DWORD i = 0;i < _pixel_num_y;i++) {

		/* 1行分のデータを出力 */
		for(DWORD j = 0;j < _pixel_num_x;j++) {
			*p++ = GetRValue(vImg[i][j]);
			*p++ = GetGValue(vImg[i][j]);
			*p++ = GetBValue(vImg[i][j]);
		}
	}
}

/*
	fputc2LowHigh 2バイトデータ書出(下位〜上位)
	
	2バイトのデータを下位〜上位の順で書き出す

	●戻り値
		int:EOF…失敗, EOF以外…成功
	●引数
		unsigned short d:[i] データ
		FILE *s:[i] 出力ストリーム
*/
int CMakeBitmap::fputc2LowHigh(unsigned short d )
{
	DWORD dwWriteSize;
	BYTE byte;
	//putc(d & 0xFF, s);
	byte = (BYTE)(d & 0xFF);
	WriteFile( _hFile, (LPCVOID)&byte, sizeof(BYTE), &dwWriteSize, NULL );
	byte = (BYTE)(d >> CHAR_BIT);
	return WriteFile( _hFile, (LPCVOID)&byte, sizeof(BYTE), &dwWriteSize, NULL );
}

/*
	fputc4LowHigh 4バイトデータ書出(下位〜上位)
	
	4バイトのデータを下位〜上位の順で書き出す

	●戻り値
		int:EOF…失敗, EOF以外…成功
	●引数
		unsigned long d:[i] データ
		FILE *s:[i] 出力ストリーム
*/
int CMakeBitmap::fputc4LowHigh(unsigned long d )
{
	DWORD dwWriteSize;
	BYTE byte;
	byte = (BYTE)(d & 0xFF);
	WriteFile( _hFile, (LPCVOID)&byte, sizeof(BYTE), &dwWriteSize, NULL );
	byte = (BYTE)((d >> CHAR_BIT) & 0xFF);
	WriteFile( _hFile, (LPCVOID)&byte, sizeof(BYTE), &dwWriteSize, NULL );
	byte = (BYTE)((d >> CHAR_BIT * 2) & 0xFF);
	WriteFile( _hFile, (LPCVOID)&byte, sizeof(BYTE), &dwWriteSize, NULL );
	byte = (BYTE)((d >> CHAR_BIT * 3) & 0xFF);
	return WriteFile( _hFile, (LPCVOID)&byte, sizeof(BYTE), &dwWriteSize, NULL );
}



/*
	putBmpHeader BMPヘッダ書出
	
	BMPファイルのヘッダを書き出す

	●戻り値
		int:0…失敗, 1…成功
	●引数
		FILE *s:[i] 出力ストリーム
		int x:[i] 画像Xサイズ(dot, 1〜)
		int y:[i] 画像Yサイズ(dot, 1〜)
		int c:[i] 色ビット数(bit/dot, 1 or 4 or 8 or 24)
*/
int CMakeBitmap::WriteBmpHeader( )
{
	DWORD color; /* 色数 */
	unsigned long int bfOffBits; /* ヘッダサイズ(byte) */
	DWORD dwWriteSize;

	/* 画像サイズが異常の場合,エラーでリターン */
	if(_pixel_num_x <= 0 || _pixel_num_y <= 0) {
		return 0;
	}

	/* 出力ストリーム異常の場合,エラーでリターン */
	if(_hFile == INVALID_HANDLE_VALUE) {
		return 0;
	}

	/* 色数を計算 */
	if(_color_bit == 24) {
		color = 0;
	}
	else {
		color = 1;
		for( DWORD i = 1;i <= _color_bit;i++) {
			color *= 2;
		}
	}

	/* ヘッダサイズ(byte)を計算 */
	/* ヘッダサイズはビットマップファイルヘッダ(14) + ビットマップ情報ヘッダ(40) + 色数 */
	bfOffBits = 14 + 40 + 4 * color;

	/* ビットマップファイルヘッダ(計14byte)を書出 */
	/* 識別文字列 */
	//fputs("BM", s);
	WriteFile( _hFile, "BM", lstrlen("BM"), &dwWriteSize, NULL );

	/* bfSize ファイルサイズ(byte) */
	fputc4LowHigh(bfOffBits + (unsigned long)_pixel_num_x * _pixel_num_y );

	/* bfReserved1 予約領域1(byte) */
	fputc2LowHigh(0);

	/* bfReserved2 予約領域2(byte) */
	fputc2LowHigh(0);

	/* bfOffBits ヘッダサイズ(byte) */
	fputc4LowHigh(bfOffBits);

	/* ビットマップ情報ヘッダ(計40byte) */
	/* biSize 情報サイズ(byte) */
	fputc4LowHigh(40);

	/* biWidth 画像Xサイズ(dot) */
	fputc4LowHigh( _pixel_num_x);

	/* biHeight 画像Yサイズ(dot) */
	fputc4LowHigh( _pixel_num_y);

	/* biPlanes 面数 */
	fputc2LowHigh(1);

	/* biBitCount 色ビット数(bit/dot) */
	fputc2LowHigh( _color_bit);

	/* biCompression 圧縮方式 */
	fputc4LowHigh(0);

	/* biSizeImage 圧縮サイズ(byte) */
	fputc4LowHigh(0);

	/* biXPelsPerMeter 水平解像度(dot/m) */
	fputc4LowHigh(0);

	/* biYPelsPerMeter 垂直解像度(dot/m) */
	fputc4LowHigh(0);

	/* biClrUsed 色数 */
	fputc4LowHigh(0);

	/* biClrImportant 重要色数 */
	fputc4LowHigh(0);

	/* 書出失敗ならエラーでリターン */
	//if (ferror(s)) {
	//	return 0;
	//}

	/* 成功でリターン */
	return 1;
}

