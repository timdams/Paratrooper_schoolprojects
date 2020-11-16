// Paratrooper DX.cpp : Defines the entry point for the application.
// Voornaamste bron : 
//		**************************************************************
//			TitelDirectX in 24
//			Auteurs:Robert Dunlop,Dale Shepherd en Mark Martin
//			Uitgeverij: Academic Services
//			ISBN:90 395 14941
//	    **************************************************************
// 
// Pre programming stuff
//		*** DirectX installatie ***
// Ga naar http://msdn.microsoft.com , en zoek naar 'directx  sdk'.
// Een van de hits zou 'download DirectX Software Development Kit' of zo moeten zijn.
// Download deze en installeer het programma. Je hebt drie zaken zeker nodig:
//	1) de runtime
//	2) de includes
//	3) de libraries
// Al de rest kan je in principe weglaten als je met hardeschijf ruimte kampt.
//	De help files zullen wel vrij onmisbaar worden eens je serieus begint te programmeren.
// Ik raad wel aan om zeker de samples eens te bekijken!

// In Visual Studio moet je er nu nog wel voor zorgen dat de comiler de juiste files kan vinden.
// 
// Start Visual Studio op en ga naar 'tools'-> 'Options'. Kies dan tab-blad: 'directories'
// Kies 'show directories for: INCLUDE files' en typ voeg de map toe van je includes (standaard c:\dxsdk\include)
// Kies 'show directories for: Library files' en typ voeg de map toe van je libs (standaard c:\dxsdk\lib)

//    **** FMOD installatie  *** 
// Ga naar http://www.fmod.org en kies daar bij downloads die installatie versie
// Installeer het programma in een map naar keuze en voeg wederom te includes en libs toe via tools->options.
// Deze keer verwijs je uiteraard naar de fmod directory.



//			//////////////////START DIRECTX TUTORIAL /////////////////////
// First things first!
//  Begin nieuw project -> Win32Apllication -> A simple Win32 application
// Als je workspace gemaakt is moet je nog wel enkele libraries toevoegen, naargelang benodigd natuurlijk:
//
//	winmm.lib		-- Voor multimediaklasse (gebruikt voor CWave klasse)	
//	FMODVC.LIB		-- Voor fmod
//  ddraw.lib		-- Voor directdraw
//	dinput8.lib		-- Voor directinput  (zonder 8 gaat ook maar dan heb je niet alle intressante opties van dx8(force feedback etc)
//	dsound.lib		-- Voor directsound
//  gdi32.lib		-- Voor gdi tekenen (normaal standaard toegevoegd)
//	
    
//	Toevoegen van libraries: Project ->Settings->Link
//	Je kan dit ook doen doen mbv #pragma,als volgt:
//			#pragma comment(lib, "Dxguid.lib")

//	*****Opgelet**********
//	Wanneer je als active build configuration (build->Set Active Configuration)  bijvoorbeeld 'Release' neemt, dan moet je hiervoor
//	ook de libraries weer toevoegen via Project->Settings->Link.
//  **********************		
// Nu ben je klaar voor directx-programmeren!
// 
//	Let op, volgend spel is weliswaar geschreven als tutorial maar het is wel vrij groot en niet altijd even evident. Een degelijke kennis 'spel'programmeren
//  is dan ook gewenst (zaken als gameloop, pageflipping,etc moet je begrijpen.)
//  De code is verre van geoptimaliseerd en op vele plaatsen zeer slordig. Belangrijkste stukken zullen echter steeds becommentarieerd zijn!

// Enkele algemene programmeertips die misschien niet meer zo evident zijn wanneer je een jaar of meer met MFC hebt gespeeld.
//		*	Wanneer je een functie ergens aanroept moet je erop letten dat deze functie BOVEN de huidige staat, anders krijg je een undefined local error
//			Wens je toch een functie 'lager' te gebruiken zorg er dan voor dat na de definities de prototypes van de functies staan
//		*	Je aanknoop punt voor het programma is de WinMain, na de nodige includes zullen we daar de tutorial voortzetten. 
//
// Hoe te debuggen:
//	Door DirectX is het niet mogelijk om te debuggen in de vorm van breakpoints te zetten en vars daaropvolgend na te zien
//	Een truc die ik meestal toepas is gewoon de gewenste vars mbv DoRenderFrame op het scherm te tonen. (zie daar naar //debug stuff ************)
//	Ook kan je indien gewenst de frame rendering vertragen door de timertijd te vergroten (zie winmain() hiervoor )

// Volgende includes zijn nodig voor compiler, hun beschrijving staat er naast
#include "stdafx.h"
#include "wingdi.h"	//Gebruikt voor tekst en loop te tekenen met een dc

//	Volgende definitie moet VOOR de header-includes van DirectX staan, anders kan de compiler niet 
//	werken met de GUID-bibliotheek (voor verwijzingen naar interfaces. Zie 'Using COM' in DirectX help:
#define INITGUID

#include <stdio.h>
#include <ddraw.h>			//DirectDraw
#include <dinput.h>			//DirectInput
#include <mmsystem.h>		//MultiMedia uitbreiding nodig om gebruik DirectSound te versimpelen
#include <dsound.h>			//Direct Sound
#include "wave.h"			//Hulpklasse om afspelen via DirectSound te vereenvoudigen
#include "fmod.h"			//FMOD-klasse gebruikt voor het afspelen van mp3-streams
#include "math.h"			//Voor cos() en sin()
#include "stdlib.h"	
#include "time.h"			//tijd gebruiken voor seed van srand (zie verder)
#include "PTDXGameObject.h" //Objectklasse (grotendeels van vorig jaar) waarin heli,para,etc beschreven zal worden

// Volgende algemene initialisaties zullen duidelijk worden naarmate tutorial vordert. Voor nu is het 
// intressant om weten dat de vele 'defines' van ints het werken met ints makkelijker maakt (HELI zegt meer dan 1)
// Ga er eventjes snel, over en spring dan na de prototypes naar winmain.

//------ Window Class Information ------
// Nodig om ons programma als klasse in windows te omschrijven

static char szClass[] = "PTDXClass";
static char szCaption[] = "ParaTrooper DX";
HWND hWnd; //Handle naar het programma
bool bHasFocus; // Bool nodig om game-loop te onderbreken wanneer programma focus verliest (via WM_ACTIVATEAPP)
bool bIsInit;   // Bool om eenmalige init van alles aan te roepen en nooit meer dan 1 keer1
//------ Global Interface Pointers naar de directdraw surfaces ------//

LPDIRECTDRAW7 lpDD=NULL;	//DirectDraw object
LPDIRECTDRAWSURFACE7 lpDDSPrimary=NULL;	//Primary Surface
LPDIRECTDRAWSURFACE7 lpDDSBack=NULL;	//Secondory surface (backbuffer)

//------ DirectDraw Surfaces for Object Storage ------//
//Surfaces waarop verschillende bitmaps zullen gestokeerd worden.
//Voor het invoegen van bitmaps geldt volgende simpele volgorde:
//	1)Surface maken
//	2)Bitmap inladen (hier in InitBitmaps())
//	3)Eventueel de colorkey instellen voor transparantie (ook in InitBitmaps())
//	4)Je bitmapsurface gebruiken (meestal in RenderFrame)
//	5)Surface verwijderen (hier in CleanUp() )

//  Info: je kan ook dezelfde surface voor meerdere bitmaps gebruiken maar dit heeft als voordeel dat programma veel minder 
//			geheugen inneemt, maar zorgt wel voor een kleine vertraging bij inladen bitmaps (=ongewenst bij snelle actiespellen)
		
LPDIRECTDRAWSURFACE7 int_surf=NULL;		// Interface
LPDIRECTDRAWSURFACE7 back_surf=NULL;	// Achtegrond
LPDIRECTDRAWSURFACE7 back2_surf=NULL;
LPDIRECTDRAWSURFACE7 back3_surf=NULL;
LPDIRECTDRAWSURFACE7 heli_surf=NULL;	
LPDIRECTDRAWSURFACE7 kanonl_surf=NULL;	//Kannon-lijf
LPDIRECTDRAWSURFACE7 kanonr_surf=NULL;
LPDIRECTDRAWSURFACE7 title_surf=NULL;	//Titelscherm
LPDIRECTDRAWSURFACE7 cursor_surf=NULL;	//Titelscherm cursor
LPDIRECTDRAWSURFACE7 bullet_surf=NULL;
LPDIRECTDRAWSURFACE7 GetReady_surf=NULL;	//Message 'getready' 	
LPDIRECTDRAWSURFACE7 loading_surf=NULL;		//Message 'loading'
LPDIRECTDRAWSURFACE7 levdone_surf=NULL;		//Message 'level done'
LPDIRECTDRAWSURFACE7 gameover_surf=NULL;	//Message 'gameover'
LPDIRECTDRAWSURFACE7 para1_surf=NULL;		//Paratrooper+animaties
LPDIRECTDRAWSURFACE7 para2_surf=NULL;
LPDIRECTDRAWSURFACE7 para3_surf=NULL;
LPDIRECTDRAWSURFACE7 chute1_surf=NULL;		//Parachute
LPDIRECTDRAWSURFACE7 Explosion1_surf=NULL;	//Explosies+animatie
LPDIRECTDRAWSURFACE7 Explosion2_surf=NULL;
LPDIRECTDRAWSURFACE7 Explosion3_surf=NULL;
LPDIRECTDRAWSURFACE7 Explosion4_surf=NULL;
LPDIRECTDRAWSURFACE7 Explosion5_surf=NULL;
LPDIRECTDRAWSURFACE7 SmallExpl1_surf=NULL;
LPDIRECTDRAWSURFACE7 SmallExpl2_surf=NULL;
LPDIRECTDRAWSURFACE7 SmallExpl3_surf=NULL;
LPDIRECTDRAWSURFACE7 TSmallExpl1_surf=NULL;
LPDIRECTDRAWSURFACE7 TSmallExpl2_surf=NULL;
LPDIRECTDRAWSURFACE7 TSmallExpl3_surf=NULL;
LPDIRECTDRAWSURFACE7 Bonus1_surf=NULL;		//Bonusvehicle
LPDIRECTDRAWSURFACE7 Bonus2_surf=NULL;


//------ Storage for Color Key Value ------//
//Hierin wordt de kleur voor de transparantie opgeslagen
DWORD KeyColor;

//------ DirectInput Object and Devices ------//

LPDIRECTINPUT8       lpDI;
LPDIRECTINPUTDEVICE8 pKeyboard;
char                keyBuffer[256];
//macro om keyinput makkelijker te verwerken (zie later)
#define KEYDOWN(buf, key) (buf[key] & 0x80)


//------ Define Number of Sounds ------//

