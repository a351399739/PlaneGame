
// PlaneGameView.h : CPlaneGameView 类的接口
//

#pragma once
#include "MyPlane.h"
#include "Boom.h"
#include "GenEnemy.h"
#include "EliteEnemy.h"
#include "Set.h"
#include "AirLiner.h"
#include "Boss.h"
#include "Item.h"
#include "GameList.h"
#include "LevelDlg.h"
#include "afxwin.h"
#include "iostream"
#include "Seeking.h"
#include "fstream"
#include "Rand.h"
#include "Help.h"
#include "atltypes.h"
using namespace std;
class  CPlaneGameView
	: public CView
{
protected: // 仅从序列化创建
	CPlaneGameView();
	DECLARE_DYNCREATE(CPlaneGameView)

// 特性
public:
	CPlaneGameDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CPlaneGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	CDC MemDC;					//双缓冲用的DC
	CBitmap MemBitmap;			//双缓冲用的位图
	CRect rect;					//当前客户区大小，会自动改变
	CMyPlane* pMe;				//我机指针
	CGameList* pEnemyList;		//敌机链表指针
	CGameList* pGoodsList;		//子弹链表指针
	CGameList* pAirLinerList;
	int *pScore;				//分数指针
	CGameList* pBoomList;		//爆炸链表执政
	CBitmap Background;			//背景位图
	CDC* BackDC;				//背景所用CDC
	int level;					//关卡
	bool pause;					//是否暂停
	bool debug;					//是否debug
	CFont TipFont;				//左上角的字体
	CFont PauseFont;			//暂停所用字体
	string IntToString(int sum);//把int数转为String
	string IntToString(long sum);//把long数转为CString
	int BackgroundPos;			//背景位置
	int Time;					//关卡时间
	int MaxEnemy;				//最多敌人数
	CBoss* pBoss;
	int sEliteChance;
	int increELiteChance;
	int sBossTimer;
	int increBossTimer;
	int oAppearAir;
	bool Start;
	CPoint BoomPos;


	//以下为自定义函数
	bool GetKey(int Key);
	void OnInitialUpdate();
	void GameIni();
	void GameEnd();
	void TextDraw(CDC* pDC,bool flag);
	void DrawBackground(CDC* pDC);
	void LoadBackimg();
	void ReadFromFile();
	void SetAllTimer();
	void KillAllTimer();
	void WriteToFile();
	void UpdateFrame();
	void Boss(int level);
	void LevelChange(int level);

	//以下为afx_msg函数
	afx_msg void OnSave();
	afx_msg void OnRead();
	afx_msg void OnImmune();
	afx_msg void OnUpdateImmune(CCmdUI *pCmdUI);
	afx_msg void OnRestart();
	afx_msg void OnDebug();
	afx_msg void OnUpdateDebug(CCmdUI *pCmdUI);
	afx_msg void OnPause();
	afx_msg void OnSetlevel();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSet();
	afx_msg void OnRand();
	afx_msg void On32800();
	
};

#ifndef _DEBUG  // PlaneGameView.cpp 中的调试版本
inline CPlaneGameDoc* CPlaneGameView::GetDocument() const
   { return reinterpret_cast<CPlaneGameDoc*>(m_pDocument); }
#endif

