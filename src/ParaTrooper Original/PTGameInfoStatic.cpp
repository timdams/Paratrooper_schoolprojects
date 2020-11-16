// PTGameInfoStatic.cpp: implementation of the CPTGameInfoStatic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PT.h"
#include "PTGameInfoStatic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPTGameInfoStatic::CPTGameInfoStatic()
{
	m_nSound=1;
	m_nDifficulty=1;
	m_strPlayerName="Not specified";

	
	info.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&info);

}

CPTGameInfoStatic::~CPTGameInfoStatic()
{

}