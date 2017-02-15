#include "stdafx.h"
#include "AirLiner.h"

CPoint CAirLiner::size = CPoint(380, 340);
CBitmap* CAirLiner::bmpDraw = new CBitmap();
CDC*  CAirLiner::memDC = new CDC();
CAirLiner::CAirLiner()
{
}


CAirLiner::~CAirLiner()
{
}

CAirLiner::CAirLiner(CRect rect)
{
	Type = AIRLINER;
	HP = 50;
	ySpeed = 0;
	if (rand() % 2)
	{
		xSpeed = 2;
		pos.x =  -size.x / 2;
	}
	else
	{
		xSpeed = -2;
		pos.x = rect.right;
	}
	pos.y = rand() % (rect.bottom / 3) - size.y + 100;
}

CRect CAirLiner::MyGetRect(int xSpeed,int type)
{
	CRect rect;
	if (xSpeed > 0)
	{
		switch (type)
		{
		case HEAD:rect = CRect(CPoint(pos.x + 290, pos.y + 148), CPoint(pos.x + 380, pos.y + 191)); break;
		case TANK:rect = CRect(CPoint(pos.x + 236, pos.y + 148), CPoint(pos.x + 289, pos.y + 191)); break;
		case ENGINE:rect = CRect(CPoint(pos.x + 184, pos.y + 191), CPoint(pos.x + 235, pos.y + 255));  break;
		case WING1:rect = CRect(CPoint(pos.x + 121, pos.y + 191), CPoint(pos.x + 183, pos.y + 271)); break;
		case WING2:rect = CRect(CPoint(pos.x + 106, pos.y + 272), CPoint(pos.x + 166, pos.y + 305)); break;
		case WING3:rect = CRect(CPoint(pos.x + 93, pos.y + 306), CPoint(pos.x + 141, pos.y + 340)); break;
		case TAIL:rect = CRect(CPoint(pos.x + 0, pos.y + 96), CPoint(pos.x + 92, pos.y + 243)); break;
		}
		
	}
	else
	{
		switch (type)
		{
		case HEAD:rect = CRect(CPoint(pos.x + 0, pos.y + 148), CPoint(pos.x + 90, pos.y + 191)); break;
		case TANK:rect = CRect(CPoint(pos.x + 91, pos.y + 148), CPoint(pos.x + 144, pos.y + 191)); break;
		case ENGINE:rect = CRect(CPoint(pos.x + 145, pos.y + 191), CPoint(pos.x + 196, pos.y + 255)); break;
		case WING1:rect = CRect(CPoint(pos.x + 197, pos.y + 191), CPoint(pos.x + 259, pos.y + 271)); break;
		case WING2:rect = CRect(CPoint(pos.x + 214, pos.y + 272), CPoint(pos.x + 274, pos.y + 305)); break;
		case WING3:rect = CRect(CPoint(pos.x + 239, pos.y + 306), CPoint(pos.x + 287, pos.y + 340)); break;
		case TAIL:rect = CRect(CPoint(pos.x + 288, pos.y + 96), CPoint(pos.x + 380, pos.y + 243)); break;
		}
	}
	return rect;
}

CPoint CAirLiner::GetSize()
{
	return size;
}


void CAirLiner::Loadimg()
{
	if (bmpDraw->m_hObject == NULL)
		bmpDraw->LoadBitmap(IDB_AIRLINER);//共有的图片
	if (memDC->m_hDC == NULL)
		memDC->CreateCompatibleDC(NULL);
}


CRect CAirLiner::GetRect()						//空实现
{
	return CRect();
}


void CAirLiner::draw(CDC* pDC)
{
	memDC->SelectObject(bmpDraw);
	if (xSpeed > 0)
	{
		pDC->TransparentBlt(pos.x, pos.y,
			size.x, size.y, memDC, size.x, 0, size.x, size.y, RGB(0, 0, 255));
		//pDC->TextOutW();
	}
	else
	{
		pDC->TransparentBlt(pos.x, pos.y,
			size.x, size.y, memDC, 0, 0, size.x, size.y, RGB(0, 0, 255));
	}
	pDC->TextOutW(MyGetRect(xSpeed,TANK).left, MyGetRect(xSpeed, TANK).top+5, L"人："+GetStr(HP));
}
int CAirLiner::ImpactWith(CObList* pList)					//一个单位和一个链表是否产生碰撞
{
	int res = 0;
	for (POSITION pos = pList->GetHeadPosition(); pos != NULL;)
	{
		CGameObject* temp = (CGameObject*)pList->GetNext(pos);
		res += this->ImpactWith(temp);
	}
	return res;
}


int CAirLiner::ImpactWith(CGameObject* pObject)				//客机和这个单位是否产生碰撞
{
	int res = 0;
	int IsImpact = 0;
	CRect tempr = MyGetRect(1,1);
	if (pObject->Type == MY_BULLET)
	{
		if (tempr.IntersectRect(MyGetRect(xSpeed, HEAD), pObject->GetRect()))		/*------机头------------*/
		{
			IsImpact = 1;
			if (rand() % 2)																//一半几率血量归零
				res += HP;
			else
				res += rand() % 10 + 5;													//或者受到5-15点伤害
		}
		else if (tempr.IntersectRect(MyGetRect(xSpeed, TANK), pObject->GetRect()))	/*------油箱--------------*/
		{
			IsImpact = 1;																	//血量归零
			res += HP;
		}
		else if (tempr.IntersectRect(MyGetRect(xSpeed, ENGINE), pObject->GetRect()))/*-------发动机-----------*/
		{	
			IsImpact = 1;
			res += rand() % 5 + 5;														//受到5-10点伤害
			ySpeed += 5;																//并且飞速下降
		}
		else if (tempr.IntersectRect(MyGetRect(xSpeed, WING1), pObject->GetRect()) ||/*-------机翼------------*/
			tempr.IntersectRect(MyGetRect(xSpeed, WING2), pObject->GetRect()) ||
			tempr.IntersectRect(MyGetRect(xSpeed, WING3), pObject->GetRect()))
		{
			res += rand() % 5 + 10;														//受到10-15点伤害
			IsImpact = 1;
		}
		else if (tempr.IntersectRect(MyGetRect(xSpeed, TAIL), pObject->GetRect()))	/*-------机尾------------*/
		{
			IsImpact = 1;																//飞机尝试逃跑
			xSpeed *= 3;
		}

		if (IsImpact)							//一旦发生过碰撞，就会（看下面）
		{
			pObject->SetDestroy();
			HP -= res;							//根据res来计算积分：保底100，掉一血20，打爆三千，最后取相反数
			HP -= rand() % 5;					//1-5个人会跳伞，这个人将不扣你的分
			res *= 20;
			if (HP <= 0)
			{
				SetDestroy();
				SetImpact();
				res += 3000;
			}
			res += 100;
		}
	}
	return -res;
}

bool CAirLiner::IsOut(CRect rect)
{
	if (pos.x + size.x + 50 < 0 || pos.x - 50 > rect.right || pos.y > rect.bottom)
		return true;
	return false;
}


string CAirLiner::ToString()
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
	a += CGameObject::IntToString(HP);
	a += " ";
	return a;
}
