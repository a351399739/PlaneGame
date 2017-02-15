#pragma once
#include "GameObject.h"
class CBoom :
	public CGameObject
{
public:
	CBoom();
	CBoom(CPoint pos,int index = 0);
	virtual ~CBoom();

	static CImageList* imglist;
	static bool Loadimg();
	
	int index;

	CPoint GetSize();
	void move();
	void draw(CDC* pDC);
	virtual string ToString();
};

