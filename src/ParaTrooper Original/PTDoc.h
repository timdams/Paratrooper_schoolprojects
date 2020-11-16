// PTDoc.h : interface of the CPTDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTDOC_H__30AD780A_2FB1_11D6_A444_444553540000__INCLUDED_)
#define AFX_PTDOC_H__30AD780A_2FB1_11D6_A444_444553540000__INCLUDED_

#include "PTInGameObject.h"	// Added by ClassView
#include "PTGameInfo.h"	// Added by ClassView


#include "PTGameInfoStatic.h"	
#include "IniFile.h"	// Added by ClassView
#include "PTGameStatistics.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPTDoc : public CDocument
{
protected: // create from serialization only
	CPTDoc();
	DECLARE_DYNCREATE(CPTDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPTDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	CPTGameStatistics GIStatistics;
	
	void WriteToIni();
	CPTInGameObject InGameObs;
	CIniFile inifile;
	CPTGameInfo GIInfo;
	CPTGameInfoStatic GIInfoStatic;

	virtual ~CPTDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPTDoc)
	afx_msg void OnGameOptions();
	afx_msg void OnGameHiscores();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PTDOC_H__30AD780A_2FB1_11D6_A444_444553540000__INCLUDED_)
