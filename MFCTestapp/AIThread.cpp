// AIThread.cpp : implementation file
//

#include "stdafx.h"
#include "MyFive.h"
#include "AIThread.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAIThread

IMPLEMENT_DYNCREATE(CAIThread, CWinThread)

CAIThread::CAIThread()
{
}

CAIThread::~CAIThread()
{
}

BOOL CAIThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CAIThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CAIThread, CWinThread)
	//{{AFX_MSG_MAP(CAIThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP

	ON_THREAD_MESSAGE(WM_THREAD,AI)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAIThread message handlers

// AI ��̨�����߳�
void CAIThread::AI(WPARAM wParam,LPARAM lParam)
{

	extern CMyFiveApp theApp;

	//�������ִ�����£���ȻӦ������Ԫ������һ��
	if( 0 == theApp.fiveChess.nCount && BLACK_CHESS == theApp.fiveChess.cComputer )
	{
		theApp.fiveChess.comX = SIZE/2;
		theApp.fiveChess.comY = SIZE/2;
		theApp.AIHaveCalculated = true;
		return ;
	}
	//���������
	if(1 == theApp.grade) //�ȼ�1
		theApp.fiveChess.grade = 1;
	else if(2 == theApp.grade) //�ȼ�2
		theApp.fiveChess.grade = 2; 

	theApp.fiveChess.AI();

	theApp.AIHaveCalculated = true; // ���������
}