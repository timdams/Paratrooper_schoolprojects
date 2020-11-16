// PTGameStatistics.cpp: implementation of the CPTGameStatistics class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PT.h"
#include "PTGameStatistics.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPTGameStatistics::CPTGameStatistics()
{
	 m_lLanded=0;
	 m_lChuteShot=0;
	 m_lTrooperShot=0;
	 m_lTrooperCreated=0;
	 m_lHeliShot=0;
	 m_lHeliCreated=0;
	 m_lShot=0;
	 m_lPlaneCreated=0;
	 m_lPlaneShot=0;
	 m_lBombCreated=0;
	 m_lBombShot=0;
	 m_nTotalLanded=0;
	 m_nCurrentLev=1;
	 m_nLandedThisLevel=0;
	 m_nLastCheck=1;
}

CPTGameStatistics::~CPTGameStatistics()
{

}


