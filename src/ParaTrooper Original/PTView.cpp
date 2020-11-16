// PTView.cpp : implementation of the CPTView class
//

#include "stdafx.h"
#include "Afxtempl.h"

#include "PT.h"

#include "PTDoc.h"
#include "PTView.h"
#include "PTGameInfo.h"
#include "PTInGameObject.h"
#include "math.h"

#include <mmsystem.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPTView

IMPLEMENT_DYNCREATE(CPTView, CView)

BEGIN_MESSAGE_MAP(CPTView, CView)
	//{{AFX_MSG_MAP(CPTView)
	ON_WM_TIMER()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTView construction/destruction

CPTView::CPTView()
{
	// TODO: add construction code here

}

CPTView::~CPTView()
{
}

BOOL CPTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPTView drawing


void CPTView::OnDraw(CDC* pDC)
{

	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	GetClientRect(&GameRect);
	
	
	m_ptGunpos.x=GameRect.Width()/2-50;
	m_ptGunpos.y=GameRect.Height()-100;
	

	CPoint hudpos;
	hudpos.x=GameRect.Width()-25;
	hudpos.y=GameRect.Height()-25;

	//Prepareer compatibele buffer DC en bitmap om op te tekene
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap save;
	save.CreateCompatibleBitmap(pDC,GameRect.Width(),GameRect.Height());
	int savedc=pDC->SaveDC();
	int savememDC=memDC.SaveDC();
	CBitmap* bmold=(CBitmap*)memDC.SelectObject(&save);
	
	//Alle te tekene zaken komen hier tussen


		
	if(!m_bNT)
	DrawBkgr(&memDC); //////////NOT NT COMPATIBLE!!!!!!!!!////////////////
	DrawLoop(&memDC,pDoc->GIInfo.m_lGunPos,m_ptGunpos);
	DrawGunBody(&memDC,m_ptGunpos);
	
	CPoint possje;
	possje.x=260;
	possje.y=300;
	DrawSpString(&memDC,possje,pDoc->GIInfo.m_strLevelStart);
	DrawAllObjects(&memDC);
	DrawHud(&memDC,hudpos,pDoc->GIInfo);

	//bufferDC naar scherm blitten
	pDC->BitBlt(0,0,GameRect.Width(),GameRect.Height(),&memDC,0,0,SRCCOPY);
//	memDC verwijderen
	memDC.SelectObject(bmold);
	
	pDC->RestoreDC(savedc);
	memDC.RestoreDC(savememDC);
}

/////////////////////////////////////////////////////////////////////////////
// CPTView diagnostics

#ifdef _DEBUG
void CPTView::AssertValid() const
{
	CView::AssertValid();
}

void CPTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPTDoc* CPTView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPTDoc)));
	return (CPTDoc*)m_pDocument;
	
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTView message handlers

void CPTView::OnTimer(UINT nIDEvent) 
{
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1://scherm updaten
		UpdateAll();
		Invalidate();
		
		CheckGameOver();
		
		break;
	case 2://tijd tussen twee schoten
		KillTimer(2);
  		pDoc->GIInfo.m_bCanShoot=TRUE;
		break;
	case 3://loop naar links bewegen
		if(pDoc->GIInfo.m_lGunPos<=170*(3.14/180) )  //Tot op 5graden 
			pDoc->GIInfo.m_lGunPos=pDoc->GIInfo.m_lGunPos+4*(3.14/180);
		else KillTimer(3);
		break;
	case 4://loop naar rechts bewegen
		if(pDoc->GIInfo.m_lGunPos>=10*(3.14/180) )
			pDoc->GIInfo.m_lGunPos=pDoc->GIInfo.m_lGunPos-4*(3.14/180);
		else KillTimer(4);
		break;
	case 10://heli/plane creeren
		{
			KillTimer(10);
			
			if(pDoc->GIInfo.GetLevel()%3==0)
			{
				//planes-level
				int timerrand=0;
				while(timerrand<100*pDoc->GIInfoStatic.m_nDifficulty+pDoc->GIInfo.GetLevel()*50)
				timerrand=CreateRandom(0,5000);
				CreatePlane();
				SetTimer(10,timerrand,NULL);
			}

			else 
			{
				int timerrand=0;
				while(timerrand<100*pDoc->GIInfoStatic.m_nDifficulty+pDoc->GIInfo.GetLevel()*50)
				timerrand=CreateRandom(0,5000);
				
				CreateHeli();
				SetTimer(10,timerrand,NULL);
				//heli-level	
			}
		}
		break;

	case 20: //nieuwe level begint
		pDoc->GIInfo.m_bInGame=TRUE;
		KillTimer(20);
		break;
	case 21: //extra info bij nieuwe level tonen
		if(pDoc->GIInfo.m_bInGame)
		{
			KillTimer(21);
			pDoc->GIInfo.m_strLevelStart="";
		}
		else pDoc->GIInfo.m_strLevelStart="  here we go";
		break;
	case 50://interne secondeteller om pauze te gebruiken
		++m_nLevSeconden;
		break;
	case 100://level is gedaan
		pDoc->GIInfo.SetLevel(pDoc->GIInfo.GetLevel()+1);
		pDoc->InGameObs.Clear();
		pDoc->GIInfo.m_strLevelStart="level finished";
		KillAllTimers();
		Invalidate();
		SetTimer(101,3000,NULL);
		break;
	case 101:
		KillTimer(101);
		pDoc->GIInfo.m_strLevelStart="";
		OnUpdate(this,NULL,NULL);
		
		break;
	
	default:
		Invalidate();
	}
	CView::OnTimer(nIDEvent);
}

void CPTView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	

	//Code om wapens te selecteren
	
	CView::OnChar(nChar, nRepCnt, nFlags);

}
	

void CPTView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
/*
		CString strDebug;
	
	strDebug.Format("Char:%i , repeatC:%i, falgs:%i",nChar,nRepCnt,nFlags);
	AfxMessageBox(strDebug);
*/
	//Delete code hierboven om OnChars te debuggen
	//Left:37, Right:39 , Up:38 , Down:40, Space:32
	
	if(nChar== 80)
		Pause();


	

	CString testt;CPoint pos;
	if(pDoc->GIInfo.m_bInGame)
	{
	switch(nChar){

/*	case 76:
		SetTimer(100,10,NULL);
		break;*/
/*	case 66:
		pos.x=GameRect.Width()/2;
		pos.y=0;
		CreateBomb(pos);
		break;*/
/*	case 67: //C letter gebruikt om items zelf te creeëren (debug)
	
		pos.x=50;pos.y=300;
		CreatePlane();
	//	testt.Format("Shot: %i\n Heli created:%i Heli shot:%i\n
	//		,pDoc->GIStatistics.m_lShot,pDoc->GIStatistics.m_lHeliCreated);
	//	AfxMessageBox(testt);
	
		break;*/

	case 37:
		SetTimer(3,50,NULL);
		KillTimer(4);
	/*	if(pDoc->GIInfo.m_lGunPos<=170*(3.14/180) )  //Tot op 5graden 
			pDoc->GIInfo.m_lGunPos=pDoc->GIInfo.m_lGunPos+2*(3.14/180);
	*/	
		break;
	case 39:
		SetTimer(4,50,NULL);
		KillTimer(3);
	/*	if(pDoc->GIInfo.m_lGunPos>=10*(3.14/180) )
			pDoc->GIInfo.m_lGunPos=pDoc->GIInfo.m_lGunPos-2*(3.14/180);
		*/
		break;
	case 38:
		KillTimer(3);
		KillTimer(4);
		break;
	case 32:
		KillTimer(3);
		KillTimer(4);
		if(pDoc->GIInfo.m_bCanShoot)
		{
			SetTimer(2,150,NULL);//tijd tussen 2kogels
			CreateBullet(pDoc->GIInfo.m_lGunPos,m_ptGunpos);

		}
		break;

	}
}
	

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CPTView::OnPrepareLevel(int m_nLevel)
{

	//Inladen van alle level relevante informatie: juiste levelachtergrond, 
	//objecten met juiste eigenschappen naargelang moeilijkheid en level
	m_nLevSeconden=0;
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	KillAllTimers();
	m_bmpLevelBackground.DeleteObject();
	CString lev;
	lev.Format("level %i",pDoc->GIInfo.GetLevel());
	switch(pDoc->GIInfo.GetLevel()%3){	//Naargelang level andere achtergrond en timer etc
		case 1:
			m_bmpLevelBackground.LoadBitmap(IDB_BKGR1);
			pDoc->GIInfo.m_strLevel=lev;
			SetTimer(100,100000,NULL);
			
			break;
		case 2:
			m_bmpLevelBackground.LoadBitmap(IDB_BKGR2);
			pDoc->GIInfo.m_strLevel=lev;
			SetTimer(100,100000,NULL);
			
			break;
	
		case 0:
			m_bmpLevelBackground.LoadBitmap(IDB_BKGR3);
			//pDoc->GIInfo.m_strLevel=lev;
			pDoc->GIInfo.m_strLevel=lev;
			SetTimer(100,20000,NULL);
	
			break;
	
			break;
		
		default:
			m_bmpLevelBackground.LoadBitmap(IDB_BKGR2);

	}
	pDoc->InGameObs.Clear();  //kogels en heli's etc resetten
	pDoc->GIInfo.m_bNewLevel=FALSE;
	pDoc->GIInfo.m_bInGame=FALSE;
	pDoc->GIInfo.m_lGunPos=3.14/2;
	pDoc->GIInfo.m_nLeftLanded=0;
	pDoc->GIInfo.m_nRightLanded=0;
	pDoc->GIInfo.m_bCanShoot=TRUE;
	
	SetTimer(10,3000,NULL);
	SetTimer(20,1800,NULL);
	SetTimer(21,800,NULL);
	SetTimer(1,50,NULL);
	SetTimer(50,1000,NULL);
	
}

