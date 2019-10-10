// MyFiveView.cpp : implementation of the CMyFiveView class
//

#include "stdafx.h"
#include "MyFive.h"

#include "MyFiveDoc.h"
#include "MyFiveView.h"
#include "MainFrm.h"
#include "Mmsystem.h"
#include "AboutDlg.h"
#include "Grade.h"
#include "AIThread.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMyFiveView

IMPLEMENT_DYNCREATE(CMyFiveView, CView)

BEGIN_MESSAGE_MAP(CMyFiveView, CView)
	//{{AFX_MSG_MAP(CMyFiveView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_P2P, OnP2p)
	ON_UPDATE_COMMAND_UI(ID_P2P, OnUpdateP2p)
	ON_COMMAND(ID_P2C, OnP2c)
	ON_UPDATE_COMMAND_UI(ID_P2C, OnUpdateP2c)
	ON_COMMAND(ID_START, OnStart)
	ON_COMMAND(ID_Regret, OnRegret)
	ON_COMMAND(IDC_SET, OnSet)
	ON_COMMAND(ID_NetBattle, OnNetBattle)
	ON_UPDATE_COMMAND_UI(ID_NetBattle, OnUpdateNetBattle)
	ON_COMMAND(ID_About, OnAbout)
	ON_WM_TIMER()
	ON_COMMAND(ID_ComputerPlayChess, OnComputerPlayChess)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFiveView construction/destruction

extern CMyFiveApp theApp;  //����

CMyFiveView::CMyFiveView():fiveChess(theApp.fiveChess) // �����ʼ������
{
	InitBmp();
	InitGame();

	//��ʼ������ģʽ���˻���ս
	isNetBattle = false;
	isP2C = true;
	isP2P = false;

	gameResult = 0;
	m_pThrd = NULL;
}

CMyFiveView::~CMyFiveView()
{
	StopAIThread();
}

BOOL CMyFiveView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyFiveView drawing

