#include "stdafx.h"
#include "Goods.h"


CGoods::CGoods()
{
	Type = GOODS;
}


CGoods::~CGoods()
{
}


//int CGoods::OnImpact(int type)					//�ӵ�������ײ������ǰ��Լ����Ϊ���ݻ�,������ʲô�ӵ�
//{
//	if ((this->Type == ENEMY_BULLET && type == MY_PLANE) ||
//		this->Type == MY_BULLET && type != MY_PLANE ||
//		!type)
//	SetDestroy();
//	return 0;
//}


string CGoods::ToString()
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
	return a;
}
