
// hkcrash.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "hkcrash_i.h"


// ChkcrashApp: 
// �йش����ʵ�֣������ hkcrash.cpp
//

class ChkcrashApp : public CWinApp
{
public:
	ChkcrashApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	BOOL ExitInstance();
};

extern ChkcrashApp theApp;