void CMyFiveView::OnDraw(CDC* pDC)
{
	CMyFiveDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	// ���ô��ڱ���
	AfxGetMainWnd()->SetWindowText("���������� 1.3");

	//�Զ����ػ����
	/*******************************************************/

	CDC boardDC,blackDC,whiteDC;
	
	boardDC.CreateCompatibleDC(pDC);//�����ڴ��豸����
	boardDC.SelectObject(&boardBmp); //��λͼѡ���ڴ��豸����

	blackDC.CreateCompatibleDC(pDC);
	blackDC.SelectObject(&blackBmp);

	whiteDC.CreateCompatibleDC(pDC);
	whiteDC.SelectObject(&whiteBmp);

	//��λͼ���ڴ��豸�����и��Ƶ�ָ�����豸������������ʾ������
	pDC->BitBlt(0,0,boardWidth,boardHeight,&boardDC,0,0,SRCCOPY); //����

	//���滭������
	// TransparentBlt()λ��Msimg32.lib
	
	extern CMyFiveApp theApp; //

	char cChess;
	int i,j;
	for(i=0;i<SIZE;++i)
	{
		for(j=0;j<SIZE;++j)
		{
			if( !theApp.AIHaveCalculated )// �����˻���ս���� AI ���ڼ�����
				cChess = paintMap[i][j];
			else
				cChess = fiveChess.GetStatus(i,j);

			if(cChess == BLACK_CHESS)
			{
				TransparentBlt(pDC->m_hDC,j*WIDTH+OFFSETX,i*WIDTH+OFFSETY,
					chessWidth,chessHeight,blackDC.m_hDC,0,0,
					chessWidth,chessHeight,CLR_MAGENTA); //���һ������ʹ������ɫ͸��
			}
			else if(cChess == WHITE_CHESS)
			{
				TransparentBlt(pDC->m_hDC,j*WIDTH+OFFSETX,i*WIDTH+OFFSETY,
					chessWidth,chessHeight,whiteDC.m_hDC,0,0,
					chessWidth,chessHeight,CLR_MAGENTA); //���һ������ʹ������ɫ͸��
			}
		}
	}
	

	if(nowX != -1 && nowY != -1)
	{
		//����ǰ���ָ��㻭��
		if(blackFall)
			pDC->Draw3dRect(nowX,nowY,chessWidth,chessHeight,CLR_BLACK,CLR_BLACK);
		else
			pDC->Draw3dRect(nowX,nowY,chessWidth,chessHeight,CLR_WHITE,CLR_WHITE);

		//����һ�����ӻ���
		if( preX != -1 || preY != -1 )
		{
			if( tempX != -1 || tempY != -1 )
			{
				pDC->Draw3dRect(preX,preY,chessWidth,chessHeight,CLR_RED,CLR_RED);
			}
		}
	}

	// ��������ӻ�������
	int x,y;
	CString msg;

	for(i=0;i<fiveChess.chessPro.size();++i)
	{
		x = fiveChess.chessPro[i].first;
		y = fiveChess.chessPro[i].second;
		msg.Format(TEXT("%d"),i+1);

		if(BLACK_CHESS == fiveChess.GetStatus(x,y))
		{
		//	pDC->SetBkColor(CLR_BK_BLACK); // �������屳����ɫ
			pDC->SetBkMode(TRANSPARENT); // ʹ����͸��
			pDC->SetTextColor(CLR_WHITE);
		}
		else
		{
		//	pDC->SetBkColor(CLR_BK_WHITE);
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(CLR_BLACK);
			
		}
		if(i<=8)
			pDC->TextOut(y*WIDTH+OFFSETX+12,x*WIDTH+OFFSETY+9,msg);
		else if(i>=9 && i<=98)
			pDC->TextOut(y*WIDTH+OFFSETX+9,x*WIDTH+OFFSETY+9,msg);
		else
			pDC->TextOut(y*WIDTH+OFFSETX+5,x*WIDTH+OFFSETY+9,msg);
	}

	// ��ʾ����
	for(i=0;i<SIZE;++i)
	{
		if(fiveChess.GetStatus(0,i) != SPACE)
			continue;
		msg.Format(TEXT("%c"),'A'+i);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(CLR_BLACK);
		pDC->TextOut(i*WIDTH+OFFSETX+11,2,msg);
	}
	for(i=0;i<SIZE;++i)
	{
		if(fiveChess.GetStatus(i,0) != SPACE)
			continue;
		msg.Format(TEXT("%d"),i+1);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(CLR_BLACK);
		if(i<=8)
			pDC->TextOut(4,i*WIDTH+OFFSETY+7,msg);
		else
			pDC->TextOut(1,i*WIDTH+OFFSETY+7,msg);
	}

	// ��ӡ�������
	if( gameResult ) // gameResult ��Ϊ0ʱ���ͱ�ʾ�����˱������
		PrintResult();


	ReleaseDC(&boardDC);
	ReleaseDC(&blackDC);
	ReleaseDC(&whiteDC);
}

/////////////////////////////////////////////////////////////////////////////
// CMyFiveView printing

BOOL CMyFiveView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation

	return DoPreparePrinting(pInfo);
}

void CMyFiveView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyFiveView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMyFiveView diagnostics

#ifdef _DEBUG
void CMyFiveView::AssertValid() const
{
	CView::AssertValid();
}

