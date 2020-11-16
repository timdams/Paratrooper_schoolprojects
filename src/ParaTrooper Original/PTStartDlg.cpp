// PTStartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PT.h"
#include "PTStartDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPTStartDlg dialog


CPTStartDlg::CPTStartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPTStartDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPTStartDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPTStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTStartDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTStartDlg, CDialog)
	//{{AFX_MSG_MAP(CPTStartDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTStartDlg message handlers
