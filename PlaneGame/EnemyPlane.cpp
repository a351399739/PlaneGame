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

int CEnemyPlane::OnImpact(int type)					//������ײ
{
	int score = 0;
	switch (type)
	{
	case MY_BULLET:
		score = rand() % 10 + 5; White = true;		//���ҷ��ӵ������ܵ�5-15���˺�
		break;
	case MY_PLANE:
		score = rand() % 20 + 30; White = true;		//�����һ��ܵ�30-50���˺�
		break;
	case SEEKING:
		score = rand() % 10 + 250; White = true;	//׷�ٵ������50-60���˺�
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
		SetImpact();								//�����ַ�ʽ�����Ϊ���ݻٵĻ���Ҳ�ᱻ���Ϊ��ײʱ�ݻ�
	}
	return score;
}

string CEnemyPlane::ToString()
{
	return "error";
}
