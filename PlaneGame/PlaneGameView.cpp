
// PlaneGameView.cpp : CPlaneGameView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "PlaneGame.h"
#endif

#include "PlaneGameDoc.h"
#include "PlaneGameView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//我定义的计时器

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

// CPlaneGameView 构造/析构


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
	//以下为测试异常是否能抛出
	//CBoom test;					//检测CGameObject::OnImpact()函数
	//test.OnImpact(0);

	//以下为测试theString运行情况
	//TheString = "";

	// TODO:  在此处添加构造代码
	srand((unsigned)time(NULL));
	MemDC.CreateCompatibleDC(NULL);
	TipFont.CreatePointFont(100, _T("粗体"));
	PauseFont.CreatePointFont(400, _T("粗体"));
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
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

/*--------------------------------------------绘图部分开始-----------------------------------------------------*/




void CPlaneGameView::DrawBackground(CDC* pDC)
{
	pDC->TransparentBlt(0, BackgroundPos - 2880,
		2000, 2880, BackDC, 0, 0, 2000, 2880,NULL);//画上面一部分
	pDC->TransparentBlt(0, BackgroundPos,
		2000, 2880, BackDC, 0, 0, 2000, 2880, NULL);//画下面一部分
}


void CPlaneGameView::TextDraw(CDC* pDC,bool flag)			//显示各种文字
{
	int index = -15;
	pDC->SetTextColor(RGB(255, 255, 0));
	if (!Start)
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SelectObject(PauseFont);
		pDC->TextOutW(rect.right / 2 - 100, rect.bottom / 2 - 40, L"按Y键开始");
		pDC->SelectObject(TipFont);
		return;
	}
	//debug模式下需要显示的
	if (debug)									
	{
		pDC->TextOutW(0, index += 15, L"DEBUG模式已开启...");
		pDC->TextOutW(0, index += 15, L"当前链表中敌军数量：" + CGameObject::GetStr(pEnemyList->GetCount()));
		pDC->TextOutW(0, index += 15, L"当前链表中客机数量：" + CGameObject::GetStr(pAirLinerList->GetCount()));
		pDC->TextOutW(0, index += 15, L"当前链表中物品数量：" + CGameObject::GetStr(pGoodsList->GetCount()));
		pDC->TextOutW(0, index += 15, L"当前链表中爆炸效果数量：" + CGameObject::GetStr(pBoomList->GetCount()));
		if (pBoss)pDC->TextOutW(0, index += 15, L"BOSS血：" + CGameObject::GetStr(pBoss->HP));
		index += 15;
	}

	//任何情况下需要显示的
	if (pMe != NULL)
	{
		pDC->TextOutW(0, index += 15, L"当前我机HP：" + CGameObject::GetStr(pMe->HP));
		pDC->TextOutW(0, index += 15, L"当前我机子弹威力：" + CGameObject::GetStr(pMe->power));
		pDC->TextOutW(0, index += 15, L"当前我机保护罩时间：" + CGameObject::GetStr(pMe->ProtectTime));
		if (pMe->IsImmune())
			pDC->TextOutW(0, index += 15, L"当前处于免疫状态");
	}
	
	pDC->TextOutW(0, index += 15, L"当前关卡：" + CGameObject::GetStr(level));
	pDC->TextOutW(0, index += 15, L"当前关卡计时器：" + CGameObject::GetStr(Time));
	pDC->TextOutW(0, index += 15, L"当前核武器数量：" + CGameObject::GetStr(pMe->nuclear));
	pDC->TextOutW(0, index += 15, L"当前积分：" + CGameObject::GetStr(*pScore));


	if (pause)									//暂停时需要显示的
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SelectObject(PauseFont);
		pDC->TextOutW(rect.right / 2 - 50, rect.bottom / 2 - 30, L"暂停");
		pDC->SelectObject(TipFont);
	}
	
}