int CPTView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
	
		return -1;
	
	// TODO: Add your specialized creation code here
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	//Inladen van alle bitmaps die in spel voorkomen.
	m_bmpExplosion.LoadBitmap(IDB_EXPLOSION);
	m_bmpExplosion2.LoadBitmap(IDB_EXPLOSION2);
	m_bmpFont.LoadBitmap(IDB_COMICFONT);
	m_bmpLifes.LoadBitmap(IDB_LIFE);
	m_bmpGunBody.LoadBitmap(IDB_GUNBODY);
	m_bmpBullet.LoadBitmap(IDB_BULLET);
	m_bmpBomb.LoadBitmap(IDB_BOMB);
	m_bmpHelitoLeftRed.LoadBitmap(IDB_HELITOLEFTRED);
	m_bmpHelitoLeftGreen.LoadBitmap(IDB_HELITOLEFTGREEN);
	m_bmpHelitoLeftPurp.LoadBitmap(IDB_HELITOLEFTPURP);
	m_bmpHelitoRightRed.LoadBitmap(IDB_HELITORIGHTRED);
	m_bmpHelitoRightGreen.LoadBitmap(IDB_HELITORIGHTGREEN);
	m_bmpHelitoRightPurp.LoadBitmap(IDB_HELITORIGHTPURP);
	m_bmpPlaneToLeftGrey.LoadBitmap(IDB_PLANETOLEFTGREY);
	m_bmpPlaneToLeftGreen.LoadBitmap(IDB_PLANETOLEFTGREEN);
	m_bmpPlaneToRightGreen.LoadBitmap(IDB_PLANETORIGHTGREEN);
	m_bmpPlaneToRightGrey.LoadBitmap(IDB_PLANETORIGHTGREY);
	m_bmpTroopFall.LoadBitmap(IDB_TROOPFALL);
	m_bmpTroopLand.LoadBitmap(IDB_TROOPLAND);
	m_bmpChute.LoadBitmap(IDB_CHUTE);
	m_bmpSplatAir.LoadBitmap(IDB_SPLATAIR);
	m_bmpSplatGround.LoadBitmap(IDB_SPLATGROUND);
	srand( (unsigned)time( NULL ) ); //random seed adhv huidige tijd

	return 0;
}

void CPTView::DrawTransparentBitmap(CDC *pDC, CBitmap *pBitmap, short xStart, short yStart, COLORREF cTransparentColor)
{
	//Code gevonde in voorbeeldprogramma op codeproject.com
	
	// Copiously plagiarized from the Microsoft Knowledge Base,
	// article Q79212...though I did convert it to C++/MFC.
	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);
	dcTemp.SelectObject(pBitmap);

	BITMAP bm;
	pBitmap->GetBitmap(&bm);
	POINT ptSize;
	ptSize.x = bm.bmWidth;		// Get width of bitmap
	ptSize.y = bm.bmHeight;		// Get height of bitmap
	dcTemp.DPtoLP(&ptSize, 1);	// Convert from device to logical points

	// Create some DCs to hold temporary data.
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CDC dcBack;
	dcBack.CreateCompatibleDC(pDC);
	CDC dcObject;
	dcObject.CreateCompatibleDC(pDC);
	CDC dcSave;
	dcSave.CreateCompatibleDC(pDC);

	// Monochrome DC
	CBitmap bmAndBack;
	bmAndBack.CreateBitmap(ptSize.x,ptSize.y,1,1,NULL);

	// Monochrome DC
	CBitmap bmAndObject;
	bmAndObject.CreateBitmap(ptSize.x,ptSize.y,1,1,NULL);

	// Color DCs
	CBitmap bmAndMem;
	bmAndMem.CreateCompatibleBitmap(pDC,ptSize.x,ptSize.y);
	CBitmap bmSave;
	bmSave.CreateCompatibleBitmap(pDC,ptSize.x,ptSize.y);

	// Each DC must select a bitmap object to store pixel data.
	CBitmap* bmBackOld = (CBitmap*)dcBack.SelectObject(&bmAndBack);
	CBitmap* bmObjectOld = (CBitmap*)dcObject.SelectObject(&bmAndObject);
	CBitmap* bmMemOld = (CBitmap*)dcMem.SelectObject(&bmAndMem);
	CBitmap* bmSaveOld = (CBitmap*)dcSave.SelectObject(&bmSave);

	// Set proper mapping mode.
	dcTemp.SetMapMode(pDC->GetMapMode());

	// Save the bitmap sent here, because it will be overwritten.
	dcSave.BitBlt(0,0,ptSize.x,ptSize.y,&dcTemp,0,0,SRCCOPY);

	// Set the background color of the source DC to the color.
	// contained in the parts of the bitmap that should be transparent
	COLORREF cColor;
	cColor = dcTemp.SetBkColor(cTransparentColor);

	// Create the object mask for the bitmap by performing a BitBlt
	// from the source bitmap to a monochrome bitmap.
	dcObject.BitBlt(0,0,ptSize.x,ptSize.y,&dcTemp,0,0,SRCCOPY);

	// Set the background color of the source DC back to the original
	// color.
	dcTemp.SetBkColor(cColor);

	// Create the inverse of the object mask.
	dcBack.BitBlt(0,0,ptSize.x,ptSize.y,&dcObject,0,0,NOTSRCCOPY);

	// Copy the background of the main DC to the destination.
	dcMem.BitBlt(0,0,ptSize.x,ptSize.y,pDC,xStart,yStart,SRCCOPY);

	// Mask out the places where the bitmap will be placed.
	dcMem.BitBlt(0,0,ptSize.x,ptSize.y,&dcObject,0,0,SRCAND);

	// Mask out the transparent colored pixels on the bitmap.
	dcTemp.BitBlt(0,0,ptSize.x,ptSize.y,&dcBack,0,0,SRCAND);

	// XOR the bitmap with the background on the destination DC.
	dcMem.BitBlt(0,0,ptSize.x,ptSize.y,&dcTemp,0,0,SRCPAINT);

	// Copy the destination to the screen.
	pDC->BitBlt(xStart,yStart,ptSize.x,ptSize.y,&dcMem,0,0,SRCCOPY);

	// Restore the original bitmap.
	dcTemp.BitBlt(0,0,ptSize.x,ptSize.y,&dcSave,0,0,SRCCOPY);

	// Delete the memory bitmaps.
	dcBack.SelectObject(bmBackOld);
	dcObject.SelectObject(bmObjectOld);
	dcMem.SelectObject(bmMemOld);
	dcSave.SelectObject(bmSaveOld);
}




