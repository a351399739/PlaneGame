#include "stdafx.h"
#include "GenEnemy.h"

CPoint  CGenEnemy::size = CPoint(35, 35);
CBitmap* CGenEnemy::bmpDraw = new CBitmap();
CDC*  CGenEnemy::memDC = new CDC();
CBitmap* CGenEnemy::bmpDraw2 = new CBitmap();		
int CGenEnemy::sShotChance = 30;
int CGenEnemy::increShotChance = 3;
int CGenEnemy::oAppearDown = 10;

CGenEnemy::CGenEnemy(CRect rect)
{
	Type = GEN_ENEMY_PLANE;
	HP = 15;
	xSpeed = rand() % 7 - 3;//x�ٶ����ȡ-3��3

	pos.x = rand() % rect.right + 1;//���ֵص�x����

	ySpeed = rand() % 5 + 1;//y�ٶ�-5��5����y�����������˳��ֵĵص㣬y�ٶȲ���Ϊ0
	if (rand() % 100 < oAppearDown)	//���Ϸ����ֵĵл��Ƚ϶�
	{
		ySpeed *= -1;
		pos.y = rect.bottom;
	}
	else pos.y = 0;

	
}


CGenEnemy::CGenEnemy()
{
}


CGenEnemy::~CGenEnemy()
{
}


void CGenEnemy::draw(CDC* pDC)
{
	if (White)
	{
		memDC->SelectObject(bmpDraw2);
		White = false;
	}
	else
		memDC->SelectObject(bmpDraw);
	if (ySpeed < 0)
		pDC->TransparentBlt(pos.x - size.x / 2, pos.y - size.y / 2,
			size.x, size.y, memDC, 35, 0, 35, 35, RGB(0, 0, 255));//ԭͼ��СΪ��70��35��,Ӧ��Ϊ�����֡�
	else
		pDC->TransparentBlt(pos.x - size.x / 2, pos.y -size.y / 2,
			size.x, size.y, memDC, 0, 0, 35, 35, RGB(0, 0, 255));
}

void CGenEnemy::shot(CObList* pList,CPoint mePos,int level)
{
	if (rand() % 100> (sShotChance+level*increShotChance)) return;
	CGoods* MBtemp1 = new CEnemyBullet(pos,mePos);
	pList->AddTail(MBtemp1);
}



CPoint CGenEnemy::GetSize()
{
	return size;
}


void CGenEnemy::Loadimg()
{
	if (bmpDraw->m_hObject == NULL)
		bmpDraw->LoadBitmap(IDB_GENENEMY);//���е�ͼƬ
	if (bmpDraw2->m_hObject == NULL)
		bmpDraw2->LoadBitmapW(IDB_WITHEGEN);
	if (memDC->m_hDC == NULL)
		memDC->CreateCompatibleDC(NULL);
}


string CGenEnemy::ToString()
{
	string a = "";
	a += CGameObject::IntToString(pos.x);
	a += " ";
	a += CGameObject::IntToString(pos.y);
	a += " ";
	a += CGameObject::IntToString(xSpeed);
	a += " ";
	a += CGameObject::IntToString(ySpeed);
	a += " ";
	a += CGameObject::IntToString(Type);
	a += " ";
	a += CGameObject::IntToString(HP);
	a += " ";
	return a;
}
