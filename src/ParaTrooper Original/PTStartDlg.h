#if !defined(AFX_PTSTARTDLG_H__518D7DA1_4C20_11D6_B526_8C2740693618__INCLUDED_)
#define AFX_PTSTARTDLG_H__518D7DA1_4C20_11D6_B526_8C2740693618__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PTStartDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPTStartDlg dialog

class CPTStartDlg : public CDialog
{
// Construction
public:
	CPTStartDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPTStartDlg)
	enum { IDD = IDD_START };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTStartDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPTStartDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTSTARTDLG_H__518D7DA1_4C20_11D6_B526_8C2740693618__INCLUDED_)
