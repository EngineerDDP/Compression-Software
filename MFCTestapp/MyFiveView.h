// MyFiveView.h : interface of the CMyFiveView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYFIVEVIEW_H__818E6AB0_3358_4472_AFC8_3461B0B72D08__INCLUDED_)
#define AFX_MYFIVEVIEW_H__818E6AB0_3358_4472_AFC8_3461B0B72D08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FiveChess.h"
#include "GameInfo.h"


class CMyFiveView : public CView
{
protected: // create from serialization only
	CMyFiveView();
	DECLARE_DYNCREATE(CMyFiveView)

// Attributes
public:
	CMyFiveDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFiveView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyFiveView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyFiveView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnP2p();
	afx_msg void OnUpdateP2p(CCmdUI* pCmdUI);
	afx_msg void OnP2c();
	afx_msg void OnUpdateP2c(CCmdUI* pCmdUI);
	afx_msg void OnStart();
	afx_msg void OnRegret();
	afx_msg void OnSet();
	afx_msg void OnNetBattle();
	afx_msg void OnUpdateNetBattle(CCmdUI* pCmdUI);
	afx_msg void OnAbout();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnComputerPlayChess();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	//����FiveChess ��������ã������������� CMyFiveApp ���е� FiveChess����
	FiveChess &fiveChess; 

	//��սģʽ�ı��
	bool isNetBattle; //����ģʽ�����˶�ս
	bool isP2P;       //����ģʽ�����˶�ս
	bool isP2C;       // ����ģʽ���˻���ս
	GameInfo gameInfo; //��Ϸ��Ϣ������ִ�ڻ���ִ�ף��н��ֻ����޽���

private:
	//λͼ��Ϣ
	CBitmap boardBmp;   //����
	CBitmap blackBmp;   //������
	CBitmap whiteBmp;   //������

	unsigned int boardWidth; //���̿��
	unsigned int boardHeight; //���̸߶�
	unsigned int chessWidth; //���ӿ��
	unsigned int chessHeight; //���Ӹ߶�

	//����λ����Ϣ
	unsigned int preX; //��һ���������
	unsigned int preY;

	unsigned int nowX; //��ǰ��һ���������
	unsigned int nowY;
	
	unsigned int tempX; //��������
	unsigned int tempY; 

	unsigned int moveX; // MouseMove()�ĸ�������
	unsigned int moveY; 

	//�߼����

	// BLACE_CHESS ��ʾ����Ӯ��WHITE_CHESS ��ʾ����Ӯ
	// SPACE ��ʾδ�ֳ�ʤ����PEACE_CHESS ��ʾ����
	bool startGame; //��ʾ��Ϸ�Ƿ�ʼ
	bool blackFall; //Ϊtrue��ʾӦ���Ǻ������ӣ�����Ӧ���ǰ�������

	// ����������
	int gameResult; //

	CWinThread *m_pThrd;  //���ں�̨ AI ����

	char paintMap[SIZE][SIZE];  // �ػ����ʱ�ã���ֹ AI �ļ�������˽���


	//�����Ǻ���

	void InitGame(); //��ʼ����Ϸ
	void InitBmp();  //����λͼ
	
	//������
	void DealP2C();

	void UpdateRect(); //������һ������λ�������Ŀ�

	// ��ʾ���
	void PrintResult();

	// ��ֹ AI �߳�
	void StopAIThread();

public:
	void DealNetBattle();

};

#ifndef _DEBUG  // debug version in MyFiveView.cpp
inline CMyFiveDoc* CMyFiveView::GetDocument()
   { return (CMyFiveDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFIVEVIEW_H__818E6AB0_3358_4472_AFC8_3461B0B72D08__INCLUDED_)