BOOL CPTView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return 1; //Om flikkeren tegen te gaan

//	return CView::OnEraseBkgnd(pDC);
}

void CPTView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	KillAllTimers();
}


void CPTView::DrawLoop(CDC *pDC,double m_lGunPos,CPoint pos)
{
	//!!!!!!!!!!!!!!!!!
	//deze code is de boosdoener waardoor er zo'n verschil is tussen NT en 98
	//!!!!!!!!!!!!!!!!!
	
	CDC memDC,bufferDC;
	CBitmap temp;
	CPen pen(PS_SOLID,5,RGB(170,170,170)); //grijze dikke loop
	//tijdeleke dc's en bitmaps maken
	memDC.CreateCompatibleDC(pDC);
	bufferDC.CreateCompatibleDC(pDC);
	CBitmap* bmold2;
	temp.CreateCompatibleBitmap(pDC,100,100);

	//stukje achtergrond naar nieuwe bitmap kopieren
	if(!m_bNT)
	bmold2=(CBitmap*)bufferDC.SelectObject(&m_bmpLevelBackground);//////////NOT NT COMPATIBLE!!!!!!!!!////////////////
	
	CBitmap* bmold=(CBitmap*)memDC.SelectObject(&temp);
	
	if(!m_bNT)
	memDC.BitBlt(0,0,100,100,&bufferDC,pos.x,pos.y-35,SRCCOPY);//////////NOT NT COMPATIBLE!!!!!!!!!////////////////
	
	if(m_bNT)
	memDC.BitBlt(0,0,100,100,&memDC,pos.x,pos.y-35,SRCCOPY);////////// NT COMPATIBLE!!!!!!!!!////////////////
	//Actuele tekene van de loop naar tijdelijke dc **lengte loop:40 ; hoek:m_lGunPos radialen
	
	CPen* penold=(CPen*)memDC.SelectObject(&pen);
	memDC.MoveTo(50,50);
	memDC.LineTo(50+38*cos(m_lGunPos),50-38*sin(m_lGunPos));  
	
	//loop+stukje hertekende achtergrond naar pDC blitten 
	pDC->BitBlt(pos.x,pos.y-35,100,100,&memDC,0,0,SRCCOPY); 
	//tijdelijke dc's verwijderen
	memDC.SelectObject(&bmold);
	memDC.SelectObject(&penold);
	if(!m_bNT)
	bufferDC.SelectObject(&bmold2);//////////NOT NT COMPATIBLE!!!!!!!!!////////////////
	
	//!!
	memDC.DeleteDC();  //zonder deze twee lijnen zal programma na bepaalde tijd vastlopen
	bufferDC.DeleteDC();
}




void CPTView::DrawGunBody(CDC *pDC,CPoint pos)
{
	DrawTransparentBitmap(pDC,&m_bmpGunBody,pos.x,pos.y,RGB(255,255,255)); //Teken body van kannon middenbenede
}

void CPTView::DrawBkgr(CDC *pDC)
{
	//Aangezien DrawTransparentBitmap() met buffers werkt kunne we die functie
	//snel hergebruiken, enige dat we moeten voor zorgen is dat we geen kleur
	//voor transparantie selecteren die in de achtergrondbitmap zal voorkomen!
	DrawTransparentBitmap(pDC,&m_bmpLevelBackground,0,0,RGB(0,0,-50));
	
}


void CPTView::DrawHud(CDC *pDC,CPoint pos, CPTGameInfo GIn)
{
	//Kopie van positie en punten nemen want zal door punten en levens in deze 
	//functie aangepast worden
	CPoint pos1=pos;
	CPoint pos2=pos;
	pos2.y=pos2.y-25;
	int temp=GIn.GetPoints();
	CPoint level;
	level.x=0;
	level.y=GameRect.Height()-30;
	DrawSpString(pDC,level,GIn.m_strLevel);
	//Punten (eerst eerte digit 'berekene', dan de rest)
	int rest;
	int posteller=0;

	rest=temp%10;
	temp=temp/10;

	DrawSpChar(pDC,pos1,rest);
	//berekenen van overige digits en digit per digit naar scherm plaatsen.
	while(temp>0) 
	{
	rest=temp%10;
	temp=temp/10;
	pos1.x=pos1.x-20;
	DrawSpChar(pDC,pos1,rest);
	}
	//
	if(m_bPauze)
	{	
		CPoint pospauze;
		pospauze.x=335;
		pospauze.y=300;
		DrawSpString(pDC,pospauze,"Paused");
	}
	//Levens
	
	if(GIn.m_bLifesChanged) //enkel hertekenen als effectie nodig is
	{
		if(GIn.GetLifes()==0)
		{
			CString str="none";
			pos2.x=pos2.x-60;
			DrawSpString(pDC,pos2,str);
		}
		else for(int i=0;i<GIn.GetLifes();i++)
		{
		DrawTransparentBitmap(pDC,&m_bmpLifes,pos2.x,pos2.y,RGB(255,255,255)); //Teken body van kannon middenbenede
		pos2.x=pos2.x-10;
		}
		GIn.m_bLifesChanged=FALSE;
	}


}

void CPTView::DrawSpChar(CDC *pDC,CPoint pos ,int ch)
{
	
	//Functie om getekende letters/cijfers op het scherm te krijge.
	//Letter en cijfers staan in bitmap m_bmpFont
	//Origineel enkel bedoelt om cijfers te tonen,vandaar de parameter ch van type int.
	//Chars moeten bijgevolg met hun ASCII-waarde naar deze functie worden doorgegeven.

	CDC memDC,bufferDC;
	CBitmap temp;
	
	memDC.CreateCompatibleDC(pDC);
	bufferDC.CreateCompatibleDC(pDC);
	
	temp.CreateCompatibleBitmap(pDC,20,20);

	

	CBitmap* bmold=(CBitmap*)memDC.SelectObject(&temp);
	CBitmap* bmold2=(CBitmap*)bufferDC.SelectObject(&m_bmpFont);
	if(ch<=0)
		memDC.BitBlt(0,0,20,20,&bufferDC,0,0,SRCCOPY);
	if(ch<10)
		memDC.BitBlt(0,0,20,20,&bufferDC,ch*20,0,SRCCOPY);
	else
		if(ch>=65)
		{
			
			if(ch<75)
			{
				ch=ch-65;
				memDC.BitBlt(0,0,20,20,&bufferDC,ch*20,80,SRCCOPY);
			}
			if(ch<85)
			{
		
				ch=ch-75;
				memDC.BitBlt(0,0,20,20,&bufferDC,ch*20,100,SRCCOPY);
			}
			if(ch<95)
			{
				ch=ch-85;
				memDC.BitBlt(0,0,20,20,&bufferDC,ch*20,120,SRCCOPY);
			}
			if(ch<107)
			{
				ch=ch-97;
				memDC.BitBlt(0,0,20,20,&bufferDC,ch*20,20,SRCCOPY);
			}
			if(ch<117)
			{	
				ch=ch-107;
				memDC.BitBlt(0,0,20,20,&bufferDC,ch*20,40,SRCCOPY);
			}
			if(ch<127)
			{
				ch=ch-117;
				memDC.BitBlt(0,0,20,20,&bufferDC,ch*20,60,SRCCOPY);
			}
			if(ch==32)
				memDC.BitBlt(0,0,20,20,&bufferDC,180,180,SRCCOPY);
		}
		if(ch==32)
				memDC.BitBlt(0,0,20,20,&bufferDC,180,180,SRCCOPY);

	memDC.SelectObject(bmold);
	bufferDC.SelectObject(bmold2);
	DrawTransparentBitmap(pDC,&temp,pos.x,pos.y,RGB(0,0,0));
	bufferDC.DeleteDC();
	memDC.DeleteDC();
}

