/********************************************************************
	Copyright (C) 2013 by Alden Pang
	@date:	    2012-7-15   15:12
	@file: 		LobbyUI.h
	@author:    Alden Pang
	@desc:		Lobby界面的布局和逻辑
*********************************************************************/

#ifndef LOBBYUI_H
#define LOBBYUI_H

#include "BaseUI.h"
#include "Table.h"
#include "TableData.h"
#include "CommonPlayer.h"
#include "TableInfo.h"

class GameList;
class GameServerNet;
class LobbyUI : public BaseUI
{
	Q_OBJECT
public:
	LobbyUI();
	~LobbyUI();

	void Init();

	GameServerNet* GetGameServerNet(){return mGameServer;}

signals:
	void SiQuit();
	void SiShowGame();
	void SiMyTable(quint32 _tableID, quint32 _seatID);

public slots:
	void StShowLobby(QVector<RoomInfo> _gameList);
	void StUpdatePlayerInfo(CommonPlayer _player);
	void StGetLoginInfo(QString _username, QString _pwd);

private slots:
	void stTableJoin(quint32 _tableID, quint32 _seatID);
	void stConnectGS(RoomInfo _roomInfo);
	void stNetError(QString _err);
	void stGSConnected();
	void stGSLoginOK();
	void stGSLoginFailed(quint32 _errCode);
	void stTableJoinResult(quint32 _res, quint32 _tableID, quint32 _seatID, TablePlayer _player);
	void stTableLeaveResult(quint32 _res, quint32 _tableID, TablePlayer _player);
	void stTableList(QMap<int, TableData> _tableData);
	void stBringMoney(quint32 _tableID, quint32 _seatID, quint32 _amount);
	void stBringMoneyRes( int _res );
	void stUpdateMoney(quint32 _goldCoin, quint32 _silverCoin);
protected:
private:
	QMap<quint32, Table*> mTableList;
	QGraphicsScene* mScene;
	GameList* mGameList;
	GameServerNet* mGameServer;
	QLabel* mSelectNotice;
	QGraphicsView* mTableListView;
	QString mUserName;
	QString mPassword;
	quint32 mMyTableID;
	quint32 mMySeatID;
	

	void regConnections();
	void initTables(quint32 _amount);
	void initGameServer();
	void refreshPlayerMoney();

};

#endif