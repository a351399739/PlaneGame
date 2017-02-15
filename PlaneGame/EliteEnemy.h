#pragma once
#include "EnemyPlane.h"
class CEliteEnemy :
	public CEnemyPlane
{
public:
	CEliteEnemy();
	CEliteEnemy(CRect rect);
	virtual ~CEliteEnemy();

	static CPoint size;
	static CBitmap* bmpDraw;
	static CBitmap* bmpDraw2;
	static CDC* memDC;
	static int oSpecial;
	static void Loadimg();
	static int sShotChance;
	static int increShotChance;

	
	int EliteType;
	
	virtual CPoint GetSize();
	virtual void draw(CDC* pDC);
	void shot(CObList* pList, CPoint mePos, int level);
	virtual string ToString();
};

