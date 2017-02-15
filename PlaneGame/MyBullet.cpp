#include "stdafx.h"
#include "MyBullet.h"

CPoint  CMyBullet::size = CPoint(10, 20);
CBitmap* CMyBullet::bmpDraw = new CBitmap();
CDC* CMyBullet::memDC = new CDC();
CMyBullet::CMyBullet()
{
}

CMyBullet::CMyBullet(CPoint pos,int xSpeed)
{
	Type = MY_BULLET;
	this->pos = pos;
	this->xSpeed = xSpeed;
	ySpeed = -15;
}


CMyBullet::~CMyBullet()
{
}


void CMyBullet::draw(CDC* pDC)
{
	memDC->SelectObject(bmpDraw);
	switch (xSpeed)
	{
	case 0:pDC->TransparentBlt(pos.x - size.x / 2, pos.y - size.y / 2,
		size.x, size.y, memDC, 0, 0, size.x, size.y, RGB(0, 0, 255)); break;
	case 2:pDC->TransparentBlt(pos.x - size.x / 2, pos.y - size.y / 2,
		size.x, size.y, memDC, 10, 0, size.x, size.y, RGB(0, 0, 255)); break;
	case 4:case 6:pDC->TransparentBlt(pos.x - size.x / 2, pos.y - size.y / 2,
		size.x, size.y, memDC, 30, 0, size.x, size.y, RGB(0, 0, 255)); break;
	case -2:pDC->TransparentBlt(pos.x - size.x / 2, pos.y - size.y / 2,
		size.x, size.y, memDC, 20, 0, size.x, size.y, RGB(0, 0, 255)); break;
	case -4:case -6:pDC->TransparentBlt(pos.x - size.x / 2, pos.y - size.y / 2,
		size.x, size.y, memDC, 40, 0, size.x, size.y, RGB(0, 0, 255)); break;
	default:
		break;
	}
}

CPoint CMyBullet::GetSize()
{
	return size;
}

void CMyBullet::Loadimg()
{
	if (bmpDraw->m_hObject == NULL)
		bmpDraw->LoadBitmap(IDB_MYBULLET);//¹²ÓÐµÄÍ¼Æ¬
	if (memDC->m_hDC == NULL)
		memDC->CreateCompatibleDC(NULL);
}


int CMyBullet::OnImpact(int type)
{
	if (type == DESTROY || type == GEN_ENEMY_PLANE || type == ELITE_ENEMY_PLANE || type == BOSS ||
		type == AIRLINER)
		SetDestroy();
	return 0;
}
