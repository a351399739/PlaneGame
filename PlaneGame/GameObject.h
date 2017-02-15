#pragma once
#include "atltypes.h"
#include "resource.h"
#include "string"
#include <sstream>
using namespace std;
//碰撞类型列表
#define GOODS	 			-2
#define ENEMY_PLANE			-1
#define DESTROY				0	//对于所有对象，如果碰撞类型为0，这个单位无条件销毁
#define MY_BULLET	 		1
#define ENEMY_BULLET		2
#define GEN_ENEMY_PLANE		3
#define	ELITE_ENEMY_PLANE	4
#define	MY_PLANE			5
#define BOSS				6
#define SEEKING				7
#define AIRLINER			8
#define ITEM_POWER			9
#define	ITEM_HP				10 //由于物品要求连续的几个数值，9-13被预留
#define ITEM_NUCLEAR		11
#define ITEM_PROTECT		12
#define ITEM_PEOPEL			13

//状态列表
#define ON_DESTROY	0
#define IS_IMPACT	1
#define	BY_SEEKED	2
#define IS_IMMUNE	10
// CGameObject 命令目标

class CGameObject : public CObject
{
public:
	CGameObject();
	CGameObject(CPoint pos);
	virtual ~CGameObject();
	CPoint pos;							//每个对象都有自己的的位置
	int state;							//和状态
	int Type;							//和标识符
	int xSpeed;							//和x速度
	int ySpeed;							//和y速度
/*
	0位 是否待摧毁  1位死亡方式是否为碰撞 2位是否已被追踪 10位是否为免疫状态
*/

	virtual CPoint GetSize() = 0;		//!!和获得这个对象的大小函数（必须实）
	virtual void draw(CDC* pDC) = 0;	//!!和画自己（必须实现）
	virtual CRect GetRect();			//和获得目前的矩形位置
	virtual bool IsDestroy();			//和是否被标记为待摧毁
	virtual void move();				//!!要求对象移动
	virtual bool IsOut(CRect rect);		//判断是否出界（可以重写此方法）
	virtual void SetDestroy();			//标记为待摧毁
	virtual void SetSeek(bool flag =true);	//标记为是否追踪
	virtual bool IsSeek();				//询问这个单位是否被追踪
	virtual void SetImpact();			//标记为因碰撞而摧毁
	virtual int ImpactWith(CObList* pList);		//和一个链表比较是否碰撞(不适用的类应该重写此方法)
	virtual int OnImpact(int type);		//!!与一个对象产生碰撞(不适用的类应该重写此方法)
	virtual bool IsImpact();			//死亡方式是否为碰撞
	virtual void SetImmune(bool flag);	//设置无敌
	virtual void SetImmune();			//设置无敌模式（取反）
	virtual bool IsImmune();			//是否无敌
	virtual string ToString();			//!!把这个对象转化为字符串以便保存（要保存的类必须重写此方法）
	virtual int ImpactWith(CGameObject* pObject);
	static string IntToString(int sum);	//把一个int转化为string
	static CString GetStr(int sum);			//把int数转为CString
	static CString GetStr(long sum);			//把long数转为CString
};


