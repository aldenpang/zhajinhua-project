// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerDlg dialog
CWinThread *pThread = NULL;

//bool m_exit = false;
//unsigned int StartServer(LPVOID lParam)
//{
//	if (!AfxSocketInit())
//	{
//		//AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
//		return 1;
//	}
//
//	m_exit = false;
//
//	CServerDlg *aDlg = (CServerDlg *)lParam;
//
//	UINT nPort = 5000;
//
//	//socket------------------------------------------------
//
//	CSocket aSocket, serverSocket;
//
//	if (!aSocket.Socket())
//	{
//		char szError[256] = {0};
//
//		sprintf(szError, "Create Faild: %d", GetLastError());
//
//		AfxMessageBox(szError);
//
//		return 1; 
//	}
//
//	BOOL bOptVal = TRUE;
//	int bOptLen = sizeof(BOOL);
//
//	aSocket.SetSockOpt(SO_REUSEADDR, (void *)&bOptVal, bOptLen, SOL_SOCKET);
//
//
//	if (!aSocket.Bind(nPort))
//	{
//		char szError[256] = {0};
//
//		sprintf(szError, "Bind Faild: %d", GetLastError());
//
//		AfxMessageBox(szError);
//
//		return 1; 
//	}
//
//	if(!aSocket.Listen(10))
//	{	
//		char szError[256] = {0};
//
//		sprintf(szError, "Listen Faild: %d", GetLastError());
//
//		AfxMessageBox(szError);
//
//		return 1;
//	}
//
//
//	CString strText;
//
//	aDlg->GetDlgItemText(IDC_LOG, strText);
//
//	strText += "Server Start! \r\n";
//
//	aDlg->SetDlgItemText(IDC_LOG, strText);
//
//	while(!m_exit)
//	{
//
//		if(aSocket.Accept(serverSocket))
//		{
//			char szRecvMsg[256] = {0};
//			char szOutMsg[256] = {0};	
//
//			serverSocket.Receive(szRecvMsg, 256);
//
//			sprintf(szOutMsg, "Receive Msg: %s \r\n", szRecvMsg);
//
//			aDlg->GetDlgItemText(IDC_LOG, strText);
//
//			strText += szOutMsg;
//
//			aDlg->SetDlgItemText(IDC_LOG, strText);
//
//			serverSocket.Send("Have Receive The Msg", 50);
//
//			serverSocket.Close();
//		}
//
//	}
//
//	aSocket.Close();
//	serverSocket.Close();
//
//	aDlg->GetDlgItemText(IDC_LOG, strText);
//
//	strText += "Have Close!";
//
//	aDlg->SetDlgItemText(IDC_LOG, strText);
//
//	return 0;
//}
//
//void StopServer()
//{
//	m_exit = true;
//}



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CServerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CServerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDCANCEL2, &CServerDlg::OnBnClickedCancel2)
END_MESSAGE_MAP()


// CServerDlg message handlers
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	_CrtDumpMemoryLeaks();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	this->GetDlgItem(IDCANCEL)->EnableWindow(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServerDlg::OnBnClickedOk()
{
	//pThread = new CWinThread(mNetLayer.StartServer, (LPVOID)this);

	//pThread->CreateThread(CREATE_SUSPENDED);

	//pThread->ResumeThread();

	mNetLayer.StartServer(5000);

	this->GetDlgItem(IDOK)->EnableWindow(false);
	this->GetDlgItem(IDCANCEL)->EnableWindow(true);	
}

void CServerDlg::OnBnClickedCancel()
{
	mNetLayer.StopServer();

	this->GetDlgItem(IDOK)->EnableWindow(true);
	this->GetDlgItem(IDCANCEL)->EnableWindow(false);

	if ( pThread == NULL )
		return;

	pThread->SuspendThread();

	delete pThread;

	pThread = NULL;

}

void CServerDlg::OnBnClickedCancel2()
{
	// TODO: Add your control notification handler code here
	OnBnClickedCancel();
	PostQuitMessage(0);
}

void CServerDlg::MessageHandler( std::string& _message )
{
	return;
}
