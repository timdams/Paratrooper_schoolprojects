// PTGameStatistics.h: interface for the CPTGameStatistics class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTGAMESTATISTICS_H__5F4151E0_4E43_11D6_B526_CBC9D644BDBB__INCLUDED_)
#define AFX_PTGAMESTATISTICS_H__5F4151E0_4E43_11D6_B526_CBC9D644BDBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPTGameStatistics  
{
public:
	int m_nLastCheck;
	int m_nLandedThisLevel;
	int m_nTotalLanded;
	int m_nCurrentLev;
	long m_lPlaneShot;
	long m_lBombCreated;
	long m_lBombShot;
	long m_lLanded;
	long m_lChuteShot;
	long m_lTrooperShot;
	long m_lTrooperCreated;
	long m_lHeliShot;
	long m_lHeliCreated;
	long m_lShot;
	long m_lPlaneCreated;
	CPTGameStatistics();
	virtual ~CPTGameStatistics();
};

#endif // !defined(AFX_PTGAMESTATISTICS_H__5F4151E0_4E43_11D6_B526_CBC9D644BDBB__INCLUDED_)
