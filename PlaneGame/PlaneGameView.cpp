
// PlaneGameView.cpp : CPlaneGameView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "PlaneGame.h"
#endif

#include "PlaneGameDoc.h"
#include "PlaneGameView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//�Ҷ���ļ�ʱ��

#define SYSTEM_TIMER			1
#define GENENEMY_APPEAR			2
#define MY_BULLET_APPEAR		3
#define ENEMY_BULLET_APPEAR		4
#define BOOM_SPEED				5
#define	SECOND					6
// CPlaneGameView

IMPLEMENT_DYNCREATE(CPlaneGameView, CView)

BEGIN_MESSAGE_MAP(CPlaneGameView, CView)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_PAUSE, &CPlaneGameView::OnPause)
	ON_COMMAND(ID_SETLEVEL, &CPlaneGameView::OnSetlevel)
	ON_COMMAND(ID_DEBUG, &CPlaneGameView::OnDebug)
	ON_UPDATE_COMMAND_UI(ID_DEBUG, &CPlaneGameView::OnUpdateDebug)
	ON_COMMAND(ID_IMMUNE, &CPlaneGameView::OnImmune)
	ON_UPDATE_COMMAND_UI(ID_IMMUNE, &CPlaneGameView::OnUpdateImmune)
	ON_COMMAND(ID_RESTART, &CPlaneGameView::OnRestart)
	ON_COMMAND(ID_SAVE, &CPlaneGameView::OnSave)
	ON_COMMAND(ID_READ, &CPlaneGameView::OnRead)
	ON_WM_KEYUP()
	ON_COMMAND(ID_SET, &CPlaneGameView::OnSet)
	ON_COMMAND(ID_RAND, &CPlaneGameView::OnRand)
	ON_COMMAND(ID_32800, &CPlaneGameView::On32800)
END_MESSAGE_MAP()

// CPlaneGameView ����/����


void CPlaneGameView::LoadBackimg()
{
	Background.LoadBitmapW(IDB_BACKGROUND);	
	BackDC = new CDC();
	BackDC->CreateCompatibleDC(NULL);
	BackDC->SelectObject(&Background);
}


CPlaneGameView::CPlaneGameView()
	: level(1)
	, pause(false)
	, debug(false)
	, BackgroundPos(0)
	, Time(0)
	, MaxEnemy(15)
	, sEliteChance(10)
	, increELiteChance(1)
	, sBossTimer(60)
	, increBossTimer(5)
	, oAppearAir(0)
	, Start(0)
{
	//����Ϊ�����쳣�Ƿ����׳�
	//CBoom test;					//���CGameObject::OnImpact()����
	//test.OnImpact(0);

	//����Ϊ����theString�������
	//TheString = "";

	// TODO:  �ڴ˴���ӹ������
	srand((unsigned)time(NULL));
	MemDC.CreateCompatibleDC(NULL);
	TipFont.CreatePointFont(100, _T("����"));
	PauseFont.CreatePointFont(400, _T("����"));
	MemDC.SelectObject(TipFont);
	MemDC.SetBkMode(TRANSPARENT);
	pBoss = NULL;
	pMe = NULL;
	BoomPos = CPoint(-200,-200);
	oAppearAir = 1;
	CBoom::Loadimg();
	CPlaneGameView::LoadBackimg();
	CEnemyBullet::Loadimg();
	CMyBullet::Loadimg();
	CGenEnemy::Loadimg();
	CEliteEnemy::Loadimg();
	CAirLiner::Loadimg();
	CItem::Loadimg();
	CMyPlane::Loadimg();
	CSeeking::Loadimg();
	InitializeCriticalSection(CSeeking::cs);
}

CPlaneGameView::~CPlaneGameView()
{
}

BOOL CPlaneGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

/*--------------------------------------------��ͼ���ֿ�ʼ-----------------------------------------------------*/




void CPlaneGameView::DrawBackground(CDC* pDC)
{
	pDC->TransparentBlt(0, BackgroundPos - 2880,
		2000, 2880, BackDC, 0, 0, 2000, 2880,NULL);//������һ����
	pDC->TransparentBlt(0, BackgroundPos,
		2000, 2880, BackDC, 0, 0, 2000, 2880, NULL);//������һ����
}


