#pragma once
#include <vector>
using namespace std;

class CMakeBitmap
{
private:
	DWORD _pixel_num_x;
	DWORD _pixel_num_y;
	unsigned short _color_bit;
	DWORD _pic_data_size;
	PBYTE _pPictData;
	HANDLE _hFile;
public:
	CMakeBitmap( DWORD dwPixel_X= 256, DWORD dwPixel_Y = 256, unsigned short Color = 24 ){
		_pixel_num_x = dwPixel_X;
		_pixel_num_y = dwPixel_Y;
		_color_bit = Color;
		_pic_data_size = _pixel_num_x * 3 * _pixel_num_y;

		_pPictData = (PBYTE)LocalAlloc(LMEM_FIXED, _pic_data_size);
	}

	~CMakeBitmap();

	BOOL CreateBitMap( LPTSTR pPath, vector<vector<COLORREF>>& vImg );
	void CreateBmpPict( vector<vector<COLORREF>>& vImg );
	int WriteBmpHeader( );
	int fputc4LowHigh(unsigned long d );
	int fputc2LowHigh(unsigned short d );
};

