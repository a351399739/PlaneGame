#include "stdafx.h"
#include "Item.h"

CPoint  CItem::size = CPoint(30, 30);
CBitmap* CItem::bmpDraw = new CBitmap();
CDC*  CItem::memDC = new CDC();
CRect CItem::rect = CRect();
CItem::CItem()
{
}


CItem::~CItem()
{
}


CItem::CItem(CPoint pos,int type)
{
	srand((unsigned)time(NULL) + pos.x + pos.y);
	if(type == 0)
		Type = rand() % 4 + 9;							//随机出现一种物品
	else
	{
		Type = type;
	}
	this->pos = pos;
	xSpeed = rand() % 5 + 2;
	ySpeed = pos.y > rect.bottom / 2 ? -1 : 1;
}


CPoint CItem::GetSize()
{
	return size;
}

void CItem::Loadimg()
{
	if (bmpDraw->m_hObject == NULL)
		bmpDraw->LoadBitmap(IDB_ITEM);//共有的图片
	if (memDC->m_hDC == NULL)
		memDC->CreateCompatibleDC(NULL);
}

void CItem::draw(CDC* pDC)
{
	//TRACE("void CItem::draw(CDC* pDC):%d %d\n", pos.x, pos.y);
	memDC->SelectObject(bmpDraw);
	pDC->TransparentBlt(pos.x - size.x / 2, pos.y - size.y / 2,
		size.x, size.y, memDC,(Type - 9) * 30, 0, size.x, size.y, RGB(0, 0, 255));
}


void CItem::move()							//不使用CGameObject提供的move
{
	pos.y += ySpeed;
	pos.x += xSpeed;
	if (rand() % 100 > 97)xSpeed = rand() % 5 - 2;
	if (pos.x - size.x / 2 < 0 && xSpeed < 0)xSpeed = -xSpeed;
	else if (pos.x + size.x / 2 > rect.right && xSpeed > 0)xSpeed = -xSpeed;
}


int CItem::OnImpact(int type)
{
	switch (type)
	{
	case DESTROY:
	case MY_PLANE:
		SetDestroy();
	default:
		break;
	}
	return 0;
}


bool CItem::IsOut(CRect rect)
{
	return pos.y + 400 < 0 || pos.x + 500 < 0
		|| pos.y - GetSize().y > rect.bottom || pos.x - 500 > rect.right;
}