void CPlaneGameView::TextDraw(CDC* pDC,bool flag)			//��ʾ��������
{
	int index = -15;
	pDC->SetTextColor(RGB(255, 255, 0));
	if (!Start)
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SelectObject(PauseFont);
		pDC->TextOutW(rect.right / 2 - 100, rect.bottom / 2 - 40, L"��Y����ʼ");
		pDC->SelectObject(TipFont);
		return;
	}
	//debugģʽ����Ҫ��ʾ��
	if (debug)									
	{
		pDC->TextOutW(0, index += 15, L"DEBUGģʽ�ѿ���...");
		pDC->TextOutW(0, index += 15, L"��ǰ�����ео�������" + CGameObject::GetStr(pEnemyList->GetCount()));
		pDC->TextOutW(0, index += 15, L"��ǰ�����пͻ�������" + CGameObject::GetStr(pAirLinerList->GetCount()));
		pDC->TextOutW(0, index += 15, L"��ǰ��������Ʒ������" + CGameObject::GetStr(pGoodsList->GetCount()));
		pDC->TextOutW(0, index += 15, L"��ǰ�����б�ըЧ��������" + CGameObject::GetStr(pBoomList->GetCount()));
		if (pBoss)pDC->TextOutW(0, index += 15, L"BOSSѪ��" + CGameObject::GetStr(pBoss->HP));
		index += 15;
	}

	//�κ��������Ҫ��ʾ��
	if (pMe != NULL)
	{
		pDC->TextOutW(0, index += 15, L"��ǰ�һ�HP��" + CGameObject::GetStr(pMe->HP));
		pDC->TextOutW(0, index += 15, L"��ǰ�һ��ӵ�������" + CGameObject::GetStr(pMe->power));
		pDC->TextOutW(0, index += 15, L"��ǰ�һ�������ʱ�䣺" + CGameObject::GetStr(pMe->ProtectTime));
		if (pMe->IsImmune())
			pDC->TextOutW(0, index += 15, L"��ǰ��������״̬");
	}
	
	pDC->TextOutW(0, index += 15, L"��ǰ�ؿ���" + CGameObject::GetStr(level));
	pDC->TextOutW(0, index += 15, L"��ǰ�ؿ���ʱ����" + CGameObject::GetStr(Time));
	pDC->TextOutW(0, index += 15, L"��ǰ������������" + CGameObject::GetStr(pMe->nuclear));
	pDC->TextOutW(0, index += 15, L"��ǰ���֣�" + CGameObject::GetStr(*pScore));


	if (pause)									//��ͣʱ��Ҫ��ʾ��
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SelectObject(PauseFont);
		pDC->TextOutW(rect.right / 2 - 50, rect.bottom / 2 - 30, L"��ͣ");
		pDC->SelectObject(TipFont);
	}
	
}


void CPlaneGameView::UpdateFrame()								//���´���
{
	CDC* pDC = GetDC();

	CPlaneGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

//	MemDC.FillSolidRect(0, 0, rect.right, rect.bottom, RGB(0, 0, 0));//��������

	DrawBackground(&MemDC);

	if (Start)
	{
		if (pMe != NULL&&!pMe->IsDestroy())					//����ҷ�����û�б����Ϊ���ݻ٣������ҷ�����
			pMe->draw(&MemDC);
		if (pBoss != NULL&&!pBoss->IsDestroy())
			pBoss->draw(&MemDC);


		pEnemyList->Draw(&MemDC);				//����ÿ������
		pAirLinerList->Draw(&MemDC);
		pGoodsList->Draw(&MemDC);
		pBoomList->Draw(&MemDC);

		if (pBoss != NULL && BoomPos.x != -200 && BoomPos.y != -200)
		{
			CDC tempDC;
			tempDC.CreateCompatibleDC(NULL);
			CBitmap tempBit;
			tempBit.LoadBitmap(IDB_BOOMAREA);
			tempDC.SelectObject(tempBit);
			MemDC.TransparentBlt(BoomPos.x - 75, BoomPos.y - 100,
				150, 200, &tempDC, 0, 0, 150, 200, RGB(255, 255, 255));
		}
	}

	TextDraw(&MemDC,Start);						//�����ı�

	pDC->BitBlt(rect.left, rect.top, rect.right, rect.bottom, &MemDC, 0, 0, SRCCOPY);
	ReleaseDC(pDC);
}


// CPlaneGameView ����
void CPlaneGameView::OnDraw(CDC* pDC)
{
	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}

/*--------------------------------------------��ͼ���ֽ���---------------------------------------------*/
// CPlaneGameView ���

#ifdef _DEBUG
void CPlaneGameView::AssertValid() const
{
	CView::AssertValid();
}

void CPlaneGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlaneGameDoc* CPlaneGameView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlaneGameDoc)));
	return (CPlaneGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlaneGameView ��Ϣ�������


void CPlaneGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	
	CDC* pDC = GetDC();
	GetClientRect(&rect);
	if (MemBitmap.m_hObject != NULL)
		MemBitmap.DeleteObject();
	MemBitmap.CreateCompatibleBitmap(pDC, rect.right, rect.bottom);
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
	ReleaseDC(pDC);
	GameIni();
	pMe->HP = 99999999;
	//OnPause();
}


void CPlaneGameView::GameIni()
{
	Time = 0;													//ʱ��
	pMe = new CMyPlane(CPoint((rect.right - CMyPlane::size.x) / 2 , rect.bottom - 100));//��ʼ���һ�
	pScore = new int(0);										//��ʼ������
	pEnemyList = new CGameList();								//��ʼ���о���BOSS������
	CSeeking::pEnemyList = this->pEnemyList;
	pGoodsList = new CGameList();								//��ʼ����Ʒ����
	pBoomList = new CGameList();								//��ʼ����ը����
	pAirLinerList = new CGameList();							//��ʼ���ͻ�����
	SetAllTimer();												//���ö�ʱ��
	BoomPos = CPoint(-200, -200);
	pause = 0;
}


void CPlaneGameView::GameEnd()
{
	delete pMe;//ɾ���Լ�
	pMe = NULL;
	if (pBoss != NULL)
	{
		delete pBoss;
		pBoss = NULL;
	}
	Time = 0;
	delete pScore;//ɾ������
	level = 1;
	pEnemyList->DeleteAll();//ɾ�����������е�Ԫ�غ����Լ�
	pGoodsList->DeleteAll();
	pBoomList->DeleteAll();
	pAirLinerList->DeleteAll();
	
	KillAllTimer();
}


void CPlaneGameView::OnDestroy()
{
	CView::OnDestroy();

	GameEnd();
	delete BackDC;
	delete CBoom::imglist;
	delete CAirLiner::bmpDraw;
	delete CAirLiner::memDC;
	delete CEliteEnemy::bmpDraw;
	delete CEliteEnemy::bmpDraw2;
	delete CEliteEnemy::memDC;
	delete CEnemyBullet::bmpDraw;
	delete CEnemyBullet::memDC;
	delete CGenEnemy::bmpDraw;
	delete CGenEnemy::memDC;
	delete CGenEnemy::bmpDraw2;
	delete CMyBullet::bmpDraw;
	delete CMyBullet::memDC;
	delete CItem::memDC;
	delete CItem::bmpDraw;
	delete CMyPlane::imglist;
	delete CSeeking::memDC;
	delete CSeeking::bmpDraw;
	delete CSeeking::imglist;
	delete CSeeking::cs;
	

	// TODO:  �ڴ˴������Ϣ����������
}