void CPTView::DrawSpString(CDC *pDC,CPoint pos, CString str)
{
	CPoint pos1=pos;
	str.MakeLower();//DrawSpChar werkt op het moment enkel met cijfers en lowercase chars
	for(int i=0;i<str.GetLength();i++)
	{
		if(str[i]>='0' && str[i]<='9')
		{
			switch(str[i])
		{
			case '0':
				DrawSpChar(pDC,pos1,0);
				break;
			case '1':
				DrawSpChar(pDC,pos1,1);
				break;
			case '2':
				DrawSpChar(pDC,pos1,2);
				break;
			case '3':
				DrawSpChar(pDC,pos1,3);
				break;
			case '4':
				DrawSpChar(pDC,pos1,4);
				break;
			case '5':
				DrawSpChar(pDC,pos1,5);
				break;
			case '6':
				DrawSpChar(pDC,pos1,6);
				break;
			case '7':
				DrawSpChar(pDC,pos1,7);
				break;
			case '8':
				DrawSpChar(pDC,pos1,8);
				break;
			case '9':
				DrawSpChar(pDC,pos1,9);
				break;
		
		}
		}
		else 
			DrawSpChar(pDC,pos1,str[i]);
		pos1.x=pos1.x+20;
	}
}

void CPTView::CreateBullet(double gunpos,CPoint pos)
{
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	

	int i=0;
	while(pDoc->InGameObs.Bullet.GetAt(i).m_bInGame && i<39)
	{
		i++;
	}
			
	if(i<39)
		{
			CPoint position(pos.x+50+40*cos(gunpos),pos.y+100-85-40*sin(gunpos));
			CPoint vector(20*cos(gunpos),20*sin(gunpos));
			CPTGameObject ob(1,position,vector,TRUE,0);
			pDoc->InGameObs.Bullet.SetAt(i,ob);
			pDoc->GIInfo.AddPoints(-1);
			pDoc->GIInfo.m_bCanShoot=FALSE;
			++pDoc->GIStatistics.m_lShot;
		}
}



void CPTView::UpdateAll()
{
	
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	UpdateBullet();
	if(pDoc->GIInfo.GetLevel()%3!=0)
	{
	UpdateHeli();
	UpdateTrooper();
	}
	else {UpdatePlane();
	UpdateBomb();}

	
	UpdateExplosion();

	
}

void CPTView::DrawAllObjects(CDC *pDC)
{

	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//bullets
	for(int i=0;i<pDoc->InGameObs.Bullet.GetSize();i++)
	{
		
		if(pDoc->InGameObs.Bullet.GetAt(i).m_bInGame)
		{
			CPoint pos=pDoc->InGameObs.Bullet.GetAt(i).m_ptPos;
			
			DrawTransparentBitmap(pDC,&m_bmpBullet,pos.x,pos.y,RGB(255,255,255));
		}		
		
	}
	//bombs
	for(int j=0;j<pDoc->InGameObs.Bomb.GetSize();j++)
	{
		
		if(pDoc->InGameObs.Bomb.GetAt(j).m_bInGame)
		{
			CPoint pos=pDoc->InGameObs.Bomb.GetAt(j).m_ptPos;
			
			DrawTransparentBitmap(pDC,&m_bmpBomb,pos.x,pos.y,RGB(255,255,255));
		}		
		
	}



	//troopers+chute
	if(pDoc->GIInfo.GetLevel()%3!=0)
	{
	for(int r=0;r<pDoc->InGameObs.Trooper.GetSize();r++)
	{
		
		if(pDoc->InGameObs.Trooper.GetAt(r).m_bInGame)
		{
			CPoint pos=pDoc->InGameObs.Trooper.GetAt(r).m_ptPos;
			if(pDoc->InGameObs.Trooper.GetAt(r).m_nFlags==555)
				DrawTransparentBitmap(pDC,&m_bmpTroopLand,pos.x,pos.y,RGB(255,0,0));
			if(pDoc->InGameObs.Trooper.GetAt(r).m_nFlags!=555)
				DrawTransparentBitmap(pDC,&m_bmpTroopFall,pos.x,pos.y,RGB(255,0,0));
				if(pDoc->InGameObs.Chute.GetAt(r).m_bInGame)
				{
					CPoint pos2=pDoc->InGameObs.Chute.GetAt(r).m_ptPos;
					DrawTransparentBitmap(pDC,&m_bmpChute,pos2.x,pos2.y,RGB(255,255,255));
				}
		}
		
		
	}
	//helis
	for(int j=0;j<pDoc->InGameObs.Heli.GetSize();j++)
	{
		
		if(pDoc->InGameObs.Heli.GetAt(j).m_bInGame)
		{
			CPoint pos=pDoc->InGameObs.Heli.GetAt(j).m_ptPos;
			if(pDoc->InGameObs.Heli.GetAt(j).m_ptVector.x<0)
			{
				switch(pDoc->InGameObs.Heli.GetAt(j).m_nFlags)
			{
				
				case 0 :
					DrawTransparentBitmap(pDC,&m_bmpHelitoLeftRed,pos.x,pos.y,RGB(255,255,255));
					break;
				case 1:
					DrawTransparentBitmap(pDC,&m_bmpHelitoLeftGreen,pos.x,pos.y,RGB(255,255,255));
					break;
				case 2:
					DrawTransparentBitmap(pDC,&m_bmpHelitoLeftPurp,pos.x,pos.y,RGB(255,255,255));
					break;
				case 3 :
					DrawTransparentBitmap(pDC,&m_bmpHelitoLeftRed,pos.x,pos.y,RGB(255,255,255));
					break;
				case 4:
					DrawTransparentBitmap(pDC,&m_bmpHelitoLeftGreen,pos.x,pos.y,RGB(255,255,255));
					break;
				case 5:
					DrawTransparentBitmap(pDC,&m_bmpHelitoLeftPurp,pos.x,pos.y,RGB(255,255,255));
					break;
			
			}
			}
			else 
			{		
					switch(pDoc->InGameObs.Heli.GetAt(j).m_nFlags)
				{
					case 0:
						DrawTransparentBitmap(pDC,&m_bmpHelitoRightRed,pos.x,pos.y,RGB(255,255,255));
						break;
					case 1:
						DrawTransparentBitmap(pDC,&m_bmpHelitoRightGreen,pos.x,pos.y,RGB(255,255,255));
						break;
					case 2:
						DrawTransparentBitmap(pDC,&m_bmpHelitoRightPurp,pos.x,pos.y,RGB(255,255,255));
						break;
					case 3 :
						DrawTransparentBitmap(pDC,&m_bmpHelitoRightRed,pos.x,pos.y,RGB(255,255,255));
						break;
					case 4:
						DrawTransparentBitmap(pDC,&m_bmpHelitoRightGreen,pos.x,pos.y,RGB(255,255,255));
						break;
					case 5:
						DrawTransparentBitmap(pDC,&m_bmpHelitoRightPurp,pos.x,pos.y,RGB(255,255,255));
						break;

				}
			}
		}		
	}}
	else 
	{//planes tekene 
		for(int j=0;j<pDoc->InGameObs.Plane.GetSize();j++)
	
		{
		
			if(pDoc->InGameObs.Plane.GetAt(j).m_bInGame)
			{
			CPoint pos=pDoc->InGameObs.Plane.GetAt(j).m_ptPos;
			if(pDoc->InGameObs.Plane.GetAt(j).m_ptVector.x<0)
			{
				switch(pDoc->InGameObs.Plane.GetAt(j).m_nFlags)
			{
				
				case 0 :
					DrawTransparentBitmap(pDC,&m_bmpPlaneToLeftGrey,pos.x,pos.y,RGB(255,255,255));
					break;
				case 1:
					DrawTransparentBitmap(pDC,&m_bmpPlaneToLeftGreen,pos.x,pos.y,RGB(255,255,255));
					break;
				case 2:
					DrawTransparentBitmap(pDC,&m_bmpPlaneToLeftGrey,pos.x,pos.y,RGB(255,255,255));
					break;
				case 3 :
					DrawTransparentBitmap(pDC,&m_bmpPlaneToLeftGreen,pos.x,pos.y,RGB(255,255,255));
					break;
			
			}
			}
			else 
			{		
					switch(pDoc->InGameObs.Plane.GetAt(j).m_nFlags)
				{
					case 0:
						DrawTransparentBitmap(pDC,&m_bmpPlaneToRightGreen,pos.x,pos.y,RGB(255,255,255));
						break;
					case 1:
						DrawTransparentBitmap(pDC,&m_bmpPlaneToRightGrey,pos.x,pos.y,RGB(255,255,255));
						break;
					case 2:
						DrawTransparentBitmap(pDC,&m_bmpPlaneToRightGreen,pos.x,pos.y,RGB(255,255,255));
						break;
					case 3 :
						DrawTransparentBitmap(pDC,&m_bmpPlaneToRightGrey,pos.x,pos.y,RGB(255,255,255));
						break;
					}
			}
			
		}
	}

	}
	//explosies
	for(int k=0;k<pDoc->InGameObs.Explosion.GetSize();k++)
	{
		
		if(pDoc->InGameObs.Explosion.GetAt(k).m_bInGame)
		{
			CPoint pos=pDoc->InGameObs.Explosion.GetAt(k).m_ptPos;
			switch(pDoc->InGameObs.Explosion.GetAt(k).m_nFlags)
			{
			case 1:
				DrawTransparentBitmap(pDC,&m_bmpExplosion2,pos.x,pos.y,RGB(255,255,255));
			case 5:
				DrawTransparentBitmap(pDC,&m_bmpSplatGround,pos.x,pos.y,RGB(255,255,255));
			case 10:
				DrawTransparentBitmap(pDC,&m_bmpSplatAir,pos.x,pos.y,RGB(255,255,255));
				break;
			default:
				DrawTransparentBitmap(pDC,&m_bmpExplosion,pos.x,pos.y,RGB(255,255,255));
			}
			
		}		
		
	}
 
}

