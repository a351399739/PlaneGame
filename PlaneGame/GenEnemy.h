#pragma once
#include "EnemyPlane.h"
#include "atltypes.h"
class CGenEnemy :
	public CEnemyPlane
{
public:
	CGenEnemy();
	CGenEnemy(CRect rect);
	virtual ~CGenEnemy();

	static CPoint size;
	static CBitmap* bmpDraw;
	static CBitmap* bmpDraw2;
	static CDC* memDC;
	static int sShotChance;
	static int increShotChance;
	static int oAppearDown;
	static void Loadimg();

	
	virtual void shot(CObList* pList,CPoint mePos,int level);
	virtual CPoint GetSize();
	virtual void draw(CDC* pDC);
	virtual string ToString();
};

