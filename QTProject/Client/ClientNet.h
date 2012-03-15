/********************************************************************
	created:	2012/03/07
	created:	7:3:2012   11:20
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\Client\ClientNet.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\Client
	file base:	ClientNet
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef ClientNet_h__
#define ClientNet_h__
#include "INetLayer.h"

class ClientNet : public INetLayer
{
public:
	ClientNet();
	~ClientNet();

	virtual void PacketHandler(Packet* _packet);

protected:
private:
	void processLogin(Packet* _packet);
	void processGameList(Packet* _packet);
};
#endif // ClientNet_h__