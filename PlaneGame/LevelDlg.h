#pragma once


// CLevelDlg �Ի���

class CLevelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLevelDlg)

public:
	CLevelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLevelDlg();

// �Ի�������
	enum { IDD = IDD_LEVELDIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	int eLevel;
	int eLevel;
	int HP;
	int nuclear;
	int power;
};
