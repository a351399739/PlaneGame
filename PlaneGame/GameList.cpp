#include "stdafx.h"
#include "GameList.h"

int CGameList::oAppearItem = 10;
int CGameList::oJump = 1;
CGameList::CGameList()
{
}


CGameList::~CGameList()
{
}


void CGameList::Draw(CDC* pDC)
{
	for (POSITION pos = GetHeadPosition(); pos != NULL;)
	{
		CGameObject* temp = (CGameObject*)GetNext(pos);
		temp->draw(pDC);
	}
}


void CGameList::DeleteAll(bool flag)
{
	for (POSITION pos = GetHeadPosition(); pos != NULL;)//ɾ��ÿ��Ԫ��
	{
		CGameObject* temp = (CGameObject*)GetNext(pos);
		delete temp;
		temp = NULL;
	}
	RemoveAll();
	if (flag)
	{
		delete this;									//ɾ������
	}
}


void CGameList::move(CRect rect,bool flag)
{
	for (POSITION pos = GetHeadPosition(); pos != NULL;)
	{
		CGameObject* temp = (CGameObject*)GetNext(pos);
		temp->move();
		if (flag && temp->IsOut(rect))
		{
			temp->SetDestroy();
		}
	}
}

//�����Ĵ�����ȡ��
//void CGameList::DeleteDestroy()
//{
//	POSITION pos1, pos2;
//	for (pos1 = GetHeadPosition(); (pos2 = pos1) != NULL;)//��BOSS���о��ƶ����Ƴ�
//	{
//		CGameObject* temp = (CGameObject*)GetNext(pos1);
//		//TRACE("���ԣ�%d\n", temp->pos.y + temp->GetSize().y);
//		if (temp->IsDestroy())
//		{
//			RemoveAt(pos2);
//			delete temp;
//		}
//	}
//}

void CGameList::DeleteDestroy(CGameList* pBoomList,CGameList *pGoodsList)
{
	POSITION pos1, pos2;
	for (pos1 = GetHeadPosition(); (pos2 = pos1) != NULL;)
	{
		CGameObject* temp = (CGameObject*)GetNext(pos1);
		//TRACE("���ԣ�%d\n", temp->pos.y + temp->GetSize().y);
		if (temp->IsDestroy())										//�ڴ�������Ʒ����ʱ������н�ɫ��������һ�����ʲ�����Ʒ
		{
			if (pGoodsList!= NULL && (rand() % 100) < oAppearItem && temp->IsImpact())
			{
				CItem* Ttemp = new CItem(temp->pos);
				pGoodsList->AddTail(Ttemp);
			}

			RemoveAt(pos2);
			if (pBoomList != NULL && temp->IsImpact())				//�ڴ����˱�ը����ʱ������н�ɫ�����ᷢ����ը
			switch (temp->Type)
			{
			case AIRLINER:					//��Щ���Ӧ��д��ÿһ�������
			{
				for (int i = (temp->pos.x); i <= (temp->pos.x + 300); i += 60)
					for (int j = (temp->pos.y); j <= (temp->pos.y + 150); j += 66)
					{
						CGameObject* Btemp = new CBoom(CPoint(i, j));
						pBoomList->AddTail(Btemp);
					}
			break;
			}

			case ELITE_ENEMY_PLANE:
			{
				for (int i = (temp->pos.x - 70); i <= (temp->pos.x + 70); i += 70)
				{
					CGameObject* Btemp = new CBoom(CPoint(i, temp->pos.y));
					pBoomList->AddTail(Btemp);
				}
				break;
			}

			default:
				CGameObject* Btemp = new CBoom(temp->pos);
				pBoomList->AddTail(Btemp);
				break;
			}
			else
			{
				if (temp->Type == AIRLINER && pGoodsList != NULL)
				{
					CItem* Ttemp = new CItem(temp->pos);
					pGoodsList->AddTail(Ttemp);
				}
			}
			delete temp;
			temp = NULL;
		}
		else
		{
			if (pGoodsList != NULL  && temp->Type == AIRLINER && (rand() % 100 < oJump))
			{
				CItem* Ttemp = new CItem(temp->pos, ITEM_PEOPEL);
				pGoodsList->AddTail(Ttemp);
			}
		}
	}
}

void CGameList::ImpactWith(CGameList* pList,int* pScore)
{
	for (POSITION pos = GetHeadPosition(); pos != NULL;)
	{
		CGameObject* temp = (CGameObject*)GetNext(pos);
		if (pScore != NULL)
			*pScore += temp->ImpactWith(pList);
	}
}


string CGameList::ToString()
{
	string a;
	for (POSITION pos = GetHeadPosition(); pos != NULL;)
	{
		CGameObject* temp = (CGameObject*)GetNext(pos);
		a += temp->ToString();
	}
	return a;
}


void CGameList::SetDestroy()
{
	for (POSITION pos = GetHeadPosition(); pos != NULL;)
	{
		CGameObject* temp = (CGameObject*)GetNext(pos);
		temp->SetDestroy();
		temp->SetImpact();
	}
}


void CGameList::Work(bool flag)
{
	for (POSITION pos = GetHeadPosition(); pos != NULL;)
	{
		CGameObject* temp = (CGameObject*)GetNext(pos);
		if (temp->Type == SEEKING)
		{
			if (flag)
				temp->SetDestroy();
			else
				((CSeeking*)temp)->Catch();
		}
		
	}
}
