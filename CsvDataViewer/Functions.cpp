#include "stdafx.h"
#include <windows.h>

void DebugPrint( char* pszFormat, ...)
{
	va_list	argp;
	char pszBuf[1024];

	va_start(argp, pszFormat);
	vsprintf_s(pszBuf, pszFormat, argp);
	va_end(argp);

	//	ExeÇÃÉJÉåÉìÉgÇ…èoóÕ
	char cFilePath[1024];
	char* pFileName;

	GetModuleFileName( NULL, cFilePath, sizeof(cFilePath) );
	pFileName = (char*)_mbsrchr((unsigned char*)cFilePath, (unsigned int)'\\');

	lstrcpy(pFileName, "\\DebugPrint.txt");

	HANDLE hFile;
	DWORD dwWrite;


	if((hFile = CreateFile(cFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE) {
		SetFilePointer(hFile, 0, 0, FILE_END);

		WriteFile(hFile, pszBuf, lstrlen(pszBuf), &dwWrite, NULL);

		CloseHandle(hFile);
	}

	return;

}