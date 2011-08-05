// ClientDLL.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ClientDLL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CClientDLLApp

BEGIN_MESSAGE_MAP(CClientDLLApp, CWinApp)
END_MESSAGE_MAP()


// CClientDLLApp construction

CClientDLLApp::CClientDLLApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CClientDLLApp object

CClientDLLApp theApp;


// CClientDLLApp initialization

BOOL CClientDLLApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	return TRUE;
}

void SendClientMessage( const char* _msg )
{
	AfxSocketInit();

	CSocket aSocket;

	CString strIP = "192.168.2.2";
	CString strPort = "5000";
	CString strText = _msg;

	// 	this->GetDlgItem(IDC_EDIT_IP)->GetWindowText(strIP);
	// 	this->GetDlgItem(IDC_EDIT_PORT)->GetWindowText(strPort);
	// 	this->GetDlgItem(IDC_EDIT_TEXT)->GetWindowText(strText);

	if(!aSocket.Create())
	{
		char szMsg[1024] = {0};

		sprintf(szMsg, "create faild: %d", aSocket.GetLastError());

		AfxMessageBox(szMsg);
		return;
	}

	int nPort = atoi(strPort);

	if(aSocket.Connect(strIP, nPort))
	{
		char szRecValue[1024] = {0};

		aSocket.Send(strText, strText.GetLength());

		aSocket.Receive((void *)szRecValue, 1024);

		AfxMessageBox(szRecValue);
	}
	else
	{
		char szMsg[1024] = {0};

		sprintf(szMsg, "create faild: %d", aSocket.GetLastError());

		AfxMessageBox(szMsg);
	}

	aSocket.Close();
}
