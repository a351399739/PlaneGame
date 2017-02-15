// Help.cpp : 实现文件
//

#include "stdafx.h"
#include "PlaneGame.h"
#include "Help.h"
#include "afxdialogex.h"


// CHelp 对话框

IMPLEMENT_DYNAMIC(CHelp, CDialogEx)

CHelp::CHelp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHelp::IDD, pParent)
{

}

CHelp::~CHelp()
{
}

void CHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHelp, CDialogEx)
END_MESSAGE_MAP()


// CHelp 消息处理程序
