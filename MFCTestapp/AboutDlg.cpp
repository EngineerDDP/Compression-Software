// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyFive.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AboutDlg dialog


AboutDlg::AboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(AboutDlg)
	m_Content = _T("");
	//}}AFX_DATA_INIT

	m_Content = TEXT("�û���֪: \r\n\r\n\
	�ڲ˵����ϣ����� --> ��սģʽ --> ��ʼ��Ϸ��\r\n\r\n\
        ���ǡ�����ģʽ��������ȷ��˫�������Ѿ����绥����\r\n\
Ȼ�����ұ߽�����Ӧ���ã�һ����Ϊ����������һ����Ϊ�ͻ��ˣ�\r\n\r\n\
(1)������Ϊ��������������˿ںţ�Ȼ�������������񡱰�ť��\r\n\
(2)������Ϊ�ͻ��ˣ���ɲ���(1)��,����Ҫ�����������IP�Ͷ˿ںţ�Ȼ���������ӡ���ť��\
���ӳɹ����������ս�ˡ�ע����ǣ�˫���Ľ��ֹ��������ͬ��ִ����ɫ���ܳ�ͻ��\
��Ȼ������鲢������ҵģ�����һ������ԺͶԷ����졣\r\n\r\n\
        ���ǡ�����ģʽ���������͡�\r\n\r\n\
ע��:\r\n\
(1)Ŀǰ����ֻ�ܴ���һ�ؽ��֣����ڶ��ؽ�����Ҫ����Լ��жϡ�\r\n\
(2)AI�����ǵ�̺ʽ�ģ������ں��˱��˵Ĳ���������˼�룬��������ױ����ִ�ܡ�\r\n\
(3)��������½��ֹ���������塣\r\n\r\n\
������������������ϵ��darksword@foxmail.com \r\n\r\n\
					���\r\n\
						2013.2.13\
	");  

}


void AboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AboutDlg)
	DDX_Text(pDX, IDC_Content, m_Content);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AboutDlg, CDialog)
	//{{AFX_MSG_MAP(AboutDlg)
	ON_BN_CLICKED(IDC_BOKE, OnBoke)
	ON_BN_CLICKED(IDC_OK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AboutDlg message handlers

void AboutDlg::OnBoke() 
{
	ShellExecute(NULL,"open",_T("http://www.darkswordzone.com/"),NULL,NULL,SW_SHOWNORMAL);
}


void AboutDlg::OnOk() 
{
	this->OnCancel();
}
