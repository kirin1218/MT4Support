// DocManager2.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDocManager2 ドキュメント

class CDocManager2 : public CDocManager
{
public:
	CDocManager2();
	DECLARE_DYNAMIC(CDocManager2)

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CDocManager2)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDocManager2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	CDocument* OpenDocumentFile(LPCTSTR lpszFileName, BOOL bAddToMRU);
	// メッセージ マップ関数の生成
protected:
	//{{AFX_MSG(CDocManager2)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
};