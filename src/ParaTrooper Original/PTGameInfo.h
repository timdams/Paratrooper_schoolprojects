// PTGameInfo.h: interface for the CPTGameInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTGAMEINFO_H__D51A0483_3074_11D6_A444_444553540000__INCLUDED_)
#define AFX_PTGAMEINFO_H__D51A0483_3074_11D6_A444_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Afxtempl.h"

class CPTGameInfo  
{

public:
	
	int m_nRightLanded;
	int m_nLeftLanded;
	CString m_strLevel;
	CString m_strLevelStart;
	
//	CArray<CPTGameObject,CPTGameObject> Bullet;
	bool m_bCanShoot;
	bool m_bLifesChanged;
	double m_lGunPos;
	bool m_bInGame;
	bool m_bNewLevel;
	
	
	void SetPoints(int points);
	void AddLifes(int lifes);
	void SetLevel(int lvl) ;
	void SetLifes(int lifes);
	int GetPoints() const;
	int GetLifes() const;
	int GetLevel()const;
	void AddPoints(int points);

	
	CPTGameInfo();
	CPTGameInfo& operator=(const CPTGameInfo obj2);
	virtual ~CPTGameInfo();
private:
	int m_nPoints;
	int m_nLevel;
	int m_nLifes;
};

#endif // !defined(AFX_PTGAMEINFO_H__D51A0483_3074_11D6_A444_444553540000__INCLUDED_)

