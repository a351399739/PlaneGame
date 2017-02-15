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

CEliteEnemy::CEliteEnemy(CRect rect)	//��Ӣ��ֻ��������·�
{
	Type = ELITE_ENEMY_PLANE;
	EliteType = rand() % 6;
	HP = 200;				//��Ӣ����200Ѫ
	xSpeed = rand() % 5 - 2;//x�ٶ����ȡ-1��1
	pos.x = rand() % rect.right + 1;//���ֵص�x����

	ySpeed = rand() % 3 + 1;//y�ٶ�-2��2����y�����������˳��ֵĵص㣬y�ٶȲ���Ϊ0
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
	if (rand() % 100 > (sShotChance+ level * increShotChance)) return;				//��һ�����ʷ��ӵ�������ݵȼ�����

	if (rand() % 100 > oSpecial)					//3/4���ʷ�����ͨ�ӵ�
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
			CEnemyBullet* temp = new CEnemyBullet(pos, mePos, 0, ySpeed + 1);	//����һ��3��
			pList->AddTail(temp);
			temp = new CEnemyBullet(pos, mePos, -2, ySpeed + 1);
			pList->AddTail(temp);
			temp = new CEnemyBullet(pos, mePos, 2, ySpeed + 1);
			pList->AddTail(temp);
			break;
		}
		case 1:
		{
			CEnemyBullet* temp = new CEnemyBullet(CPoint(pos.x,pos.y+10), mePos, 0, ySpeed + 1);//��Բ3��
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(pos.x, pos.y), mePos, -2, ySpeed + 1);
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(pos.x, pos.y - 10), mePos, 2, ySpeed + 1);
			pList->AddTail(temp);
			break;
		}
		case 2:
		{
			CEnemyBullet* temp = new CEnemyBullet(CPoint(pos.x, pos.y + 30), mePos, 0, ySpeed + 1);//����ûx�ٶ�
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(pos.x, pos.y), mePos, 0, ySpeed + 1);
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(pos.x, pos.y - 30), mePos, 0, ySpeed + 1);
			pList->AddTail(temp);
			break;
		}
		case 3:
		{
			CEnemyBullet* temp = new CEnemyBullet(CPoint(pos.x + 30, pos.y), mePos, 0, ySpeed + 1);//����һ��3��
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(pos.x, pos.y), mePos, 0, ySpeed + 1);
			pList->AddTail(temp);
			temp = new CEnemyBullet(CPoint(pos.x - 30, pos.y), mePos, 0, ySpeed + 1);
			pList->AddTail(temp);
			break;
		}
		case 4:															//�������5�ţ�Ҳ�ǱȽ���������
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
			CEnemyBullet* temp = new CEnemyBullet(pos, mePos, 0, 4);//��boss͵ѧ�����ļ���-��-��һ���ܷ�7��
			pList->AddTail(temp);									//�ð��Ҿ�������д��QAQ
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
		bmpDraw->LoadBitmap(IDB_ELITEENEMY);//���е�ͼƬ
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
