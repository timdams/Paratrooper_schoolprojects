// PTGameInfoStatic.h: interface for the CPTGameInfoStatic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTGAMEINFOSTATIC_H__187EE1E0_47F0_11D6_B526_DA209741C579__INCLUDED_)
#define AFX_PTGAMEINFOSTATIC_H__187EE1E0_47F0_11D6_B526_DA209741C579__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPTGameInfoStatic  
{
public:
	OSVERSIONINFO info;
	CString m_strPlayerName;
	bool m_nSound;
	int m_nDifficulty;
	CPTGameInfoStatic();
	virtual ~CPTGameInfoStatic();

};

#endif // !defined(AFX_PTGAMEINFOSTATIC_H__187EE1E0_47F0_11D6_B526_DA209741C579__INCLUDED_)
