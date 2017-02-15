#include "stdafx.h"
#include "Boss.h"


int CBoss::sHP = 5000;
int CBoss::increHP = 200;
int CBoss::sShotChance = 30;
int CBoss::increShotChance = 3;
int CBoss::sBulletSum = 10;
int CBoss::increBulletSum = 2;
CBoss::CBoss()
	: level(0)
	, White(false)
{
	Type = BOSS;
	size = CPoint(200, 144);
	if (bmpDraw.m_hObject == NULL)
		bmpDraw.LoadBitmap(IDB_BOSS);				//图片
	if (bmpDraw2.m_hObject == NULL)
		bmpDraw2.LoadBitmapW(IDB_WHITEBOSS);
	if (memDC.m_hDC == NULL)
		memDC.CreateCompatibleDC(NULL);
}


CBoss::~CBoss()
{
}

CBoss::CBoss(int level,CRect rect)							//boss初始化
{
	Type = BOSS;
	this->level = level;
	size = CPoint(200, 144);
	if (bmpDraw.m_hObject == NULL)
		bmpDraw.LoadBitmap(IDB_BOSS);					//图片
	if (bmpDraw2.m_hObject == NULL)
		bmpDraw2.LoadBitmapW(IDB_WHITEBOSS);
	if (memDC.m_hDC == NULL)
		memDC.CreateCompatibleDC(NULL);
	HP = sHP + level * increHP;								//boss机至少有5000血
	//HP = 10;
	xSpeed = rand() % 5 - 2;								//x速度随机取-2到2
	pos.x = rand() % rect.right + 1;						//出现地点x坐标
	ySpeed = 1;												//y速度取1
	pos.y = 0 - size.y/2;

	this->rect = rect;
}


void CBoss::draw(CDC* pDC)
{
	if (White)
	{
		White = false;
		memDC.SelectObject(bmpDraw2);
	}
	else
		memDC.SelectObject(bmpDraw);
	pDC->TransparentBlt(pos.x - size.x / 2, pos.y - size.y / 2,
		size.x, size.y, &memDC, 0, 0, size.x, size.y, RGB(0, 0, 255));
}

void CBoss::shot(CObList* pList, CPoint mePos, int level)
{
	if (rand() % 100 > (sShotChance+level*(increShotChance)))return;
	int BulletType = rand() % 3;
	switch (BulletType)
	{
	case 0:															//普通的七发子弹
	{
		CEnemyBullet* temp;
		temp = new CEnemyBullet(pos, mePos, 0, 4);
		pList->AddTail(temp);
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
	case 1:															//21发子弹
	{
		CEnemyBullet* temp;
		for (int i = pos.x - 20; i <= pos.x + 20; i += 20)
		{
			temp = new CEnemyBullet(CPoint(i,pos.y), mePos, 0, 4);
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(i,pos.y), mePos, -2, 1);
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(i,pos.y), mePos, -2, 2);
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(i,pos.y), mePos, 2, 1);
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(i,pos.y), mePos, 2, 2);
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(i,pos.y), mePos, -1, 3);
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(i,pos.y), mePos, 1, 3);
			pList->AddTail(temp);
		}
		break;
	}

	default:															//
		CEnemyBullet* temp;
		CPoint bpos;
		for (int i = 1; i <= (sBulletSum + level * increBulletSum); i++)
		{
			bpos.x = rand() % size.x + pos.x - size.x / 2;
			bpos.y = rand() % size.y + pos.y - size.y / 2;
			temp = new CEnemyBullet(CPoint(bpos.x,bpos.y), mePos,rand()%5-2,rand()%2+1);
			pList->AddTail(temp);
		}
		temp = new CEnemyBullet(pos, mePos);
		pList->AddTail(temp);
		break;
	}

}
CPoint CBoss::GetSize()
{
	return size;
}


void CBoss::move()									
{
	pos.y += ySpeed;
	pos.x += xSpeed;
	if (rand() % 100 > 97)xSpeed = rand() % 5 - 2;
	if (rand() % 100 > 97)ySpeed = -ySpeed;
	if (pos.x - size.x / 2 < 0 && xSpeed < 0)xSpeed = -xSpeed;
	else if (pos.x + size.x / 2 > rect.right && xSpeed > 0)xSpeed = -xSpeed;
	if (pos.y - size.y / 2 < 0 && ySpeed < 0)ySpeed = -ySpeed;
	else if (pos.y + size.y / 2 > rect.bottom / 3 && ySpeed > 0)ySpeed = -ySpeed;
}


int CBoss::OnImpact(int type)						//发生碰撞
{
	int score = 0;
	switch (type)
	{
	case MY_BULLET:
		score = rand() % 10 + 5; White = true;					//被我方子弹打中受到5-15点伤害
		break;
	case MY_PLANE:
		score = rand() % 20 + 30; White = true;					//遭遇我机受到30-50点伤害
		break;
	case SEEKING:
		score = rand() % 10 + 50; White = true;					//追踪弹造成50-60伤害
		break;
	default:
		break;
	}
	HP -= score;
	if (HP <= 0)
	{
		score *= 3;
		SetDestroy();
		SetImpact();								//以这种方式被标记为待摧毁的话，也会被标记为碰撞时摧毁
	}
	return score;
}
