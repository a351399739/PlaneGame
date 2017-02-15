#pragma once
#include "GameList.h"
#include "MyBullet.h"
#include "Seeking.h"
#include "Plane.h"
class CMyPlane :
	public CPlane
{
public:
	CMyPlane();
	CMyPlane(CPoint pos);
	virtual ~CMyPlane();

	static void Loadimg();
	static CImageList* imglist;
	static int oPowerDowm;
	static int oSeeking;
	static CPoint size;

	int power;
	int nuclear;
	int index;
	int ProtectTime;
	CDC* memDC;

	virtual void draw(CDC* pDC);
	virtual CPoint GetSize();
	virtual void shot(CObList* pList);
	virtual void move();
	virtual int OnImpact(int type);
	virtual string ToString();
	
};