#define NUMSOUNDS 17 //Hangt maw rechstreex van het aantal ingeladen geluiden bij InitSounds()

//------ DirectSound Object and Buffers ------//

LPDIRECTSOUND       lpDS;
LPDIRECTSOUNDBUFFER lpDSBSounds[NUMSOUNDS];

// FMOD Streams
FSOUND_STREAM *stream1; //gebruikt om een ingame MP3 af te spelen

// --------- Enkele definities om te vergemakkelijken ---------------//
#define EMPTY 0
#define BULLET	1
#define HELI 2
#define PARAMAN 3
#define CHUTE 4
#define NOCHUTE 0
#define EXPLOSION 5
#define SMALLEXPLOSION 6
#define TSMALLEXPLOSION 7
#define BONUSPLANE 8
#define LANDED 25
#define MAXOBJECTS 150
#define MAXLAND  4


#define MAXCOUNTERS 10  // 1 ,2, 3  ,5,8 ,9  in gebruik
#define BONUSTIME 3
#define BONUS 5
#define GAMEOVER 9
#define LEVDONE 1
#define NEWGAME 0

//------------ ObjectArray --------///
CPTGameObject ObjArray[MAXOBJECTS];
//CPTGameObject ExplosionArray[MAXOBJECTS];

//Definieert aantal-1 keuzemogelijkheden in titelscherm (nu newgame,exit,options)
#define TITLE_DIALOGS 2

//------------Dimensies vd ingame objecten---------//

//Gebruikt om DoRenderFrame() wat sneller te laten werken.
struct ObjDims{
WORD dim_heli_height;
WORD dim_heli_width;
WORD dim_kanon_height;
WORD dim_kanon_width;
WORD dim_cursor_height;
WORD dim_cursor_width;
WORD dim_bullet_width;
WORD dim_bullet_height;
WORD dim_message_height;
WORD dim_message_width;
WORD dim_explosion_height;
WORD dim_explosion_width;
WORD dim_loop_width;
WORD dim_loop_height;
WORD dim_para_height;
WORD dim_para_width;
WORD dim_chute_height;
WORD dim_chute_width;
WORD dim_smallexpl_height;
WORD dim_smallexpl_width;
WORD dim_tsmallexpl_height;
WORD dim_tsmallexpl_width;
WORD dim_gameover_height;
WORD dim_gameover_width;
WORD dim_bonus_height;
WORD dim_bonus_width;
} Dimensions;
int posx;
int posy;


RECT rctbullet;
RECT rctkannon;
RECT rctbkgr;
RECT rctheli;
RECT rctexpl;
RECT rctpara;
RECT rctchute;
RECT rctsmallexpl;
RECT rcttsmallexpl;
RECT rctbonus;

//------------Alle player/game info (punten,etc) ---------------//


struct PlayerInfo{
double m_lGunPos; //positie van de loop
int m_lPoints;
bool m_bPlaying; //Is speler aan het spelen of zitten we in het titelscherm?
bool m_bSounds;
int m_nCursorPos; //Gebruikt om positie van cursor in het titelscherm weer te geven/verplaatsen
int m_bOptions;
int mini_counter; //Teller gebruikt om aantal toetsaanslagen die in één toets worden gedetecteert te verlagen.(zie DoInputCheck())
int m_nLevel;
bool m_bContinue; //Cheatboolean gebruikt om te blijven spelen ook al is speler game-over
bool m_bContEnabled; //dito: gebruikt om cheat te kunnen enablen
int m_nKeepPlaying[2]; //dito: gebruikt om cheat te kunnen enablen
bool m_bRapidFire;
bool m_bWind; //niet gebruikte boolean: gepland om als bonusboolean dienst te doen
bool m_bNoChutesBon;
bool m_bBonusOnce; //ervoor zorgen dat speler maar één bonus per keer kan hebben
int game_counters[MAXCOUNTERS];
}PIInfo;



//------ Error Return String ------//

const char *ErrStr=NULL;	

//------ Error Messages ------//

const char Err_Reg_Class[]			= "Error Registering Window Class";
const char Err_Create_Win[]			= "Error Creating Window";
const char Err_DirectDrawCreate[]	= "DirectDrawCreate FAILED";
const char Err_Query[]				= "QueryInterface FAILED";
const char Err_Coop[]				= "SetCooperativeLevel FAILED";
const char Err_CreateSurf[]			= "CreateSurface FAILED";
const char Err_LoadBMP[]			= "Error Loading Image";
const char Err_DispMode[]			= "Error Setting Display Mode";
const char Err_LoadImage[]			= "Error Loading Image";
const char Err_DirectSoundCreate[]  = "DirectSoundCreate FAILED";
const char Err_CreateBuff[]         = "CreateBuffer FAILED";
const char Err_LoadWAV[]			= "Error Loading Sound";
const char Err_DirectInputCreate[]  = "DirectInputCreate FAILED";
const char Err_CreateDevice[]       = "CreateDevice FAILED";

//---Enkele functie prototypes----
void BulletCollisionCheck(int bulletnr);
void CreateExplosion(int obj);
void BlitMessage(int msg);
BOOL RestoreDSBuffers();
BOOL InitImages();
//******************************************
// Ga nu naar WinMain()
//*******************************************


// Hulp fucntie om 'random getal tussen min en max te krijgen
int CreateRandom(int min, int max)
{
	if(min<=0)
		min=1;
	return (min+rand()*(max-min)/RAND_MAX);
}

// Functie die in de objarray een object 'ingame' zal brengen
void SpawnObject(int type, int objcop)
{
	int l=0;
	int k=0;
	switch(type) //naargelang type andere eigenschappen voor het object
	{
	case BONUSPLANE:
		while(ObjArray[l].m_bInGame==TRUE) //enkel invoegen wanneer we leeg slot vinden, of slot waarin 'dood' object zit
			l++;
		if(l<MAXOBJECTS)
		{
			ObjArray[l].m_bInGame=TRUE;
			ObjArray[l].m_nObjType=BONUSPLANE;
			ObjArray[l].m_nMoreFlags=0;
			ObjArray[l].posy=70+CreateRandom(1,80);
				if(rand()>(RAND_MAX/2))
				{
					ObjArray[l].vectx=3;
					ObjArray[l].posx=-Dimensions.dim_bonus_width;
				}
				else
				{
					ObjArray[l].vectx=-3;
					ObjArray[l].posx=800;
				}
			ObjArray[l].vecty=0;
		}
		break;
	case BULLET:	
		while(ObjArray[l].m_bInGame==TRUE)
			l++;
		if(l<MAXOBJECTS)
		{
			ObjArray[l].m_bInGame=TRUE;
			ObjArray[l].m_nObjType=BULLET;
			ObjArray[l].m_nMoreFlags=0;
			//Vector en positie afhankelijk van huidige stand van loop
			ObjArray[l].posx=398+50*cos(PIInfo.m_lGunPos*(6.28/360));
			ObjArray[l].posy=550-50*sin(PIInfo.m_lGunPos*(6.28/360));
			ObjArray[l].vectx=(8)*cos(PIInfo.m_lGunPos*(6.28/360));
			ObjArray[l].vecty=(-8)*sin(PIInfo.m_lGunPos*(6.28/360));
			//Niet te snel achtereen kogels laten afvuren
			if(PIInfo.m_bRapidFire==FALSE)
				PIInfo.game_counters[BULLET]=10;
			else PIInfo.game_counters[BULLET]=3;
		}
		break;
	case PARAMAN:
		while(ObjArray[l].m_bInGame==TRUE)
			l++;
		if(l<MAXOBJECTS-1) //-1 anders gaat hij geen parachute hebben!We moeten dus 1 slot voor parachute zeker over hebben.
		{
			ObjArray[l].m_bInGame=TRUE;
			ObjArray[l].m_nObjType=PARAMAN;
			ObjArray[l].m_nMoreFlags=0;
			ObjArray[l].m_nFlags=NOCHUTE;
			ObjArray[l].posy=ObjArray[objcop].posy;
			ObjArray[l].posx=ObjArray[objcop].posx;
			
			ObjArray[l].vecty=1;
			ObjArray[l].vectx=0;
			//Chute meemaken!
			while(ObjArray[k].m_bInGame==TRUE )
				k++;
				if(k<MAXOBJECTS)
				{
					if( PIInfo.m_bNoChutesBon==FALSE)
					ObjArray[k].m_bInGame=TRUE;
					else
						ObjArray[k].m_bInGame=FALSE; //Als bonus nochute aanstaat krijgt para geen chute *grijns*
					ObjArray[k].m_nObjType=CHUTE;
					ObjArray[k].m_nFlags=l ;
					ObjArray[l].m_nMoreFlags=0;
					ObjArray[k].posy=ObjArray[objcop].posy-Dimensions.dim_chute_height;
					ObjArray[k].posx=ObjArray[objcop].posx;
			
					ObjArray[k].vecty=1;
					ObjArray[k].vectx=0;
				}
			if(PIInfo.m_bNoChutesBon==FALSE)
				ObjArray[l].m_nFlags=k ; //Onze para moet weten welke chute van hem is!
			}
			else //als para geen chute krijgt laten we hem eens goed krijsen!
				if(PIInfo.m_bSounds)
				{
					DWORD dwStatus;
				lpDSBSounds[9]->GetStatus(&dwStatus);	
				if (dwStatus & DSBSTATUS_BUFFERLOST)
					 RestoreDSBuffers();
				lpDSBSounds[9]->Play(0, 0, 0);
				}
		break;
	case HELI:
		while(ObjArray[l].m_bInGame==TRUE)
			l++;
		if(l<MAXOBJECTS)
		{
			ObjArray[l].m_bInGame=TRUE;
			ObjArray[l].m_nObjType=HELI;
			ObjArray[l].m_nMoreFlags=0;
			
			ObjArray[l].posy=70+CreateRandom(1,80);
				if(rand()>(RAND_MAX/2))
				{
					ObjArray[l].vectx=2;
					ObjArray[l].posx=-Dimensions.dim_heli_width;
				}
				else
				{
					ObjArray[l].vectx=-2;
					ObjArray[l].posx=800;
				}
			ObjArray[l].vecty=0;
			//Volgende heli wordt gespawned wanneer de random-tijd verlopen is.
			//Random-tijd wordt steeds korter naarmate we in een hogere level zitten.
			PIInfo.game_counters[HELI]=CreateRandom(30-PIInfo.m_nLevel,200-10*PIInfo.m_nLevel);//nieue teller voor volgende random heli
		}
		break;
	}

}
//Nakijken of speler game over is
bool CheckGameOver()
{
	int left=0;
	int right=0;
	for(int i=0;i<MAXOBJECTS;i++)
	{
		//Telkens para geland is verhogen we de left/right var. Die aangeeft hoeveel er aan welke kant geland zijn
		if(ObjArray[i].m_nObjType==PARAMAN && ObjArray[i].m_nMoreFlags==LANDED && ObjArray[i].m_bInGame==TRUE)
		{
			if(ObjArray[i].posx<400)
				left++;
			else right++;
		}
	}
	if(left>=MAXLAND || right>=MAXLAND)
		return TRUE;
	else 
		return FALSE;
}
//------- Functie om playervariabelen bij nieuw spel terug op origenele waarden te zetten ------
void PrepareNewLevel(int lvl)
{		
	for(int l=0;l<MAXOBJECTS;l++)
	{
		ObjArray[l].m_bInGame=FALSE;
	//	ExplosionArray[l].m_bInGame=FALSE;
	}

	PIInfo.game_counters[8]=250; //Om levelstart vertraging te hebben
	PIInfo.game_counters[9]=7200; //totale level tijd (60tics = 1seconde) = 2minuten
	PIInfo.game_counters[HELI]=CreateRandom(200,300);//spawn van eerste heli
	PIInfo.game_counters[BONUS]=CreateRandom(500,6000);
	PIInfo.game_counters[BULLET]=0; //tijd tussen schieten
	PIInfo.m_nLevel=lvl;
	PIInfo.m_bRapidFire=FALSE;
	PIInfo.m_bNoChutesBon=FALSE;
	PIInfo.m_bWind=FALSE;
	PIInfo.m_bBonusOnce=1;
	int lvls=lvl%3;
	//Naargelang de huidige level een andere MP3 afspelen.(mbv FMOD)
	switch(lvls)
	{
	FSOUND_Stream_Stop(stream1)	;
	case 1:
		stream1=FSOUND_Stream_OpenFile("level1.mp3",FSOUND_MPEGACCURATE | FSOUND_LOOP_NORMAL,0);
		break;
	case 2:
		stream1=FSOUND_Stream_OpenFile("level2.mp3",FSOUND_MPEGACCURATE | FSOUND_LOOP_NORMAL,0);
		break;
	case 0:
		stream1=FSOUND_Stream_OpenFile("level3.mp3",FSOUND_MPEGACCURATE | FSOUND_LOOP_NORMAL,0);

		break;
	}
	FSOUND_Stream_Play(0,stream1); 
	

}

