/********************************************************************
	Copyright (C) 2005 by Bele Group
	@date:	    2011-12-31   14:48
	@file: 		ZJHGameServer.h
	@author:    Snake
	@desc:		
*********************************************************************/
#ifndef _ZJHGAMESERVER_H_
#define _ZJHGAMESERVER_H_

#include "PublicHeader.h"
#include "IServerLayer.h"
#include "ISocketInstance.h"


class ZjhGameServer : public IServerLayer
{
	Q_OBJECT
public slots:

private slots:
		
public:
	ZjhGameServer();
	~ZjhGameServer();

	virtual void PacketHandler(ISocketInstancePtr _incomeSocket, Packet* _packet);

protected:
	
private:
	void processLogin(ISocketInstancePtr _incomeSocket, Packet* _packet);
	void processTableJoin(ISocketInstancePtr _incomeSocket, Packet* _packet);

};

#endif //_ZJHGAMESERVER_H_

/*
 *
 * [Revision 1.0   2011-12-31   14:48  Administrator] Created
 *
 */
