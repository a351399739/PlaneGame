#pragma once
#include "Goods.h"
#include "atltypes.h"
class CItem :
	public CGoods
{
public:
	CItem();
	CItem(CPoint pos,int type = 0);
	virtual ~CItem();
	static CPoint size;
	static CBitmap* bmpDraw;
	static CDC* memDC;
	static CRect rect;
	static void Loadimg();

	virtual CPoint GetSize();
	virtual void draw(CDC* pDC);
	virtual void move();
	virtual int OnImpact(int type);
	virtual bool IsOut(CRect rect);
};

