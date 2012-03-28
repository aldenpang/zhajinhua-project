/********************************************************************
	created:	2012/03/28
	created:	28:3:2012   17:23
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer\WalletServerLayer.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer
	file base:	WalletServerLayer
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef WalletServerLayer_h__
#define WalletServerLayer_h__
#include "INetLayer.h"

class WalletServerLayer : public INetLayer
{
public:
	virtual void PacketHandler(Packet& _packet);
protected:
private:
};

#endif // WalletServerLayer_h__