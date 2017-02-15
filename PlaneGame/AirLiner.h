#pragma once
#include "Plane.h"
#include "atltypes.h"
//����λ�õı��
#define HEAD		1
#define TANK		2
#define ENGINE		3
#define	WING1		4
#define WING2		5
#define WING3		6
#define TAIL		7
class CAirLiner :
	public CPlane
{
public:
	CAirLiner();
	CAirLiner(CRect rect);
	virtual ~CAirLiner();

	static CPoint size;
	static CBitmap* bmpDraw;
	static CDC* memDC;
	static void Loadimg();

	CRect MyGetRect(int xSpeed, int Type);
	virtual CPoint GetSize();
	virtual CRect GetRect();
	virtual void draw(CDC* pDC);
	virtual int ImpactWith(CObList* pList);		//��һ������Ƚ��Ƿ���ײ
	virtual int ImpactWith(CGameObject* pObject);
	virtual bool IsOut(CRect rect);
	virtual string ToString();
};

