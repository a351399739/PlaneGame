#pragma once
#include "EnemyBullet.h"
#include "Plane.h"
class CEnemyPlane :
	public CPlane
{
public:
	CEnemyPlane();
	virtual ~CEnemyPlane();

	bool White;

	virtual void shot(CObList* plist,CPoint mePos,int level) = 0;
	virtual int OnImpact(int type);
	virtual string ToString();
	
};

