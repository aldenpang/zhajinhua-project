/********************************************************************
	created:	2011/09/06
	created:	6:9:2011   17:36
	filename: 	d:\My Projects\ZJHClient&Server\src\Shared\ServerNetLayer.h
	file path:	d:\My Projects\ZJHClient&Server\src\Shared
	file base:	ServerNetLayer
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef ServerNetLayer_h__
#define ServerNetLayer_h__

#include "Packet.h"

class ServerNetLayer
{
public:
	ServerNetLayer();
	~ServerNetLayer();

	unsigned int StartServer(int _port);
	void StopServer();

	//void RegisterMessageHandler(void(*(CServerDlg::_messageHandler))(std::string&)){ mMessageHandler = _messageHandler; }

protected:
private:
	char mIP[64];
	int mPort;

	CWinThread *mThread;

	void(*mMessageHandler)(std::string&);

private:
	void sendMessage(std::string& _message);

};

#endif // ServerNetLayer_h__