#pragma once
#include "Goods.h"
class CEnemyBullet :
	public CGoods
{
public:
	CEnemyBullet();
	CEnemyBullet(CPoint pos, CPoint mePos, int xSpeed = 0, int ySpeed = 0);
	virtual ~CEnemyBullet();

	static CPoint size;
	static CBitmap* bmpDraw;
	static CDC* memDC;

	virtual void draw(CDC* pDC);
	virtual CPoint GetSize();
	static void Loadimg();
	virtual int OnImpact(int type);
};

