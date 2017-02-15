#include "stdafx.h"
#include "Boom.h"

CImageList* CBoom::imglist = new CImageList();
CBoom::CBoom()
	: index(0)
{
}
CBoom::CBoom(CPoint pos,int index)
{
	this->index = index;
	this->pos = pos;
	state = 0;
}

CBoom::~CBoom()
{
}



bool CBoom::Loadimg()
{
	imglist->Create(66, 66, ILC_COLOR24 | ILC_MASK, 1, 1);
	CBitmap bitmap;
	bitmap.LoadBitmapW(IDB_BOOM);
	imglist->Add(&bitmap, RGB(0, 0, 0));
	return false;
}


void CBoom::draw(CDC* pDC)
{
	if (index > 7){ SetDestroy(); return; }
	imglist->Draw(pDC, index , pos, NULL);
}



void CBoom::move()
{
	index ++;
}


CPoint CBoom::GetSize()
{
	return CPoint();
}


string CBoom::ToString()
{
	string a = "";
	a += CGameObject::IntToString(pos.x);
	a += " ";
	a += CGameObject::IntToString(pos.y);
	a += " ";
	a += CGameObject::IntToString(index);
	a += " ";
	return a;
}