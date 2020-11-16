// PTGameObject.h: interface for the CPTGameObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTGAMEOBJECT_H__96FA6FE0_4727_11D6_B526_E4B4B57D131D__INCLUDED_)
#define AFX_PTGAMEOBJECT_H__96FA6FE0_4727_11D6_B526_E4B4B57D131D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPTGameObject  
{
public:
	int m_nFlags;
	
	CPoint m_ptVector;
	CPoint m_ptPos;
	bool m_bInGame;
	int m_nObjType;  //1=bullet

	CPTGameObject();
	CPTGameObject(const CPTGameObject& obj);
	CPTGameObject(int ObjType,CPoint pos,CPoint vect,bool InGame,int Flag);
	CPTGameObject& operator=(const CPTGameObject& obj2);
	virtual ~CPTGameObject();
};

#endif // !defined(AFX_PTGAMEOBJECT_H__96FA6FE0_4727_11D6_B526_E4B4B57D131D__INCLUDED_)
