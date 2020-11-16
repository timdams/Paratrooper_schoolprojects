

class CPTGameObject  
{
public:
	int m_nFlags;
	int m_nMoreFlags;
	
	int vectx;
	int vecty;
	
	int posx;
	int posy;
	bool m_bInGame;
	int m_nObjType;  

	CPTGameObject();
	CPTGameObject(const CPTGameObject& obj);
	CPTGameObject(int ObjType,int x,int y,int vx,int vy,bool InGame,int Flag);
	CPTGameObject& operator=(const CPTGameObject& obj2);
	virtual ~CPTGameObject();
};

