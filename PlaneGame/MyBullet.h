#pragma once
#include "Goods.h"
#include "resource.h"
class CMyBullet :
	public CGoods
{
public:
	CMyBullet();
	CMyBullet(CPoint pos, int xsSpeed = 0);
	virtual ~CMyBullet();


	static CPoint size;
	static CBitmap* bmpDraw;
	static CDC* memDC;
	static void Loadimg();

	virtual void draw(CDC* pDC);
	virtual CPoint GetSize();
	virtual int OnImpact(int type);
};

