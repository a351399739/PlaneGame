#pragma once
#include "Goods.h"
#include "EnemyPlane.h"
class CSeeking :
	public CGoods
{
public:
	CSeeking();
	virtual ~CSeeking();
	CSeeking(CPoint pos);

	virtual CPoint GetSize();
	virtual void draw(CDC* pDC);
	static void Loadimg();
	virtual int OnImpact(int type);
	CEnemyPlane* pTarget;								//记录被追踪的角色
	
	static CObList* pEnemyList;
	static CPoint size;
	static CBitmap* bmpDraw;
	static CDC* memDC;
	static CImageList* imglist;
	static CRITICAL_SECTION* cs;
	int index;
	void Catch();
};

