
// 3DModeling.h : 3DModeling Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMy3DModelingApp:
// �йش����ʵ�֣������ 3DModeling.cpp
//

class CMy3DModelingApp : public CWinApp
{
public:
	CMy3DModelingApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMy3DModelingApp theApp;
