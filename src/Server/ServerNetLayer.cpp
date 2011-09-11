#include "stdafx.h"
#include "ServerNetLayer.h"

bool mExit = false;
void startServer(int _port);

ServerNetLayer::ServerNetLayer()
: mMessageHandler(NULL)
, mThread(NULL)
{
	memset(mIP, 0, 64);
}

ServerNetLayer::~ServerNetLayer()
{

}

unsigned int ServerNetLayer::StartServer(int _port)
{
	mPort = _port;

	mThread = new CWinThread(startServer, (LPVOID)&mPort);
	mThread->CreateThread(CREATE_SUSPENDED);
	mThread->ResumeThread();

}

void ServerNetLayer::StopServer()
{
	mExit = true;
}

void ServerNetLayer::sendMessage(std::string& _message)
{
	//if ( mMessageHandler )
	//{
	//	mMessageHandler(_message);
	//}
}

void startServer(int _port)
{
	if (!AfxSocketInit())
	{
		//AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return 1;
	}

	mExit = false;

	//	CServerDlg *aDlg = (CServerDlg *)lParam;
	sendMessage(std::string("Server Started!"));

	UINT nPort = _port;

	//socket------------------------------------------------

	CSocket aSocket, serverSocket;

	if (!aSocket.Socket())
	{
		char szError[256] = {0};

		sprintf(szError, "Create Faild: %d", GetLastError());

		AfxMessageBox(szError);

		return 1; 
	}

	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);

	aSocket.SetSockOpt(SO_REUSEADDR, (void *)&bOptVal, bOptLen, SOL_SOCKET);


	if (!aSocket.Bind(nPort))
	{
		char szError[256] = {0};

		sprintf(szError, "Bind Faild: %d", GetLastError());

		AfxMessageBox(szError);

		return 1; 
	}

	if(!aSocket.Listen(10))
	{	
		char szError[256] = {0};

		sprintf(szError, "Listen Faild: %d", GetLastError());

		AfxMessageBox(szError);

		return 1;
	}


	CString strText;

	//	aDlg->GetDlgItemText(IDC_LOG, strText);

	strText += "Server Start! \r\n";

	//	aDlg->SetDlgItemText(IDC_LOG, strText);

	while(!mExit)
	{

		if(aSocket.Accept(serverSocket))
		{
			char szRecvMsg[256] = {0};
			char szOutMsg[256] = {0};	

			serverSocket.Receive(szRecvMsg, 256);

			sprintf(szOutMsg, "Receive Msg: %s \r\n", szRecvMsg);

			//			aDlg->GetDlgItemText(IDC_LOG, strText);

			strText += szOutMsg;

			//			aDlg->SetDlgItemText(IDC_LOG, strText);

			serverSocket.Send("Have Receive The Msg", 50);

			serverSocket.Close();
		}
		//Sleep(100);
	}

	aSocket.Close();
	serverSocket.Close();

	//	aDlg->GetDlgItemText(IDC_LOG, strText);

	strText += "Have Close!";

	//	aDlg->SetDlgItemText(IDC_LOG, strText);

	return 0;
}