void CPTView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	KillAllTimers();
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	m_bPauze=FALSE;

	OnPrepareLevel(pDoc->GIInfo.GetLevel());
	m_bNT=FALSE;
	if(pDoc->GIInfoStatic.info.dwPlatformId==VER_PLATFORM_WIN32_NT)
		m_bNT=TRUE;


}

void CPTView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	KillAllTimers();
	if (pDoc->GIInfo.m_bNewLevel)
	{	
		OnPrepareLevel(pDoc->GIInfo.GetLevel());
	}

}

void CPTView::CreateHeli() //geen positie meegeven, die wordt hier random gegenereerd
{
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	int i=0;
	
	int flagcolor=rand();

	if(flagcolor<=12000)
		flagcolor=0;
	else if(flagcolor<=24000)
		flagcolor=1;
	else if(flagcolor<=36000)
		flagcolor=2;

	CPTGameObject obje;
	while(pDoc->InGameObs.Heli.GetAt(i).m_bInGame && i<pDoc->InGameObs.Heli.GetSize())
	{
		i++;
	}

	if(i<pDoc->InGameObs.Heli.GetSize()&&!pDoc->InGameObs.Heli.GetAt(i).m_bInGame)
		{
		int posrand=rand()/1000;	
			if(rand()<(RAND_MAX)/2)
			{
				
				CPoint position(0,25+posrand);
				CPoint vector(1,0);
				CPTGameObject ob(2,position,vector,TRUE,flagcolor);
				pDoc->InGameObs.Heli.SetAt(i,ob);
				++pDoc->GIStatistics.m_lHeliCreated;
			
			}
			else
			{
				
			
				CPoint position(GameRect.Width(),40+posrand);
				CPoint vector(-1,0);
				CPTGameObject ob(2,position,vector,TRUE,flagcolor);
				pDoc->InGameObs.Heli.SetAt(i,ob);
				++pDoc->GIStatistics.m_lHeliCreated;
			}
		
			
		}	

		
}

void CPTView::CreateExplosion(CPoint pos,int flag)
{	
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	int i=0;

	if(pDoc->GIInfoStatic.m_nSound>0){
	if(flag==5)
	sndPlaySound("wavs\\SPLAT.WAV",SND_ASYNC|SND_NODEFAULT );
	if(flag==0 || flag==1)
	sndPlaySound("wavs\\expl.WAV",SND_ASYNC|SND_NODEFAULT );
	if(flag==10)
	sndPlaySound("wavs\\splatair.WAV",SND_ASYNC|SND_NODEFAULT );
	}
	while(pDoc->InGameObs.Explosion.GetAt(i).m_bInGame && i<pDoc->InGameObs.Explosion.GetSize())
	{
		i++;
	}

	if(i<pDoc->InGameObs.Explosion.GetSize()-1)
		{
		
				CPoint vect;
				vect.x=4;
				CPTGameObject ob(10,pos,vect,TRUE,flag);
				pDoc->InGameObs.Explosion.SetAt(i,ob);
						
		}	
}

void CPTView::KillAllTimers()
{
	for(int i=0;i<5;i++)
		KillTimer(i);
	KillTimer(10);
	KillTimer(20);
	KillTimer(21);
	KillTimer(50);
	KillTimer(100);
	KillTimer(101);
}	

void CPTView::CreateTrooper(CPoint pos)
{
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	int i=0;

	while(pDoc->InGameObs.Trooper.GetAt(i).m_bInGame && i<pDoc->InGameObs.Trooper.GetSize())
	{
		i++;
	}

	if(i<pDoc->InGameObs.Trooper.GetSize()-1 && !pDoc->InGameObs.Trooper.GetAt(i).m_bInGame)
		{
		
				CPoint vect;
				vect.x=0;
				vect.y=1;
				//de trooper
				CPTGameObject ob(3,pos,vect,TRUE,0);
				CPoint pos2;
				pos2.x=pos.x;
				pos2.y=pos.y-25;
				//een parachutje voor de trooper
				CPTGameObject ob2(11,pos2,vect,TRUE,0);
				pDoc->InGameObs.Trooper.SetAt(i,ob);
				pDoc->InGameObs.Chute.SetAt(i,ob2);
				++pDoc->GIStatistics.m_lTrooperCreated;
		}
}

int CPTView::CreateRandom(int min, int max)
{
return (min+rand()*(max-min)/RAND_MAX);
}