void CPlaneGameView::UpdateFrame()								//更新窗口
{
	CDC* pDC = GetDC();

	CPlaneGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

//	MemDC.FillSolidRect(0, 0, rect.right, rect.bottom, RGB(0, 0, 0));//背景设置

	DrawBackground(&MemDC);

	if (Start)
	{
		if (pMe != NULL&&!pMe->IsDestroy())					//如果我方机器没有被标记为待摧毁，绘制我方机器
			pMe->draw(&MemDC);
		if (pBoss != NULL&&!pBoss->IsDestroy())
			pBoss->draw(&MemDC);


		pEnemyList->Draw(&MemDC);				//画出每个链表
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

	TextDraw(&MemDC,Start);						//绘制文本

	pDC->BitBlt(rect.left, rect.top, rect.right, rect.bottom, &MemDC, 0, 0, SRCCOPY);
	ReleaseDC(pDC);
}


// CPlaneGameView 绘制
void CPlaneGameView::OnDraw(CDC* pDC)
{
	// TODO:  在此处为本机数据添加绘制代码
}

/*--------------------------------------------绘图部分结束---------------------------------------------*/
// CPlaneGameView 诊断

#ifdef _DEBUG
void CPlaneGameView::AssertValid() const
{
	CView::AssertValid();
}

void CPlaneGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlaneGameDoc* CPlaneGameView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlaneGameDoc)));
	return (CPlaneGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlaneGameView 消息处理程序


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
	Time = 0;													//时间
	pMe = new CMyPlane(CPoint((rect.right - CMyPlane::size.x) / 2 , rect.bottom - 100));//初始化我机
	pScore = new int(0);										//初始化分数
	pEnemyList = new CGameList();								//初始化敌军非BOSS机链表
	CSeeking::pEnemyList = this->pEnemyList;
	pGoodsList = new CGameList();								//初始化物品链表
	pBoomList = new CGameList();								//初始化爆炸链表
	pAirLinerList = new CGameList();							//初始化客机链表
	SetAllTimer();												//设置定时器
	BoomPos = CPoint(-200, -200);
	pause = 0;
}


void CPlaneGameView::GameEnd()
{
	delete pMe;//删除自己
	pMe = NULL;
	if (pBoss != NULL)
	{
		delete pBoss;
		pBoss = NULL;
	}
	Time = 0;
	delete pScore;//删除积分
	level = 1;
	pEnemyList->DeleteAll();//删除各个链表中的元素和他自己
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
	

	// TODO:  在此处添加消息处理程序代码
}



