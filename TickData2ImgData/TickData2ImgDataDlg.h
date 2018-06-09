
// TickData2ImgDataDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxwin.h"

#include "MT4_Supoort.h"

// �C���N���[�h
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>  // ��istringstream���g�����߂ɕK�v
// ���O���
using namespace std;

//#define IMG_SIZE		(224)
#define IMG_SIZE		(256)
#define TICKBAR_WIDTH	(3)
#define TICK_SPACE		(1)
#define IMG_DATA_SIZE	((DWORD)((IMG_SIZE+(TICKBAR_WIDTH+TICK_SPACE)-1)/(TICKBAR_WIDTH+TICK_SPACE)))

// CTickData2ImgDataDlg �_�C�A���O
class CTickData2ImgDataDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CTickData2ImgDataDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TICKDATA2IMGDATA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_EdSrc;

	vector<ADF_DATA> m_TickDataList;
	vector<TICK_INFO> m_70TickDataList;
	vector< vector< vector<BYTE> >> m_ImgList;//(IMG_SIZE, vector<BYTE>(IMG_SIZE) );
	vector<BYTE>m_LabelList;
	DWORD MakeTickDataList( std::vector<std::string>& PathList );
	DWORD Make70TickDataList( DWORD dwPair );
	DWORD ConvertTick2Img();
	int GetLabelNo( DWORD dwStart, DWORD dwStartVal );
	vector< vector<COLORREF> > MakeImg( DWORD dwStart, DWORD dwEnd, DWORD dwOverLine, DWORD dwCenter, DWORD dwUnderLine );
	BOOL ImgFileOut();
};
