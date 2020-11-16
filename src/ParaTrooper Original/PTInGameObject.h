// PTInGameObject.h: interface for the CPTInGameObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTINGAMEOBJECT_H__313E1240_48AA_11D6_B526_FF4D97EEEB30__INCLUDED_)
#define AFX_PTINGAMEOBJECT_H__313E1240_48AA_11D6_B526_FF4D97EEEB30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "AfxTempl.h"
#include "PTGameObject.h"
class CPTInGameObject  
{
public:
	void Clear();
	
	CPTInGameObject();
	virtual ~CPTInGameObject();
	CArray<CPTGameObject,CPTGameObject&>Explosion;
	CArray<CPTGameObject,CPTGameObject&>Bullet;
	CArray<CPTGameObject,CPTGameObject&>Bomb;
	CArray<CPTGameObject,CPTGameObject&>Heli;
	CArray<CPTGameObject,CPTGameObject&>Trooper;
	CArray<CPTGameObject,CPTGameObject&>Chute;
	CArray<CPTGameObject,CPTGameObject&>Plane;
	CPTInGameObject(const CPTInGameObject& obj);
	CPTInGameObject& operator=(const CPTInGameObject& obj2);
};

#endif // !defined(AFX_PTINGAMEOBJECT_H__313E1240_48AA_11D6_B526_FF4D97EEEB30__INCLUDED_)
