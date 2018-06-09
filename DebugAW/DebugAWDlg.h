
// DebugAWDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once

// �C���N���[�h
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>  // ��istringstream���g�����߂ɕK�v
// ���O���
using namespace std;


// CDebugAWDlg �_�C�A���O
class CDebugAWDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CDebugAWDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEBUGAW_DIALOG };
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
	afx_msg void OnBnClickedBtnInit();
	afx_msg void OnRegisterTickInfo();
	afx_msg void OnBnClickedBtnOrdercheck();
	afx_msg void OnBnClickedBtnClosecheck();
	afx_msg void OnBtnRegAsk();
	afx_msg void OnBtnOrderCheck();
	afx_msg void OnBnClickedBtnOrder();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBtnRefs();
	afx_msg void OnBtnRegAskFromFile();

	bool ReadCsvData(const char* filename, vector<vector<string>>& vec_table);
};
