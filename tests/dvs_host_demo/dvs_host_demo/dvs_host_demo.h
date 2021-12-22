
// dvs_host_demo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CdvshostdemoApp:
// See dvs_host_demo.cpp for the implementation of this class
//

class CdvshostdemoApp : public CWinApp
{
public:
	CdvshostdemoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CdvshostdemoApp theApp;
