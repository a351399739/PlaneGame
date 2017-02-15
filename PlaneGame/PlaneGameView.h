
// PlaneGameView.h : CPlaneGameView ��Ľӿ�
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
protected: // �������л�����
	CPlaneGameView();
	DECLARE_DYNCREATE(CPlaneGameView)

// ����
public:
	CPlaneGameDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CPlaneGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	CDC MemDC;					//˫�����õ�DC
	CBitmap MemBitmap;			//˫�����õ�λͼ
	CRect rect;					//��ǰ�ͻ�����С�����Զ��ı�
	CMyPlane* pMe;				//�һ�ָ��
	CGameList* pEnemyList;		//�л�����ָ��
	CGameList* pGoodsList;		//�ӵ�����ָ��
	CGameList* pAirLinerList;
	int *pScore;				//����ָ��
	CGameList* pBoomList;		//��ը����ִ��
	CBitmap Background;			//����λͼ
	CDC* BackDC;				//��������CDC
	int level;					//�ؿ�
	bool pause;					//�Ƿ���ͣ
	bool debug;					//�Ƿ�debug
	CFont TipFont;				//���Ͻǵ�����
	CFont PauseFont;			//��ͣ��������
	string IntToString(int sum);//��int��תΪString
	string IntToString(long sum);//��long��תΪCString
	int BackgroundPos;			//����λ��
	int Time;					//�ؿ�ʱ��
	int MaxEnemy;				//��������
	CBoss* pBoss;
	int sEliteChance;
	int increELiteChance;
	int sBossTimer;
	int increBossTimer;
	int oAppearAir;
	bool Start;
	CPoint BoomPos;


	//����Ϊ�Զ��庯��
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

	//����Ϊafx_msg����
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

#ifndef _DEBUG  // PlaneGameView.cpp �еĵ��԰汾
inline CPlaneGameDoc* CPlaneGameView::GetDocument() const
   { return reinterpret_cast<CPlaneGameDoc*>(m_pDocument); }
#endif

