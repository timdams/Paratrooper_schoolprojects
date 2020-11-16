// PTDoc.cpp : implementation of the CPTDoc class
//

#include "stdafx.h"
#include "PT.h"

#include "PTDoc.h"
#include "PTGameInfo.h"
#include "PTGameInfoStatic.h"
#include "PTInGameObject.h"
#include "PTGameStatistics.h"

#include "PTOptions.h"
#include "PTHiScores.h"
#include "math.h"
#include "IniFile.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPTDoc

IMPLEMENT_DYNCREATE(CPTDoc, CDocument)

BEGIN_MESSAGE_MAP(CPTDoc, CDocument)
	//{{AFX_MSG_MAP(CPTDoc)
	ON_COMMAND(ID_GAME_OPTIONS, OnGameOptions)
	ON_COMMAND(ID_GAME_HISCORES, OnGameHiscores)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTDoc construction/destruction

CPTDoc::CPTDoc()
{
	// TODO: add one-time construction code here
	GIInfo.m_bInGame=FALSE;
}

CPTDoc::~CPTDoc()
{
}

BOOL CPTDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	
	CPTGameInfo newgame;
	CPTGameInfoStatic newgame2; 
	inifile.SetPath("settings.ini");
	if (inifile.ReadFile())
	{
		newgame2.m_strPlayerName=inifile.GetValue("User Info","Name");
		newgame2.m_nDifficulty=atoi(inifile.GetValue("Game Info","Difficulty"));
		newgame2.m_nSound=atoi(inifile.GetValue("Game Info","Sound"));
	}
	else
	{
		AfxMessageBox("Settings.ini not found.Click 'OK' in the next screen to make one.");
		OnGameOptions();
	}
	if(newgame2.m_nDifficulty>3 || newgame2.m_nDifficulty<1)
			newgame2.m_nDifficulty=1;
	GIInfo=newgame;
	GIInfoStatic=newgame2;
	CPTGameStatistics ntemp;
	GIStatistics=ntemp;
	GIInfo.SetPoints(0);
	GIInfo.SetLifes(3);
	GIInfo.SetLevel(1);

	InGameObs.Clear();
	UpdateAllViews(NULL);
	return TRUE;  
}



/////////////////////////////////////////////////////////////////////////////
// CPTDoc serialization

void CPTDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPTDoc diagnostics

#ifdef _DEBUG
void CPTDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPTDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTDoc commands



void CPTDoc::OnGameOptions() 
{
	// TODO: Add your command handler code here
	int leveltemp;
	CPTOptions dlg;
	leveltemp=dlg.m_nLevel=GIInfo.GetLevel();
	dlg.m_nLifes=GIInfo.GetLifes();
	dlg.m_nPoints=GIInfo.GetPoints();
	dlg.m_strName=GIInfoStatic.m_strPlayerName;
	dlg.m_nDifficulty=GIInfoStatic.m_nDifficulty;
	dlg.m_nSound=GIInfoStatic.m_nSound;
	if(dlg.DoModal()==IDOK)
	{	
		GIInfo.SetLevel(dlg.m_nLevel);
		{
			if(leveltemp!=dlg.m_nLevel)
			UpdateAllViews(NULL);
		}
		GIInfo.m_bNewLevel=TRUE;
		GIInfoStatic.m_nDifficulty=dlg.m_nDifficulty;
		GIInfoStatic.m_nSound=dlg.m_nSound;
		GIInfo.SetLifes(dlg.m_nLifes);
		GIInfo.SetPoints(dlg.m_nPoints);
		GIInfoStatic.m_strPlayerName=dlg.m_strName;
		
		WriteToIni();

		
	}


	
	
}

