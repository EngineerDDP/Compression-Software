// RView.cpp : implementation file
//

#include "stdafx.h"
#include "MyFive.h"
#include "RView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RView

IMPLEMENT_DYNCREATE(RView, CFormView)

RView::RView()
	: CFormView(RView::IDD)
{
	//{{AFX_DATA_INIT(RView)
	m_ChatContent = _T("");
	m_LocalPort = 6666;
	m_SrvPort = 7777;
	m_name = _T("·�˼�");
	m_SendMsg = _T("");
	m_IP = _T("127.0.0.1");
	//}}AFX_DATA_INIT

	isSrv = TRUE; 
	haveLink = FALSE;	
}

RView::~RView()
{
	if(clientSocket)
		closesocket(clientSocket);
	if(srvSocket)
		closesocket(srvSocket);
	if(sSocket)
		closesocket(sSocket);
}

void RView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RView)
	DDX_Text(pDX, IDC_ChatContent, m_ChatContent);
	DDX_Text(pDX, IDC_LocalPort, m_LocalPort);
	DDX_Text(pDX, IDC_SrvPort, m_SrvPort);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_SendMsg, m_SendMsg);
	DDX_Text(pDX, IDC_IP, m_IP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RView, CFormView)
	//{{AFX_MSG_MAP(RView)
	ON_BN_CLICKED(IDC_StartServer, OnStartServer)
	ON_BN_CLICKED(IDC_LINK, OnLink)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_SOCK,OnSock)  //�Զ���
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RView diagnostics

#ifdef _DEBUG
void RView::AssertValid() const
{
	CFormView::AssertValid();
}

void RView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// RView message handlers

//��������ʼ���׽���
BOOL RView::InitSocket()
{
	//����sockete������������ʽ�׽���
	srvSocket = socket(AF_INET,SOCK_STREAM,0);
	clientSocket = socket(AF_INET,SOCK_STREAM,0);
	if(INVALID_SOCKET == srvSocket || INVALID_SOCKET == clientSocket)
	{
		AfxMessageBox(TEXT("�����׽���ʧ��"));
		return FALSE;
	}

	//�׽��� srvSocket �뱾�ص�ַ����
	SOCKADDR_IN addrSock;
	addrSock.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSock.sin_family = AF_INET;
	addrSock.sin_port = htons(m_LocalPort);

	//�׽��� clientSocket �뱾�ص�ַ����
	SOCKADDR_IN addrSock2;
	addrSock2.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSock2.sin_family = AF_INET;
	addrSock2.sin_port = htons(m_LocalPort+rand()%1000); //

	if(SOCKET_ERROR == bind(srvSocket,(SOCKADDR*)&addrSock,sizeof(SOCKADDR)) ||
		SOCKET_ERROR == bind(clientSocket,(SOCKADDR*)&addrSock2,sizeof(SOCKADDR))
		)
	{
		AfxMessageBox(TEXT("��ʧ��"));
		return FALSE;
	}

	//ע�������¼�
	if(SOCKET_ERROR == WSAAsyncSelect(srvSocket,m_hWnd,UM_SOCK,FD_READ|FD_ACCEPT|FD_CLOSE|FD_CONNECT) ||
		SOCKET_ERROR == WSAAsyncSelect(clientSocket,m_hWnd,UM_SOCK,FD_READ|FD_ACCEPT|FD_CLOSE|FD_CONNECT)
		)
	{
		AfxMessageBox(TEXT("ע�������¼�ʧ��"));
		return FALSE;
	}

	//���ü���״̬
	listen(srvSocket,10);	

	return TRUE;
}

