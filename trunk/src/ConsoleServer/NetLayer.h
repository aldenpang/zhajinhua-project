/********************************************************************
	created:	2011/09/09
	created:	9:9:2011   17:04
	filename: 	d:\My Projects\ZJHClient&Server\src\ConsoleServer\NetLayer.h
	file path:	d:\My Projects\ZJHClient&Server\src\ConsoleServer
	file base:	NetLayer
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef NetLayer_h__
#define NetLayer_h__

#include "pthread.h"

#define LOOP_INTERVAL 100

class NetLayer
{
public:
	NetLayer();	
	~NetLayer();

	int Initialize();
	int Start(const int _port);
	int Uninitialize();
protected:
	int parseMessages(Packet& _packet);

	static void *NetLayer::AcceptSocketMT(void* _arg);
	static void *NetLayer::ReceiveMessageMT(void* _arg);

	void removeSocket(SOCKET _socket);

private:
	static std::vector<SOCKET> mClientList;
};
#endif // NetLayer_h__