void CMyFiveView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyFiveDoc* CMyFiveView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyFiveDoc)));
	return (CMyFiveDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyFiveView message handlers

//��ʼ����Ϸ
void CMyFiveView::InitGame()
{
	fiveChess.InitChessMap();

	startGame = false;

	preX = preY = nowX = nowY = tempX = tempY = -1;
	moveX = moveY = -1;

	extern CMyFiveApp theApp;
	theApp.AIHaveCalculated = true;
}

//���ز���ʼ��λͼ��Ϣ
void CMyFiveView::InitBmp()
{
	BITMAP bm; //����λͼ�ṹ
	//��������
	boardBmp.LoadBitmap(IDB_board); //����λͼ��Դ
	boardBmp.GetBitmap(&bm);  //�õ�λͼ��Ϣ
	boardWidth = bm.bmWidth; //���̿��
	boardHeight = bm.bmHeight; //���̸߶�

	blackBmp.LoadBitmap(IDB_black);
	whiteBmp.LoadBitmap(IDB_white);

	blackBmp.GetBitmap(&bm);
	chessWidth = bm.bmWidth; //���ӿ��
	chessHeight = bm.bmHeight; //���Ӹ߶�
}


void CMyFiveView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!startGame)  //δ��ʼ�㷵��
		return ;

	extern CMyFiveApp theApp;
	if(!theApp.AIHaveCalculated)
	{
		AfxMessageBox(TEXT("AI�߳����ڼ��㣬���Ե�"));
		return ;
	}

	if(point.x-OFFSETX < 0 || point.y-OFFSETY < 0) //����
		return ;

	int nX = static_cast<int>( (point.x-OFFSETX) / WIDTH );
	int nY = static_cast<int>( (point.y-OFFSETY) / WIDTH );

	if( nX > 14 || nY > 14) //����
		return ;

	char cChess = fiveChess.GetStatus(nY,nX);
	if( cChess != SPACE) //λ�������������򷵻�
		return ;

	//////////////////////////////////////////////////////////////////////////

	//�˻���ս
	if(isP2C)
	{
		if(gameInfo.isBlack && blackFall) //��ִ�ڲ��Ҹú�������
		{
			blackFall = false;
			cChess = BLACK_CHESS;
		}
		else if(!gameInfo.isBlack && !blackFall)   //��ִ�ײ��Ҹð�������
		{
			blackFall = true;
			cChess = WHITE_CHESS;
		}
		else  //����͸öԷ�����
			return ;
	}

	//���˶�ս
	else if(isP2P)
	{
		if(blackFall) //��������
		{
			blackFall = false;
			cChess = BLACK_CHESS;
		}
		else
		{
			blackFall = true;
			cChess = WHITE_CHESS;
		}
	}

	//����ģʽ�����˶�ս
	else if(isNetBattle)
	{
		if(gameInfo.isBlack && blackFall) //��ִ�ڲ��Ҹú�������
		{
			blackFall = false;
			cChess = BLACK_CHESS;
		}
		else if(!gameInfo.isBlack && !blackFall)   //��ִ�ײ��Ҹð�������
		{
			blackFall = true;
			cChess = WHITE_CHESS;
		}
		else  //����͸öԷ�����
			return ;
	}
	//////////////////////////////////////////////////////////////////////////
	UpdateRect(); //������һ������λ�������Ŀ�

	//
	char ch; 
	fiveChess.SetChess(nY,nX,cChess);
	
	if(isNetBattle) //��������ģʽ������Է������ҵ�������Ϣ
	{
		CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
		pFrm->m_RView->SendMsg(2);
	}

	ch = fiveChess.JudgeWin(cChess); //�ж��Ƿ�Ӯ��

	//�н��ֲ��ҵ�ǰ����Ϊ���壬����Ҫ�ж�����Ƿ��ǽ���
	if(gameInfo.isForbid && BLACK_CHESS == cChess) //
	{
		int res = fiveChess.IsKinjite(nY,nX);

		if( res ) // ����
		{
			startGame = false;
			/*
			CString msg;
			if( 1 == res )
				msg.Format(TEXT("�������֣��ڸ�"));
			else if( 2 == res )
				msg.Format(TEXT("�������֣��ڸ�"));
			else if( 3 == res )
				msg.Format(TEXT("���Ľ��֣��ڸ�"));
			AfxMessageBox(msg);
			*/
			gameResult = res;
			PrintResult(); //
		}
	}
	if(startGame)  
	{
		if(ch == cChess || ch == PEACE_CHESS) //Ӯ������
		{
			startGame = false;
		}
		//Ӯ��
		if(!startGame) 
		{
			/*
			CString msg;
			if(BLACK_CHESS == ch)
				msg.Format(TEXT("�����飬��Ӯ"));
			else if(WHITE_CHESS == ch)
				msg.Format(TEXT("�����飬��Ӯ"));
			else if(PEACE_CHESS == ch)
				msg.Format(TEXT("����"));
			AfxMessageBox(msg);
			*/
			int res; 
			if( BLACK_CHESS == ch )
				res = 4;
			else if( WHITE_CHESS == ch )
				res = 5;
			else if( PEACE_CHESS == ch )
				res = 6;

			gameResult = res;
			PrintResult(); //
		}
	}

	// ���ǵ���ģʽ���˻���ս
	if(isP2C)
		DealP2C();

	CView::OnLButtonDown(nFlags, point);
}