void PrepareNewGame()
{
PIInfo.m_bPlaying=TRUE;
PIInfo.m_lGunPos=90;

if(PIInfo.m_nKeepPlaying[0]!=9)
{
	PIInfo.m_lPoints=0;
	PIInfo.m_nLevel=1;
}


PrepareNewLevel(PIInfo.m_nLevel);
PIInfo.m_bContinue=0;
PIInfo.m_bContEnabled=0;

}


//------ Function to Load a Bitmap into a DirectDraw Surface ------//
// Bron + uitleg  in 'DirectX7 in 24uur'
LPDIRECTDRAWSURFACE7 bitmap_surface(LPCTSTR file_name,RECT *dims=NULL)
{
	HDC hdc;
	HBITMAP bit;
	LPDIRECTDRAWSURFACE7 surf;

	// load the interface bitmap

	bit=(HBITMAP) LoadImage(NULL,file_name,IMAGE_BITMAP,0,0,
								LR_DEFAULTSIZE|LR_LOADFROMFILE);
	if (!bit) 

		// failed to load, return failure to caller

		return NULL;

	// get bitmap dimensions

	BITMAP bitmap;
    GetObject( bit, sizeof(BITMAP), &bitmap );
	int surf_width=bitmap.bmWidth;
	int surf_height=bitmap.bmHeight;

	// create surface

	HRESULT ddrval;
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT ;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; 
	ddsd.dwWidth = surf_width;
	ddsd.dwHeight = surf_height; 

	// attempt to create surface

	ddrval=lpDD->CreateSurface(&ddsd,&surf,NULL);

	// created ok?

	if (ddrval!=DD_OK) {

		// no, release the bitmap and return failure to caller

		DeleteObject(bit);
		return NULL;

	} else {

		// yes, get a DC for the surface

		surf->GetDC(&hdc);

		// generate a compatible DC

		HDC bit_dc=CreateCompatibleDC(hdc);

		// blit the interface to the surface

		SelectObject(bit_dc,bit);
		BitBlt(hdc,0,0,surf_width,surf_height,bit_dc,0,0,SRCCOPY);

		// release the DCs

		surf->ReleaseDC(hdc);
		DeleteDC(bit_dc);

		// save the dimensions if rectangle pointer provided

		if (dims) {
			dims->left=0;
			dims->top=0;
			dims->right=surf_width;
			dims->bottom=surf_height;
		}
	}

	// clear bitmap 

	DeleteObject(bit);

	// return pointer to caller

	return surf;
}

//------ Cleanup Function to Release Objects ------//
//Macro voor Release 
#define SafeRelease(x) if (x) { x->Release(); x=NULL;}

void Cleanup()
{
	// SafeRelease is een macro gedefinieert als volgt:
	//		#define SafeRelease(x) if (x) {x->Release();x=NULL;}
	// release loaded image surfaces

	SafeRelease(back_surf);
	SafeRelease(back2_surf);
	SafeRelease(back3_surf);
	SafeRelease(int_surf);
	SafeRelease(heli_surf);	
	SafeRelease(kanonl_surf);
	SafeRelease(kanonr_surf);
	SafeRelease(title_surf);
	SafeRelease(cursor_surf);
	SafeRelease(bullet_surf);
	SafeRelease(GetReady_surf);
	SafeRelease(gameover_surf);
	SafeRelease(levdone_surf);
	SafeRelease(loading_surf);
	SafeRelease(Bonus1_surf);
	SafeRelease(Bonus2_surf);
	SafeRelease(para1_surf);
	SafeRelease(para2_surf);
	SafeRelease(para3_surf);
	SafeRelease(chute1_surf);
	SafeRelease(Explosion1_surf);
	SafeRelease(Explosion2_surf);
	SafeRelease(Explosion3_surf);
	SafeRelease(Explosion4_surf);
	SafeRelease(Explosion5_surf);
	SafeRelease(SmallExpl1_surf);
	SafeRelease(SmallExpl2_surf);
	SafeRelease(SmallExpl3_surf);
	SafeRelease(TSmallExpl1_surf);
	SafeRelease(TSmallExpl2_surf);
	SafeRelease(TSmallExpl3_surf);
	// release DirectDraw interfaces

	SafeRelease(lpDDSPrimary);
	SafeRelease(lpDD);
	// release DirectInput interfaces
     if (pKeyboard)
    {
         pKeyboard->Unacquire();
         pKeyboard->Release();
         pKeyboard = NULL;
    }
    SafeRelease(lpDI);

	// release DirectSound interface

    SafeRelease(lpDS);
	

	
    // display error if one thrown

	if (ErrStr) {
		MessageBox(NULL, ErrStr, szCaption, MB_OK);
		ErrStr=NULL;
	}
	FSOUND_Stream_Close(stream1);
}
//------ Windows Message Handler ------//