//------------------------------------��ʱ���ɴ˿�ʼ-------------------------------------------------//
void CPlaneGameView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
/*-------------------------------ϵͳ��ʱ�����𣺳��ӵ��ⰴ����⣬�о��ƶ�,��Ļˢ�¡�----------------------------*/
	case SYSTEM_TIMER:
	{
		if (GetKey(VK_DOWN) && pMe->pos.y <= rect.bottom)							//������ⲿ��
			pMe->pos.y += pMe->ySpeed;
		if (GetKey(VK_UP) && pMe->pos.y >= 0)
			pMe->pos.y -= pMe->ySpeed;
		if (GetKey(VK_LEFT) && pMe->pos.x >= 0)
			pMe->pos.x -= pMe->xSpeed;
		if (GetKey(VK_RIGHT) && pMe->pos.x <= rect.right)
			pMe->pos.x += pMe->xSpeed;

		if (BackgroundPos == 2880)
			BackgroundPos = 0;
		BackgroundPos += 1;

		if (pBoss)pBoss->move();
		pEnemyList->move(rect);					//�о��ƶ�
		pGoodsList->move(rect);					//��Ʒ�ƶ�
		pAirLinerList->move(rect);

		pGoodsList->ImpactWith(pEnemyList, pScore);				//��Ʒ�͵о��ɻ�������ײ�ж�
		pAirLinerList->ImpactWith(pGoodsList, pScore);			//��Ʒ��ͻ����������ײ�ж�
		*pScore += (pMe->ImpactWith(pEnemyList));				//�Һ͵о��ɻ�������ײ�ж�
		*pScore += (pMe->ImpactWith(pGoodsList));				//�Һ���Ʒ������ײ�ж�
		if (*pScore < 0)*pScore = 0; 
		if (pBoss)
		{
			*pScore += (pBoss->ImpactWith(pGoodsList));		//boss����Ʒ������ײ�ж�
			*pScore += pMe->ImpactWith(pBoss);
		}

		//����������֮ǰ��Ҫ����
		if (pEnemyList != NULL)
			CSeeking::pEnemyList = this->pEnemyList;			//����׷���ӵ��е���Ϣ
		pGoodsList->Work(pBoss!=NULL);
		//������������������Ѵ��ݻٶ���ݻ�
		pEnemyList->DeleteDestroy(pBoomList,pGoodsList);					//�л��ͻ��б�ը��Ч
		pAirLinerList->DeleteDestroy(pBoomList, pGoodsList);
		pGoodsList->DeleteDestroy();
		pBoomList->DeleteDestroy();
		

		
		if (pMe->IsDestroy())									//�һ��������򵯳��Ի���
		{
			OnPause();
			UINT restart = MessageBox(L"����ˣ�����һ�ΰɣ�", L"��������", MB_YESNO);
			if (restart == IDNO)
			{
				/*AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE);
				OnPause();*/
				exit(0);
			}
			else
			{
				GameEnd();
				GameIni();
			}
		}
		else if (pBoss != NULL && pBoss->IsDestroy())			//�����ж�boss�Ƿ�����
		{
			OnPause();
			MessageBox(L"��ϲ����������Enter��ʼ��һ��", L"���أ�");
			delete pBoss;
			pBoss = NULL;
			BoomPos = CPoint(-200, -200);
			*pScore += 1500 + level * 100;
			LevelChange(level + 1);
			OnPause();
		}
		this->UpdateFrame();									//ÿ30���붼���ػ�
		break;
	}


/*--------------------------------------���˲��������𣺲����л��Ϳͻ�-------------------------------------------*/
	case GENENEMY_APPEAR:
	{
		if (pEnemyList->GetCount() >= MaxEnemy || pBoss != NULL)break;//���˵�������� /*������15 Ӧ������һ����������������*/
		GetClientRect(rect);
		CEnemyPlane* temp;
		if (rand() % 100 < oAppearAir)					//�ͻ���С�ļ��ʳ���
		{
			CAirLiner* Atemp = new CAirLiner(rect);
			pAirLinerList->AddTail(Atemp);
		}
		if (rand() % 100<= (sEliteChance + level * increELiteChance))		//��������ϵȼ�����������90�ͻ������Ӣ�ɻ�
		{
			temp = new CEliteEnemy(rect);
		}
		else
		{
			temp = new CGenEnemy(rect);
		}
		pEnemyList->AddTail(temp);
		break;
	}

/*-----------------------------------------�Ҿ��ӵ��������������Ҿ��ӵ�----------------------------------------------*/
	case MY_BULLET_APPEAR:
	{
		if (GetKey(VK_SPACE))
		{
			pMe->shot(pGoodsList);
		}
		break;
	}


/*-------------------------------------------�о��ӵ��������������о��ӵ�------------------------------------------*/
	case ENEMY_BULLET_APPEAR:
	{
		if (pGoodsList->GetCount() > 100)break;
		if (pBoss)
			pBoss->shot(pGoodsList, pMe->pos, level);
		for (POSITION pos = pEnemyList->GetHeadPosition(); pos != NULL;)//�о������ӵ�...���ܼ����������....
		{
			CEnemyPlane* EPtemp = (CEnemyPlane*)pEnemyList->GetNext(pos);
			EPtemp->shot(pGoodsList, pMe->pos, level);
		}
		break;
	}
/*-------------------------------------------��ը�ٶȿ����������Ʊ�ը�ٶ�------------------------------------------*/
	case BOOM_SPEED:
	{
		pBoomList->move(rect,false);
		break;
	}

