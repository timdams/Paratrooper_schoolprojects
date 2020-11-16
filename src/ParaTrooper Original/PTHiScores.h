#if !defined(AFX_PTHISCORES_H__D51A0485_3074_11D6_A444_444553540000__INCLUDED_)
#define AFX_PTHISCORES_H__D51A0485_3074_11D6_A444_444553540000__INCLUDED_

#include "PTGameStatistics.h"	// Added by ClassView
#include "BtnST.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTHiScores.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPTHiScores dialog

class CPTHiScores : public CDialog
{
// Construction
public:
	CButtonST m_btnOK;
	CPTHiScores(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CPTHiScores)
	enum { IDD = IDD_HISCORES };
	CString	m_SHOTBULLETS;
	CString	m_TROOPS;
	CString	m_TROOPC;
	CString	m_HELIC;
	CString	m_HELIS;
	CString	m_BOMBC;
	CString	m_BOMBS;
	CString	m_PLANEC;
	CString	m_PLANES;
	CString	m_CHUTES;
	CString	m_TROOPL;
	CString	m_ACCURACY;
	CString	m_SKILL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTHiScores)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPTHiScores)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTHISCORES_H__D51A0485_3074_11D6_A444_444553540000__INCLUDED_)