void CPTView::CheckGameOver()
{
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	//heli level
	if(pDoc->GIInfo.GetLevel()%3!=0)
	{
		int r=0;
		int left=0;
		int right=0;
		CPoint position;
		while(r<pDoc->InGameObs.Trooper.GetSize())
		{
			position=pDoc->InGameObs.Trooper.GetAt(r).m_ptPos;
			//enkel als in game is
			if(pDoc->InGameObs.Trooper.GetAt(r).m_bInGame && pDoc->InGameObs.Trooper.GetAt(r).m_nFlags==555)
			{
				if(position.x<GameRect.Width()/2-70)
					left++;
				if(position.x>GameRect.Width()/2+70)
					right++;
			}
		r++;
		}
		
		if(left>3 || right>3)
		{
			//leven kwijt
			pDoc->GIInfo.AddLifes(-1);
			if(pDoc->GIInfo.GetLifes()==0)
			{
				pDoc->GIInfo.m_strLevelStart="  Game Over";
				KillAllTimers();
				Invalidate();
				pDoc->InGameObs.Clear();
				pDoc->GIInfo.m_bInGame=FALSE;
				
			
			}
			else
			{
				pDoc->GIInfo.m_strLevelStart="  try again";
				KillAllTimers();
				Invalidate();
				pDoc->InGameObs.Clear();
				pDoc->GIInfo.SetLevel(pDoc->GIInfo.GetLevel());
				OnUpdate(this,NULL,NULL);
			}
		}

	}
	else
	{
		BITMAP gun;
		m_bmpGunBody.GetBitmap(&gun);

		int s=0;
		while(s<pDoc->InGameObs.Bomb.GetSize())
		{
			CPoint posg=m_ptGunpos;
			CPoint posb=pDoc->InGameObs.Bomb.GetAt(s).m_ptPos;
			//enkel als in game is
			if(pDoc->InGameObs.Bomb.GetAt(s).m_bInGame )
			{
			 if(posb.x>posg.x && posb.x<posg.x+gun.bmWidth && posb.y>posg.y&&posb.y<posg.y+gun.bmHeight)
			 {
				CreateExplosion(posb,1);
				
				 //leven kwijt
				pDoc->GIInfo.AddLifes(-1);
				if(pDoc->GIInfo.GetLifes()==0)
				{
				pDoc->GIInfo.m_strLevelStart="  Game Over";
				
					KillAllTimers();
					//Invalidate();
					pDoc->InGameObs.Clear();
					pDoc->GIInfo.m_bInGame=FALSE;
				}
				else
				{
					pDoc->GIInfo.m_strLevelStart="  try again";
					KillAllTimers();
				//	Invalidate();
					pDoc->InGameObs.Clear();
					pDoc->GIInfo.SetLevel(pDoc->GIInfo.GetLevel());
					OnUpdate(this,NULL,NULL);
				}
			 }
		}
		s++;
		}
	}
}

void CPTView::CreatePlane()
{
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	int i=0;
	
	int flagcolor=rand();

	if(flagcolor<=18000)
		flagcolor=0;
	else 
		flagcolor=1;

	CPTGameObject obje;
	while(pDoc->InGameObs.Plane.GetAt(i).m_bInGame && i<pDoc->InGameObs.Plane.GetSize())
	{
		i++;
	}

	if(i<pDoc->InGameObs.Plane.GetSize()-1&&!pDoc->InGameObs.Plane.GetAt(i).m_bInGame)
		{
		int posrand=rand()/1000;	
			if(rand()<(RAND_MAX)/2)
			{
				
				CPoint position(0,25+posrand);
				CPoint vector(1,0);
				CPTGameObject ob(6,position,vector,TRUE,flagcolor);
				pDoc->InGameObs.Plane.SetAt(i,ob);
				++pDoc->GIStatistics.m_lPlaneCreated;
			
			}
			else
			{
				
			
				CPoint position(GameRect.Width(),40+posrand);
				CPoint vector(-1,0);
				CPTGameObject ob(6,position,vector,TRUE,flagcolor);
				pDoc->InGameObs.Plane.SetAt(i,ob);
				++pDoc->GIStatistics.m_lPlaneCreated;
			}
		
			

		}	

		
}

void CPTView::CreateBomb(CPoint pos)
{
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	int i=0;

	while(pDoc->InGameObs.Bomb.GetAt(i).m_bInGame && i<pDoc->InGameObs.Bomb.GetSize())
	{
		i++;
	}

	if(i<pDoc->InGameObs.Bomb.GetSize()-1 && !pDoc->InGameObs.Bomb.GetAt(i).m_bInGame)
		{
		
				CPoint vect;
				vect.x=(m_ptGunpos.x+50-pos.x)/50; //delen door snelheid bom
				vect.y=-(m_ptGunpos.y-10-pos.y)/50;
				CPTGameObject ob(12,pos,vect,TRUE,0);
				pDoc->InGameObs.Bomb.SetAt(i,ob);
				++pDoc->GIStatistics.m_lBombCreated;
		}
}

void CPTView::UpdateHeli()
{
	
	
	BITMAP heli;
	m_bmpHelitoLeftRed.GetBitmap(&heli);
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	int j=0;
	//hele array doorlopen
	while(j<pDoc->InGameObs.Heli.GetSize())
	{
		//enkel updaten als heli ingame is!
		if(pDoc->InGameObs.Heli.GetAt(j).m_bInGame)
		{
			CPoint oldPos=pDoc->InGameObs.Heli.GetAt(j).m_ptPos;
			CPoint vector=pDoc->InGameObs.Heli.GetAt(j).m_ptVector;
			CPoint position(oldPos.x+pDoc->GIInfoStatic.m_nDifficulty*3*vector.x,oldPos.y-pDoc->GIInfoStatic.m_nDifficulty*3*vector.y);
			int flags=pDoc->InGameObs.Heli.GetAt(j).m_nFlags;
			
			//als heli's buiten scherm zijn => verwijderen
			if(position.x<-50 || position.y<0 || position.x>GameRect.Width()|| position.y>GameRect.Height())
			{	CPTGameObject ob(2,(0,0),(0,0),FALSE,0);
				pDoc->InGameObs.Heli.SetAt(j,ob);
		
			}


			//positie heli updaten +eventueel paratroopers maken
			else
			{
				
				//////////////////////////////////////
				//////Paratrooper spawnen/////////////
				//////////////////////////////////////
				if(rand()<=100*pDoc->GIInfoStatic.m_nDifficulty+pDoc->GIInfo.GetLevel()*30 )
				{
					if(position.x<GameRect.Width()/2-70 || position.x>GameRect.Width()/2+70)
					{
						if(position.x>30 && position.x<GameRect.Width()-30)
						{
							if(flags<3)
							{
								CreateTrooper(position);
								flags=flags+3;
							}
						}
					}
				}		
				//////////////////////////////////////////
				///////Einde paratrooper spawnen//////////
				//////////////////////////////////////////
				CPTGameObject ob(2,position,vector,TRUE,flags);
				pDoc->InGameObs.Heli.SetAt(j,ob);
			}//einde positie heli updaten
		}		
		j++;
	}	
}

void CPTView::UpdateTrooper()
{
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	BITMAP trooper;
	BITMAP chute;
	m_bmpTroopFall.GetBitmap(&trooper);

	m_bmpChute.GetBitmap(&chute);

	int r=0;
	int flagstrooper=0;
	//hele array doorlopen
	while(r<pDoc->InGameObs.Trooper.GetSize())
	{
		//enkel als in game is
		if(pDoc->InGameObs.Trooper.GetAt(r).m_bInGame)
		{
			CPoint oldPos=pDoc->InGameObs.Trooper.GetAt(r).m_ptPos;
			CPoint vector=pDoc->InGameObs.Trooper.GetAt(r).m_ptVector;
			CPoint position;
			CPoint temp;
			//als trooper chute heeft valt hij 'gewoon'
			if(pDoc->InGameObs.Chute.GetAt(r).m_bInGame) //valsnelheid: als geen chute wordt 'else' uitgevoerd
			{
				temp.x=oldPos.x;temp.y=oldPos.y+3*vector.y;
				position=temp;
				flagstrooper=0;
				CPoint positionchute;
				positionchute.x=position.x;
				positionchute.y=position.y-chute.bmHeight;
				CPTGameObject ob4(11,positionchute,vector,TRUE,flagstrooper);
				pDoc->InGameObs.Chute.SetAt(r,ob4);
			}
			//anders valt hij dood!
			else 
			{
				//if(pDoc->InGameObs.Trooper.GetAt(r).m_nFlags!=555 &&pDoc->InGameObs.Trooper.GetAt(r).m_nFlags!=554)
				//{
				temp.x=oldPos.x;temp.y=oldPos.y+10*vector.y;
				position=temp;
				flagstrooper=666;
				//}
				
			}
			//als troopers buiten scherm zijn => verwijderen
			if(position.x<-50 || position.y<0 || position.x>GameRect.Width()|| position.y>GameRect.Height())
			{	
				CPTGameObject ob2(3,(0,0),(0,0),FALSE,0);
				pDoc->InGameObs.Trooper.SetAt(r,ob2);
				CPTGameObject ob(11,(0,0),(0,0),FALSE,0);
				pDoc->InGameObs.Chute.SetAt(r,ob);
		
			}
			//positie troopers updaten
			else
			{
				
				//echter als geland//
				if(pDoc->InGameObs.Trooper.GetAt(r).m_nFlags!=666 && position.y+trooper.bmHeight>=GameRect.Height())
				{
					vector.y=0; //geen valsnelheid meer => eagle had landed
					vector.x=0;
					flagstrooper=555;
					//if(flagstrooper!=555){
					//	++pDoc->GIStatistics.m_lLanded;
					//	flagstrooper=555;}
					CPTGameObject ob2(11,(0,0),(0,0),FALSE,0); //chute niet meer nodig
					pDoc->InGameObs.Chute.SetAt(r,ob2);
					
					
				}
				if(position.y+trooper.bmHeight>=GameRect.Height() && flagstrooper==666)//val dood op andere trooper
				{
					//alle landed nazien
					int y=0;
					while(y<pDoc->InGameObs.Trooper.GetSize())
						{
							if(pDoc->InGameObs.Trooper.GetAt(y).m_bInGame&& pDoc->InGameObs.Trooper.GetAt(y).m_nFlags==555 &&pDoc->InGameObs.Trooper.GetAt(y).m_ptPos.x-position.x<trooper.bmWidth*0.80 && pDoc->InGameObs.Trooper.GetAt(y).m_ptPos.x-position.x>=-(trooper.bmWidth*0.80))
							{
								pDoc->GIInfo.AddPoints(3*pDoc->GIInfoStatic.m_nDifficulty);
								CPTGameObject ob(1,(0,0),(0,0),FALSE,0);
  								pDoc->InGameObs.Trooper.SetAt(y,ob);
														
							}
							y++;
						}
					CreateExplosion(position,5);
				}
				CPTGameObject ob(3,position,vector,TRUE,flagstrooper);
				
				
				pDoc->InGameObs.Trooper.SetAt(r,ob);
			
			}
		}		
		r++;
	

	}
}

