// PTGameInfo.cpp: implementation of the CPTGameInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PT.h"
#include "PTGameInfo.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPTGameInfo::CPTGameInfo()
{
m_nLevel=1;
m_bInGame=FALSE;
m_bNewLevel=TRUE;
m_lGunPos=3.14/2; //loodrecht naar boven (pi/2 rad)
SetPoints(0);
m_nLifes=0;
m_bLifesChanged=TRUE;
m_bCanShoot=TRUE;
m_strLevelStart="  here we go";
m_nLeftLanded=0;
m_nRightLanded=0;

}

CPTGameInfo::~CPTGameInfo()
{

}

void CPTGameInfo::AddPoints(int points)
{
m_nPoints=m_nPoints+points;
if (m_nPoints<0)
	m_nPoints=0;
}

int CPTGameInfo::GetPoints() const
{
return m_nPoints;
}

void CPTGameInfo::SetLevel(int lvl) 
{
	m_nLevel=lvl;
	m_bNewLevel=TRUE;
	if(m_nLevel<0)
	{
		CString cstr;
		cstr.Format("Trying to load incorrect level! Level set to 1");
		AfxMessageBox(cstr);
		m_nLevel=1;
	}

}

int CPTGameInfo::GetLevel() const
{
	return m_nLevel;
}

int CPTGameInfo::GetLifes() const
{
	return m_nLifes;
}

void CPTGameInfo::SetLifes(int lifes)
{
	m_nLifes=lifes;
	if(m_nLifes<0)
	{
		
		//Trying to load incorrect lifes! Lifes set to 0
		m_nLifes=1;
	}

}

void CPTGameInfo::AddLifes(int lifes)
{
m_nLifes=m_nLifes+lifes;
}

void CPTGameInfo::SetPoints(int points)
{
m_nPoints=points;
}


CPTGameInfo& CPTGameInfo::operator=(const CPTGameInfo obj2)
{

	m_bCanShoot=obj2.m_bCanShoot;
	m_nLifes=obj2.m_nLifes;
	m_bCanShoot=obj2.m_bCanShoot;
	m_bLifesChanged=obj2.m_bLifesChanged;
	m_lGunPos=obj2.m_lGunPos;
	m_bInGame=obj2.m_bInGame;
	m_bNewLevel=obj2.m_bNewLevel;
	m_nLeftLanded=obj2.m_nLeftLanded;
	m_nRightLanded=obj2.m_nRightLanded;
	return *this;
}