//��������
void RView::OnStartServer() 
{
	UpdateData(TRUE);
	if(m_LocalPort<1025 || m_LocalPort>65535)
	{
		AfxMessageBox(TEXT("�˿ں��� 1025 --- 65535 ֮��"));
		return ;
	}
	if(InitSocket())
	{
		GetDlgItem(IDC_LocalPort)->EnableWindow(FALSE);
		GetDlgItem(IDC_StartServer)->EnableWindow(FALSE);
	
		GetDlgItem(IDC_LINK)->EnableWindow(TRUE);
		GetDlgItem(IDC_IP)->EnableWindow(TRUE);
		GetDlgItem(IDC_SrvPort)->EnableWindow(TRUE);
		GetDlgItem(IDC_NAME)->EnableWindow(TRUE);

		AfxMessageBox(TEXT("��������ɹ�"));
	}
}


// UM_SOCK��Ϣ����Ӧ����
void RView::OnSock(WPARAM wParam,LPARAM lParam)
{
	int len;
	int iErrorCode;

	switch (LOWORD(lParam))
	{
	case FD_ACCEPT:
		{
			if(haveLink)  //�Ѿ�����
				break;
			SOCKADDR_IN addrClient;
			len = sizeof(SOCKADDR);
			sSocket = accept(srvSocket,(SOCKADDR*)&addrClient,&len);
			haveLink = TRUE; //������
			isSrv = TRUE;  //��Ϊ������

			SendMsg(1); //���͵�¼��Ϣ

			AfxMessageBox(TEXT("���ͻ������ӳɹ�"));
		}
		break;

	case FD_READ:
		{
			int iRecv = 0,nCharRecv;
			if(isSrv)  //��Ϊ����������ô��ͻ���ͨ�ŵ��׽����� sSocket 
			{
				while(iRecv<sizeof(NetMsg)) //�պý���һ���ṹ��
				{
					nCharRecv = recv(sSocket,(char*)(&recvNetMsg+iRecv),sizeof(NetMsg)-iRecv,0);
					iRecv += nCharRecv;
				}
			}
			else  // ����ͨ�ŵ��׽����� clientSocket
			{
				while(iRecv<sizeof(NetMsg))
				{
					nCharRecv = recv(clientSocket,(char*)(&recvNetMsg+iRecv),sizeof(NetMsg)-iRecv,0);
					iRecv += nCharRecv;
				}
			}
			DealMsg();  //������Ϣ
		}
		break;

	case FD_CLOSE:
		{	
			if(isSrv)
				closesocket(sSocket);
			else
				closesocket(clientSocket);

			AfxMessageBox(TEXT("�Ͽ�����"));
		}
		break;

	case FD_CONNECT:
		{
			iErrorCode = WSAGETSELECTERROR(lParam);
			if(0 == iErrorCode)
			{
				haveLink = TRUE; //������
				isSrv = FALSE;  //��Ϊ�ͻ���

				SendMsg(1); //���͵�¼��Ϣ
				AfxMessageBox(TEXT("���ӷ������ɹ�"));
			}
			else
				AfxMessageBox(TEXT("���ӷ�����ʧ��"));
		}
		break;
	}
}