//------------------------------------计时器由此开始-------------------------------------------------//
void CPlaneGameView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
/*-------------------------------系统定时器负责：除子弹外按键检测，敌军移动,屏幕刷新。----------------------------*/
	case SYSTEM_TIMER:
	{
		if (GetKey(VK_DOWN) && pMe->pos.y <= rect.bottom)							//按键检测部分
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
		pEnemyList->move(rect);					//敌军移动
		pGoodsList->move(rect);					//物品移动
		pAirLinerList->move(rect);

		pGoodsList->ImpactWith(pEnemyList, pScore);				//物品和敌军飞机进行碰撞判断
		pAirLinerList->ImpactWith(pGoodsList, pScore);			//物品与客机链表进行碰撞判断
		*pScore += (pMe->ImpactWith(pEnemyList));				//我和敌军飞机进行碰撞判断
		*pScore += (pMe->ImpactWith(pGoodsList));				//我和物品进行碰撞判断
		if (*pScore < 0)*pScore = 0; 
		if (pBoss)
		{
			*pScore += (pBoss->ImpactWith(pGoodsList));		//boss和物品进行碰撞判断
			*pScore += pMe->ImpactWith(pBoss);
		}

		//在死神来临之前需要做的
		if (pEnemyList != NULL)
			CSeeking::pEnemyList = this->pEnemyList;			//更新追踪子弹中的信息
		pGoodsList->Work(pBoss!=NULL);
		//各个链表的死神工作，把待摧毁对象摧毁
		pEnemyList->DeleteDestroy(pBoomList,pGoodsList);					//敌机就会有爆炸特效
		pAirLinerList->DeleteDestroy(pBoomList, pGoodsList);
		pGoodsList->DeleteDestroy();
		pBoomList->DeleteDestroy();
		

		
		if (pMe->IsDestroy())									//我机已死亡则弹出对话框
		{
			OnPause();
			UINT restart = MessageBox(L"你挂了，再来一次吧！", L"死掉啦！", MB_YESNO);
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
		else if (pBoss != NULL && pBoss->IsDestroy())			//否则判断boss是否死亡
		{
			OnPause();
			MessageBox(L"恭喜过关啦，按Enter开始下一关", L"过关！");
			delete pBoss;
			pBoss = NULL;
			BoomPos = CPoint(-200, -200);
			*pScore += 1500 + level * 100;
			LevelChange(level + 1);
			OnPause();
		}
		this->UpdateFrame();									//每30毫秒都会重绘
		break;
	}


/*--------------------------------------敌人产生器负责：产生敌机和客机-------------------------------------------*/
	case GENENEMY_APPEAR:
	{
		if (pEnemyList->GetCount() >= MaxEnemy || pBoss != NULL)break;//敌人的最大数量 /*这里是15 应当定义一个变量！！！！！*/
		GetClientRect(rect);
		CEnemyPlane* temp;
		if (rand() % 100 < oAppearAir)					//客机很小的几率出现
		{
			CAirLiner* Atemp = new CAirLiner(rect);
			pAirLinerList->AddTail(Atemp);
		}
		if (rand() % 100<= (sEliteChance + level * increELiteChance))		//随机数加上等级修正量大于90就会产生精英飞机
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

/*-----------------------------------------我军子弹发生器：产生我军子弹----------------------------------------------*/
	case MY_BULLET_APPEAR:
	{
		if (GetKey(VK_SPACE))
		{
			pMe->shot(pGoodsList);
		}
		break;
	}


/*-------------------------------------------敌军子弹发生器：产生敌军子弹------------------------------------------*/
	case ENEMY_BULLET_APPEAR:
	{
		if (pGoodsList->GetCount() > 100)break;
		if (pBoss)
			pBoss->shot(pGoodsList, pMe->pos, level);
		for (POSITION pos = pEnemyList->GetHeadPosition(); pos != NULL;)//敌军发射子弹...不能加入链表对象....
		{
			CEnemyPlane* EPtemp = (CEnemyPlane*)pEnemyList->GetNext(pos);
			EPtemp->shot(pGoodsList, pMe->pos, level);
		}
		break;
	}
/*-------------------------------------------爆炸速度控制器：控制爆炸速度------------------------------------------*/
	case BOOM_SPEED:
	{
		pBoomList->move(rect,false);
		break;
	}

/*-------------------------------------------全局时间控制负责：创建boss机器,我方保护剩余时间------------------------------------------*/

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

//--------------------------------------------计时器到此结束------------------------------------------

bool CPlaneGameView::GetKey(int Key)
{
	return (GetKeyState(Key) & 0x8000) ? true : false;
}


BOOL CPlaneGameView::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
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
	// TODO:  在此处添加消息处理程序代码
}


void CPlaneGameView::SetAllTimer()
{
	SetTimer(SYSTEM_TIMER, 30, NULL);							//系统计时器
	SetTimer(GENENEMY_APPEAR, 1500 - level * 30, NULL);			//敌军刷新计时器
	SetTimer(MY_BULLET_APPEAR, 100, NULL);						//我军子弹计时器
	SetTimer(ENEMY_BULLET_APPEAR, 1000, NULL);					//敌军子弹计时器
	SetTimer(BOOM_SPEED, 50, NULL);								//炸弹速度
	SetTimer(SECOND, 1000, NULL);								//每个关卡大型计时器里的一个轮回
}


void CPlaneGameView::KillAllTimer()
{
	KillTimer(SYSTEM_TIMER);									//删除每个计时器
	KillTimer(GENENEMY_APPEAR);
	KillTimer(MY_BULLET_APPEAR);
	KillTimer(ENEMY_BULLET_APPEAR);
	KillTimer(BOOM_SPEED);
	KillTimer(SECOND);
}


void CPlaneGameView::OnPause()									//暂停的话，简单粗暴的把所有定时器都杀掉了
{
	if (!pause)
		KillAllTimer();
	else
		SetAllTimer();
	UpdateFrame();
	pause = !pause;
	this->UpdateFrame();
}

void CPlaneGameView::OnSetlevel()								//设置会自动暂停
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
	if (MessageBox(TEXT("未保存的进度将丢失\n\r重新开始吗？"), TEXT("重新开始!"), 4) == IDYES)
	{
		OnPause();
		GameEnd();
		GameIni();
	}
	else
		OnPause();
}


//--------------------------------------保存读取-------------------------------------------------------//

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
	if (pMe == NULL || pMe->IsDestroy() || pBoss != NULL)//我机未生成或是被摧毁时，不可保存
	{
		MessageBox(L"BOSS没打死或者游戏没开始时不允许保存！", L"错误");
	}
	else if (MessageBox(TEXT("将覆盖原来的存档\n\r确认吗？"), TEXT("保存..."), 4) == IDYES)
		WriteToFile();
	OnPause();
}


void CPlaneGameView::OnRead()
{
	pause = 0;
	OnPause();
	if (MessageBox(TEXT("未保存的进度将丢失\n\r读取吗？"), TEXT("读取..."), 4) == IDYES)
		ReadFromFile();
	OnPause();
}


