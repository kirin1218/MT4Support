// DocManager2.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDocManager2 �h�L�������g

class CDocManager2 : public CDocManager
{
public:
	CDocManager2();
	DECLARE_DYNAMIC(CDocManager2)

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CDocManager2)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CDocManager2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	CDocument* OpenDocumentFile(LPCTSTR lpszFileName, BOOL bAddToMRU);
	// ���b�Z�[�W �}�b�v�֐��̐���
protected:
	//{{AFX_MSG(CDocManager2)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG
};