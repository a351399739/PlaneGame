// GameObject.cpp : ʵ���ļ�
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


// CGameObject ��Ա����
CString CGameObject::GetStr(int sum)//����ȡ��int��������CString��ʾ������
{
	CString str;
	str.Format(_T("%d"), sum);
	return str;
}

CString CGameObject::GetStr(long sum)//����ȡ��long��������CString��ʾ������
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

bool CGameObject::IsDestroy()			//��ѯ�������λ�Ƿ��Ѿ������Ϊ����
{
	return state >> ON_DESTROY & 1;
}


bool CGameObject::IsOut(CRect rect)		//��ѯ�������λ�Ƿ����
{
	return pos.y + GetSize().y < 0 || pos.x + GetSize().x < 0
		|| pos.y - GetSize().y > rect.bottom || pos.x - GetSize().x > rect.right;
}


void CGameObject::SetImpact()								//�������λ���Ϊ������ײ���ݻ�
{
	state = state | (1 << IS_IMPACT);
}

int CGameObject::ImpactWith(CObList* pList)					//һ����λǰ���һ�������Ƿ������ײ
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


bool CGameObject::IsImpact()								//�����λ�Ƿ���Ѫ�����Ͷ�������
{
	return state >> IS_IMPACT & 1;
}

bool CGameObject::IsImmune()								//ѯ�������λ�Ƿ�������
{
	return state >> IS_IMMUNE & 1;
}

bool CGameObject::IsSeek()								//ѯ�������λ�Ƿ��Ѿ���׷��
{
	return state >> BY_SEEKED & 1;
}

void CGameObject::SetDestroy()								//�������λ���Ϊ�Ѵݻ�
{
	state = state | (1 << ON_DESTROY);
}

void CGameObject::SetSeek(bool flag)						//���������λ��׷��
{
	if (flag)
		state = state | (1 << BY_SEEKED);
	else
		state = state & ~(1 << BY_SEEKED);
}

void CGameObject::SetImmune()								//���������λ������ȡ��
{
	if (this->IsImmune())
		state = state & ~(1 << IS_IMMUNE);
	else
		state = state | (1 << IS_IMMUNE);
}

void CGameObject::SetImmune(bool flag)						//���������λΪ�������
{
	if (flag)
		state = state | (1 << IS_IMMUNE);
	else
		state = state & ~(1 << IS_IMMUNE);
}

int CGameObject::OnImpact(int type)							//�����ܲ�����ײ�¼��Ķ�Ӧ�ø�д�˷���
{
	throw "CGameObject::OnImpact��Ӧ�ñ����ã�";
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
	throw "CGameObject::OnToString��Ӧ�ñ����ã�";
	return "CGameObject::OnToString��Ӧ�ñ����ã�";
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
