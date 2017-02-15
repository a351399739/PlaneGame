// LevelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlaneGame.h"
#include "LevelDlg.h"
#include "afxdialogex.h"


// CLevelDlg 对话框

IMPLEMENT_DYNAMIC(CLevelDlg, CDialogEx)

CLevelDlg::CLevelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLevelDlg::IDD, pParent)
	, eLevel(0)
	, HP(0)
	, nuclear(0)
	, power(0)
{

}

CLevelDlg::~CLevelDlg()
{
}

void CLevelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_LEVEL, eLevel);
	//  DDV_MinMaxInt(pDX, eLevel, 1, 20);
	DDX_Text(pDX, IDC_LEVEL, eLevel);
	DDV_MinMaxInt(pDX, eLevel, 1, 29);
	DDX_Text(pDX, IDC_HP, HP);
	DDV_MinMaxInt(pDX, HP, 1, 10000);
	DDX_Text(pDX, IDC_NUCLEAR, nuclear);
	DDV_MinMaxInt(pDX, nuclear, 0, 99999);
	DDX_Text(pDX, IDC_POWER, power);
}


BEGIN_MESSAGE_MAP(CLevelDlg, CDialogEx)
END_MESSAGE_MAP()


// CLevelDlg 消息处理程序
