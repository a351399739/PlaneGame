#include "stdafx.h"
#include "EliteEnemy.h"

CPoint  CEliteEnemy::size = CPoint(100, 75);
CBitmap* CEliteEnemy::bmpDraw = new CBitmap();
CBitmap* CEliteEnemy::bmpDraw2 = new CBitmap();
CDC*  CEliteEnemy::memDC = new CDC();
int CEliteEnemy::sShotChance = 30;
int CEliteEnemy::increShotChance = 3;
int CEliteEnemy::oSpecial = 25;
CEliteEnemy::CEliteEnemy()
{
}


CEliteEnemy::~CEliteEnemy()
{
}

CEliteEnemy::CEliteEnemy(CRect rect)	//精英机只会从上往下飞
{
	Type = ELITE_ENEMY_PLANE;
	EliteType = rand() % 6;
	HP = 200;				//精英机有200血
	xSpeed = rand() % 5 - 2;//x速度随机取-1到1
	pos.x = rand() % rect.right + 1;//出现地点x坐标

	ySpeed = rand() % 3 + 1;//y速度-2到2并且y的正负决定了出现的地点，y速度不能为0
	pos.y = 0;
}


void CEliteEnemy::draw(CDC* pDC)
{
	if (White)
	{
		White = false;
		memDC->SelectObject(bmpDraw2);
	}
	else
		memDC->SelectObject(bmpDraw);
	pDC->TransparentBlt(pos.x - size.x / 2, pos.y - size.y / 2,
		size.x, size.y, memDC, EliteType * 100, 0, size.x, size.y, RGB(0, 0, 255));
}

void CEliteEnemy::shot(CObList* pList, CPoint mePos, int level)
{
	//TRACE("CEliteEnemy::shot\n");
	srand((unsigned)time(NULL)+ pList->GetCount());
	if (rand() % 100 > (sShotChance+ level * increShotChance)) return;				//有一定几率发子弹，会根据等级调整

	if (rand() % 100 > oSpecial)					//3/4几率发射普通子弹
	{
		CEnemyBullet* temp = new CEnemyBullet(pos, mePos);
		pList->AddTail(temp);
	}
	else
	{
		switch (EliteType)
		{
		case 0:
		{
			CEnemyBullet* temp = new CEnemyBullet(pos, mePos, 0, ySpeed + 1);	//横着一排3个
			pList->AddTail(temp);
			temp = new CEnemyBullet(pos, mePos, -2, ySpeed + 1);
			pList->AddTail(temp);
			temp = new CEnemyBullet(pos, mePos, 2, ySpeed + 1);
			pList->AddTail(temp);
			break;
		}
		case 1:
		{
			CEnemyBullet* temp = new CEnemyBullet(CPoint(pos.x,pos.y+10), mePos, 0, ySpeed + 1);//半圆3个
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(pos.x, pos.y), mePos, -2, ySpeed + 1);
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(pos.x, pos.y - 10), mePos, 2, ySpeed + 1);
			pList->AddTail(temp);
			break;
		}
		case 2:
		{
			CEnemyBullet* temp = new CEnemyBullet(CPoint(pos.x, pos.y + 30), mePos, 0, ySpeed + 1);//横着没x速度
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(pos.x, pos.y), mePos, 0, ySpeed + 1);
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(pos.x, pos.y - 30), mePos, 0, ySpeed + 1);
			pList->AddTail(temp);
			break;
		}
		case 3:
		{
			CEnemyBullet* temp = new CEnemyBullet(CPoint(pos.x + 30, pos.y), mePos, 0, ySpeed + 1);//竖着一排3个
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(pos.x, pos.y), mePos, 0, ySpeed + 1);
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(pos.x - 30, pos.y), mePos, 0, ySpeed + 1);
			pList->AddTail(temp);
			break;
		}
		case 4:															//随机发射5颗，也是比较厉害的了
		{
			CEnemyBullet* temp = new CEnemyBullet(CPoint(pos.x+1,pos.y), mePos);
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(pos.x - 1, pos.y), mePos);
			pList->AddTail(temp);
			temp = new CEnemyBullet(pos, mePos);
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(pos.x, pos.y+1), mePos);
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(pos.x, pos.y-1), mePos);
			pList->AddTail(temp);
		}
		case 5:
		{
			CEnemyBullet* temp = new CEnemyBullet(pos, mePos, 0, 4);//从boss偷学过来的技能-。-，一次能发7个
			pList->AddTail(temp);									//好吧我就是懒得写了QAQ
			temp = new CEnemyBullet(pos, mePos, -2, 1);
			pList->AddTail(temp);
			temp = new CEnemyBullet(pos, mePos, -2, 2);
			pList->AddTail(temp);
			temp = new CEnemyBullet(pos, mePos, 2, 1);
			pList->AddTail(temp);
			temp = new CEnemyBullet(pos, mePos, 2, 2);
			pList->AddTail(temp);
			temp = new CEnemyBullet(pos, mePos, -1, 3);
			pList->AddTail(temp);
			temp = new CEnemyBullet(pos, mePos, 1, 3);
			pList->AddTail(temp);
			break;
		}
		default:
			break;
		}
		
	}
	
}
CPoint CEliteEnemy::GetSize()
{
	return size;
}



void CEliteEnemy::Loadimg()
{
	if (bmpDraw->m_hObject == NULL)
		bmpDraw->LoadBitmap(IDB_ELITEENEMY);//共有的图片
	if (bmpDraw2->m_hObject == NULL)
		bmpDraw2->LoadBitmapW(IDB_WHITEELITE);
	if (memDC->m_hDC == NULL)
		memDC->CreateCompatibleDC(NULL);
}


string CEliteEnemy::ToString()
{
	string a = "";
	a += CGameObject::IntToString(this->pos.x);
	a += " ";
	a += CGameObject::IntToString(this->pos.y);
	a += " ";
	a += CGameObject::IntToString(this->xSpeed);
	a += " ";
	a += CGameObject::IntToString(this->ySpeed);
	a += " ";
	a += CGameObject::IntToString(this->Type);
	a += " ";
	a += CGameObject::IntToString(HP);
	a += " ";
	a += CGameObject::IntToString(EliteType);
	a += " ";
	return a;
}