void CPTDoc::OnGameHiscores() 
{
	// TODO: Add your command handler code here
	
	CPTHiScores dlg;
	char Shot[12];
	int r=0;
	int landed=0;
		while(r<InGameObs.Trooper.GetSize())
		{
			CPoint position=InGameObs.Trooper.GetAt(r).m_ptPos;
			//enkel als in game is
			if(InGameObs.Trooper.GetAt(r).m_bInGame && InGameObs.Trooper.GetAt(r).m_nFlags==555)
			{
				landed++;
			}
		r++;
		}
	GIStatistics.m_nCurrentLev=GIInfo.GetLevel();
	if(GIStatistics.m_nCurrentLev!=GIStatistics.m_nLastCheck)
	{
		GIStatistics.m_nLastCheck=GIStatistics.m_nCurrentLev;
			if(GIStatistics.m_nLandedThisLevel!=landed)
		GIStatistics.m_nTotalLanded=GIStatistics.m_nTotalLanded+GIStatistics.m_nLandedThisLevel;
		GIStatistics.m_nLandedThisLevel=0;
	}
	
	//vanwege foutje in code moet landed hier effectief nagezien worden.Sorry

	
	if(GIStatistics.m_nLandedThisLevel!=landed&&GIStatistics.m_nCurrentLev==GIStatistics.m_nLastCheck)
	{	
		GIStatistics.m_nTotalLanded=GIStatistics.m_nTotalLanded-GIStatistics.m_nLandedThisLevel+landed;
		GIStatistics.m_nLandedThisLevel=landed;
	}
	

	//omzetten van alle float-waarden naar CString omin statics te zetten
	_ltoa(GIStatistics.m_lShot,Shot,10);
	dlg.m_SHOTBULLETS=(CString)Shot;
	_ltoa(GIStatistics.m_lBombCreated,Shot,10);
	dlg.m_BOMBC=(CString)Shot;
	_ltoa(GIStatistics.m_lBombShot,Shot,10);
	dlg.m_BOMBS=(CString)Shot;
	_ltoa(GIStatistics.m_lChuteShot,Shot,10);
	dlg.m_CHUTES=(CString)Shot;
	_ltoa(GIStatistics.m_lHeliCreated,Shot,10);
	dlg.m_HELIC=(CString)Shot;
	_ltoa(GIStatistics.m_lHeliShot,Shot,10);
	dlg.m_HELIS=(CString)Shot;
	_ltoa(GIStatistics.m_nTotalLanded,Shot,10);
	dlg.m_TROOPL=(CString)Shot;
	_ltoa(GIStatistics.m_lPlaneCreated,Shot,10);
	dlg.m_PLANEC=(CString)Shot;
	_ltoa(GIStatistics.m_lPlaneShot,Shot,10);
	dlg.m_PLANES=(CString)Shot;
	_ltoa(GIStatistics.m_lTrooperShot,Shot,10);
	dlg.m_TROOPS=(CString)Shot;
	_ltoa(GIStatistics.m_lTrooperCreated,Shot,10);
	dlg.m_TROOPC=(CString)Shot;
	

	//Accuracy berekenen
	CString strAcc;
	long hit=GIStatistics.m_lBombShot+GIStatistics.m_lChuteShot+GIStatistics.m_lHeliShot+GIStatistics.m_lPlaneShot+GIStatistics.m_lTrooperShot;
	double acc=0;
	if(GIStatistics.m_lShot!=0)
		acc=hit/ double(GIStatistics.m_lShot);
	acc=acc*100;
	strAcc.Format("%2.2lf%%",acc);
	dlg.m_ACCURACY=strAcc;
	//Wise man says stukje
	CString strSkill;
	if(acc==0)
	{
		strSkill="Read the manual again";
	}
	else if(acc<10)
	{
		strSkill="Aim for the enemy.";
	}
	else if(acc<20)
	{
		strSkill="Learn how to aim";
	}
	else if(acc<30)
	{
		strSkill="You do know the rules huh?!";
	}
	else if(acc<40)
	{
		strSkill="Never go to war please";
	}
	else if(acc<50)
	{
		strSkill="One day you will be good";
	}
	else if(acc<60)
	{
		strSkill="Getting better";
	}
	else if(acc<70)
	{
		strSkill="Nice shooting";
	}
	else if(acc<80)
	{
		strSkill="Marksmen spotted";
	}
	else if(acc<90)
	{
		strSkill="Woah! You deserve a statue!";
	}
	else if(acc<95)
	{
		strSkill="Heroes do exist!!";
	}
	else if(acc<=99)
	{
		strSkill="Are you a god?";
	}
	else if(acc==100)
	{
		strSkill="Godlike!";
	}
	dlg.m_SKILL=strSkill;
	dlg.DoModal();
	
	GIStatistics.m_nLandedThisLevel==landed;
}


void CPTDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	WriteToIni();
	CDocument::OnCloseDocument();
}

void CPTDoc::WriteToIni()
{
		inifile.Reset();
		inifile.SetValue("User Info","Name",GIInfoStatic.m_strPlayerName);
		CString diff;
		if(GIInfoStatic.m_nDifficulty>3 || GIInfoStatic.m_nDifficulty<1)
			GIInfoStatic.m_nDifficulty=1;
		diff.Format("%i",GIInfoStatic.m_nDifficulty);
		inifile.SetValue("Game Info","Difficulty",diff);
		CString sound;
		sound.Format("%i",GIInfoStatic.m_nSound);
		inifile.SetValue("Game Info","Sound",sound);
		inifile.WriteFile();
}
