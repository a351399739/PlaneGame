#include "stdafx.h"
#include "EnemyPlane.h"

CEnemyPlane::CEnemyPlane()
	: White(false)
{
	Type = ENEMY_PLANE;
}


CEnemyPlane::~CEnemyPlane()
{
}

int CEnemyPlane::OnImpact(int type)					//发生碰撞
{
	int score = 0;
	switch (type)
	{
	case MY_BULLET:
		score = rand() % 10 + 5; White = true;		//被我方子弹打中受到5-15点伤害
		break;
	case MY_PLANE:
		score = rand() % 20 + 30; White = true;		//遭遇我机受到30-50点伤害
		break;
	case SEEKING:
		score = rand() % 10 + 250; White = true;	//追踪弹将造成50-60点伤害
		break;
	case DESTROY:
		HP = 0;
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

string CEnemyPlane::ToString()
{
	return "error";
}
