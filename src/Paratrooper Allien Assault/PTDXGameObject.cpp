// PTGameObject.cpp: implementation of the CPTGameObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PTDXGameObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPTGameObject::CPTGameObject()
{
	m_nObjType=0;
	m_bInGame=FALSE;
	posx=-1;
	posy=-1;
	vectx=0;
	vecty=0;

	m_nFlags=0;
	m_nMoreFlags=0;
}

CPTGameObject::~CPTGameObject()
{

}



CPTGameObject::CPTGameObject(const CPTGameObject& obj)
{
	m_nObjType=obj.m_nObjType;
	m_bInGame=obj.m_bInGame;
	vectx=obj.vectx;
	vecty=obj.vecty;
	posx=obj.posx;
	posy=obj.posy;
	m_nFlags=obj.m_nFlags;
}

CPTGameObject::CPTGameObject(int ObjType,int x,int y,int vx,int vy,bool InGame,int Flag)
{
	m_nObjType=ObjType;
	m_bInGame=InGame;
	posx=x;
	posy=y;
	vectx=vx;
	vecty=vy;
	m_nFlags=Flag;
}


CPTGameObject& CPTGameObject::operator=(const CPTGameObject& obj2)
{
m_nObjType=obj2.m_nObjType;
m_bInGame=obj2.m_bInGame;
vectx=obj2.vectx;
vecty=obj2.vecty;
posx=obj2.posx;
posy=obj2.posy;

m_nFlags=obj2.m_nFlags;
return *this;
}
