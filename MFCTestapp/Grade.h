#if !defined(AFX_GRADE_H__661B7D51_E92E_4CC6_B665_8ECD963331FE__INCLUDED_)
#define AFX_GRADE_H__661B7D51_E92E_4CC6_B665_8ECD963331FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Grade.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Grade dialog

class Grade : public CDialog
{
// Construction
public:
	Grade(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Grade)
	enum { IDD = IDD_Grade };
	UINT	m_depth;
	UINT	m_de_depth;
	UINT	m_depth_end;
	UINT	m_depth_dis;
	UINT	m_de_depth_dis;
	UINT	m_de_depth_end;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Grade)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Grade)
	afx_msg void OnGrade1();
	afx_msg void OnGrade2();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int grade;  // grade Ϊ 1 ��ʾ�ȼ�Ϊ grade1 , Ϊ 2 ��ʾ Ϊ grade2

	// VC ������ȡ�Ĳ������ȴ�С��ȿ�ʼ�ѣ�û���ҵ���Ӯ��Ͳ��ϵļӴ�������ȣ�����һ��Ҫ��1��
	// ���Ǽ�һ���ض���ֵ������ǵ���ֵ��ͬ��Ҳ�з�����ȡ������������Դ�����AI�Ĺ����ͷ���������
	// �����ѵ�ʱ����ܽ϶࣬��������Ϊ�û��Լ����룬�����������㲻ͬ���û���
	// �����������QQ��Ϸ���������Ҫ��һ�㣬ƽʱ��ľ���Ҫ�ܸߵ���������Щ���������������øı��ٶȺ�������

	int VCDEPTH;  // AI ���� VC �����Ŀ�ʼ���
	int VCDEPTH_END;  // AI ����VC������������
	int VCDEPTH_DIS;  // AI ���� VC ��������� ����ֵ

	int VCDE_DEPTH;  // AI ���� VC �����Ŀ�ʼ���
	int VCDE_DEPTH_END; // AI ���� VC ������������
	int VCDE_DEPTH_DIS; // AI ���� VC ��������� ����ֵ
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADE_H__661B7D51_E92E_4CC6_B665_8ECD963331FE__INCLUDED_)
