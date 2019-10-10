// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__9690BC7D_3001_4B1E_B512_564C6B13857E__INCLUDED_)
#define AFX_STDAFX_H__9690BC7D_3001_4B1E_B512_564C6B13857E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT



/*
     SIZE Ϊ���̵ĳ��Ϳ�
	 OFFSETX,OFFSETY �ֱ�Ϊ���̱߽�����
	 WIDTH Ϊ�����

	 WHITE_CHESS ��ʾ ����
	 BLACK_CHESS ��ʾ ����
	 SPACE ��ʾ�ո�ͬʱ����ֵҲ�ɱ�ʾδ�ֳ�ʤ��
	 PEACE_CHESS ��ʾ����

 */

#define SIZE 15
#define OFFSETX 7 
#define OFFSETY 5
#define WIDTH 35 


#define WHITE_CHESS 'W'
#define BLACK_CHESS 'B'
#define SPACE ' '
#define PEACE_CHESS '='


#define CLR_BLACK RGB(0,0,0)
#define CLR_WHITE RGB(255,255,255)
#define CLR_RED   RGB(255,0,0)

// ����ı���ɫ
#define CLR_BK_WHITE RGB (226,226,226)
#define CLR_BK_BLACK RGB (53,53,53)

//���ӵı���ɫ
#define CLR_MAGENTA RGB(255,0,255)



#include <Winsock2.h>   //��Ϊ������ winsock 2.0 �汾�ĺ���



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__9690BC7D_3001_4B1E_B512_564C6B13857E__INCLUDED_)
