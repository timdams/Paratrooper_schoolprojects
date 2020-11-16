// PTInGameObject.cpp: implementation of the CPTInGameObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PT.h"
#include "PTInGameObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPTInGameObject::CPTInGameObject()
{
	for(int i=0;i<20;++i)
		Bullet.Add(CPTGameObject(1,(0,0),(0,0),FALSE,0));
	for(int j=0;j<25;++j)
	{
		Heli.Add(CPTGameObject(2,(0,0),(0,0),FALSE,0));
		Explosion.Add(CPTGameObject(10,(0,0),(0,0),FALSE,0));
		Trooper.Add(CPTGameObject(3,(0,0),(0,0),FALSE,0));
		Chute.Add(CPTGameObject(12,(0,0),(0,0),FALSE,0));
		Plane.Add(CPTGameObject(6,(0,0),(0,0),FALSE,0));
		Bomb.Add(CPTGameObject(12,(0,0),(0,0),FALSE,0));
	}
}

CPTInGameObject::~CPTInGameObject()
{

}

CPTInGameObject::CPTInGameObject(const CPTInGameObject& obj)
{

	for(int j=0;j<20;++j)
		Bullet.SetAt(j,obj.Bullet.GetAt(1));


	for(int l=0;l<25;++l)
	{
		Explosion.SetAt(2,obj.Bullet.GetAt(1));
		Heli.SetAt(l,obj.Bullet.GetAt(1));
		Chute.SetAt(l,obj.Chute.GetAt(l));
		Trooper.SetAt(l,obj.Trooper.GetAt(l));
		Plane.SetAt(l,obj.Plane.GetAt(1));
		Bomb.SetAt(l,obj.Bomb.GetAt(1));
	}
}



void CPTInGameObject::Clear()
{
	CPTGameObject ob(1,(0,0),(0,0),FALSE,0);
	for(int i=0;i<20;++i)
	{
		Bullet.SetAt(i,ob);
	}
	CPTGameObject ob5(3,(0,0),(0,0),FALSE,0);
	CPTGameObject ob4(11,(0,0),(0,0),FALSE,0);
	CPTGameObject ob3(10,(0,0),(0,0),FALSE,0);
	CPTGameObject ob2(2,(0,0),(0,0),FALSE,0);
	CPTGameObject ob6(6,(0,0),(0,0),FALSE,0);
	CPTGameObject ob7(2,(0,0),(0,0),FALSE,0);
	for(int j=0;j<25;j++)
	{
		Explosion.SetAt(j,ob3);
		Heli.SetAt(j,ob2);
		Trooper.SetAt(j,ob4);
		Chute.SetAt(j,ob5);
		Plane.SetAt(j,ob6);
		Bomb.SetAt(j,ob7);
	}

}

CPTInGameObject& CPTInGameObject::operator=(const CPTInGameObject& obj2)
{

	for(int j=0;j<20;++j)
		Bullet.SetAt(j,obj2.Bullet.GetAt(1));

	for(int m=0;m<25;++m)
	{
		Heli.SetAt(m,obj2.Bullet.GetAt(1));
		Explosion.SetAt(m,obj2.Heli.GetAt(1));
		Chute.SetAt(m,obj2.Chute.GetAt(1));
		Trooper.SetAt(m,obj2.Trooper.GetAt(1));
		Plane.SetAt(m,obj2.Plane.GetAt(1));
		Bomb.SetAt(m,obj2.Bomb.GetAt(1));
	}
	return *this;
}



