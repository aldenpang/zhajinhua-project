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
#include "GSPlayer.h"
#include <QTimer>
#include "SharedData.h"

using namespace SharedData;

class ZjhGameServer : public IServerLayer
{
	Q_OBJECT
public slots:

private slots:
		
public:
	ZjhGameServer();
	~ZjhGameServer();

	virtual void PacketHandler(ISocketInstancePtr _incomeSocket, Packet& _packet);
	virtual void ClientDisconnected(ISocketInstancePtr _clientSocket);

	void SetRoomInfo(RoomInfo _info);

private:
	void processLogin(ISocketInstancePtr _incomeSocket, Packet& _packet);
	void processTableJoin(ISocketInstancePtr _incomeSocket, Packet& _packet);
	void processTableLeave(ISocketInstancePtr _incomeSocket, Packet& _packet);
	void processSyncStart(ISocketInstancePtr _incomeSocket, Packet& _packet);
	void processFollow(ISocketInstancePtr _incomeSocket, Packet& _packet);
	void processBringMoney(ISocketInstancePtr _incomeSocket, Packet& _packet);
	void processGiveUp(ISocketInstancePtr _incomeSocket, Packet& _packet);
	
private slots:
	void stRefershTables();
private:
	QTimer mTimer;
	QList<GSPlayerPtr> mPlayerList;
	RoomInfo mRoomInfo;

private:
	void sendTableInfo(GSPlayerPtr _to);
	GSPlayerPtr findPlayer(ISocketInstancePtr _input);
	void deletePlayer(GSPlayerPtr _player);
};

#endif //_ZJHGAMESERVER_H_

/*
 *
 * [Revision 1.0   2011-12-31   14:48  Administrator] Created
 *
 */
