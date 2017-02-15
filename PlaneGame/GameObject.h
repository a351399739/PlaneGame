#pragma once
#include "atltypes.h"
#include "resource.h"
#include "string"
#include <sstream>
using namespace std;
//��ײ�����б�
#define GOODS	 			-2
#define ENEMY_PLANE			-1
#define DESTROY				0	//�������ж��������ײ����Ϊ0�������λ����������
#define MY_BULLET	 		1
#define ENEMY_BULLET		2
#define GEN_ENEMY_PLANE		3
#define	ELITE_ENEMY_PLANE	4
#define	MY_PLANE			5
#define BOSS				6
#define SEEKING				7
#define AIRLINER			8
#define ITEM_POWER			9
#define	ITEM_HP				10 //������ƷҪ�������ļ�����ֵ��9-13��Ԥ��
#define ITEM_NUCLEAR		11
#define ITEM_PROTECT		12
#define ITEM_PEOPEL			13

//״̬�б�
#define ON_DESTROY	0
#define IS_IMPACT	1
#define	BY_SEEKED	2
#define IS_IMMUNE	10
// CGameObject ����Ŀ��

class CGameObject : public CObject
{
public:
	CGameObject();
	CGameObject(CPoint pos);
	virtual ~CGameObject();
	CPoint pos;							//ÿ���������Լ��ĵ�λ��
	int state;							//��״̬
	int Type;							//�ͱ�ʶ��
	int xSpeed;							//��x�ٶ�
	int ySpeed;							//��y�ٶ�
/*
	0λ �Ƿ���ݻ�  1λ������ʽ�Ƿ�Ϊ��ײ 2λ�Ƿ��ѱ�׷�� 10λ�Ƿ�Ϊ����״̬
*/

	virtual CPoint GetSize() = 0;		//!!�ͻ���������Ĵ�С����������ʵ��
	virtual void draw(CDC* pDC) = 0;	//!!�ͻ��Լ�������ʵ�֣�
	virtual CRect GetRect();			//�ͻ��Ŀǰ�ľ���λ��
	virtual bool IsDestroy();			//���Ƿ񱻱��Ϊ���ݻ�
	virtual void move();				//!!Ҫ������ƶ�
	virtual bool IsOut(CRect rect);		//�ж��Ƿ���磨������д�˷�����
	virtual void SetDestroy();			//���Ϊ���ݻ�
	virtual void SetSeek(bool flag =true);	//���Ϊ�Ƿ�׷��
	virtual bool IsSeek();				//ѯ�������λ�Ƿ�׷��
	virtual void SetImpact();			//���Ϊ����ײ���ݻ�
	virtual int ImpactWith(CObList* pList);		//��һ������Ƚ��Ƿ���ײ(�����õ���Ӧ����д�˷���)
	virtual int OnImpact(int type);		//!!��һ�����������ײ(�����õ���Ӧ����д�˷���)
	virtual bool IsImpact();			//������ʽ�Ƿ�Ϊ��ײ
	virtual void SetImmune(bool flag);	//�����޵�
	virtual void SetImmune();			//�����޵�ģʽ��ȡ����
	virtual bool IsImmune();			//�Ƿ��޵�
	virtual string ToString();			//!!���������ת��Ϊ�ַ����Ա㱣�棨Ҫ������������д�˷�����
	virtual int ImpactWith(CGameObject* pObject);
	static string IntToString(int sum);	//��һ��intת��Ϊstring
	static CString GetStr(int sum);			//��int��תΪCString
	static CString GetStr(long sum);			//��long��תΪCString
};


