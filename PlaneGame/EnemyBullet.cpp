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

	
	if (xSpeed != 0 || ySpeed != 0)								//����ٶ��ѱ��ϼ��������޸�
	{
		this->xSpeed = xSpeed;
		this->ySpeed = ySpeed;
		return;						
	}
	if (rand() % 2)												//����һ��ļ�������ҷ�
	{
		while ((this->xSpeed == this->ySpeed) && this->xSpeed == 0)
		{
			this->xSpeed = rand() % 11 - 5;
			this->ySpeed = rand() % 11 - 5;
		}
	}
	else														//��һ��ļ�����ͼ��׼�Լ�
	{
		if (pos.x == mePos.x)					//�����һ��ֱ����
		{
			this->xSpeed = 0;
			this->ySpeed = pos.y > mePos.y ? -1: 1;
		}
		else if (pos.y == mePos.y)				//�����һˮƽ����
		{
			this->ySpeed = 0;
			this->xSpeed = pos.y > mePos.y ? -1 : 1;
		}
		else									//�������һֱ����
		{
			if (abs(pos.x - mePos.x) > abs(pos.y - mePos.y))//xƫ�ƴ�
			{
				this->ySpeed = (pos.y > mePos.y) ? -2 : 2;
				this->xSpeed = ((pos.x > mePos.x) ? -1 : 1) *
					2 * (pos.x - mePos.x) / (pos.y - mePos.y);
			}
			else											//yƫ�ƴ�			
			{
				this->xSpeed = (pos.x > mePos.x) ? -2 : 2;
				this->ySpeed = ((pos.y > mePos.y) ? -1 : 1) *
					2 * (pos.y - mePos.y) / (pos.x - mePos.x);
			}

		}
		if (this->xSpeed > 5)this->xSpeed = 5;				//���ǲ���̫��
		else if (this->xSpeed < -5)this->xSpeed = -5;
		if (this->ySpeed > 5)this->ySpeed = 5;
		else if (this->ySpeed < -5) this->ySpeed = -5;
	}
}



void CEnemyBullet::draw(CDC* pDC)
{
	memDC->SelectObject(bmpDraw);
	pDC->TransparentBlt(pos.x - size.x / 2, pos.y - size.y / 2,
		size.x, size.y, memDC, 0, 0, size.x, size.y, RGB(0, 0, 255));//ԭͼ��СΪ��70��35��,Ӧ��Ϊ�����֡�
}




CPoint CEnemyBullet::GetSize()
{
	return size;
}

void CEnemyBullet::Loadimg()
{
	if (bmpDraw->m_hObject == NULL)
		bmpDraw->LoadBitmap(IDB_ENEMYBULLET);//���е�ͼƬ
	if (memDC->m_hDC == NULL)
		memDC->CreateCompatibleDC(NULL);
}


int CEnemyBullet::OnImpact(int type)
{
	if (type == DESTROY || type == MY_PLANE)
		SetDestroy();
	return 0;
}
