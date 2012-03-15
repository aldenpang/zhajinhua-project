/********************************************************************
	created:	2012/03/05
	created:	5:3:2012   14:47
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\LoginServer\LoginServer.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\LoginServer
	file base:	LoginServer
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef LoginServer_h__
#define LoginServer_h__

#include "IServerLayer.h"

class LoginServer : public IServerLayer
{
public:
	virtual void PacketHandler(ISocketInstancePtr _incomeSocket, Packet* _packet);

protected:
private:
	void processClientLogin(ISocketInstancePtr _incomeSocket, Packet* _packet);
	void processClientReqGameList(ISocketInstancePtr _incomeSocket, Packet* _packet);

	void sendGameList(int _gameType, ISocketInstancePtr _toSocket);
};

#endif // LoginServer_h__