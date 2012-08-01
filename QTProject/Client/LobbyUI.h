/********************************************************************
	Copyright (C) 2005 by Pang Shuo
	@date:	    2012-7-15   15:12
	@file: 		LobbyUI.h
	@author:    Snake
	@desc:		Lobby界面的布局和逻辑
*********************************************************************/

#ifndef LOBBYUI_H
#define LOBBYUI_H

#include "BaseUI.h"
#include "Table.h"
#include "TableData.h"

class GameList;
class GameServerNet;
class LobbyUI : public BaseUI
{
	Q_OBJECT
public:
	LobbyUI();
	~LobbyUI();

	void Init();


signals:
	void SiQuit();
public slots:
	void StShowLobby(QVector<RoomInfo> _gameList);
private slots:
	void stTableJoin(quint32 _tableID, quint32 _seatID);
	void stConnectGS(QString _ip, quint32 _port);
	void stNetError(QString _err);
	void stGSConnected();
	void stGSLoginOK();
	void stGSLoginFailed(quint32 _errCode);
	void stTableJoinResult(quint32 _res, quint32 _tableID, quint32 _seatID);
	void stTableList(QMap<int, TableData> _tableData);
protected:
private:
	QMap<quint32, Table*> mTableList;
	QGraphicsScene* mScene;
	GameList* mGameList;
	GameServerNet* mGameServer;

	void regConnections();

	void initTables(quint32 _amount);

	void initGameServer();

};

#endif