void CMyFiveView::OnMouseMove(UINT nFlags, CPoint point) 
{	
	if(!startGame) //δ��ʼ
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		return ;
	}

	extern CMyFiveApp theApp;
	
	if(!theApp.AIHaveCalculated) // �˻���ս��AI ���ڼ���
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		return ;
	}


	if(point.x-OFFSETX < 0 || point.y-OFFSETY < 0) //����
		return ;

	int nX = static_cast<int>( (point.x-OFFSETX) / WIDTH );
	int nY = static_cast<int>( (point.y-OFFSETY) / WIDTH );

	if( nX > 14 || nY > 14) //����
		return ;
	
	// �������
	if(fiveChess.GetStatus(nY,nX) != SPACE)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS)); 
		return ;
	}
	else
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_hand));
	}


	nowX = nX*WIDTH + OFFSETX;
	nowY = nY*WIDTH + OFFSETY;


	CRect rect;
	//������һ�ι�괦
	rect.left = moveX;
	rect.top = moveY;
	rect.right = rect.left + WIDTH;
	rect.bottom = rect.top + WIDTH;
	InvalidateRect(&rect,0);

	//���µ�ǰ��괦
	rect.left = nowX;
	rect.top = nowY;
	rect.right = rect.left + WIDTH;
	rect.bottom = rect.top + WIDTH;

	InvalidateRect(&rect,0);

	//���� moveX,moveY
	moveX = nowX;
	moveY = nowY;

	CView::OnMouseMove(nFlags, point);
}

//����ģʽ������
void CMyFiveView::OnNetBattle() 
{
	isNetBattle = true;
	isP2C = isP2P = false;
}