void CPTView::UpdateBullet()
{

	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	BITMAP trooper;
	BITMAP chute;
	BITMAP bomb;
	BITMAP heli;
	BITMAP plane;
	m_bmpPlaneToLeftGrey.GetBitmap(&plane);
	m_bmpBomb.GetBitmap(&bomb);
	m_bmpTroopFall.GetBitmap(&trooper);
	m_bmpChute.GetBitmap(&chute);
	m_bmpHelitoLeftRed.GetBitmap(&heli);

	int i=0;
	while(i<pDoc->InGameObs.Bullet.GetSize())
	{
		
		if(pDoc->InGameObs.Bullet.GetAt(i).m_bInGame)
		{
			CPoint oldPos=pDoc->InGameObs.Bullet.GetAt(i).m_ptPos;
			CPoint vector=pDoc->InGameObs.Bullet.GetAt(i).m_ptVector;
			CPoint position(oldPos.x+vector.x,oldPos.y-vector.y);
			//Kogels verwijderen als ze buiten scherm zijn
			if(position.x<=0 || position.y<=0 || position.x>GameRect.Width()|| position.y>GameRect.Height())
			{	
				CPTGameObject ob(1,(0,0),(0,0),FALSE,0);
				pDoc->InGameObs.Bullet.SetAt(i,ob);
			}
			//Positie kogel updaten
			else
			{
				CPTGameObject ob(1,position,vector,TRUE,0);
				pDoc->InGameObs.Bullet.SetAt(i,ob);
	//********************collision detection**************************
		///////////////////////////////////////////////////////////
		////////////// Collision met trooper(enkel in levels 1en2//
		///////////////////////////////////////////////////////////
		if(pDoc->GIInfo.GetLevel()%3!=0)
		{
			if(pDoc->InGameObs.Bullet.GetAt(i).m_bInGame)
			{
				for(int g=0;g<pDoc->InGameObs.Trooper.GetSize();g++)
				{
					CPoint postroop=pDoc->InGameObs.Trooper.GetAt(g).m_ptPos;
					bool alive=pDoc->InGameObs.Trooper.GetAt(g).m_bInGame;
					
					
					if(alive && postroop.x<=position.x && position.x<=trooper.bmWidth+postroop.x && postroop.y<=position.y && position.y<=trooper.bmHeight+postroop.y )
					{
	
						CreateExplosion(pDoc->InGameObs.Trooper.GetAt(g).m_ptPos,10);
						pDoc->GIInfo.AddPoints(10);
						CPTGameObject ob(1,(0,0),(0,0),FALSE,0);
						pDoc->InGameObs.Bullet.SetAt(i,ob);
						CPTGameObject dest(3,(0,0),(0,0),FALSE,0);
						pDoc->InGameObs.Trooper.SetAt(g,dest);
						CPTGameObject chu(11,(0,0),(0,0),FALSE,0);
						pDoc->InGameObs.Chute.SetAt(g,chu);
						g=pDoc->InGameObs.Trooper.GetSize()-1;
						++pDoc->GIStatistics.m_lTrooperShot;
						
					}
				}			
			}
		/////////////////////////////////////////////////////////
		////////////// Collision met chute(enkel in levels 1en2)//
		/////////////////////////////////////////////////////////

			if(pDoc->InGameObs.Bullet.GetAt(i).m_bInGame)
			{
				for(int h=0;h<pDoc->InGameObs.Chute.GetSize();h++)
				{
					CPoint poschute=pDoc->InGameObs.Chute.GetAt(h).m_ptPos;
					bool alive=pDoc->InGameObs.Chute.GetAt(h).m_bInGame;
					
					
					if(alive && poschute.x<=position.x && position.x<=chute.bmWidth+poschute.x && poschute.y<=position.y && position.y<=chute.bmHeight+poschute.y )
					{
							
  							
						CreateExplosion(pDoc->InGameObs.Chute.GetAt(h).m_ptPos,0);					
						pDoc->GIInfo.AddPoints(10);
						CPTGameObject ob(1,(0,0),(0,0),FALSE,0);
						pDoc->InGameObs.Bullet.SetAt(i,ob);
						CPTGameObject dest(11,(0,0),(0,0),FALSE,0);
						pDoc->InGameObs.Chute.SetAt(h,dest);
						h=pDoc->InGameObs.Trooper.GetSize()-1;
						++pDoc->GIStatistics.m_lChuteShot;
						if(pDoc->GIInfoStatic.m_nSound>0)
						sndPlaySound("wavs\\FALL.WAV",SND_ASYNC|SND_NODEFAULT );
					}
				}			
			}		


		////////////////////////////////////////////////////////
		//////////// Collision met heli's(enkel in level 1en2///
		////////////////////////////////////////////////////////
			if(pDoc->InGameObs.Bullet.GetAt(i).m_bInGame)
			{
				for(int l=0;l<pDoc->InGameObs.Heli.GetSize();l++)
				{
					CPoint posheli=pDoc->InGameObs.Heli.GetAt(l).m_ptPos;
					bool alive=pDoc->InGameObs.Heli.GetAt(l).m_bInGame;
					
					
					if(alive && posheli.x<=position.x && position.x<=heli.bmWidth+posheli.x && posheli.y<=position.y && position.y<=heli.bmHeight+posheli.y )
					{
							
  							
						CreateExplosion(pDoc->InGameObs.Heli.GetAt(l).m_ptPos,0);
						pDoc->GIInfo.AddPoints(15);
						CPTGameObject ob(1,(0,0),(0,0),FALSE,0);
						pDoc->InGameObs.Bullet.SetAt(i,ob);
						CPTGameObject dest(2,(0,0),(0,0),FALSE,0);
						pDoc->InGameObs.Heli.SetAt(l,dest);
						l=pDoc->InGameObs.Heli.GetSize()-1;
						++pDoc->GIStatistics.m_lHeliShot;
					}
				}
			}
		}
	else 

		////////////////////////////////////////////////////////
		//////////// Collision met planes(enkel in level 3   ///
		////////////////////////////////////////////////////////
		{
		if(pDoc->InGameObs.Bullet.GetAt(i).m_bInGame)
			{
				for(int l=0;l<pDoc->InGameObs.Plane.GetSize();l++)
				{
					CPoint posheli=pDoc->InGameObs.Plane.GetAt(l).m_ptPos;
					bool alive=pDoc->InGameObs.Plane.GetAt(l).m_bInGame;
					
					
					if(alive && posheli.x<=position.x && position.x<=plane.bmWidth+posheli.x && posheli.y<=position.y && position.y<=plane.bmHeight+posheli.y )
					{
							
  							
						CreateExplosion(pDoc->InGameObs.Plane.GetAt(l).m_ptPos,0);

						pDoc->GIInfo.AddPoints(20);
						CPTGameObject ob(1,(0,0),(0,0),FALSE,0);
						pDoc->InGameObs.Bullet.SetAt(i,ob);
						CPTGameObject dest(2,(0,0),(0,0),FALSE,0);
						pDoc->InGameObs.Plane.SetAt(l,dest);
						l=pDoc->InGameObs.Plane.GetSize()-1;
						++pDoc->GIStatistics.m_lPlaneShot;
					}
				}
			}
		}
			////////////////////////////////////////////////////////
		    //////////// Collision met bommen   (enkel in level 3///
		    ////////////////////////////////////////////////////////
				if(pDoc->InGameObs.Bomb.GetAt(i).m_bInGame)
			{
				for(int h=0;h<pDoc->InGameObs.Bomb.GetSize();h++)
				{
					CPoint posBomb=pDoc->InGameObs.Bomb.GetAt(h).m_ptPos;
					bool alive=pDoc->InGameObs.Bomb.GetAt(h).m_bInGame;
					
						if(alive && posBomb.x-position.x<=bomb.bmWidth+3 && position.y-posBomb.y<=bomb.bmHeight+3)
						{
							if(posBomb.x-position.x>=-bomb.bmWidth-3 && position.y-posBomb.y>=-bomb.bmHeight-3)
							{
							CreateExplosion(pDoc->InGameObs.Bomb.GetAt(h).m_ptPos,0);					
							pDoc->GIInfo.AddPoints(25);
							CPTGameObject ob(1,(0,0),(0,0),FALSE,0);
							pDoc->InGameObs.Bullet.SetAt(i,ob);
							CPTGameObject dest(11,(0,0),(0,0),FALSE,0);
							pDoc->InGameObs.Bomb.SetAt(h,dest);
							}
						++pDoc->GIStatistics.m_lBombShot;
						
					}
				}			
			}		

			}
		}	//////////////////////////////////////////////////////////
		////////////////////Collision gedaan ////////////////////
			//////////////////////////////////////////////////////////	
		i++;
	}	
}

