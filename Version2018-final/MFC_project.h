
// MFC_project.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFC_projectApp:
// See MFC_project.cpp for the implementation of this class
//

class CMFC_projectApp : public CWinApp
{
public:
	CMFC_projectApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFC_projectApp theApp;