/*-------------------------------------------ȫ��ʱ����Ƹ��𣺴���boss����,�ҷ�����ʣ��ʱ��------------------------------------------*/

	case SECOND:
	{
		Time += 1;
		if (pMe != NULL && pMe->ProtectTime > 0)
		{
			pMe->ProtectTime--;
		}
		if (pBoss==NULL&& Time > (sBossTimer + level * increBossTimer))
		{
			pBoss = new CBoss(level, rect);
		}
		if (pBoss != NULL&& pMe != NULL && !(pMe->IsDestroy()) && !(pBoss->IsDestroy()))
		{
			if(Time % 10 == 0)
				BoomPos = pMe->pos;
			else if ((Time - 10) % 3 == 0)
			{
				CRect BoomRect = CRect(CPoint(BoomPos.x - 75, BoomPos.y - 100), CPoint(BoomPos.x + 75, BoomPos.y + 100));
				if (BoomRect.IntersectRect(BoomRect, pMe->GetRect()))
				{
					if (!(pMe->IsImmune()))
					pMe->SetDestroy();
				}
				for (int i = BoomPos.x - 75; i < BoomPos.x + 75; i += 50)
					for (int j = BoomPos.y - 100; j < BoomPos.y + 100; j += 50)
					{
					CGameObject* Btemp = new CBoom(CPoint(i, j));
					pBoomList->AddTail(Btemp);
					}
				BoomPos = CPoint(-200, -200);
			}
		}

	}
	}
	CView::OnTimer(nIDEvent);
}

//--------------------------------------------��ʱ�����˽���------------------------------------------

bool CPlaneGameView::GetKey(int Key)
{
	return (GetKeyState(Key) & 0x8000) ? true : false;
}


BOOL CPlaneGameView::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return false;
}


void CPlaneGameView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CDC* pDC = GetDC();
	GetClientRect(&rect);
	if (MemBitmap.m_hObject != NULL)
		MemBitmap.DeleteObject();
	MemBitmap.CreateCompatibleBitmap(pDC, rect.right, rect.bottom);
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);

	
	if (pMe != NULL && rect.right !=0  &&rect.bottom != 0 && pMe->IsOut(rect))
			pMe->pos = CPoint(rect.right / 2, rect.bottom - 100);
	if (pBoss != NULL)pBoss->rect = rect;
	CItem::rect = rect;
	ReleaseDC(pDC);
	// TODO:  �ڴ˴������Ϣ����������
}


void CPlaneGameView::SetAllTimer()
{
	SetTimer(SYSTEM_TIMER, 30, NULL);							//ϵͳ��ʱ��
	SetTimer(GENENEMY_APPEAR, 1500 - level * 30, NULL);			//�о�ˢ�¼�ʱ��
	SetTimer(MY_BULLET_APPEAR, 100, NULL);						//�Ҿ��ӵ���ʱ��
	SetTimer(ENEMY_BULLET_APPEAR, 1000, NULL);					//�о��ӵ���ʱ��
	SetTimer(BOOM_SPEED, 50, NULL);								//ը���ٶ�
	SetTimer(SECOND, 1000, NULL);								//ÿ���ؿ����ͼ�ʱ�����һ���ֻ�
}


void CPlaneGameView::KillAllTimer()
{
	KillTimer(SYSTEM_TIMER);									//ɾ��ÿ����ʱ��
	KillTimer(GENENEMY_APPEAR);
	KillTimer(MY_BULLET_APPEAR);
	KillTimer(ENEMY_BULLET_APPEAR);
	KillTimer(BOOM_SPEED);
	KillTimer(SECOND);
}


void CPlaneGameView::OnPause()									//��ͣ�Ļ����򵥴ֱ��İ����ж�ʱ����ɱ����
{
	if (!pause)
		KillAllTimer();
	else
		SetAllTimer();
	UpdateFrame();
	pause = !pause;
	this->UpdateFrame();
}

void CPlaneGameView::OnSetlevel()								//���û��Զ���ͣ
{
	pause = 0;
	OnPause();
	CLevelDlg d;
	d.eLevel = level;
	if (pMe != NULL)
	{
		d.HP = pMe->HP;
		d.nuclear = pMe->nuclear;
		d.power = pMe->power;
	}
	if (d.DoModal() == IDOK)
	{
		LevelChange(d.eLevel);
		if (pMe != NULL)
		{
			pMe->HP = d.HP;
			pMe->nuclear = d.nuclear;
			pMe ->power = d.power;
		}
	}
	OnPause();
}


void CPlaneGameView::OnDebug()
{
	debug = !debug;
}


void CPlaneGameView::OnUpdateDebug(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(debug);
}

void CPlaneGameView::OnImmune()
{
	if (pMe != NULL)
		pMe->SetImmune();
}

void CPlaneGameView::OnUpdateImmune(CCmdUI *pCmdUI)
{
	if (pMe != NULL)
		pCmdUI->SetCheck(pMe->IsImmune() ? 1 : 0);
}