LRESULT CALLBACK 
WindowProc(HWND hWnd, unsigned uMsg, WPARAM wParam, LPARAM lParam)
{
	int lvls=0;
switch (uMsg)
    {
	//Rudimentaire controle op scope verliezen.
	//Indien programma scope verleist wordt het spel gestopt en
	// nadien hertekend.

	case WM_ACTIVATEAPP:
		//if(LOWORD(wParam)==WA_INACTIVE)
		if((BOOL)wParam==FALSE)
		{
			bHasFocus=FALSE;
			FSOUND_Stream_Stop(stream1)	;
		}
		else
			bHasFocus=TRUE;
			if(bIsInit)
			{
				if (!lpDD->TestCooperativeLevel()  )
					lpDD->RestoreAllSurfaces();
			}
			
			if(PIInfo.m_bPlaying)
			{
				lvls=PIInfo.m_nLevel%3;

				switch(lvls)
				{
				FSOUND_Stream_Stop(stream1)	;
				case 1:
					stream1=FSOUND_Stream_OpenFile("level1.mp3",FSOUND_MPEGACCURATE | FSOUND_LOOP_NORMAL,0);
					break;
				case 2:
					stream1=FSOUND_Stream_OpenFile("level2.mp3",FSOUND_MPEGACCURATE | FSOUND_LOOP_NORMAL,0);
					break;
				case 0:
					stream1=FSOUND_Stream_OpenFile("level3.mp3",FSOUND_MPEGACCURATE | FSOUND_LOOP_NORMAL,0);

					break;
				}
			}
		break;
			
		
		case WM_DESTROY:
			Cleanup();
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
	return 0L;

}

//------ Function to load object sounds and set up buffers ------//
BOOL InitSounds()
{
//********************
//	Info:
//	 Wanneer je zelfgemaakte waves geen geluid produceren, of ze lijken te kraken (of welke rare klank dan ook) kijk dan goed 
//	 na of je geluid niet boven een bepaald geluidsniveau uitkomt. 
//	De exacte dB waarde weet ik niet maar in cooledit komt dit neer op ongeveer 75% van de blauwe stippellijntjes
//	Voor meer info hieromtrent, contacteer me op tim_dams@hotmail.com
//**********************

	// Initialize waves
    CWave waves[NUMSOUNDS];
    waves[0].Create("select.wav");
    waves[1].Create("smallexp.wav");
    waves[2].Create("gun.wav");
    waves[3].Create("splat.wav");
    waves[4].Create("splatair.wav");
    waves[5].Create("expl.wav");
    waves[6].Create("expl2.wav");
    waves[7].Create("expl3.wav");
    waves[8].Create("gun.wav");
	waves[9].Create("fall.wav");
	waves[10].Create("gameover.wav");
	waves[11].Create("nextlevel.wav");
	waves[12].Create("gready.wav");
	waves[13].Create("rapidf.wav");
	waves[14].Create("1000p.wav");
	waves[15].Create("500p.wav");
	waves[16].Create("nochutes.wav");

    // Initialize secondary DS buffers
    for (int i = 0; i < NUMSOUNDS; i++) {
        // Get the wave information
        DWORD         dwDataLen = waves[i].GetDataLen();
        WAVEFORMATEX  wfFormat;
        waves[i].GetFormat(wfFormat);

        // Setup the DS buffer description
        DSBUFFERDESC  dsbdDesc;
        ZeroMemory(&dsbdDesc, sizeof(DSBUFFERDESC));
        dsbdDesc.dwSize = sizeof(DSBUFFERDESC);
        dsbdDesc.dwFlags = DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN |
					DSBCAPS_CTRLVOLUME | DSBCAPS_STATIC;
        dsbdDesc.dwBufferBytes = dwDataLen;
        dsbdDesc.lpwfxFormat = &wfFormat;

        // Create the DS buffer
        if (lpDS->CreateSoundBuffer(&dsbdDesc,
            &lpDSBSounds[i], NULL) != DS_OK)
        {
            ErrStr = Err_CreateBuff;
            return FALSE;
        }

        // Lock the DS buffer
        BYTE* pDSBuffData;
        if (lpDSBSounds[i]->Lock(0, dwDataLen, (void**)&pDSBuffData,
            &dwDataLen, NULL, 0, 0) != DS_OK)
        {
            ErrStr = Err_LoadWAV;
            return FALSE;
        }

        // Write wave data to the DS buffer
        dwDataLen = waves[i].GetData(pDSBuffData, dwDataLen);

        // Unlock the DS buffer
        if (lpDSBSounds[i]->Unlock(pDSBuffData, dwDataLen, NULL, 0) !=
            DS_OK)
        {
            ErrStr = Err_LoadWAV;
            return FALSE;
        }
    }
 return TRUE;
}
//Alle objecten doorlopen en verplaatsen, vernietigen, actie laten uitvoeren indien nodig.
void UpdateAll()
{

	for(int j=0;j<MAXOBJECTS;j++)
	{
		if(ObjArray[j].m_bInGame) //enkle de ingame object bekijken
		{
			//Wanneer object zich buiten het speelscherm bevindt: vernietigen
			if(ObjArray[j].posx<-100 || ObjArray[j].posx>900|| ObjArray[j].posy<-10|| ObjArray[j].posy>800)
				ObjArray[j].m_bInGame=FALSE;
			else
			{
				//Positie adhv vector updaten
				ObjArray[j].posx=ObjArray[j].posx+ObjArray[j].vectx;
				ObjArray[j].posy=ObjArray[j].posy+ObjArray[j].vecty;
				if(ObjArray[j].m_nObjType==BULLET)
				{
					//Extra controle van kogels die buiten scherm kunnen zijn na verplaatsing 
					if(ObjArray[j].posx<0 || ObjArray[j].posx>800||  ObjArray[j].posy<0)
						ObjArray[j].m_bInGame=FALSE;
					if(ObjArray[j].m_bInGame)
						//Kijken of kogels iets raakt
						BulletCollisionCheck(j);
				}
				if(ObjArray[j].m_nObjType==PARAMAN)
				{
					if (ObjArray[j].m_nFlags==NOCHUTE && ObjArray[j].m_nMoreFlags!=LANDED)
					{
						ObjArray[j].vecty=3;
						if(ObjArray[j].posy>560-Dimensions.dim_para_height)
						{
							CreateExplosion(j);
							ObjArray[j].m_bInGame=FALSE;
						//doodvallen op nen anderen 
							for(int i =0;i<MAXOBJECTS;i++)
							{
								if(ObjArray[j].posx+Dimensions.dim_para_width-5>=ObjArray[i].posx && ObjArray[j].posx+Dimensions.dim_para_width-5<=ObjArray[i].posx+Dimensions.dim_para_width && ObjArray[i].m_nObjType==PARAMAN && ObjArray[i].m_bInGame==TRUE && ObjArray[i].m_nMoreFlags==LANDED)
								{
									ObjArray[i].m_bInGame=FALSE;
									PIInfo.m_lPoints=PIInfo.m_lPoints+15;
								}

							}
						if(PIInfo.m_bSounds)
						{							
							DWORD dwStatus;
							lpDSBSounds[4]->GetStatus(&dwStatus);	
							if (dwStatus & DSBSTATUS_BUFFERLOST)
							 RestoreDSBuffers();
							lpDSBSounds[4]->Play(0, 0, 0);
						}
							
						}
					}
					else if(ObjArray[j].m_nMoreFlags!=LANDED)
						if(ObjArray[j].posy>590-Dimensions.dim_para_height)
						{
							ObjArray[ObjArray[j].m_nFlags].m_bInGame=FALSE;
							ObjArray[j].vecty=0;
							ObjArray[j].m_nMoreFlags=LANDED;
						}
							
				}	
				if(ObjArray[j].m_nObjType==EXPLOSION || ObjArray[j].m_nObjType==SMALLEXPLOSION || ObjArray[j].m_nObjType==TSMALLEXPLOSION)
					{
					ObjArray[j].m_nFlags--;
					if(ObjArray[j].m_nFlags==0)
						ObjArray[j].m_bInGame=FALSE;
					}
				//////////////////////////////////////
				//////Paratrooper spawnen/////////////
				//////////////////////////////////////
				if(ObjArray[j].m_nObjType==HELI)
				{
					if(rand()<=100+PIInfo.m_nLevel*30 )
					{	
						if(ObjArray[j].m_bInGame && (ObjArray[j].posx< 330 || ObjArray[j].posx>470) && ObjArray[j].posx>Dimensions.dim_para_width+15 && ObjArray[j].posx<(800-Dimensions.dim_para_width-15) )
					
								SpawnObject(PARAMAN,j);						
			
					}
				}		
				//////////////////////////////////////////
				///////Einde paratrooper spawnen//////////
				//////////////////////////////////////////

				
			}
		}
	}
	
}
// Deze rects hebben we nodig voor het renderen  van de frames. Om te vermijden dat alle rects telkens
// ingevuld moeten worden doen we het grootste deel hier reeds.
// RECT is gewoon type dat een rechthoek beschrijft(in pixels),om te Blitten (bitmap naar surface) hebben we de grootte
// van deze bitmaps nodig,die vullen we hier in nadat we de benodigde dimensies in InitImages hebben ingeladen.
void InitRects()
{
	rctbkgr.left=0;
	rctbkgr.top=0;
	rctbullet.left=0;
	rctbullet.top=0;
	rctbullet.bottom=Dimensions.dim_bullet_height;
	rctbullet.right=Dimensions.dim_bullet_width;
	rctkannon=rctbullet;
	rctheli=rctbullet;
	rctexpl=rctbullet;
	rctpara=rctbullet;
	rctchute=rctbullet;
	rctbonus=rctbullet;
	rctsmallexpl=rctbullet;
	rcttsmallexpl=rctbullet;
	rctbkgr.bottom=600;
	rctbkgr.right=800;
	rctkannon.right=Dimensions.dim_kanon_width;
	rctkannon.bottom=Dimensions.dim_kanon_height;
	rctbonus.right=Dimensions.dim_bonus_width;
	rctbonus.bottom=Dimensions.dim_bonus_height;
	rctchute.bottom=Dimensions.dim_chute_height;
	rctchute.right=Dimensions.dim_chute_width;
	rctpara.bottom=Dimensions.dim_para_height;
	rctpara.right=Dimensions.dim_para_width;
	rctheli.bottom=Dimensions.dim_heli_height;
	rctheli.right=Dimensions.dim_heli_width;
	rctexpl.bottom=Dimensions.dim_explosion_height;
	rctexpl.right=Dimensions.dim_explosion_width;
	rctsmallexpl.right=Dimensions.dim_smallexpl_width;
	rctsmallexpl.bottom=Dimensions.dim_smallexpl_height;
	rcttsmallexpl.right=Dimensions.dim_tsmallexpl_width;
	rcttsmallexpl.bottom=Dimensions.dim_tsmallexpl_height;
}

BOOL InitImages()
{
	//Opgelet! Zorg ervoor dat al je images van dezelfde bitsize zijn. In mijn geval 24bits/pixel. 
	//Dit kan je instellen in ieder degelijk tekenprogramma
	// Wil je toch verschillende types zorg er dan voor dat je voor het instellen van iedere bitmap een eigen 
	// colorkey hebt. (die afhangt van het pixelformaat in dit programma omdat we een simpel masker nemen en
	// veronderstellen dat dit overal hetzelfde is)

	RECT rct;
	
	// load the interface image
	int_surf=bitmap_surface("hud.bmp");
	if (!int_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}

	// load the background image
	back_surf=bitmap_surface("background.bmp");
	if (!back_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
		// load the background image
	back2_surf=bitmap_surface("background2.bmp");
	if (!back2_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
			// load the background image
	back3_surf=bitmap_surface("background3.bmp");
	if (!back2_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	// load the title image
	title_surf=bitmap_surface("titlescreen.bmp");
	if (!title_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	// load the cursor image
	cursor_surf=bitmap_surface("cursor.bmp",&rct);
	if (!cursor_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	Dimensions.dim_cursor_height=rct.bottom;
	Dimensions.dim_cursor_width=rct.right;


	// load the message images
	GetReady_surf=bitmap_surface("messready.bmp",&rct);
	if (!GetReady_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	levdone_surf=bitmap_surface("messLevDone.bmp");
	if (!GetReady_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	loading_surf=bitmap_surface("messLoad.bmp");
	if (!GetReady_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	Dimensions.dim_message_height=rct.bottom;
	Dimensions.dim_message_width=rct.right;
	gameover_surf=bitmap_surface("gameover.bmp",&rct);
	if (!gameover_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	Dimensions.dim_gameover_height=rct.bottom;
	Dimensions.dim_gameover_width=rct.right;
	// load the kanonl image
	kanonl_surf=bitmap_surface("kanontole.bmp");
	if (!kanonl_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	// load the kanonr image
	kanonr_surf=bitmap_surface("kanontori.bmp",&rct);
	if (!kanonr_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	Dimensions.dim_kanon_height=rct.bottom;
	Dimensions.dim_kanon_width=rct.right;
	//load explosions
	Explosion1_surf=bitmap_surface("expl1.bmp",&rct);
	if (!Explosion1_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	Dimensions.dim_explosion_height=rct.bottom;
	Dimensions.dim_explosion_width=rct.right;
	
	Explosion2_surf=bitmap_surface("expl2.bmp",&rct);
	if (!Explosion2_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	Explosion3_surf=bitmap_surface("expl3.bmp",&rct);
	if (!Explosion3_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	Explosion4_surf=bitmap_surface("expl4.bmp",&rct);
	if (!Explosion4_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	Explosion5_surf=bitmap_surface("expl5.bmp",&rct);
	if (!Explosion5_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
		//load small explosions 
	SmallExpl1_surf=bitmap_surface("smallexpl1.bmp",&rct);
	if (!SmallExpl1_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	Dimensions.dim_smallexpl_height=rct.bottom;
	Dimensions.dim_smallexpl_width=rct.right;
	
	SmallExpl2_surf=bitmap_surface("smallexpl2.bmp",&rct);
	if (!SmallExpl2_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	SmallExpl3_surf=bitmap_surface("smallexpl3.bmp",&rct);
	if (!SmallExpl3_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
		//load small explosions 2
	TSmallExpl1_surf=bitmap_surface("2smallexpl1.bmp",&rct);
	if (!TSmallExpl1_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	Dimensions.dim_tsmallexpl_height=rct.bottom;
	Dimensions.dim_tsmallexpl_width=rct.right;
	
	TSmallExpl2_surf=bitmap_surface("2smallexpl2.bmp",&rct);
	if (!TSmallExpl2_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	TSmallExpl3_surf=bitmap_surface("2smallexpl3.bmp",&rct);
	if (!TSmallExpl3_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	// load the bonus image
	Bonus1_surf=bitmap_surface("bonus1.bmp",&rct);
	if (!Bonus1_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	Dimensions.dim_bonus_height=rct.bottom;
	Dimensions.dim_bonus_width=rct.right;

	Bonus2_surf=bitmap_surface("bonus2.bmp",&rct);
	if (!Bonus2_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	// load the bullet image
	bullet_surf=bitmap_surface("bullet.bmp",&rct);
	if (!bullet_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	Dimensions.dim_bullet_height=rct.bottom;
	Dimensions.dim_bullet_width=rct.right;

	// load helil
	heli_surf=bitmap_surface("helitole.bmp",&rct);
	if (!heli_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	Dimensions.dim_heli_height=rct.bottom;
	Dimensions.dim_heli_width=rct.right;
	// load chute
	chute1_surf=bitmap_surface("chute1.bmp",&rct);
	if (!chute1_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	Dimensions.dim_chute_height=rct.bottom;
	Dimensions.dim_chute_width=rct.right;
	// load paraman
	para1_surf=bitmap_surface("para1.bmp",&rct);
	if (!para1_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	Dimensions.dim_para_height=rct.bottom;
	Dimensions.dim_para_width=rct.right;
	para2_surf=bitmap_surface("para2.bmp");
	if (!para2_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	para3_surf=bitmap_surface("para3.bmp");
	if (!para3_surf) {
		ErrStr=Err_LoadImage;
		return FALSE;
	}
	// -------------- Instellen transparante kleur (de colorkey) ------------
	// get the pixel format 
	

	DDPIXELFORMAT ddpf;
	ddpf.dwSize=sizeof(ddpf);
	lpDDSPrimary->GetPixelFormat(&ddpf);

	// Determine proper key for pixel format

	KeyColor = ddpf.dwGBitMask; //Puur groen (RGB(0,255,0)) is de transparantie 

	// set color keys

	DDCOLORKEY key;
	key.dwColorSpaceLowValue = KeyColor;
	key.dwColorSpaceHighValue = KeyColor;
	int_surf->SetColorKey(DDCKEY_SRCBLT, &key);
	heli_surf->SetColorKey(DDCKEY_SRCBLT, &key);
	kanonl_surf->SetColorKey(DDCKEY_SRCBLT, &key);
	kanonr_surf->SetColorKey(DDCKEY_SRCBLT, &key);
	cursor_surf->SetColorKey(DDCKEY_SRCBLT, &key);
	bullet_surf->SetColorKey(DDCKEY_SRCBLT, &key);
	para1_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	para2_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	para3_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	chute1_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	GetReady_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	levdone_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	loading_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	gameover_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	Explosion1_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	Explosion2_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	Explosion3_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	Explosion4_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	Explosion5_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	SmallExpl1_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	SmallExpl2_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	SmallExpl3_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	TSmallExpl1_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	TSmallExpl2_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	TSmallExpl3_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	Bonus1_surf->SetColorKey(DDCKEY_SRCBLT,&key);
	Bonus2_surf->SetColorKey(DDCKEY_SRCBLT,&key);
		
	
	InitRects();//Snellere frames
	// return success to caller

	return TRUE;
}

static BOOL Init(HINSTANCE hInstance, int nCmdShow)
{
    //Init van gehele spel. En dan vooral de DirectXobjecten!
	
	WNDCLASS                    wc;
    HRESULT                     ddrval;

    // Set up and register window class

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(DWORD);
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szClass;
    if (!RegisterClass(&wc)) {
		ErrStr=Err_Reg_Class;
        return FALSE;
	}

    // Get dimensions of display

    int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Create a window and display

	HWND hWnd;

    hWnd = CreateWindow(szClass,							// class
                        szCaption,							// caption
						WS_VISIBLE|WS_POPUP,				// style 
						0,									// left
						0,									// top
						ScreenWidth,						// width
						ScreenHeight,						// height
                        NULL,								// parent window
                        NULL,								// menu 
                        hInstance,							// instance
                        NULL);								// parms
    if (!hWnd) {
		ErrStr=Err_Create_Win;
        return FALSE;
	}
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

  // Create the main DirectDraw object
	ddrval = DirectDrawCreateEx(NULL, (LPVOID*)&lpDD, IID_IDirectDraw7, NULL);
	if (ddrval != DD_OK) {
		ErrStr=Err_DirectDrawCreate;
		return FALSE;
	}

    // Set our cooperative level
	// We want fullscreen!

    ddrval = lpDD->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
    if (ddrval != DD_OK) {
		ErrStr=Err_Coop;
		return FALSE;
	}

	// Set the display mode
	// 800*600 @ 16bit colors

	ddrval = lpDD->SetDisplayMode( 800, 600, 16, 0, 0);
	if (ddrval !=DD_OK) {
		ErrStr=Err_DispMode;
		return FALSE;
	}

    // Create the primary surface with 1 back buffer

    DDSURFACEDESC2 ddsd;
	DDSCAPS2 ddscaps;
	ZeroMemory(&ddsd,sizeof(ddsd));
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
                          DDSCAPS_FLIP | 
                          DDSCAPS_COMPLEX;
    ddsd.dwBackBufferCount = 1;
    ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
	if (ddrval!=DD_OK) {
		ErrStr=Err_CreateSurf;
		return FALSE;
	}

	// Fetch back buffer interface

	ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
  ddscaps.dwCaps2 = 0;
  ddscaps.dwCaps3 = 0;
  ddscaps.dwCaps4 = 0;
	ddrval=lpDDSPrimary->GetAttachedSurface(&ddscaps,&lpDDSBack);
	if (ddrval!=DD_OK) {
		ErrStr=Err_CreateSurf;
		return FALSE;
	}

	// load the images an set up the layers

	if (!InitImages()) 

		// return with error if failed

		return FALSE;
    
  

	// Create the DirectInput object
    if (DirectInput8Create(hInstance, DIRECTINPUT_VERSION,IID_IDirectInput8,(VOID**)&lpDI, NULL) != DI_OK)
    {
		ErrStr = Err_DirectInputCreate;
        return FALSE;
    }


    // Create and initialize the keyboard device
    if (lpDI->CreateDevice(GUID_SysKeyboard, &pKeyboard, NULL) != DI_OK)
    {
		ErrStr = Err_CreateDevice;
		return FALSE;
    }
    if (pKeyboard->SetDataFormat(&c_dfDIKeyboard) != DI_OK)
    {
        SafeRelease(pKeyboard);
        return FALSE;
    }
    if (pKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND |
        DISCL_NONEXCLUSIVE) != DI_OK)
    {
        SafeRelease(pKeyboard);
        return FALSE;
    }
    if (pKeyboard->Acquire() != DI_OK)
    {
        SafeRelease(pKeyboard);
        return FALSE;
    }

// Create the DirectSound object
if (DirectSoundCreate(NULL, &lpDS, NULL) != DS_OK)
{
    ErrStr = Err_DirectSoundCreate;
    return FALSE;
}

// Set the cooperation level for the DirectSound object
if (lpDS->SetCooperativeLevel(hWnd, DSSCL_NORMAL) != DS_OK)
{
    ErrStr = Err_Coop;
    return FALSE;
}

// Initialize the DirectSound buffers
if (!InitSounds())
{
    return FALSE;
}
	
//Init FMOD
	FSOUND_Init(44100, 6, 0);
	//Reset some cheats :)
	for(int c=0;c<=2;c++)
		PIInfo.m_nKeepPlaying[c]=0; 

	 // return success to caller
	return TRUE;
}

//------ Function to Restore DirectSound Buffers ------//

BOOL RestoreDSBuffers()
{
    // Restore the buffers
    for (int i = 0; i < NUMSOUNDS; i++)
        if (lpDSBSounds[i]->Restore() != DS_OK)
            return FALSE;

    // Re-initialize the buffers
    return InitSounds();
}
void DoInputCheck()
{

	// Obtain input from the keyboard
                if (pKeyboard->GetDeviceState(sizeof(keyBuffer),(LPVOID)&keyBuffer)
                    == DIERR_INPUTLOST)
                {
                    // Reacquire the keyboard and try again
                    if (pKeyboard->Acquire() == DI_OK)
                        pKeyboard->GetDeviceState(sizeof(keyBuffer),(LPVOID)&keyBuffer);
                }
                if (KEYDOWN(keyBuffer, DIK_LEFT))
                {
					if(PIInfo.m_lGunPos<=180)
					PIInfo.m_lGunPos=PIInfo.m_lGunPos+2;
                }
                if (KEYDOWN(keyBuffer,DIK_T))
				{
					
					PIInfo.m_nKeepPlaying[0]=1;	
					if(PIInfo.m_bSounds)
						{
						DWORD dwStatus;
						lpDSBSounds[1]->GetStatus(&dwStatus);	
						 if (dwStatus & DSBSTATUS_BUFFERLOST)
							 RestoreDSBuffers();
						lpDSBSounds[1]->Play(0, 0, 0);
						}
				}
				if (KEYDOWN(keyBuffer,DIK_I))
				{
					if(PIInfo.m_nKeepPlaying[0]==1)
					{PIInfo.m_nKeepPlaying[1]=1;				
						if(PIInfo.m_bSounds)
						{
						DWORD dwStatus;
						lpDSBSounds[1]->GetStatus(&dwStatus);	
						 if (dwStatus & DSBSTATUS_BUFFERLOST)
							 RestoreDSBuffers();
						lpDSBSounds[1]->Play(0, 0, 0);
					}
					}
				}
				if (KEYDOWN(keyBuffer,DIK_N))
				{
					if(PIInfo.m_nKeepPlaying[1]==1)
					{
						PIInfo.m_bNoChutesBon=TRUE;
						PIInfo.game_counters[BONUSTIME]=7200;
						if(PIInfo.m_bSounds)
						{
						DWORD dwStatus;
						lpDSBSounds[16]->GetStatus(&dwStatus);	
						 if (dwStatus & DSBSTATUS_BUFFERLOST)
							 RestoreDSBuffers();
						lpDSBSounds[16]->Play(0, 0, 0);
						}
					}
					
				}
				if (KEYDOWN(keyBuffer,DIK_F))
				{
					if(PIInfo.m_nKeepPlaying[1]==1)
					{
						PIInfo.game_counters[BONUSTIME]=7200;
						PIInfo.m_bRapidFire=TRUE;
						if(PIInfo.m_bSounds)
						{
						DWORD dwStatus;
						lpDSBSounds[13]->GetStatus(&dwStatus);	
						 if (dwStatus & DSBSTATUS_BUFFERLOST)
							 RestoreDSBuffers();
						lpDSBSounds[13]->Play(0, 0, 0);
						}
					}
					
						
				}
				if (KEYDOWN(keyBuffer,DIK_P))
				{
					if(PIInfo.m_nKeepPlaying[1]==1)
					{
						PIInfo.m_nKeepPlaying[0]=9;
						if(PIInfo.m_bSounds)
						{
						DWORD dwStatus;
						lpDSBSounds[10]->GetStatus(&dwStatus);	
						 if (dwStatus & DSBSTATUS_BUFFERLOST)
							 RestoreDSBuffers();
						lpDSBSounds[10]->Play(0, 0, 0);
						}
					}

				}


				if (KEYDOWN(keyBuffer, DIK_RIGHT))
                {
					if(PIInfo.m_lGunPos>=0 )
					PIInfo.m_lGunPos=PIInfo.m_lGunPos-2;
                }

				if(KEYDOWN(keyBuffer,DIK_UP))
				{
					if(PIInfo.mini_counter==0 && PIInfo.m_bPlaying==0)
					{	
						//soundbuffers nazien
						if(PIInfo.m_bSounds)
						{
						DWORD dwStatus;
						lpDSBSounds[0]->GetStatus(&dwStatus);	
						 if (dwStatus & DSBSTATUS_BUFFERLOST)
							 RestoreDSBuffers();
						lpDSBSounds[0]->Play(0, 0, 0);
						}

						if(PIInfo.m_nCursorPos==0 && PIInfo.m_bOptions==FALSE)
							PIInfo.m_nCursorPos=TITLE_DIALOGS;	
						else if(PIInfo.m_bOptions==FALSE)
							PIInfo.m_nCursorPos--;
						if(PIInfo.m_nCursorPos==0 && PIInfo.m_bOptions==TRUE)
							PIInfo.m_nCursorPos=1;	
						else if(PIInfo.m_bOptions==TRUE)
							PIInfo.m_nCursorPos--;
						PIInfo.mini_counter=10;
					}
				}
				if (KEYDOWN(keyBuffer, DIK_C))
				{
					if(PIInfo.m_bPlaying==1 && PIInfo.m_bContEnabled)
					{
						PIInfo.m_bContinue=1;
					}
				}
				if (KEYDOWN(keyBuffer, DIK_SPACE))
				{

						if(PIInfo.m_bPlaying==1 && PIInfo.game_counters[BULLET]==0)		
						{
						if(PIInfo.m_bSounds)
						{							
							DWORD dwStatus;
							lpDSBSounds[2]->GetStatus(&dwStatus);	
							 if (dwStatus & DSBSTATUS_BUFFERLOST)
								 RestoreDSBuffers();
							lpDSBSounds[2]->Play(0, 0, 0);
						}
							SpawnObject(BULLET,NULL);
							if(PIInfo.m_lPoints>0 && PIInfo.m_bRapidFire==FALSE)
								PIInfo.m_lPoints--;
						}

					
				}
                if (KEYDOWN(keyBuffer, DIK_DOWN))
                {
					if(PIInfo.mini_counter==0 && PIInfo.m_bPlaying==0)
					{
						if(PIInfo.m_bSounds)
						{						//soundbuffers nazien
						DWORD dwStatus;
						lpDSBSounds[0]->GetStatus(&dwStatus);	
						 if (dwStatus & DSBSTATUS_BUFFERLOST)
							 RestoreDSBuffers();
						lpDSBSounds[0]->Play(0, 0, 0);
						}

						if(PIInfo.m_nCursorPos==TITLE_DIALOGS && PIInfo.m_bOptions==FALSE)
							PIInfo.m_nCursorPos=0;
						else if(PIInfo.m_bOptions==FALSE)
							PIInfo.m_nCursorPos++;  
						if(PIInfo.m_nCursorPos==1 && PIInfo.m_bOptions==TRUE)
							PIInfo.m_nCursorPos=0;
						else if(PIInfo.m_bOptions==TRUE)
							PIInfo.m_nCursorPos++; 
						PIInfo.mini_counter=10;
						
					}
                }
				if (KEYDOWN(keyBuffer,DIK_RETURN))
				{
					if(PIInfo.mini_counter==0 && PIInfo.m_bPlaying==0)
					{
					if(PIInfo.m_bPlaying==0 && PIInfo.m_bOptions==FALSE)
					{
						switch(PIInfo.m_nCursorPos)
						{
						case 0:
							//soundbuffers nazien
						if(PIInfo.m_bSounds)
						{						
							DWORD dwStatus;
						lpDSBSounds[1]->GetStatus(&dwStatus);	
						 if (dwStatus & DSBSTATUS_BUFFERLOST)
							 RestoreDSBuffers();
						lpDSBSounds[1]->Play(0, 0, 0);
						}
							BlitMessage(NEWGAME);
							PrepareNewGame();
							break;
						case 1:
							PIInfo.m_bOptions=TRUE;	
							break;
						case 2:
							Cleanup();
							PostQuitMessage(0);
							DestroyWindow(hWnd);
							break;
						}
					}
					else if(PIInfo.m_bPlaying==0 && PIInfo.m_bOptions==TRUE)
						switch(PIInfo.m_nCursorPos)
					{
						case 0:
							//soundbuffers nazien
						if(PIInfo.m_bSounds)
						{
							DWORD dwStatus;
						lpDSBSounds[0]->GetStatus(&dwStatus);	
						 if (dwStatus & DSBSTATUS_BUFFERLOST)
							 RestoreDSBuffers();
						lpDSBSounds[0]->Play(0, 0, 0);
						}
							if(PIInfo.m_bSounds==TRUE)
							PIInfo.m_bSounds=0;
							else PIInfo.m_bSounds=1;
							break;
						case 1:
							PIInfo.m_bOptions=FALSE;	
							break;
						
					}
				PIInfo.mini_counter=10;	
				}
				}
                if (KEYDOWN(keyBuffer, DIK_ESCAPE))
                {
					if(PIInfo.m_bPlaying==1)
					{
						PIInfo.m_bPlaying=0;
						FSOUND_Stream_Stop(stream1)	;
					}

                }
	
}

// Functie om enkele belangrijke boodschappen op scherm te tonen.
void BlitMessage(int msg)
{
	RECT rct;

	rct.left=0;
	rct.top=0;
	rct.right=Dimensions.dim_message_width;
	rct.bottom=Dimensions.dim_message_height;
	switch(msg)
	{
	case LEVDONE:
		lpDDSBack->BltFast(400-Dimensions.dim_message_width/2,300-Dimensions.dim_message_height,levdone_surf,&rct,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		break;
	case NEWGAME:
		lpDDSBack->BltFast(400-Dimensions.dim_message_width/2,300-Dimensions.dim_message_height,loading_surf,&rct,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		break;
	case GAMEOVER:
		rct.right=Dimensions.dim_gameover_width;
		rct.bottom=Dimensions.dim_gameover_height;
		lpDDSBack->BltFast(400-Dimensions.dim_gameover_width/2,400-Dimensions.dim_gameover_height,gameover_surf,&rct,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		break;
	}
	// flip to the primary surface
	lpDDSPrimary->Flip(0,DDFLIP_WAIT);
}
void RenderBackGround()
{
	



	//Teken achtergrond
	int lvls=PIInfo.m_nLevel%3;

	switch(lvls)
	{
	case 1:
		lpDDSBack->BltFast(0,0,back_surf,&rctbkgr,DDBLTFAST_WAIT);
		break;
	case 2:
		lpDDSBack->BltFast(0,0,back3_surf,&rctbkgr,DDBLTFAST_WAIT);
		break;
	case 0:
		lpDDSBack->BltFast(0,0,back2_surf,&rctbkgr,DDBLTFAST_WAIT);
		break;
	
	}
}
void DoRenderFrame()
{
	RECT rct;
	rct.top=0;
	rct.left=0;
	rct.bottom=600;
	rct.right=800;
	HDC hdc;
	
	if(PIInfo.m_bPlaying==TRUE)
	{
		RenderBackGround();
	
	


	//Loop 'achter' kanon
	
	if(DD_OK==lpDDSBack->GetDC(&hdc)){
		//Gouden oude GDI. Eigenlijk niet meer geondersteund maar voor dit kleine stukje toch het snelst.
		HPEN hloop;
		hloop=CreatePen(PS_SOLID,4,RGB(170,170,170));
		hloop=(HPEN)SelectObject(hdc,hloop);
		MoveToEx(hdc,400,550,NULL);
		LineTo(hdc,400+50*cos(PIInfo.m_lGunPos*(6.28/360)),550-50*sin(PIInfo.m_lGunPos*(6.28/360)));  
		hloop=(HPEN)SelectObject(hdc,hloop);
		DeleteObject(hloop);
		lpDDSBack->ReleaseDC(hdc);
		}	
	//Kanon 
	//Indien gewenst kan je twee tekening voor het kanon (bv ventje in stoel) dat dan 'roteert' op 90g°


	if(PIInfo.m_lGunPos>90)
		lpDDSBack->BltFast(400-Dimensions.dim_kanon_width/2,600-Dimensions.dim_kanon_height,kanonl_surf,&rctkannon,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	else
		lpDDSBack->BltFast(400-Dimensions.dim_kanon_width/2,600-Dimensions.dim_kanon_height,kanonr_surf,&rctkannon,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//IngameObjecten
	
	for(int l=0;l<MAXOBJECTS;l++)
	{
		if(ObjArray[l].m_bInGame==TRUE)
		{
				int randan=rand();
			switch(ObjArray[l].m_nObjType)
			{
				case BULLET:
					lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,bullet_surf,&rctbullet,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					break;
				case BONUSPLANE:
					if(ObjArray[l].posx<=0)
						rctbonus.left=-ObjArray[l].posx;
					else if(ObjArray[l].posx+Dimensions.dim_bonus_width>800)
						rctbonus.right=800-ObjArray[l].posx;
					lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,Bonus1_surf,&rctbonus,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					rctbonus.right=Dimensions.dim_bonus_width;
					rctbonus.left=0;
					break;
				case HELI:
					if(ObjArray[l].posx<=0)
						rctheli.left=-ObjArray[l].posx;
					else if(ObjArray[l].posx+Dimensions.dim_heli_width>800)
						rctheli.right=800-ObjArray[l].posx;
					lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,heli_surf,&rctheli,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					rctheli.right=Dimensions.dim_heli_width;
					rctheli.left=0;
					break;
				case EXPLOSION:
					if(ObjArray[l].posx<=0)
						rctexpl.left=-ObjArray[l].posx;
					else if(ObjArray[l].posx+Dimensions.dim_explosion_width>800)
						rctexpl.right=800-ObjArray[l].posx;
					if(ObjArray[l].m_nFlags>12)
						lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,Explosion1_surf,&rctexpl,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					else
						if(ObjArray[l].m_nFlags>9)
						lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,Explosion2_surf,&rctexpl,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					else 
						if(ObjArray[l].m_nFlags>6)
						lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,Explosion3_surf,&rctexpl,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					else 
						if(ObjArray[l].m_nFlags>3)
						lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,Explosion4_surf,&rctexpl,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					else	
						lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,Explosion5_surf,&rctexpl,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					rctexpl.right=Dimensions.dim_explosion_width;
					rctexpl.left=0;
					break;
				case SMALLEXPLOSION:
					if(ObjArray[l].m_nFlags>10)
						lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,SmallExpl1_surf,&rctsmallexpl,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					else
						if(ObjArray[l].m_nFlags>5)
						lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,SmallExpl2_surf,&rctsmallexpl,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					else 
						if(ObjArray[l].m_nFlags>0)
						lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,SmallExpl3_surf,&rctsmallexpl,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					break;
				case TSMALLEXPLOSION:
					if(ObjArray[l].posy>600-Dimensions.dim_tsmallexpl_height)
						rcttsmallexpl.bottom=rcttsmallexpl.bottom-40;
					if(ObjArray[l].m_nFlags>10)
						lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,TSmallExpl1_surf,&rcttsmallexpl,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					else
						if(ObjArray[l].m_nFlags>5)
						lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,TSmallExpl2_surf,&rcttsmallexpl,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					else 
						if(ObjArray[l].m_nFlags>0)
						lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,TSmallExpl3_surf,&rcttsmallexpl,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					break;
				case PARAMAN:
				
					if(randan<RAND_MAX/3)
					lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,para1_surf,&rctpara,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					else if(randan<RAND_MAX*2/3)
						lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,para2_surf,&rctpara,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
					else
						lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,para3_surf,&rctpara,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);

					break;
				case CHUTE:
						lpDDSBack->BltFast(ObjArray[l].posx,ObjArray[l].posy,chute1_surf,&rctchute,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);

					break;
			}

		}		
	}
	// blit HUD
	rct.right=800;
	rct.bottom=600;
	lpDDSBack->BltFast(0,0,int_surf,&rct,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	//Text op hud
	if(DD_OK==lpDDSBack->GetDC(&hdc)){
		//Eigen fontje maken
		HFONT hfont;
		hfont=CreateFont(20,15,0,0,400,0,0,0,DEFAULT_CHARSET
,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH
,"Ikke");
		hfont=(HFONT)SelectObject(hdc,hfont);
		
		SetTextColor(hdc,RGB(220,0,50));
		//Text transparant plaatsen
		SetBkMode(hdc,TRANSPARENT);
		char punten[10];
		char level[10];
		sprintf(level,"Level:%i",PIInfo.m_nLevel);
		sprintf(punten,"Points: %i",PIInfo.m_lPoints);
		TextOut(hdc,20,10,punten,strlen(punten));
		TextOut(hdc,20,25,level,strlen(level));
		/*//debug stuff**********************
		int deb=0;
		for(int d=0;d<MAXOBJECTS;d++)
		{
			if(ObjArray[d].m_nObjType==HELI && ObjArray[d].m_bInGame)
				deb++;
		}
		char debug[10];
		sprintf(debug,"%i",deb);
		TextOut(hdc,250,10,debug,strlen(debug));
		// end debug stuff*********************/
		hfont=(HFONT)SelectObject(hdc,hfont);
		DeleteObject(hfont);
		lpDDSBack->ReleaseDC(hdc);
	}	
	if(PIInfo.game_counters[8]!=0)
	{
		rct.left=0;
		rct.top=0;
		rct.right=Dimensions.dim_message_width;
		rct.bottom=Dimensions.dim_message_height;
		lpDDSBack->BltFast(400-Dimensions.dim_message_width/2,300-Dimensions.dim_message_height,GetReady_surf,&rct,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
	}

}
	else
	{	
		//Titelscherm tekenen indien niet ingame
		lpDDSBack->BltFast(0,0,title_surf,&rct,DDBLTFAST_WAIT);
		rct.right=Dimensions.dim_cursor_width;
		rct.bottom=Dimensions.dim_cursor_height;
		lpDDSBack->BltFast(250,400+40*(PIInfo.m_nCursorPos),cursor_surf,&rct,DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
		//Opties in titlescherm:
		if(DD_OK==lpDDSBack->GetDC(&hdc)){
		
		HFONT hfont;
		hfont=CreateFont(20,15,0,0,400,0,0,0,DEFAULT_CHARSET
,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH
,"Ikke");
		hfont=(HFONT)SelectObject(hdc,hfont);
		
		SetTextColor(hdc,RGB(229,229,229));
		SetBkMode(hdc,TRANSPARENT);
		if(PIInfo.m_bOptions==FALSE)
		{
			TextOut(hdc,300,400,"New Game",8);
			TextOut(hdc,300,440,"Options",7);
			TextOut(hdc,300,480,"Exit",4);
		}
		else
		{
			if(PIInfo.m_bSounds==TRUE)
			TextOut(hdc,300,400,"Sounds on",9);
			else TextOut(hdc,300,400,"Sounds off",10);
			TextOut(hdc,300,440,"Back",4);
		}
		hfont=(HFONT)SelectObject(hdc,hfont);
		DeleteObject(hfont);
		lpDDSBack->ReleaseDC(hdc);
	}	
	}
	// flip to the primary surface
	lpDDSPrimary->Flip(0,DDFLIP_WAIT);
}

void CreateExplosion(int obj)
{
	int l=0;
	while(ObjArray[l].m_bInGame)
		l++;
	if(l<MAXOBJECTS)
	{
		ObjArray[l].posx=ObjArray[obj].posx;
		ObjArray[l].posy=ObjArray[obj].posy;
		ObjArray[l].m_bInGame=TRUE;
		ObjArray[l].m_nMoreFlags=0;
		ObjArray[l].m_nFlags=15;
		if(ObjArray[obj].m_nObjType==HELI)
			ObjArray[l].m_nObjType=EXPLOSION;
		else if(ObjArray[obj].m_nObjType==CHUTE || ObjArray[obj].m_nObjType==BONUSPLANE)
			ObjArray[l].m_nObjType=TSMALLEXPLOSION;
		else if(ObjArray[obj].m_nObjType==PARAMAN)
			ObjArray[l].m_nObjType=SMALLEXPLOSION;
		ObjArray[l].vectx=ObjArray[obj].vectx;
		ObjArray[l].vecty=0;
	}

}


void BulletCollisionCheck(int bulletnr)
{
	int p=0;
	int tpb=0;
	for(int l=0;l<MAXOBJECTS;l++)
	{
		switch(ObjArray[l].m_nObjType)
		{
			case BONUSPLANE:
				if(ObjArray[l].m_bInGame && ObjArray[bulletnr].posy<300)
				{
					if(ObjArray[bulletnr].posy-ObjArray[l].posy<Dimensions.dim_heli_height &&ObjArray[bulletnr].posy-ObjArray[l].posy>=0)
						if(ObjArray[bulletnr].posx-ObjArray[l].posx<Dimensions.dim_heli_width &&ObjArray[bulletnr].posx-ObjArray[l].posx>=0)
						{
							int whatbonus;
							whatbonus=rand();
							 
							 if(whatbonus<=RAND_MAX/4)
							 {
								PIInfo.game_counters[BONUSTIME]=1000;
								PIInfo.m_bRapidFire=TRUE;
								tpb=13;
							 }
							else if(whatbonus<=(RAND_MAX/4)*2)
							{
								tpb=14;
								PIInfo.m_lPoints=PIInfo.m_lPoints+1000;
							}
							else if(whatbonus<=(RAND_MAX/4)*3)
							{
								PIInfo.m_lPoints=PIInfo.m_lPoints+500;
								tpb=15;
							}
							else 
							{
								PIInfo.game_counters[BONUSTIME]=1000;
								PIInfo.m_bNoChutesBon=TRUE;								
								tpb=16;
							}
							
						
								
							if(PIInfo.m_bSounds)
							{
								DWORD dwStatus;
								lpDSBSounds[tpb]->GetStatus(&dwStatus);	
								if (dwStatus & DSBSTATUS_BUFFERLOST)
								 RestoreDSBuffers();
								lpDSBSounds[tpb]->Play(0, 0, 0);
							}
	
								ObjArray[l].m_bInGame=FALSE;
								ObjArray[bulletnr].m_bInGame=FALSE;
						
								CreateExplosion(l);
						}
				}
			break;

			case BULLET:
				break;
			case CHUTE:
				if(ObjArray[l].m_bInGame)
				{
					if(ObjArray[bulletnr].posy-ObjArray[l].posy<Dimensions.dim_chute_height &&ObjArray[bulletnr].posy-ObjArray[l].posy>=0)
						if(ObjArray[bulletnr].posx-ObjArray[l].posx<Dimensions.dim_chute_width &&ObjArray[bulletnr].posx-ObjArray[l].posx>=0)
						{
							DWORD dwStatus;
						if(PIInfo.m_bSounds)
						{						
							
							lpDSBSounds[1]->GetStatus(&dwStatus);	
							if (dwStatus & DSBSTATUS_BUFFERLOST)
							 RestoreDSBuffers();
							lpDSBSounds[1]->Play(0, 0, 0);
						}
							ObjArray[ObjArray[l].m_nFlags].m_nFlags=NOCHUTE;
							ObjArray[l].m_bInGame=FALSE;
							ObjArray[bulletnr].m_bInGame=FALSE;
							PIInfo.m_lPoints=PIInfo.m_lPoints+5;
							CreateExplosion(l);
						if(PIInfo.m_bSounds)
						{							
							lpDSBSounds[9]->GetStatus(&dwStatus);	
							if (dwStatus & DSBSTATUS_BUFFERLOST)
							 RestoreDSBuffers();
							lpDSBSounds[9]->Play(0, 0, 0);
						}
						}
				}
				break;
			case PARAMAN:
				if(ObjArray[l].m_bInGame)
				{
					
					if(ObjArray[bulletnr].posy-ObjArray[l].posy<Dimensions.dim_para_height &&ObjArray[bulletnr].posy-ObjArray[l].posy>=0)
						if(ObjArray[bulletnr].posx-ObjArray[l].posx<Dimensions.dim_para_width &&ObjArray[bulletnr].posx-ObjArray[l].posx>=0)
						{
						if(PIInfo.m_bSounds)
						{						
							DWORD dwStatus;
							lpDSBSounds[4]->GetStatus(&dwStatus);	
							if (dwStatus & DSBSTATUS_BUFFERLOST)
							 RestoreDSBuffers();
							lpDSBSounds[4]->Play(0, 0, 0);
						}

							ObjArray[l].m_bInGame=FALSE;
							ObjArray[bulletnr].m_bInGame=FALSE;
							if(ObjArray[l].m_nFlags!=NOCHUTE)
								ObjArray[ObjArray[l].m_nFlags].m_bInGame=FALSE;
							PIInfo.m_lPoints=PIInfo.m_lPoints+5;
							CreateExplosion(l);
						}
				}
				break;
			case HELI:
				if(ObjArray[l].m_bInGame && ObjArray[bulletnr].posy<300)
				{
					if(ObjArray[bulletnr].posy-ObjArray[l].posy<Dimensions.dim_heli_height &&ObjArray[bulletnr].posy-ObjArray[l].posy>=0)
						if(ObjArray[bulletnr].posx-ObjArray[l].posx<Dimensions.dim_heli_width &&ObjArray[bulletnr].posx-ObjArray[l].posx>=0)
						{
							int whichexpl;
							whichexpl=rand();
							int tp=5;
							if(whichexpl>12000)
								tp=6;
							if(whichexpl>32000)
								tp=7;
						if(PIInfo.m_bSounds)
						{
							DWORD dwStatus;
							lpDSBSounds[tp]->GetStatus(&dwStatus);	
							if (dwStatus & DSBSTATUS_BUFFERLOST)
							 RestoreDSBuffers();
							lpDSBSounds[tp]->Play(0, 0, 0);
						}

							ObjArray[l].m_bInGame=FALSE;
							ObjArray[bulletnr].m_bInGame=FALSE;
							PIInfo.m_lPoints=PIInfo.m_lPoints+10;
							CreateExplosion(l);
						}
				}
				break;
		}
	}

}
//	-----Opgelet-----
//
//Nogmaals :Deze functie moet onderaan staan (zie eerste les programmeren)
//Wil je dit niet doen zorg er dan voor dat je eerst de prototypes van je andere functies
//hier voor schrijft!
//
//------ Application Loop ------//
// In WinMain() zal programme grootste deel van de tijd zitten, hierin gebeurt echter het 'minste'
//	voor de programmeur
// Wat hier gebeurt:
//	1) Alles eenmalig initialiseren mbv Init()
//	2) Teller aanmaken, elke keer als teller 'op' is moet het spel geupdate/hertekend worden
//  3) Alle messages ondertussen verwerken
//	4) Kijken of gebruiker iets invoer mbv DoInputCheck()


// Telkens je een functie-aanroep ziet volg je die om te kijken wat hij doet. De eerste zal dus zodadelijk
// Init()zijn. (deze staat static aangegeven om meerdere inits te verhinderen, hierdoor staat hij wel niet 
// in classview en moet je dus handmatig scrollen.

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;				// message from queue for message handling
	LONGLONG cur_time;       // current time
	BOOL notDone=TRUE;       // flag for thread completion
	DWORD  time_count=16;    // ms per frame, default if no performance counter
	LONGLONG perf_cnt;       // performance timer frequency
	BOOL perf_flag=FALSE;    // flag determining which timer to use
	LONGLONG next_time=0;    // time to render next frame
	LONGLONG last_time=0;	 // time of previous frame
	double time_elapsed;	 // time since previous frame
	double time_scale;		 // scaling factor for time
	
	// initialize the application, exit on failure
	// We doen dit eenmalig! Op deze manier kunnen we de WM_ACTIVATEAPP message handig verwerken (zie later)
	bIsInit=0;
    if (!Init(hInstance, nCmdShow)) {
		Cleanup(); 
        return FALSE;
	}
	bIsInit=1;

	// Enkele spelersvariabelen op default waarden zetten
	PIInfo.m_lGunPos=90;
	PIInfo.m_bPlaying=FALSE;
	PIInfo.m_nCursorPos=0;
	PIInfo.mini_counter=0;
	PIInfo.m_nLevel=1;
	PIInfo.m_bRapidFire=FALSE;
	PIInfo.m_bOptions=FALSE;
	PIInfo.m_bSounds=TRUE;
	PIInfo.m_lPoints=0;
	srand( (unsigned)time( NULL ) ); //random seed adhv huidige tijd

	//Instellen van de Timer! Aangezien WM_TIMER net iets te 'druk' is (veel geheugen nodig, lage resolutie)
	//gaan we eens kijken of het huidige systeem geen HighPerformance Counter aanwezig heeft.
	//Deze timer zit standaard in alle nieuwe systemen (vanaf pentium zeker, meeste 486's ook nog)
	//
	//We kijken of systeem er 1 heeft
	if (QueryPerformanceFrequency((LARGE_INTEGER *) &perf_cnt)) { 

		// zo ja, instellen. In dit geval 60tics per seconde

		perf_flag=TRUE;
		time_count=perf_cnt/60;
		QueryPerformanceCounter((LARGE_INTEGER *) &next_time); 
		time_scale=1.0/perf_cnt;

	} else { 

		// zo nee, werken met huidige tijd en telkens kijken of tijd vertreken is

		next_time=timeGetTime(); 
		time_scale=0.001;
	} 

	// laatste tijd opslaan

	last_time=next_time;


	// Standaard message loop
	//
	// run till completed 
	while (notDone) { 
		
		// is there a message to process? 

		if (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE)) { 

			// yes, is it a quit message? 

			if (msg.message==WM_QUIT) 

				notDone=FALSE; 

			// dispatch the message 

			TranslateMessage(&msg);
			DispatchMessage(&msg); 

		} else {
//******************			GAME LOOP *********************
			
			
			// use the appropriate method to get time 
			// and calculate elapsed time since last frame
			if (perf_flag) 
				QueryPerformanceCounter((LARGE_INTEGER *) &cur_time); 
			else 
				cur_time=timeGetTime(); 

			// is it time to render the frame? 

			if (cur_time>next_time  &&  bHasFocus==TRUE) { 

				// calculate elapsed time

				time_elapsed=(cur_time-last_time)*time_scale;

				// save frame time

				last_time=cur_time;
				
				
				// yes, render the frame 
				DoRenderFrame();
				// Klein tellertje om ervoor te zorgen dat enkele toetsaanslagen door DInput
				// niet als lange reeks aanslagen wordt beschouwd. Zie DoInputCheck()
				if(PIInfo.mini_counter!=0)
					PIInfo.mini_counter--;

				//update de timers
				for(int j=0;j<MAXCOUNTERS;j++)
				{
					if(PIInfo.game_counters[j]!=0)
						PIInfo.game_counters[j]--;
				}
				if(PIInfo.game_counters[BONUSTIME]==0) //Wanneer bonustijd verstreken is, alle bonussen af
				{
					if(PIInfo.m_bRapidFire=1)
						PIInfo.m_bRapidFire=0;
					if(PIInfo.m_bNoChutesBon=1)
						PIInfo.m_bNoChutesBon=0;
					if(PIInfo.m_bWind=1)
						PIInfo.m_bWind=0;

				}
				
				//Als we ingame zijn, allerlei zaken in gameloop uitvoeren
				if(PIInfo.m_bPlaying)
				{
					// Update objectlocaties
					UpdateAll();
					
					int countfly=0; //verder nodig

					//HEli spawnen wanneer random tijd verstreken is, maar enkel indien leveltijd nog niet verstreken is
					if(PIInfo.game_counters[HELI]==0 && PIInfo.game_counters[9]!=0) 
						SpawnObject(HELI,NULL);
					// Bonusvehicle spawnen wanneer tijd verstreken is, maar enkel indien er deze level nog geen is gespawnt
					if(PIInfo.game_counters[BONUS]==0 && PIInfo.m_bBonusOnce)
					{
						SpawnObject(BONUSPLANE,NULL);
						PIInfo.m_bBonusOnce=0;
					}
					if(PIInfo.game_counters[9]==0) //Wanneer leveltijd (2minuten,7200tics) verstreken is.
					{
						//Minder abrupt level einde. We stoppen pas als alle heli's weg zijn
						//Hiervoor tellen we ze na die ingame zijn (ook de paras boven de 100pixels van onderkant)
						PIInfo.game_counters[HELI]=-1;
						for(int i=0;i<MAXOBJECTS;i++)
						{
							if(ObjArray[i].m_bInGame && ObjArray[i].m_nObjType==HELI )
								countfly++;
							if(ObjArray[i].m_bInGame && ObjArray[i].m_nObjType==PARAMAN && ObjArray[i].posy<500)
								countfly++;
						}
						//Als level mag beeindigt worden
						if(countfly==0)
						{
							//Message tonen 
							BlitMessage(LEVDONE); 
							//en geluidje afspelen
							if(PIInfo.m_bSounds)
							{
							DWORD dwStatus;
							lpDSBSounds[11]->GetStatus(&dwStatus);	
							 if (dwStatus & DSBSTATUS_BUFFERLOST)
								 RestoreDSBuffers();
							lpDSBSounds[11]->Play(0, 0, 0);
							}
							//Volgende level klaarmaken
							PrepareNewLevel(PIInfo.m_nLevel+1);
						}
					}
					//Iedere tic kijken of speler gameOver is
					if(CheckGameOver() && PIInfo.m_nKeepPlaying[0]!=9)
					{
						//Zo ja, muziek stoppen
						FSOUND_Stream_Stop(stream1)	;						
						// geluidje afspelen van gameover
						if(PIInfo.m_bSounds)
						{							
							DWORD dwStatus;
							lpDSBSounds[10]->GetStatus(&dwStatus);	
							 if (dwStatus & DSBSTATUS_BUFFERLOST)
								 RestoreDSBuffers();
							lpDSBSounds[10]->Play(0, 0, 0);
						}
						//Instellen dat gebruiker op C kan drukken om terug naar titelscherm te gaan
						PIInfo.m_bContEnabled=1;
						//Message tonen
						BlitMessage(GAMEOVER);
						//Spelloop onderbreken tot gebruiker op c drukt 
						while(PIInfo.m_bContinue!=1)
							DoInputCheck();
						PIInfo.m_bPlaying=FALSE;
					}

				}
				//Kijken of gebruiker iets invoert
				DoInputCheck();


				// set time for next frame 
				next_time = cur_time + time_count; 
			}
		}
	}
	
	// exit returning final message

    return (msg.wParam);
}

