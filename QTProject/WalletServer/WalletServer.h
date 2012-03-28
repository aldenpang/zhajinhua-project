/********************************************************************
	created:	2012/03/26
	created:	26:3:2012   16:09
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\WalletServer\WalletServer.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\WalletServer
	file base:	WalletServer
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef WalletServer_h__
#define WalletServer_h__
#include "IServerLayer.h"

class WalletServer : public IServerLayer
{
public:
	WalletServer();
	~WalletServer();
	virtual void PacketHandler(ISocketInstancePtr _incomeSocket, Packet& _packet);
protected:
	void processQueryUserWallet( ISocketInstancePtr _incomeSocket, Packet& _p);
	void processQueryTableWallet( ISocketInstancePtr _incomeSocket, Packet& _p);
	void processInserTableWallet( ISocketInstancePtr _incomeSocket, Packet& _p);

private:
};


#endif // WalletServer_h__