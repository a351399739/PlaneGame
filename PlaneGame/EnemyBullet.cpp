#include "stdafx.h"
#include "EnemyBullet.h"

CPoint  CEnemyBullet::size = CPoint(8, 8);
CBitmap* CEnemyBullet::bmpDraw = new CBitmap();
CDC*  CEnemyBullet::memDC = new CDC();
CEnemyBullet::CEnemyBullet()
{
}


CEnemyBullet::~CEnemyBullet()
{
}


CEnemyBullet::CEnemyBullet(CPoint pos,CPoint mePos,int xSpeed,int ySpeed)
{
	Type = ENEMY_BULLET;
	srand((unsigned)time(NULL) + pos.x + mePos.y);
	this->pos = pos;

	
	if (xSpeed != 0 || ySpeed != 0)								//如果速度已被上级定义则不修改
	{
		this->xSpeed = xSpeed;
		this->ySpeed = ySpeed;
		return;						
	}
	if (rand() % 2)												//否则一半的几率随机乱发
	{
		while ((this->xSpeed == this->ySpeed) && this->xSpeed == 0)
		{
			this->xSpeed = rand() % 11 - 5;
			this->ySpeed = rand() % 11 - 5;
		}
	}
	else														//另一半的几率试图瞄准自己
	{
		if (pos.x == mePos.x)					//如果在一垂直线上
		{
			this->xSpeed = 0;
			this->ySpeed = pos.y > mePos.y ? -1: 1;
		}
		else if (pos.y == mePos.y)				//如果在一水平线上
		{
			this->ySpeed = 0;
			this->xSpeed = pos.y > mePos.y ? -1 : 1;
		}
		else									//如果不在一直线上
		{
			if (abs(pos.x - mePos.x) > abs(pos.y - mePos.y))//x偏移大
			{
				this->ySpeed = (pos.y > mePos.y) ? -2 : 2;
				this->xSpeed = ((pos.x > mePos.x) ? -1 : 1) *
					2 * (pos.x - mePos.x) / (pos.y - mePos.y);
			}
			else											//y偏移大			
			{
				this->xSpeed = (pos.x > mePos.x) ? -2 : 2;
				this->ySpeed = ((pos.y > mePos.y) ? -1 : 1) *
					2 * (pos.y - mePos.y) / (pos.x - mePos.x);
			}

		}
		if (this->xSpeed > 5)this->xSpeed = 5;				//但是不能太大
		else if (this->xSpeed < -5)this->xSpeed = -5;
		if (this->ySpeed > 5)this->ySpeed = 5;
		else if (this->ySpeed < -5) this->ySpeed = -5;
	}
}



void CEnemyBullet::draw(CDC* pDC)
{
	memDC->SelectObject(bmpDraw);
	pDC->TransparentBlt(pos.x - size.x / 2, pos.y - size.y / 2,
		size.x, size.y, memDC, 0, 0, size.x, size.y, RGB(0, 0, 255));//原图大小为（70，35）,应分为两部分。
}




CPoint CEnemyBullet::GetSize()
{
	return size;
}

void CEnemyBullet::Loadimg()
{
	if (bmpDraw->m_hObject == NULL)
		bmpDraw->LoadBitmap(IDB_ENEMYBULLET);//共有的图片
	if (memDC->m_hDC == NULL)
		memDC->CreateCompatibleDC(NULL);
}


int CEnemyBullet::OnImpact(int type)
{
	if (type == DESTROY || type == MY_PLANE)
		SetDestroy();
	return 0;
}
