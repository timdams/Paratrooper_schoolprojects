// PTOptions.cpp : implementation file
//

#include "stdafx.h"
#include "PT.h"
#include "PTOptions.h"
#include "PTGameInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPTOptions dialog


CPTOptions::CPTOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CPTOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPTOptions)
	m_nLifes = 0;
	m_nPoints = 0;
	m_strName = _T("");
	m_nLevel = 0;
	m_nDifficulty = 0;
	m_nSound = FALSE;
	//}}AFX_DATA_INIT
}


void CPTOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTOptions)
	DDX_Control(pDX, IDC_DIFFSPIN, m_DiffTeller);
	DDX_Text(pDX, IDC_Lifes, m_nLifes);
	DDV_MinMaxInt(pDX, m_nLifes, 0, 100);
	DDX_Text(pDX, IDC_Points, m_nPoints);
	DDX_Text(pDX, IDC_Name, m_strName);
	DDV_MaxChars(pDX, m_strName, 60);
	DDX_Text(pDX, IDC_Level, m_nLevel);
	DDV_MinMaxInt(pDX, m_nLevel, 0, 100);
	DDX_Text(pDX, IDC_DIFF, m_nDifficulty);
	DDV_MinMaxInt(pDX, m_nDifficulty, 1, 3);
	DDX_Check(pDX, IDC_SOUND, m_nSound);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTOptions, CDialog)
	//{{AFX_MSG_MAP(CPTOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTOptions message handlers





BOOL CPTOptions::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_DiffTeller.SetRange(1,3);
	//m_DiffTeller.SetPos(m_nDifficulty);
	m_btnOK.SubclassDlgItem(IDOK, this);
	m_btnOK.DrawTransparent();
	m_btnOK.SetBitmaps(IDB_OKBTNF,RGB(0,100,150),IDB_OKBTN,RGB(0,100,150));

	m_btnNO.SubclassDlgItem(IDCANCEL, this);
	m_btnNO.DrawTransparent();
	m_btnNO.SetBitmaps(IDB_NOBTNF,RGB(0,100,150),IDB_NOBTN,RGB(0,100,150));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}