void CPlaneGameView::OnRestart()
{
	if(!pause)OnPause();
	if (MessageBox(TEXT("δ����Ľ��Ƚ���ʧ\n\r���¿�ʼ��"), TEXT("���¿�ʼ!"), 4) == IDYES)
	{
		OnPause();
		GameEnd();
		GameIni();
	}
	else
		OnPause();
}


//--------------------------------------�����ȡ-------------------------------------------------------//

string CPlaneGameView::IntToString(int sum)
{
	char t[32];
	string s;
	sprintf_s(t, "%d", sum);
	s = t;
	return s;
}

string CPlaneGameView::IntToString(long sum)
{
	int a = (int)sum;
	TRACE("%ld\n", sum);
	string s;
	char t[32];
	sprintf_s(t, "%d", a);
	s = t;
	return s;
}

void CPlaneGameView::OnSave()
{
	pause = 0;
	OnPause();
	if (pMe == NULL || pMe->IsDestroy() || pBoss != NULL)//�һ�δ���ɻ��Ǳ��ݻ�ʱ�����ɱ���
	{
		MessageBox(L"BOSSû����������Ϸû��ʼʱ�������棡", L"����");
	}
	else if (MessageBox(TEXT("������ԭ���Ĵ浵\n\rȷ����"), TEXT("����..."), 4) == IDYES)
		WriteToFile();
	OnPause();
}


void CPlaneGameView::OnRead()
{
	pause = 0;
	OnPause();
	if (MessageBox(TEXT("δ����Ľ��Ƚ���ʧ\n\r��ȡ��"), TEXT("��ȡ..."), 4) == IDYES)
		ReadFromFile();
	OnPause();
}


void CPlaneGameView::WriteToFile()		//��Ӧ�ñ����������String��ʽ����������
{
	
	string line = "";					//��ʼ��
	ofstream out("Game.data");

	line = debug ? "1" : "0";			//��һ������Ƿ����ģʽ
	//TRACE("%s\n",line.data());
	out << line << endl;

	line = IntToString(level);			//�ڶ������Ϊ�ؿ�
	//TRACE("%s\n",line.data());
	out << line << endl;

	line = IntToString(*pScore);		//���������Ϊ����
	//TRACE("%s\n",line.data());
	out << line << endl;

	line = IntToString(Time);			//���������Ϊ�ؿ���ʱ��
	//TRACE("%s\n",line.data());
	out << line << endl;
	
	line = pMe->ToString();				//������Ϊ�һ���Ϣ
	//TRACE("%s\n",line.data());
	out << line << endl;

	line = pBoomList->ToString();		//���������Ϊ��ը������Ϣ
	//TRACE("%s\n",line.data());
	out << line << endl;

	line = pEnemyList->ToString();		//���������Ϊ�о�������Ϣ
	//TRACE("%s\n",line.data());
	out << line << endl;

	line = pGoodsList->ToString();		//�ڰ������Ϊ�о�������Ϣ
	//TRACE("%s\n",line.data());
	out << line << endl;

	line = pAirLinerList->ToString();	//�ھ������Ϊ�ͻ�������Ϣ
	//TRACE("%s\n",line.data());
	out << line << endl;

	CRect rect;							//��ʮ�����Ϊ����λ��
	HWND hWnd = AfxGetMainWnd()->m_hWnd;
	CWnd* ptempwnd = CWnd::FromHandle(hWnd);
	ptempwnd->GetWindowRect(&rect);
	line = IntToString(rect.left);
	line += " ";
	line += IntToString(rect.top);
	line += " ";
	line += IntToString(rect.Width());
	line += " ";
	line += IntToString(rect.Height());
	//TRACE("%s\n", line.data());
	out << line << endl;

	MessageBox(L"�ѱ��浽Game.data��",L"�������");
}


