// Set.cpp : 实现文件
//

#include "stdafx.h"
#include "PlaneGame.h"
#include "Set.h"
#include <sstream>
#include "afxdialogex.h"

// CSet 对话框

IMPLEMENT_DYNAMIC(CSet, CDialogEx)

CSet::CSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSet::IDD, pParent)
	, BossHP1(0)
	, BossHP2(0)
	, BossShotChance1(0)
	, BossShotChance2(0)
	, sBulletSum(0)
	, increBulletSum(0)
	, EliteShotChance1(0)
	, EliteShotChance2(0)
	, EliteChance1(0)
	, EliteChance2(0)
	, BossTimer1(0)
	, BossTimer2(0)
	, EnemyAppear1(0)
	, EnemyAppear2(0)
	, genShotChance1(0)
	, genShotChance2(0)
{

}

CSet::~CSet()
{
}

void CSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, BossHP1);
	DDX_Text(pDX, IDC_EDIT2, BossHP2);
	DDX_Text(pDX, IDC_SHOTCHANCE, BossShotChance1);
	DDX_Text(pDX, IDC_ISHOTCHANCE, BossShotChance2);
	DDX_Text(pDX, IDC_SHOTSUM1, sBulletSum);
	DDX_Text(pDX, IDC_SHOTSUM2, increBulletSum);
	DDX_Text(pDX, IDC_SHOTCHANCE3, EliteShotChance1);
	DDX_Text(pDX, IDC_SHOTCHANCE4, EliteShotChance2);
	DDX_Text(pDX, IDC_ELITECHANCE1, EliteChance1);
	DDX_Text(pDX, IDC_ELITECHANCE2, EliteChance2);
	DDX_Text(pDX, IDC_BOSSTIMER1, BossTimer1);
	DDX_Text(pDX, IDC_BOSSTIMER2, BossTimer2);
	DDX_Text(pDX, IDC_GENSSHOTCHANCE1, genShotChance1);
	DDX_Text(pDX, IDC_GENSSHOTCHANCE2, genShotChance2);
}


BEGIN_MESSAGE_MAP(CSet, CDialogEx)
	ON_BN_CLICKED(IDRESET, &CSet::OnBnClickedReset)
	ON_BN_CLICKED(IDSAVE, &CSet::OnBnClickedSave)
	ON_BN_CLICKED(IDREAD, &CSet::OnBnClickedRead)
END_MESSAGE_MAP()


// CSet 消息处理程序



void CSet::OnBnClickedReset()
{
	BossHP1 = 5000;
	BossHP2 = 200;
	BossShotChance1 =30;
	BossShotChance2 = 3;
	sBulletSum = 10;
	increBulletSum = 2;
	EliteShotChance1 = 30;
	EliteShotChance2 = 3;
	EliteChance1 = 10;
	EliteChance2 = 1;
	BossTimer1 = 60;
	BossTimer2 = 50;
	UpdateData(false);
}


string CSet::IntToString(int sum)
{
	char t[32];
	string s;

	sprintf_s(t, "%d", sum);
	s = t;
	return s;
}


void CSet::OnBnClickedSave()
{
	UpdateData(true);						//先把控件的值传给变量
	string line = "";					
	ofstream out("Set.data");

	line += (IntToString(BossHP1) + " " +
		IntToString(BossHP2) + " " +
		IntToString(BossShotChance1) + " " +
		IntToString(BossShotChance2) + " " +
		IntToString(sBulletSum) + " " +
		IntToString(increBulletSum) + " " +
		IntToString(EliteShotChance1) + " " +
		IntToString(EliteShotChance2) + " " +
		IntToString(EliteChance1) + " " +
		IntToString(EliteChance2) + " " +
		IntToString(BossTimer1) + " " +
		IntToString(BossTimer2) + " "+
		IntToString(genShotChance1) + " " +
		IntToString(genShotChance2) + " "
		);
	out << line << endl;
	MessageBox(L"已保存到Set.data中", L"保存设置");
}


void CSet::OnBnClickedRead()
{
	ifstream in("Set.data");
	if (!in)
	{
		MessageBox(L"存档不存在或者被破坏", L"错误！");
		return;
	}
	string line;
	getline(in, line);
	istringstream str;
	str.str(line);
	str >> BossHP1 >> BossHP2 >> BossShotChance1 >> BossShotChance2 >> sBulletSum >> increBulletSum
		>> EliteShotChance1 >> EliteShotChance2 >> EliteChance1 >> EliteChance2 >> 
		BossTimer1 >> BossTimer2 >> genShotChance1 >> genShotChance2;
	UpdateData(false);									//还原
}
