#pragma once
#include "EnemyBullet.h"
#include "Plane.h"
class CBoss :
	public CPlane
{
public:
	CBoss();
	virtual ~CBoss();
	CBoss(int level,CRect rect);

	static int sHP;
	static int increHP;
	static int sShotChance;
	static int increShotChance;
	static int sBulletSum;
	static int increBulletSum;

	CPoint size;
	CBitmap bmpDraw;
	CBitmap bmpDraw2;
	CDC memDC;
	int HP;
	CRect rect;
	int level;
	bool White;

	virtual void draw(CDC* pDC);
	void shot(CObList* pList, CPoint mePos, int level);
	virtual CPoint GetSize();
	virtual void move();
	int OnImpact(int type);
	
};
