
// ConvetLogFileDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once

// �C���N���[�h
#include "MT4_Supoort.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>  // ��istringstream���g�����߂ɕK�v
// ���O���
using namespace std;

// CConvetLogFileDlg �_�C�A���O
class CConvetLogFileDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CConvetLogFileDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONVETLOGFILE_DIALOG };
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
	afx_msg void OnBnClickedBtnRefs();

	bool ReadCsvData(const char* filename, vector<vector<string>>& vec_table);
	bool ReadTickFileVer1(char* filepath, vector<TICK_INFO_VER1>& vec_table);
	bool WriteTickFileVer1to2(char* filepath, vector<TICK_INFO_VER1>& vec_table);
	bool WriteTickFileVer0to2(char* filepath, vector<vector<string>>& vec_table);
};