void CPTView::UpdateExplosion()
{
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	int k=0;
	while(k<pDoc->InGameObs.Explosion.GetSize())
	{
		CPTGameObject expl=pDoc->InGameObs.Explosion.GetAt(k);
		expl.m_ptVector.x=expl.m_ptVector.x-1;
		pDoc->InGameObs.Explosion.SetAt(k,expl);
		if(pDoc->InGameObs.Explosion.GetAt(k).m_bInGame && pDoc->InGameObs.Explosion.GetAt(k).m_ptVector.x==0  )
		{
		
				CPTGameObject ob(10,(0,0),(0,0),FALSE,0);
				pDoc->InGameObs.Explosion.SetAt(k,ob);
		}	
	k++;
	}
	
}

void CPTView::UpdatePlane()
{
//PLANES
	
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	  {
	int w=0;
	//hele array doorlopen
	while(w<pDoc->InGameObs.Plane.GetSize())
	{
		//enkel updaten als plane ingame is!
		if(pDoc->InGameObs.Plane.GetAt(w).m_bInGame)
		{
			CPoint oldPos=pDoc->InGameObs.Plane.GetAt(w).m_ptPos;
			CPoint vector=pDoc->InGameObs.Plane.GetAt(w).m_ptVector;
			CPoint position(oldPos.x+pDoc->GIInfoStatic.m_nDifficulty*5*vector.x,oldPos.y-pDoc->GIInfoStatic.m_nDifficulty*3*vector.y);
			int flags=pDoc->InGameObs.Plane.GetAt(w).m_nFlags;
			
			//als planes buiten scherm zijn => verwijderen
			if(position.x<-50 || position.y<0 || position.x>GameRect.Width()|| position.y>GameRect.Height())
			{	CPTGameObject ob(2,(0,0),(0,0),FALSE,0);
				pDoc->InGameObs.Plane.SetAt(w,ob);
		
			}


			//positie heli updaten +eventueel bom gooien
			else
			{
				
				//////////////////////////////////////
				//////Bom spawnen/////////////
				//////////////////////////////////////
				if(rand()<=100*pDoc->GIInfoStatic.m_nDifficulty+pDoc->GIInfo.GetLevel()*60 )
				{
					if(position.x<GameRect.Width()/2-70 || position.x>GameRect.Width()/2+70)
					{
						if((vector.x>0 && position.x<GameRect.Width())||(vector.x<0 && position.x>GameRect.Width()))
						{
						if(position.x>30 && position.x<GameRect.Width()-30)
						{
							if(flags<2)
							{
								CreateBomb(position);
								flags=flags+2;
							}
						}
						}
					}
				}		
				//////////////////////////////////////////
				///////Einde bom spawnen//////////
				//////////////////////////////////////////
				CPTGameObject ob(2,position,vector,TRUE,flags);
				pDoc->InGameObs.Plane.SetAt(w,ob);
			}//einde positie heli updaten
		}		
		w++;
	  }}

}

void CPTView::UpdateBomb()
{
	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	int y=0;
	while(y<pDoc->InGameObs.Bomb.GetSize())
	{
		
		if(pDoc->InGameObs.Bomb.GetAt(y).m_bInGame)
		{
			CPoint oldPos=pDoc->InGameObs.Bomb.GetAt(y).m_ptPos;
			CPoint vector=pDoc->InGameObs.Bomb.GetAt(y).m_ptVector;
			CPoint position(oldPos.x+vector.x,oldPos.y-vector.y);
			//Kogels verwijderen als ze buiten scherm zijn
			if(position.x<=0 || position.y<=0 || position.x>GameRect.Width()|| position.y>GameRect.Height())
			{	
				CPTGameObject ob(1,(0,0),(0,0),FALSE,0);
				pDoc->InGameObs.Bomb.SetAt(y,ob);
			}
			//Positie kogel updaten
			else
			{
				CPTGameObject ob(1,position,vector,TRUE,0);
				pDoc->InGameObs.Bomb.SetAt(y,ob);
			}
		}
	y++;
	}


}

void CPTView::Pause()
{

	CPTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->GIInfo.m_bInGame && m_bPauze==FALSE)
		{
			KillAllTimers();
			pDoc->GIInfo.m_bInGame=FALSE;
			m_bPauze=TRUE;
			Invalidate();
		}
		else if(m_bPauze==TRUE)
		{
		m_bPauze=FALSE;
		pDoc->GIInfo.m_bInGame=TRUE;
		SetTimer(1,50,NULL);
		SetTimer(50,1000,NULL);
		int timerrand=0;
		while(timerrand<100*pDoc->GIInfoStatic.m_nDifficulty+pDoc->GIInfo.GetLevel()*50)
		{	
			timerrand=CreateRandom(0,5000);
			SetTimer(10,timerrand,NULL);
		}
		switch(pDoc->GIInfo.GetLevel()%3){	//Naargelang level andere achtergrond en timer etc
			case 1:
				SetTimer(100,100000-1000*m_nLevSeconden,NULL);
				break;
			case 2:
				SetTimer(100,100000-1000*m_nLevSeconden,NULL);
				break;
			case 0:
				SetTimer(100,20000-1000*m_nLevSeconden,NULL);
				break;
			}
		}
}
