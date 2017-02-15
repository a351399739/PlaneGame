// Rand.cpp : 实现文件
//

#include "stdafx.h"
#include "PlaneGame.h"
#include "Rand.h"
#include "afxdialogex.h"
#include "sstream"


// CRand 对话框

IMPLEMENT_DYNAMIC(CRand, CDialogEx)

CRand::CRand(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRand::IDD, pParent)
	, oSpecial(0)
	, oSeek(0)
	, oPowerDown(0)
	, oAppearItem(0)
	, oApparDown(0)
	, oAppearAirliner(0)
	, oJump(0)
{

}

CRand::~CRand()
{
}

void CRand::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OSPECIAL, oSpecial);
	DDX_Text(pDX, IDC_OSEEK, oSeek);
	DDX_Text(pDX, IDC_OPOWERDOWN, oPowerDown);
	DDX_Text(pDX, IDC_OIAPPEARTEM, oAppearItem);
	DDX_Text(pDX, IDC_OAPPEARDOWN, oApparDown);
	DDX_Text(pDX, IDC_OAPPEARAIRLINER, oAppearAirliner);
	DDX_Text(pDX, IDC_OJUMP, oJump);
}


BEGIN_MESSAGE_MAP(CRand, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CRand::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRand::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CRand::OnBnClickedButton3)
END_MESSAGE_MAP()


// CRand 消息处理程序


void CRand::OnBnClickedButton1()
{
	oApparDown = 10;
	oAppearAirliner = 1;
	oAppearItem = 10;
	oSeek = 1;
	oSpecial = 25;
	oPowerDown = 40;
}


string CRand::IntToString(int sum)
{
	char t[32];
	string s;

	sprintf_s(t, "%d", sum);
	s = t;
	return s;
}



void CRand::OnBnClickedButton2()		//保存
{
	UpdateData(true);						//先把控件的值传给变量
	string line = "";
	ofstream out("Rand.data");

	line += (IntToString(oPowerDown) + " " +
		IntToString(oSeek) + " " +
		IntToString(oAppearItem) + " " +
		IntToString(oSpecial) + " " +
		IntToString(oAppearAirliner) + " " +
		IntToString(oApparDown) + " " +
		IntToString(oJump)
		);
	out << line << endl;
	MessageBox(L"已保存到Rand.data中", L"保存设置");
}


void CRand::OnBnClickedButton3()		//读取
{
	ifstream in("Rand.data");
	if (!in)
	{
		MessageBox(L"存档不存在或者被破坏", L"错误！");
		return;
	}
	string line;
	getline(in, line);
	istringstream str;
	str.str(line);
	str >> oPowerDown >> oSeek >> oAppearItem >> oSpecial >> oAppearAirliner >> oApparDown >> oJump;
	UpdateData(false);
}