void CPlaneGameView::WriteToFile()		//把应该保存的数据以String方式保存起来。
{
	
	string line = "";					//初始化
	ofstream out("Game.data");

	line = debug ? "1" : "0";			//第一行输出是否调试模式
	//TRACE("%s\n",line.data());
	out << line << endl;

	line = IntToString(level);			//第二行输出为关卡
	//TRACE("%s\n",line.data());
	out << line << endl;

	line = IntToString(*pScore);		//第三行输出为分数
	//TRACE("%s\n",line.data());
	out << line << endl;

	line = IntToString(Time);			//第四行输出为关卡计时器
	//TRACE("%s\n",line.data());
	out << line << endl;
	
	line = pMe->ToString();				//第五行为我机信息
	//TRACE("%s\n",line.data());
	out << line << endl;

	line = pBoomList->ToString();		//第六行输出为爆炸链表信息
	//TRACE("%s\n",line.data());
	out << line << endl;

	line = pEnemyList->ToString();		//第七行输出为敌军链表信息
	//TRACE("%s\n",line.data());
	out << line << endl;

	line = pGoodsList->ToString();		//第八行输出为敌军链表信息
	//TRACE("%s\n",line.data());
	out << line << endl;

	line = pAirLinerList->ToString();	//第九行输出为客机链表信息
	//TRACE("%s\n",line.data());
	out << line << endl;

	CRect rect;							//第十行输出为窗口位置
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

	MessageBox(L"已保存到Game.data中",L"保存进度");
}


void CPlaneGameView::ReadFromFile()		//这个函数中某些语句应该写到对象里！！
{
	ifstream in("Game.data");
	if (!in)
	{
		MessageBox(L"存档不存在或者被破坏", L"错误！");
		return;
	}

	if (pMe != NULL)					//我机已生成生成时，先调用结束游戏函数
	{
		GameEnd();
	}
	GameIni();
	string line;
	int temp[7] = {0};

	istringstream str;
	//以下用于测试读取的文件
	//while (getline(in, line))
	//{
	//	TRACE("%s\n",line.data());
	//}
	
	getline(in, line);
	debug = (line == "1") ? true : false;	//第一行是否调试模式
	TRACE("void CPlaneGameView::ReadFromFile()1");

	getline(in, line);
	level = atoi(line.data());					//第二行关卡难度
	TRACE("void CPlaneGameView::ReadFromFile()2");
	
	getline(in, line);
	*pScore = atoi(line.data());				//第三行分数
	TRACE("void CPlaneGameView::ReadFromFile()3");

	getline(in, line);
	Time = atoi(line.data());					//第四行计时器
	TRACE("void CPlaneGameView::ReadFromFile()4");

	getline(in, line);							//第五行我机信息
	str.clear();
	str.str(line);
	str >> temp[0] >> temp[1] >> temp[2] >> temp[3] >> temp[4] >> temp[5];
	pMe->pos = CPoint(temp[0], temp[1]);
	pMe->HP = temp[2];
	pMe->power = temp[3];
	pMe->nuclear = temp[4];
	pMe->SetImmune(temp[5] == 1 ? true : false);
	TRACE("void CPlaneGameView::ReadFromFile()5");


	getline(in, line);							//第六行爆炸效果链表信息
	str.clear();
	str.str(line);
	while (str >> temp[0])
	{
		str >> temp[1] >> temp[2]; 
		CGameObject* Btemp = new CBoom(CPoint(temp[1],temp[2]),temp[3]);
		pBoomList->AddTail(Btemp);
	}
	TRACE("void CPlaneGameView::ReadFromFile()6");
	
	getline(in, line);							//第七行敌军信息
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
		//	throw "飞机类型错误";
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

	getline(in, line);							//第八行物品信息
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
			throw "物品类型错误";
		}
		Btemp->pos = CPoint(temp[0], temp[1]);
		Btemp->xSpeed = temp[2];
		Btemp->ySpeed = temp[3];
		Btemp->Type = temp[4];
		pGoodsList->AddTail(Btemp);
	}
	TRACE("void CPlaneGameView::ReadFromFile()8");

	getline(in, line);							//第九行客机链表信息
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

	getline(in, line);							//第10行窗口长宽(我的天！)
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
		
		if (pMe!=NULL && pMe->nuclear > 0)					//核武器会把所有飞机和物品标记为待摧毁
		{													//只有自己和boss例外，但boss会受到1500点伤害
			pMe->nuclear--;									//核武器不会为你增加任何积分，同理，也不会减少
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
