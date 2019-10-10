#if !defined(AFX_RVIEW_H__10C169A3_EB04_4EEE_986A_1C08B46B3C6A__INCLUDED_)
#define AFX_RVIEW_H__10C169A3_EB04_4EEE_986A_1C08B46B3C6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

/*
     Ϊ�˼������ֱ�ӷ���һ���ṹ����Ϣ���ɣ��������ݶ��ŵ�������棬
	 ������ճ�����⡣
*/

#pragma  pack(push,1)
struct NetMsg
{
	int type;   //��Ϣ���ͣ�Ϊ1��ʾ��¼��Ϣ��Ϊ2��ʾ�����λ����Ϣ��Ϊ3��ʾ������Ϣ,Ϊ4��ʾ������Ϣ
	char name[20]; //�ǳ�(��¼��Ϣ��
	bool is_black;  //Ϊtrue��ʾִ��
	bool is_forbid;  // Ϊtrue��ʾΪ�н���
	int row,col;  //�����λ��
	char str[100];  //������ַ���
};
#pragma  pack(pop)


//һ��ָ���������¼�����ʱ������ϵͳ�ͻᷢ�͸��Զ�����Ϣ֪ͨ�����̡߳�
#define UM_SOCK WM_USER+1  //�����Զ�����Ϣ

class RView : public CFormView
{
protected:
	RView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(RView)

// Form Data
public:
	//{{AFX_DATA(RView)
	enum { IDD = IDD_RVIEW };
	CString	m_ChatContent;
	UINT	m_LocalPort;
	UINT	m_SrvPort;
	CString	m_name;
	CString	m_SendMsg;
	CString	m_IP;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RView)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~RView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(RView)
	afx_msg void OnStartServer();
	afx_msg void OnLink();
	afx_msg void OnSend();   
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSock(WPARAM,LPARAM); //�Զ������Ϣ��Ӧ��������Ӧ UM_SOCK ��Ϣ
//////////////////////////////////////////////////////////////////////////
public:
    NetMsg sendNetMsg;  //�Լ�����ȥ��������Ϣ
	NetMsg recvNetMsg;  //�Է�������������Ϣ

public:
	void SendMsg(int type);   // ���������һ�˷�����Ϣ�������� CMyFiveChess �����
	BOOL haveLink;  //Ϊtrue��ʾ�Ѿ���������

private:
	SOCKET srvSocket; //�������׽��������������������ӣ�
	SOCKET clientSocket; //�ͻ����׽���������������

	SOCKET sSocket;  //����Ϊ����������Ҫ����ͻ��˵��׽��֣���֮ͨ��

	BOOL isSrv;  //Ϊtrue��ʾ��Ϊ������������Ϊ�ͻ���

	BOOL InitSocket(); //��������ʼ���׽���
	void DealMsg();   //������յ�����Ϣ

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RVIEW_H__10C169A3_EB04_4EEE_986A_1C08B46B3C6A__INCLUDED_)
