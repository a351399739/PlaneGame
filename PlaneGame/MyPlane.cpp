#include "stdafx.h"
#include "MyPlane.h"

CImageList* CMyPlane::imglist = new CImageList();
CPoint  CMyPlane::size = CPoint(50, 60);
int CMyPlane::oPowerDowm = 40;
int CMyPlane::oSeeking = 1;
CMyPlane::CMyPlane()
	: power(1)
	, nuclear(0)
	, index(0)
	, ProtectTime(0)
{
	memDC = new CDC();
	memDC->CreateCompatibleDC(NULL);
	CBitmap bmpDraw;
	bmpDraw.LoadBitmap(IDB_MYPLANE);
	memDC->SelectObject(&bmpDraw);
}

CMyPlane::CMyPlane(CPoint pos)
{
	index = 0;
	power = 1;
	nuclear = 3;
	ProtectTime = 5;
	HP = 30;
	Type = MY_PLANE;
	xSpeed = 5;
	ySpeed = 5;
	this->pos = pos;
	memDC = new CDC();
	memDC->CreateCompatibleDC(NULL);
	CBitmap bmpDraw;
	bmpDraw.LoadBitmap(IDB_MYPLANE);
	memDC->SelectObject(&bmpDraw);
}


CMyPlane::~CMyPlane()
{
	delete memDC;
}


void CMyPlane::draw(CDC* pDC)
{
	if (IsImmune() || ProtectTime > 0)
		imglist->Draw(pDC, index / 10, CPoint(pos.x-size.x,pos.y-40), NULL);
	pDC->TransparentBlt(pos.x - size.x/2, pos.y - size.y/2,
		50, 60, memDC, 0, 0, 50, 60, RGB(0, 0, 255));
	index++;
	if (index >= 70)index = 0;
	//TRACE("void CMyPlane::draw(CDC* pDC)%d\n",imglist->GetImageCount());
}

CPoint CMyPlane::GetSize()
{
	return size;
}

void CMyPlane::shot(CObList* pList)
{ 
	srand((unsigned)time(NULL) + pos.x + pos.y);
	CGoods* temp1 = new CMyBullet(this->pos);
	pList->AddHead(temp1);
	if (power >= 2)
	{
		temp1 = new CMyBullet(this->pos,2);
		pList->AddHead(temp1);
		temp1 = new CMyBullet(this->pos, -2);
		pList->AddHead(temp1);
	}
	if (power >= 3)
	{
		temp1 = new CMyBullet(this->pos, 4);
		pList->AddHead(temp1);
		temp1 = new CMyBullet(this->pos, -4);
		pList->AddHead(temp1);
	}
	if (power >= 4)
	{
		temp1 = new CMyBullet(this->pos, 6);
		pList->AddHead(temp1);
		temp1 = new CMyBullet(this->pos, -6);
		pList->AddHead(temp1);
	}
	for (int i = 0; i < power; i++)
	{
		if (rand() % 100 < oSeeking)
		{
			temp1 = new CSeeking(pos);
			pList->AddHead(temp1);
		}
	}
}


void CMyPlane::move()
{
}

int CMyPlane::OnImpact(int type)
{
	if (IsImmune())return 0;							//无敌模式下不会受到伤害，也不能获得增益
	if (ProtectTime > 0 && (type == ENEMY_BULLET||type == GEN_ENEMY_PLANE 
		|| type == ELITE_ENEMY_PLANE || type == BOSS))
	{													//保护时间内，不会受到伤害，但是能获得增益
		type = -10;
	}
	switch (type)
	{
	case ENEMY_BULLET:
		HP -= rand() % 5 + 5;						//掉血就可能降低自己的power
		power -= (rand() % 100) < oPowerDowm ? 1 : 0; break;	//遭遇子弹承受5-10点伤害
	case GEN_ENEMY_PLANE:
		HP -= rand() % 10 + 5; break;				//遭遇一般飞机承受5-15点伤害
		power -= (rand() % 100) < oPowerDowm ? 1 : 0; break;
	case ELITE_ENEMY_PLANE:
		HP -= rand() % 10 + 10; break;				//遭遇精英级承受10-20点伤害
		power -= (rand() % 100) < oPowerDowm ? 1 : 0; break;
	case ITEM_HP:									//各种增益
		if(HP <= 30)HP += 15; break;
	case ITEM_POWER:
		if (power < 4)power++; break;
	case ITEM_NUCLEAR:
		nuclear++; break;
	case ITEM_PROTECT:
		ProtectTime += 20; break;
	case BOSS:case DESTROY:case AIRLINER:
		HP = 0; break;									//遭遇BOSS血量归零
	default:
		break;
	}
	if (power < 1)power = 1;
	if (HP <= 0)
		SetDestroy();
	return 0;
}


string CMyPlane::ToString()
{
	string a = "";
	a += CGameObject::IntToString(pos.x);
	a += " ";
	a += CGameObject::IntToString(pos.y);
	a += " ";
	a += CGameObject::IntToString(HP);
	a += " ";
	a += CGameObject::IntToString(power);
	a += " ";
	a += CGameObject::IntToString(nuclear);
	a += " ";
	a += IsImmune() ? "1" : "0";
	a += "";
	return a;
}

void CMyPlane::Loadimg()
{
	imglist->Create(100, 85, ILC_COLOR8 | ILC_MASK, 1, 1);
	CBitmap bitmap;
	bitmap.LoadBitmapW(IDB_IMMUNE);
	imglist->Add(&bitmap, RGB(0, 0, 0));
	//TRACE("bool CMyPlane::LoadImmune()%d\n",imglist->GetImageCount());
	return;
}