//����
void RView::OnLink() 
{
	if(haveLink)
	{
		AfxMessageBox(TEXT("�Ѿ�����"));
		return ;
	}

	UpdateData(TRUE);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(m_IP); //��������IP
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(m_SrvPort);  //�������Ķ˿�

	//�������������������
	connect(clientSocket,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
}


/*   
    NetMsg �ṹ�����£�
	int type;   //��Ϣ����
	char name[20]; //�ǳ�(��¼��Ϣ��
	bool is_black;  //Ϊtrue��ʾִ��
	bool is_forbid;  // Ϊtrue��ʾΪ�н���
	int row,col;  //�����λ��
	char str[100];  //������ַ���
*/

//������Ϣ�����������¼��Ϣ���������Ϣ��������Ϣ��������Ϣ
void RView::SendMsg(int type)
{		
	if(!haveLink)
		return ;
	
	// .... �� netMsg ��ֵ
	memset(&sendNetMsg,0,sizeof(sendNetMsg)); //
	sendNetMsg.type = type;
	char *pName = m_name.GetBuffer(m_name.GetLength()+1);
	memcpy(sendNetMsg.name,pName,m_name.GetLength()+1);

	char *pChat = m_SendMsg.GetBuffer(m_SendMsg.GetLength()+1);
	memcpy(sendNetMsg.str,pChat,m_SendMsg.GetLength()+1);

	CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();  
	sendNetMsg.row = pFrm->m_MyFiveView->fiveChess.currentX;
	sendNetMsg.col = pFrm->m_MyFiveView->fiveChess.currentY;
	sendNetMsg.is_black = pFrm->m_MyFiveView->gameInfo.isBlack;
	sendNetMsg.is_forbid = pFrm->m_MyFiveView->gameInfo.isForbid;
	
	
	int iSend = 0,nCharSend;
	if(isSrv)
	{
		while(iSend<sizeof(NetMsg))
		{
			nCharSend = send(sSocket,(char*)(&sendNetMsg+iSend),sizeof(NetMsg)-iSend,0);
			iSend += nCharSend;
		}
	}
	else
	{
		while(iSend<sizeof(NetMsg))
		{
			nCharSend = send(clientSocket,(char*)(&sendNetMsg+iSend),sizeof(NetMsg)-iSend,0);
			iSend += nCharSend;
		}
	}
}


//������Ͱ�ť�����͵���������Ϣ
void RView::OnSend() 
{
	GetDlgItemText(IDC_SendMsg,m_SendMsg);
	if("" == m_SendMsg)
	{
		AfxMessageBox(TEXT("������Ϣ����Ϊ��"));
		return ;
	}
	SendMsg(3);  //����������Ϣ

	CString temp;
	temp.Format(TEXT("��˵��%s"),sendNetMsg.str);
	temp += "\r\n";

	CString sTime;
	SYSTEMTIME st;
	GetLocalTime(&st);  //�õ�ϵͳʱ��
	sTime.Format(TEXT("%d:%d:%d"),st.wHour,st.wMinute,st.wSecond);
	sTime += "\r\n";

	m_ChatContent = sTime + temp + m_ChatContent;
	SetDlgItemText(IDC_ChatContent,m_ChatContent);

	SetDlgItemText(IDC_SendMsg,"");
}

//������յ�����Ϣ
void RView::DealMsg()
{
	CString temp;
	if(1 == recvNetMsg.type) //�����¼��Ϣ
	{
		temp.Format(TEXT("%s����������!"),recvNetMsg.name);
		temp += "\r\n";
		
		CString sTime;
		SYSTEMTIME st;
		GetLocalTime(&st);  //�õ�ϵͳʱ��
		sTime.Format(TEXT("%d:%d:%d"),st.wHour,st.wMinute,st.wSecond);
		sTime += "\r\n";
		
		m_ChatContent = sTime + temp + m_ChatContent;
		SetDlgItemText(IDC_ChatContent,m_ChatContent);
	}
	else if( 2 == recvNetMsg.type)  //����������Ϣ
	{
		CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
		pFrm->m_MyFiveView->DealNetBattle();  
	}
	else if(3 == recvNetMsg.type) //����������Ϣ
	{
		temp.Format(TEXT("%s˵��%s"),recvNetMsg.name,recvNetMsg.str);
		temp += "\r\n";
		
		CString sTime;
		SYSTEMTIME st;
		GetLocalTime(&st);  //�õ�ϵͳʱ��
		sTime.Format(TEXT("%d:%d:%d"),st.wHour,st.wMinute,st.wSecond);
		sTime += "\r\n";
		
		m_ChatContent = sTime + temp + m_ChatContent;
		SetDlgItemText(IDC_ChatContent,m_ChatContent);
	}
	else if(4 == recvNetMsg.type) //���������Ϣ
	{
		CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
		pFrm->m_MyFiveView->fiveChess.RetractChess();
		pFrm->m_MyFiveView->Invalidate(0); //��������
	}
}

