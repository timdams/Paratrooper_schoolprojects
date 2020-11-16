#if !defined(AFX_PTOPTIONS_H__D51A0484_3074_11D6_A444_444553540000__INCLUDED_)
#define AFX_PTOPTIONS_H__D51A0484_3074_11D6_A444_444553540000__INCLUDED_

#include "PTGameInfo.h"	// Added by ClassView
#include "BtnST.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTOptions.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPTOptions dialog

class CPTOptions : public CDialog
{
// Construction
public:
	CButtonST m_btnNO;
	CButtonST m_btnOK;
	
	
	CPTOptions(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPTOptions)
	enum { IDD = IDD_OPTIONS };
	CSpinButtonCtrl	m_DiffTeller;
	int		m_nLifes;
	int		m_nPoints;
	CString	m_strName;
	int		m_nLevel;
	int		m_nDifficulty;
	BOOL	m_nSound;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPTOptions)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTOPTIONS_H__D51A0484_3074_11D6_A444_444553540000__INCLUDED_)
