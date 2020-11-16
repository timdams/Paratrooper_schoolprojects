// PTHiScores.cpp : implementation file
//

#include "stdafx.h"
#include "PT.h"
#include "PTHiScores.h"
#include "PTGameStatistics.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPTHiScores dialog


CPTHiScores::CPTHiScores(CWnd* pParent /*=NULL*/)
	: CDialog(CPTHiScores::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPTHiScores)
	m_SHOTBULLETS = _T("");
	m_TROOPS = _T("");
	m_TROOPC = _T("");
	m_HELIC = _T("");
	m_HELIS = _T("");
	m_BOMBC = _T("");
	m_BOMBS = _T("");
	m_PLANEC = _T("");
	m_PLANES = _T("");
	m_CHUTES = _T("");
	m_TROOPL = _T("");
	m_ACCURACY = _T("");
	m_SKILL = _T("");
	//}}AFX_DATA_INIT
}


void CPTHiScores::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTHiScores)
	DDX_Text(pDX, IDC_SHOTBULLETS, m_SHOTBULLETS);
	DDX_Text(pDX, IDC_TROOPS, m_TROOPS);
	DDX_Text(pDX, IDC_TROOPC, m_TROOPC);
	DDX_Text(pDX, IDC_HELIC, m_HELIC);
	DDX_Text(pDX, IDC_HELIS, m_HELIS);
	DDX_Text(pDX, IDC_BOMBC, m_BOMBC);
	DDX_Text(pDX, IDC_BOMBS, m_BOMBS);
	DDX_Text(pDX, IDC_PLANEC, m_PLANEC);
	DDX_Text(pDX, IDC_PLANES, m_PLANES);
	DDX_Text(pDX, IDC_CHUTES, m_CHUTES);
	DDX_Text(pDX, IDC_TROOPL, m_TROOPL);
	DDX_Text(pDX, IDC_ACCURACY, m_ACCURACY);
	DDX_Text(pDX, IDC_SKILL, m_SKILL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTHiScores, CDialog)
	//{{AFX_MSG_MAP(CPTHiScores)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTHiScores message handlers


BOOL CPTHiScores::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_btnOK.SubclassDlgItem(IDOK, this);
	m_btnOK.DrawTransparent();
	m_btnOK.SetBitmaps(IDB_OKBTNF,RGB(0,100,150),IDB_OKBTN,RGB(0,100,150));


	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPTHiScores::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	pDC->SetBkColor(RGB(255,255,255));
	// TODO: Change any attributes of the DC here
		if(pWnd->GetDlgCtrlID()==IDC_SKILL)
	{
		
		CFont m_Font;
		LOGFONT* m_pLF;
		m_pLF=(LOGFONT*)calloc(1,sizeof(LOGFONT));
		strncpy(m_pLF->lfFaceName,"Times New Roman",31);
		m_pLF->lfHeight=20;
		m_pLF->lfWeight=500;
		m_pLF->lfItalic=1;
		m_pLF->lfUnderline=0;

		m_Font.CreateFontIndirect(m_pLF);
		CFont* old=(CFont*)pDC->SelectObject(&m_Font);

		pDC->SetTextColor(RGB(0,0,0));
	
	//	pDC->SelectObject(&old);
		free(m_pLF);
		
	}
	if(pWnd->GetDlgCtrlID()==IDC_ACCURACY)
	{

		CFont m_Font;
		LOGFONT* m_pLF;
		m_pLF=(LOGFONT*)calloc(1,sizeof(LOGFONT));
		strncpy(m_pLF->lfFaceName,"Times New Roman",31);
		m_pLF->lfHeight=30;
		m_pLF->lfWeight=600;
		m_pLF->lfItalic=0;
		m_pLF->lfUnderline=0;

		m_Font.CreateFontIndirect(m_pLF);
		CFont* old=(CFont*)pDC->SelectObject(&m_Font);

		pDC->SetTextColor(RGB(255,0,0));
	//	pDC->SelectObject(&old);
		free(m_pLF);
	}

	// TODO: Return a different brush if the default is not desired
	CBrush bla;
	bla.CreateSolidBrush(RGB(255,255,255));
	return bla;
//	return hbr;
}

void CPTHiScores::OnButton1() 
{
	// TODO: Add your control notification handler code here
	
}
