// PTGameObject.cpp: implementation of the CPTGameObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PT.h"
#include "PTGameObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPTGameObject::CPTGameObject()
{
	m_nObjType=0;
	m_bInGame=FALSE;
	m_ptPos=(-50,-50);
	m_ptVector=(0,0);
	m_nFlags=0;

}

CPTGameObject::~CPTGameObject()
{

}



CPTGameObject::CPTGameObject(const CPTGameObject& obj)
{
	m_nObjType=obj.m_nObjType;
	m_bInGame=obj.m_bInGame;
	m_ptVector=obj.m_ptVector;
	m_ptPos=obj.m_ptPos;
	m_nFlags=obj.m_nFlags;
}

CPTGameObject::CPTGameObject(int ObjType,CPoint pos,CPoint vect,bool InGame,int Flag)
{
	m_nObjType=ObjType;
	m_bInGame=InGame;
	m_ptVector=vect;
	m_ptPos=pos;
	m_nFlags=Flag;
}


CPTGameObject& CPTGameObject::operator=(const CPTGameObject& obj2)
{
m_nObjType=obj2.m_nObjType;
m_bInGame=obj2.m_bInGame;
m_ptVector=obj2.m_ptVector;
m_ptPos=obj2.m_ptPos;
m_nFlags=obj2.m_nFlags;
return *this;
}