void CMyFiveView::OnUpdateNetBattle(CCmdUI* pCmdUI) 
{
	if(isNetBattle)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

//����ģʽ�����˶�ս
void CMyFiveView::OnP2p() 
{
	isP2P = true;
	isP2C = isNetBattle = false;
}

void CMyFiveView::OnUpdateP2p(CCmdUI* pCmdUI) 
{
	if(isP2P)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

//����ģʽ���˻���ս
void CMyFiveView::OnP2c() 
{
	isP2C = true;
	isP2P = isNetBattle = false;
	fiveChess.haveKinjite = gameInfo.isForbid;
	fiveChess.cComputer = gameInfo.isBlack ? WHITE_CHESS : BLACK_CHESS;
	fiveChess.cPeople = gameInfo.isBlack ? BLACK_CHESS : WHITE_CHESS;
}

void CMyFiveView::OnUpdateP2c(CCmdUI* pCmdUI) 
{
	if(isP2C)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

//��ʼ��Ϸ
void CMyFiveView::OnStart() 
{	
	StopAIThread();

	fiveChess.haveKinjite = gameInfo.isForbid;
	fiveChess.cComputer = gameInfo.isBlack ? WHITE_CHESS : BLACK_CHESS;
	fiveChess.cPeople = gameInfo.isBlack ? BLACK_CHESS : WHITE_CHESS;
	
	gameResult = 0;

	if(isP2P)
	{
		// ...����Ҫ
	}
	else if(isP2C)
	{
		if(BLACK_CHESS == fiveChess.cComputer)  //�����ִ��
		{
			// DealP2C(); //�ŵ�����ִ��
		}
	}
	else if(isNetBattle)
	{
		CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
		if(!pFrm->m_RView->haveLink)
		{
			AfxMessageBox(TEXT("��δ�ͶԷ�����!\r\n\n�뵽�ұ���������"));
			return ;
		}
		
		//�ٴ���Է������ҵĵ�¼��Ϣ�������ӵ�ʱ����Ѿ����͹�һ�ε�¼��Ϣ��
		pFrm->m_RView->SendMsg(1);

		//���Ѿ����ӣ���Ȼ�Ѿ��յ��Է����͵ĵ�һ����Ϣ����
		if(gameInfo.isBlack == pFrm->m_RView->recvNetMsg.is_black)
		{
			AfxMessageBox(TEXT("˫������ִͬ����ɫ�����ӣ�\r\n ����������"));
			return ;
		}
		if(gameInfo.isForbid != pFrm->m_RView->recvNetMsg.is_forbid)
		{
			AfxMessageBox(TEXT("˫�����ڽ��ֵ����ò�һ��! \r\n ����������"));
			return ;
		}

	}

	InitGame();
	startGame = true;
	blackFall = true;
	Invalidate(0);

	if(isP2C && BLACK_CHESS == fiveChess.cComputer)
		DealP2C();
}

//������ģʽ�˻���ս
void CMyFiveView::DealP2C()
{
	if(!startGame)
		return ;
	if(!blackFall && BLACK_CHESS == fiveChess.cComputer)
		return ;
	if(blackFall && WHITE_CHESS == fiveChess.cComputer)
		return ;

	extern CMyFiveApp theApp;

	theApp.grade = gameInfo.m_grade.grade; // �õ� AI  �ĵȼ�
	theApp.AIHaveCalculated = false; // ��ʼ������� AI �Ƿ���ɼ���

	// VC �Ĺ�����Ⱥͷ�����������������ʼ�������Զ�̬����
	fiveChess.VCDEPTH = gameInfo.m_grade.VCDEPTH; 
	fiveChess.VCDEPTH_END = gameInfo.m_grade.VCDEPTH_END;
	fiveChess.VCDEPTH_DIS = gameInfo.m_grade.VCDEPTH_DIS;

	fiveChess.VCDE_DEPTH = gameInfo.m_grade.VCDE_DEPTH;
	fiveChess.VCDE_DEPTH_END = gameInfo.m_grade.VCDE_DEPTH_END;
	fiveChess.VCDE_DEPTH_DIS = gameInfo.m_grade.VCDE_DEPTH_DIS;

	memcpy(paintMap,fiveChess.chessMap,sizeof(paintMap));  // ���浱ǰ��������� �ػ����

	CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
	pFrm->m_wndStatusBar.SetPaneText(0,TEXT("���������˼��......�����ĵȴ�"),1);

	// ����
	m_pThrd = AfxBeginThread(RUNTIME_CLASS(CAIThread));

	// ִ��
	m_pThrd->PostThreadMessage(WM_THREAD,NULL,NULL);

	// ���ö�ʱ������ AI �Ƿ���ɼ���
	SetTimer(1,200,NULL);
}

//��������ģʽ���˶�ս��ֻ��void RView::DealMsg()�е��øú���
// �Է�����
void CMyFiveView::DealNetBattle()
{
	CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();

	char cChess = pFrm->m_RView->recvNetMsg.is_black ? BLACK_CHESS : WHITE_CHESS;
	int row = pFrm->m_RView->recvNetMsg.row;
	int col = pFrm->m_RView->recvNetMsg.col;

	nowX = col*WIDTH + OFFSETX;
	nowY = row*WIDTH + OFFSETY;


	fiveChess.SetChess(row,col,cChess);
	char ch = fiveChess.JudgeWin(cChess); //�ж��Ƿ�Ӯ��
	UpdateRect(); //���¿�

	ch = fiveChess.JudgeWin(cChess); //�ж��Ƿ�Ӯ��
	
	//�н��ֲ��ҵ�ǰ����Ϊ���壬����Ҫ�ж�����Ƿ��ǽ���
	if(gameInfo.isForbid && BLACK_CHESS == cChess) //
	{
		int res = fiveChess.IsKinjite(row,col);
		
		if( res ) // ����
		{
			startGame = false;

			gameResult = res;
			PrintResult(); //
		}
	}
	if(startGame)  
	{
		if(ch == cChess || ch == PEACE_CHESS) //Ӯ������
		{
			startGame = false;
		}
		//Ӯ��
		if(!startGame) 
		{
			int res;
			if( BLACK_CHESS == ch )
				res = 4;
			else if( WHITE_CHESS == ch )
				res = 5;
			else if( PEACE_CHESS == ch )
				res = 6;

			gameResult = res;
			PrintResult(); //
		}
	}

	blackFall = gameInfo.isBlack; //�����ҿ�������
}

//������һ������λ�������Ŀ�
void CMyFiveView::UpdateRect()
{
	tempX = preX;
	tempY = preY;
	preX = nowX;
	preY = nowY;
	
	CRect rect;
	//������ǰ��괦
	rect.left = tempX;
	rect.top = tempY;
	rect.right = rect.left + WIDTH;
	rect.bottom = rect.top + WIDTH;
	InvalidateRect(&rect,0);
	
	//���µ�ǰ���
	rect.left = nowX;
	rect.top = nowY;
	rect.right = rect.left + WIDTH;
	rect.bottom = rect.top + WIDTH;
	InvalidateRect(&rect,0);

	if(gameInfo.isVoice)
	{
		//�������ӵ�����
		PlaySound(MAKEINTRESOURCE(IDR_PUTSTONE),AfxGetResourceHandle(),
			SND_ASYNC|SND_RESOURCE|SND_NODEFAULT|SND_NOWAIT);
	}
}


//����
void CMyFiveView::OnRegret() 
{
	if(fiveChess.chessPro.size()<=1)
		return ;
	fiveChess.RetractChess(); 
	Invalidate(0);

	//��Է����ͻ�����Ϣ
	CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
	pFrm->m_RView->SendMsg(4);
}


//����
void CMyFiveView::OnSet() 
{
	gameInfo.DoModal();
}

//����
void CMyFiveView::OnAbout() 
{
	AboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// ��ʾ���
void CMyFiveView::PrintResult()
{
	CDC *dc = GetDC();
	CFont font;
	font.CreatePointFont(300,"�����п�",NULL);
	dc->SelectObject(&font);

	dc->SetBkMode(TRANSPARENT);
	dc->SetTextColor(RGB(60,130,40));
	
	CRect rect;
	GetClientRect(&rect);

	int cx = (rect.left+rect.right)/2-100 , cy = (rect.top+rect.bottom)/4;

	switch(gameResult)
	{
	case 1: 
		dc->TextOut(cx,cy,TEXT("��������,�ڸ�!"));
		break;
	case 2:
		dc->TextOut(cx,cy,TEXT("��������,�ڸ�!"));
		break;
	case 3:
		dc->TextOut(cx,cy,TEXT("���Ľ���,�ڸ�!"));
		break;
	case 4:
		dc->TextOut(cx,cy,TEXT("������,��ʤ!"));
		break;
	case 5:
		dc->TextOut(cx,cy,TEXT("������,��ʤ!"));
		break;
	case 6:
		dc->TextOut(cx,cy,TEXT("����!"));
		break;
	}

	ReleaseDC(dc);
}

// �������� AI ��̨����
void CMyFiveView::OnTimer(UINT nIDEvent) 
{
	extern CMyFiveApp theApp;
	if( !theApp.AIHaveCalculated ) // AI û�м����� �򷵻�
		return;

	// ���洦����������

	int row,col;
	row = fiveChess.comX;
	col = fiveChess.comY;
	
	char cChess = fiveChess.cComputer;
	
	nowX = col*WIDTH + OFFSETX;
	nowY = row*WIDTH + OFFSETY;
	
	fiveChess.SetChess(row,col,cChess);
	char ch = fiveChess.JudgeWin(cChess); //�ж��Ƿ�Ӯ��
	UpdateRect(); //���¿�
	
	ch = fiveChess.JudgeWin(cChess); //�ж��Ƿ�Ӯ��
	
	//�н��ֲ��ҵ�ǰ����Ϊ���壬����Ҫ�ж�����Ƿ��ǽ���
	if(gameInfo.isForbid && BLACK_CHESS == cChess) //
	{
		int res = fiveChess.IsKinjite(row,col);
		
		if( res ) // ����
		{
			startGame = false;
			
			gameResult = res;
			PrintResult(); //
		}
	}
	if(startGame)  
	{
		if(ch == cChess || ch == PEACE_CHESS) //Ӯ������
		{
			startGame = false;
		}
		//Ӯ��
		if(!startGame) 
		{
			int res;
			if( BLACK_CHESS == ch )
				res = 4;
			else if( WHITE_CHESS == ch )
				res = 5;
			else if( PEACE_CHESS == ch )
				res = 6;
			
			gameResult = res;
			PrintResult(); //
		}
	}
		
	blackFall = !blackFall; //�����ҿ�������

	theApp.AIHaveCalculated = true; // ����

	KillTimer(1);

	CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
	CString msg;
	if( 0 == fiveChess.AIState )
		msg.Format(TEXT("�����ֵ�������ӡ�"));
	else if( 1 == fiveChess.AIState )
		msg.Format(TEXT("�����ֵ�������ӡ� ��������ڽ��� VCF ����"));
	else if( 2 == fiveChess.AIState )
		msg.Format(TEXT("�����ֵ�������ӡ� �����������ֹ��ҵ� VCF ����"));
	else if( 3 == fiveChess.AIState )
		msg.Format(TEXT("�����ֵ�������ӡ� ��������ڽ��� VCF �� VCT ���Ϲ���"));
	else if( 4 == fiveChess.AIState )
		msg.Format(TEXT("�����ֵ�������ӡ� �����������ֹ��ҵ� VCF �� VCT ���Ϲ���"));

	pFrm->m_wndStatusBar.SetPaneText(0,msg,1);

	CView::OnTimer(nIDEvent);
}


//��ֹ AI �߳�
void CMyFiveView::StopAIThread()
{
	// �����߳�
	if(m_pThrd)
	{
		HANDLE hp = m_pThrd->m_hThread;
		if(hp)
		{
			if(WaitForSingleObject(hp,1) != WAIT_OBJECT_0) 
			{
				TerminateThread(hp,0);
			}
			CloseHandle(hp);
		}
	}
}

// ����������������
// ͬʱҲ������������������� AI ������
void CMyFiveView::OnComputerPlayChess() 
{
	if(!startGame)
	{
		AfxMessageBox(TEXT("���ȵ����ʼ��Ϸ"));
		return ;
	}

	extern CMyFiveApp theApp;
	if(!theApp.AIHaveCalculated)
	{
		AfxMessageBox(TEXT("AI�߳����ڼ��㣬���Ե�"));
		return;
	}

	if(blackFall) 
	{
		fiveChess.cComputer = BLACK_CHESS;
		fiveChess.cPeople = WHITE_CHESS;
	}
	else
	{
		fiveChess.cComputer = WHITE_CHESS;
		fiveChess.cPeople = BLACK_CHESS;
	}

	DealP2C();
}
