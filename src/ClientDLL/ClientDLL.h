// ClientDLL.h : main header file for the ClientDLL DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CClientDLLApp
// See ClientDLL.cpp for the implementation of this class
//

class CClientDLLApp : public CWinApp
{
public:
	CClientDLLApp();

// Overrides
public:
	virtual BOOL InitInstance();


	DECLARE_MESSAGE_MAP()
};

void SendClientMessage(const char* _msg);
