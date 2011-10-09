// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "../Shared/Packet.h"
#include "../ConsoleDll/ConsoleDll.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientDlg dialog




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CClientDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	ClientNetLayer_Initialize();

	ClientNetLayer_Connect(std::string("192.168.2.2"), 5000);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientDlg::OnBnClickedOk()
{
	Packet pk;
	pk.SetMessage(102);
	int len = pk.GetDataLength();
	pk.Put(100);
	len = pk.GetDataLength();
	pk.Put(200);
	len = pk.GetDataLength();
	pk.Put(300);
	len = pk.GetDataLength();
	pk.SetHeader();
	//char temp[MAX_PACKET_SIZE] = {0};
	//pk.GetData(temp);
	const char* temp = pk.GetData();
	len = pk.GetDataLength();

	ClientNetLayer_SendPacket(&pk);
	//ClientNetLayer_Send("abcd0000abcd");

	////CClientDLLApp dllApp;
	////dllApp.SendMessage("MessageFromClientDLL");

	////SendClientMessage("MessageFromClientDLL");

	//AfxSocketInit();

	//CSocket aSocket;

	//CString strIP = "192.168.1.101";
	//CString strPort = "5000";
	//CString strText = "pangshuopangshuo0";

	//// 	this->GetDlgItem(IDC_EDIT_IP)->GetWindowText(strIP);
	//// 	this->GetDlgItem(IDC_EDIT_PORT)->GetWindowText(strPort);
	//// 	this->GetDlgItem(IDC_EDIT_TEXT)->GetWindowText(strText);

	//if(!aSocket.Create())
	//{
	//	char szMsg[1024] = {0};

	//	sprintf(szMsg, "create faild: %d", aSocket.GetLastError());

	//	AfxMessageBox(szMsg);
	//	return;
	//}

	//int nPort = atoi(strPort);

	//if(aSocket.Connect(strIP, nPort))
	//{
	//	char szRecValue[1024] = {0};

	//	aSocket.Send(strText, strText.GetLength());

	//	aSocket.Receive((void *)szRecValue, 1024);

	//	AfxMessageBox(szRecValue);
	//}
	//else
	//{
	//	char szMsg[1024] = {0};

	//	sprintf(szMsg, "create faild: %d", aSocket.GetLastError());

	//	AfxMessageBox(szMsg);
	//}

	//aSocket.Close();
}
