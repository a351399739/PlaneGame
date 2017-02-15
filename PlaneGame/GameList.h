#pragma once
#include "afxcoll.h"
#include "Item.h"
#include "Boom.h"
#include "Seeking.h"
class CGameList :
	public CObList
{
public:
	CGameList();
	virtual ~CGameList();

	static int oAppearItem;
	static int oJump;

	virtual void Draw(CDC* pDC);
	virtual void DeleteAll(bool flag = true);
	virtual void move( CRect rect,bool flag = true);
	virtual void DeleteDestroy(CGameList* pBoomList = NULL,CGameList* pGoodsList = NULL);
	virtual void ImpactWith(CGameList* pList, int* pScore = 0);
	virtual string ToString();
	virtual void SetDestroy();
	virtual void Work(bool flag);
};