void CPlaneGameView::ReadFromFile()		//���������ĳЩ���Ӧ��д���������
{
	ifstream in("Game.data");
	if (!in)
	{
		MessageBox(L"�浵�����ڻ��߱��ƻ�", L"����");
		return;
	}

	if (pMe != NULL)					//�һ�����������ʱ���ȵ��ý�����Ϸ����
	{
		GameEnd();
	}
	GameIni();
	string line;
	int temp[7] = {0};

	istringstream str;
	//�������ڲ��Զ�ȡ���ļ�
	//while (getline(in, line))
	//{
	//	TRACE("%s\n",line.data());
	//}
	
	getline(in, line);
	debug = (line == "1") ? true : false;	//��һ���Ƿ����ģʽ
	TRACE("void CPlaneGameView::ReadFromFile()1");

	getline(in, line);
	level = atoi(line.data());					//�ڶ��йؿ��Ѷ�
	TRACE("void CPlaneGameView::ReadFromFile()2");
	
	getline(in, line);
	*pScore = atoi(line.data());				//�����з���
	TRACE("void CPlaneGameView::ReadFromFile()3");

	getline(in, line);
	Time = atoi(line.data());					//�����м�ʱ��
	TRACE("void CPlaneGameView::ReadFromFile()4");

	getline(in, line);							//�������һ���Ϣ
	str.clear();
	str.str(line);
	str >> temp[0] >> temp[1] >> temp[2] >> temp[3] >> temp[4] >> temp[5];
	pMe->pos = CPoint(temp[0], temp[1]);
	pMe->HP = temp[2];
	pMe->power = temp[3];
	pMe->nuclear = temp[4];
	pMe->SetImmune(temp[5] == 1 ? true : false);
	TRACE("void CPlaneGameView::ReadFromFile()5");


	getline(in, line);							//�����б�ըЧ��������Ϣ
	str.clear();
	str.str(line);
	while (str >> temp[0])
	{
		str >> temp[1] >> temp[2]; 
		CGameObject* Btemp = new CBoom(CPoint(temp[1],temp[2]),temp[3]);
		pBoomList->AddTail(Btemp);
	}
	TRACE("void CPlaneGameView::ReadFromFile()6");
	
	getline(in, line);							//�����ео���Ϣ
	str.clear();
	str.str(line);
	while (str >> temp[0])
	{
		str >> temp[1] >> temp[2] >> temp[3] >> temp[4] >> temp[5]; 
		TRACE("%d %d %d %d %d %d\n", temp[0], temp[1], temp[2], temp[3],temp[4],temp[5]);
		CEnemyPlane* Etemp = NULL;
		switch (temp[4])
		{
		case GEN_ENEMY_PLANE:
			Etemp = new CGenEnemy(); break;
		case ELITE_ENEMY_PLANE:
			Etemp = new CEliteEnemy(); str >> temp[6];
			((CEliteEnemy*)Etemp)->EliteType = temp[6]; break;
		//default:
		//	throw "�ɻ����ʹ���";
		}
		Etemp->pos = CPoint(temp[0], temp[1]);
		Etemp->xSpeed = temp[2];
		Etemp->ySpeed = temp[3];
		Etemp->Type = temp[4];
		Etemp->HP = temp[5];
		pEnemyList->AddTail(Etemp);
	}
	TRACE("void CPlaneGameView::ReadFromFile()7");
	//MessageBox(L"", L"");

	getline(in, line);							//�ڰ�����Ʒ��Ϣ
	str.clear();
	str.str(line);
	while (str >> temp[0])
	{
		str >> temp[1] >> temp[2] >> temp[3] >> temp[4];
		CGoods* Btemp;
		switch (temp[4])
		{
		case MY_BULLET:
			Btemp = new CMyBullet(); break;
		case ENEMY_BULLET:
			Btemp = new CEnemyBullet(); break;
		default:
			throw "��Ʒ���ʹ���";
		}
		Btemp->pos = CPoint(temp[0], temp[1]);
		Btemp->xSpeed = temp[2];
		Btemp->ySpeed = temp[3];
		Btemp->Type = temp[4];
		pGoodsList->AddTail(Btemp);
	}
	TRACE("void CPlaneGameView::ReadFromFile()8");

	getline(in, line);							//�ھ��пͻ�������Ϣ
	str.clear();
	str.str(line);
	while (str >> temp[0])
	{
		str  >> temp[1] >> temp[2] >> temp[3] >> temp[4];
		CAirLiner* Atemp = new CAirLiner(rect);
		Atemp->pos = CPoint(temp[0], temp[1]);
		Atemp->xSpeed = temp[2];
		Atemp->ySpeed = temp[3];
		Atemp->HP = temp[4];
		pAirLinerList->AddTail(Atemp);
	}
	TRACE("void CPlaneGameView::ReadFromFile()9");

	getline(in, line);							//��10�д��ڳ���(�ҵ��죡)
	str.clear();
	str.str(line);								
	HWND hWnd = AfxGetMainWnd()->m_hWnd;
	CWnd* ptempwnd = CWnd::FromHandle(hWnd);
	str >> temp[0] >> temp[1] >> temp[2] >> temp[3];
	ptempwnd->MoveWindow(temp[0], temp[1], temp[2], temp[3], TRUE);
	TRACE("void CPlaneGameView::ReadFromFile()10");
	//CalcWindowRect(&rect);
	
	UpdateFrame();
	return;
}



