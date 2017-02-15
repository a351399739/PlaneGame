#pragma once

#include "iostream"
#include "fstream"

using namespace std;
// CRand 对话框

class CRand : public CDialogEx
{
	DECLARE_DYNAMIC(CRand)

public:
	CRand(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRand();

// 对话框数据
	enum { IDD = IDD_RAND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int oSpecial;
	int oSeek;
	int oPowerDown;
	int oAppearItem;
	int oApparDown;
	int oAppearAirliner;
	string IntToString(int sum);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	int oJump;
};
