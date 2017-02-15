#include "stdafx.h"
#include "Seeking.h"

CImageList* CSeeking::imglist = new CImageList();
CPoint  CSeeking::size = CPoint(20, 20);
CBitmap* CSeeking::bmpDraw = new CBitmap();
CDC*  CSeeking::memDC = new CDC();
CObList* CSeeking::pEnemyList = NULL;
CRITICAL_SECTION* CSeeking::cs = new CRITICAL_SECTION();
CSeeking::CSeeking()
	: index(0)
{
}


CSeeking::~CSeeking()
{
	pTarget = NULL;
}

CSeeking::CSeeking(CPoint pos)
{
	pTarget = NULL;
	this->pos = pos;
	Type = SEEKING;
	Catch();
}


void CSeeking::Loadimg()
{
	imglist->Create(20, 20, ILC_COLOR24 | ILC_MASK, 1, 1);
	CBitmap bitmap;
	bitmap.LoadBitmapW(IDB_SEEKING);
	imglist->Add(&bitmap, RGB(0, 0, 0));
	//TRACE("bool CMyPlane::LoadImmune()%d\n",imglist->GetImageCount());
	return;
}

void CSeeking::draw(CDC* pDC)
{
	imglist->Draw(pDC, index, CPoint(pos.x - size.x, pos.y - 40), NULL);
	index++;
	if (index >= 13)index = 0;
}

int CSeeking::OnImpact(int type)
{
	switch (type)
	{
	case GEN_ENEMY_PLANE:case ELITE_ENEMY_PLANE:case BOSS:
	{
		SetDestroy();
		if (pTarget != NULL)
		{
			pTarget = NULL;
		}
		break;
	}
	default:
		break;
	}
	return 0;
}

CPoint CSeeking::GetSize()
{
	return size;
}



void CSeeking::Catch()
{
	//EnterCriticalSection(cs);
	CGameObject* temp;
	if (pTarget == NULL)//-------------------------------------------------如果没有目标，则试图寻找目标
	{
		for (POSITION pos = pEnemyList->GetHeadPosition(); pos != NULL;)//找到的话就记录并标记
		{
			temp = (CGameObject*)pEnemyList->GetNext(pos);
			if (!(temp->IsSeek()) && !(temp->IsDestroy()))
			{
				temp->SetSeek();
				pTarget = (CEnemyPlane*)temp;
				break;
			}
		}
		if (pTarget == NULL)											//找不到就以缓慢的速度往前走
		{
			xSpeed = 0;
			ySpeed = -1;
		}
	}
	else//-------------------------------------------------------------------------------已有目标则追踪
	{
		//TRACE("void CSeeking::Catch()%d %d\n", pTarget->pos.x, pTarget->pos.y);
		if (pTarget->IsDestroy())
		{
			pTarget = NULL;
			return;
		}

		if (abs(pos.x-pTarget->pos.x) < 15)				//几乎如果在一垂直线上
		{
			this->xSpeed = 0;
			this->ySpeed = (pos.y > pTarget->pos.y) ? -7 : 7;
		}
		else if (abs(pos.y - pTarget->pos.y) < 15)		//如果几乎在一水平线上
		{
			this->ySpeed = 0;
			this->xSpeed = (pos.y > pTarget->pos.y) ? -5 : 5;
		}
		else if (abs(abs(pos.x - pTarget->pos.x) > abs(pos.y - pTarget->pos.y)) < 15)
		{
			this->xSpeed = (pos.x > pTarget->pos.x) ? -4 : 4;
			this->ySpeed = (pos.y > pTarget->pos.y) ? -4 : 4;
		}
		else									//如果不在一直线上
		{
			if (abs(pos.x - pTarget->pos.x) > abs(pos.y - pTarget->pos.y))//x偏移大
			{
				//this->ySpeed = (pos.y > pTarget->pos.y) ? -4 : 4;
	/*			this->xSpeed = ((pos.x > pTarget->pos.x) ? -1 : 1) *
					4 * (pos.x - pTarget->pos.x) / (pos.y - pTarget->pos.y);*/
				this->ySpeed = (pos.y > pTarget->pos.y) ? -4 : 4;
				this->xSpeed = 0;
			}
			else											//y偏移大			
			{
				//this->xSpeed = (pos.x > pTarget->pos.x) ? -4 : 4;
				//this->ySpeed = ((pos.y > pTarget->pos.y) ? -1 : 1) *
				//	4 * (pos.y - pTarget->pos.y) / (pos.x - pTarget->pos.x);
				this->xSpeed = (pos.x > pTarget->pos.x) ? -4 : 4;
				this->ySpeed = 0;
			}

		}
		if (this->xSpeed > 15)this->xSpeed = 15;			//过大不会调整（测试）//测试结果不可行:(
		else if (this->xSpeed < -15)this->xSpeed = -15;
		if (this->ySpeed > 15)this->ySpeed = 15;
		else if (this->ySpeed < -15) this->ySpeed = -15;
	}
	//LeaveCriticalSection(cs);
	return;
}
