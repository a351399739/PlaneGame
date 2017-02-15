// GameObject.cpp : 实现文件
//

#include "stdafx.h"
#include "PlaneGame.h"
#include "GameList.h"
#include "GameObject.h"


// CGameObject

CGameObject::CGameObject()
	: pos(0)
	, state(0)
	, Type(0)
{
}

CGameObject::~CGameObject()
{
}


// CGameObject 成员函数
CString CGameObject::GetStr(int sum)//可以取得int的数并用CString表示出来！
{
	CString str;
	str.Format(_T("%d"), sum);
	return str;
}

CString CGameObject::GetStr(long sum)//可以取得long的数并用CString表示出来！
{
	CString str;
	str.Format(_T("%ld"), sum);
	return str;
}

void CGameObject::move()
{
	pos.x += xSpeed;
	pos.y += ySpeed;
}

bool CGameObject::IsDestroy()			//被询问这个单位是否已经被标记为销毁
{
	return state >> ON_DESTROY & 1;
}


bool CGameObject::IsOut(CRect rect)		//被询问这个单位是否出界
{
	return pos.y + GetSize().y < 0 || pos.x + GetSize().x < 0
		|| pos.y - GetSize().y > rect.bottom || pos.x - GetSize().x > rect.right;
}


void CGameObject::SetImpact()								//把这个单位标记为由于碰撞而摧毁
{
	state = state | (1 << IS_IMPACT);
}

int CGameObject::ImpactWith(CObList* pList)					//一个单位前侧和一个链表是否产生碰撞
{
	int res = 0;
	for (POSITION pos = pList->GetHeadPosition(); pos != NULL;)
	{
		CGameObject* temp = (CGameObject*)pList->GetNext(pos);
		res += this->ImpactWith(temp);
	}
	return res;
}


CGameObject::CGameObject(CPoint pos)
{
	this->pos = pos;
}


bool CGameObject::IsImpact()								//这个单位是否因血量过低而被销毁
{
	return state >> IS_IMPACT & 1;
}

bool CGameObject::IsImmune()								//询问这个单位是否是免疫
{
	return state >> IS_IMMUNE & 1;
}

bool CGameObject::IsSeek()								//询问这个单位是否已经被追踪
{
	return state >> BY_SEEKED & 1;
}

void CGameObject::SetDestroy()								//把这个单位标记为已摧毁
{
	state = state | (1 << ON_DESTROY);
}

void CGameObject::SetSeek(bool flag)						//设置这个单位被追踪
{
	if (flag)
		state = state | (1 << BY_SEEKED);
	else
		state = state & ~(1 << BY_SEEKED);
}

void CGameObject::SetImmune()								//设置这个单位的免疫取反
{
	if (this->IsImmune())
		state = state & ~(1 << IS_IMMUNE);
	else
		state = state | (1 << IS_IMMUNE);
}

void CGameObject::SetImmune(bool flag)						//设置这个单位为免疫与否
{
	if (flag)
		state = state | (1 << IS_IMMUNE);
	else
		state = state & ~(1 << IS_IMMUNE);
}

int CGameObject::OnImpact(int type)							//所有能产生碰撞事件的都应该复写此方法
{
	throw "CGameObject::OnImpact不应该被调用！";
	return 0;
}

CRect CGameObject::GetRect()
{
	return CRect(CPoint(pos.x - GetSize().x / 2, pos.y - GetSize().y / 2),
		CPoint(pos.x + GetSize().x / 2, pos.y + GetSize().y / 2));
}

string CGameObject::IntToString(int sum)
{
	char t[32];
	string s;

	sprintf_s(t, "%d", sum);
	s = t;
	return s;
}

string CGameObject::ToString()
{
	throw "CGameObject::OnToString不应该被调用！";
	return "CGameObject::OnToString不应该被调用！";
}


int CGameObject::ImpactWith(CGameObject* pObject)
{
	int res = 0;
	if (this->GetRect().IntersectRect(this->GetRect(), pObject->GetRect()))
	{
		res += this->OnImpact(pObject->Type);
		res += pObject->OnImpact(this->Type);
	}
	return res;
}
