// PTView.h : interface of the CPTView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTVIEW_H__30AD780C_2FB1_11D6_A444_444553540000__INCLUDED_)
#define AFX_PTVIEW_H__30AD780C_2FB1_11D6_A444_444553540000__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPTView : public CView
{
protected: // create from serialization only
	CPTView();
	DECLARE_DYNCREATE(CPTView)

// Attributes
public:
	CPTDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Pause();
	bool m_bPauze;
	int m_nLevSeconden;
	bool m_bNT;
	void UpdateBomb();
	void UpdatePlane();
	void UpdateExplosion();
	void UpdateBullet();
	void UpdateTrooper();
	void UpdateHeli();
	void CreateBomb(CPoint pos);
	CBitmap m_bmpBomb;
	CBitmap m_bmpPlaneToLeftGrey;
	CBitmap m_bmpPlaneToLeftGreen;
	CBitmap m_bmpPlaneToRightGreen;
	CBitmap m_bmpPlaneToRightGrey;
	void CreatePlane();
	void CheckGameOver();
	int CreateRandom(int min,int max);
	CBitmap m_bmpTroopLand;
	CBitmap m_bmpSplatAir;
	CBitmap m_bmpSplatGround;
	CBitmap m_bmpChute;
	CBitmap m_bmpTroopFall;
	void CreateTrooper(CPoint pos);
	void KillAllTimers();
	
	void CreateExplosion(CPoint pos,int flag);
	CBitmap m_bmpHelitoLeftRed;
	CBitmap m_bmpHelitoLeftGreen;
	CBitmap m_bmpHelitoLeftPurp;
	CBitmap m_bmpHelitoRightRed;
	CBitmap m_bmpHelitoRightPurp;
	CBitmap m_bmpHelitoRightGreen;
	CBitmap m_bmpExplosion;
	CBitmap m_bmpExplosion2;
	void CreateHeli();

	CBitmap m_bmpBullet;
	void DrawAllObjects(CDC *pDC);
	void UpdateAll();
	CPoint m_ptGunpos;


	void CreateBullet(double gunpos,CPoint pos);
	void DrawSpString(CDC *pDC,CPoint pos,CString str);
	CBitmap m_bmpGunBody;
	CBitmap m_bmpLifes;
	void DrawSpChar(CDC *pDC,CPoint pos,int ch);
	void DrawHud(CDC *pDC,CPoint pos,CPTGameInfo GI);
	
	void DrawBkgr(CDC *pDC);
	void DrawGunBody(CDC *pDC,CPoint pos);
	CBitmap m_bmpFont;
	CBitmap m_bmpLevelBackground;
	void DrawLoop(CDC *pDC,double m_lGunPos,CPoint pos);
	void DrawTransparentBitmap(CDC* pDC, CBitmap* pBitmap, short xStart,short yStart, COLORREF cTransparentColor);
	CRect GameRect;
	void OnPrepareLevel(int m_nLevel);
	
	virtual ~CPTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPTView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PTView.cpp
inline CPTDoc* CPTView::GetDocument()
   { return (CPTDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTVIEW_H__30AD780C_2FB1_11D6_A444_444553540000__INCLUDED_)