void CPlaneGameView::LevelChange(int level)
{
	if (level == this->level)return;

	pMe->pos = CPoint(rect.right/2,rect.bottom-100);
	pGoodsList->DeleteAll(false);
	pEnemyList->DeleteAll(false);
	pAirLinerList->DeleteAll(false);
	if (level == 30)level = 1;
	this->level = level;
	if (pBoss)
	{
		delete pBoss;
		pBoss = NULL;
	}
	Time = 0;
}


void CPlaneGameView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'Z')
	{
		
		if (pMe!=NULL && pMe->nuclear > 0)					//������������зɻ�����Ʒ���Ϊ���ݻ�
		{													//ֻ���Լ���boss���⣬��boss���ܵ�1500���˺�
			pMe->nuclear--;									//����������Ϊ�������κλ��֣�ͬ��Ҳ�������
			pEnemyList->SetDestroy();
			pAirLinerList->SetDestroy();
			pGoodsList->SetDestroy();
			if (pBoss != NULL)
				pBoss->HP -= 1500;
		}
	}
	else if (nChar == 'Y')
	{
		if (!Start)
		{
			GameEnd();
			GameIni();
			Start = !Start;
		}
	}
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}




void CPlaneGameView::OnSet()
{
	pause = 0;
	OnPause();
	CSet s;
	
	s.BossHP1 = CBoss::sHP;
	s.BossHP2 = CBoss::increHP;
	s.BossShotChance1 = CBoss::sShotChance;
	s.BossShotChance2 = CBoss::increShotChance;
	s.sBulletSum = CBoss::sBulletSum;
	s.increBulletSum = CBoss::increBulletSum;
	s.EliteShotChance1 = CEliteEnemy::sShotChance;
	s.EliteShotChance2 = CEliteEnemy::increShotChance;
	s.EliteChance1 = sEliteChance;
	s.EliteChance2 = increELiteChance;
	s.BossTimer1 = sBossTimer;
	s.BossTimer2 = increBossTimer;
	s.genShotChance1 = CGenEnemy::sShotChance;
	s.genShotChance2 = CGenEnemy::increShotChance;

	int res = s.DoModal();
	if (res == IDOK)
	{
		CBoss::sHP = s.BossHP1;
		CBoss::increHP = s.BossHP2;
		CBoss::sShotChance = s.BossShotChance1;
		CBoss::increShotChance = s.BossShotChance2;
		CBoss::increBulletSum = s.increBulletSum;
		CBoss::sBulletSum = s.sBulletSum;
		CEliteEnemy::sShotChance = s.EliteShotChance1;
		CEliteEnemy::increShotChance = s.EliteShotChance2;
		sEliteChance = s.EliteChance1;
		increELiteChance = s.EliteChance2;
		sBossTimer = s.BossTimer1;
		increBossTimer = s.BossTimer2;
		CGenEnemy::increShotChance = s.genShotChance1;
		CGenEnemy::sShotChance = s.genShotChance2;
	}
	OnPause();
}


void CPlaneGameView::OnRand()
{
	pause = 0;
	OnPause();
	CRand r;

	r.oPowerDown = CMyPlane::oPowerDowm;
	r.oSeek = CMyPlane::oSeeking;
	r.oAppearItem = CGameList::oAppearItem;
	r.oSpecial = CEliteEnemy::oSpecial;
	r.oAppearAirliner = oAppearAir;
	r.oApparDown = CGenEnemy::oAppearDown;
	r.oJump = CGameList::oJump;

	int res = r.DoModal();
	if (res == IDOK)
	{
		CMyPlane::oPowerDowm = r.oPowerDown;
		CMyPlane::oSeeking = r.oSeek;
		CGameList::oAppearItem = r.oAppearItem;
		CEliteEnemy::oSpecial = r.oSpecial;
		oAppearAir = r.oAppearAirliner;
		CGenEnemy::oAppearDown = r.oApparDown;
		CGameList::oJump = r.oJump;
	}
	OnPause();
}


void CPlaneGameView::On32800()
{
	CHelp h;
	h.DoModal();
}
