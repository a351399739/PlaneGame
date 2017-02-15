#pragma once
#include "GameObject.h"
class CGoods :
	public CGameObject
{
public:
	CGoods();
	virtual ~CGoods();
	//virtual int  OnImpact(int type);
	virtual string ToString();
};

