#pragma once
#include "string"
#include "iostream"
#include "fstream"

using namespace std;

// CSet �Ի���

class CSet : public CDialogEx
{
	DECLARE_DYNAMIC(CSet)

public:
	CSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSet();

// �Ի�������
	enum { IDD = IDD_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int BossHP1;
	int BossHP2;
	int BossShotChance1;
	int BossShotChance2;
	int sBulletSum;
	int increBulletSum;
	int EliteShotChance1;
	int EliteShotChance2;
	int EliteChance1;
	int EliteChance2;
	int BossTimer1;
	int BossTimer2;
	int EnemyAppear1;
	int EnemyAppear2;
	string IntToString(int sum);
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedRead();
	int genShotChance1;
	int genShotChance2;
};
