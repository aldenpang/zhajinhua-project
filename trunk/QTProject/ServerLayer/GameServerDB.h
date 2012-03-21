/********************************************************************
	created:	2012/03/09
	created:	9:3:2012   15:26
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer\GameServerDB.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer
	file base:	GameServerDB
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef GameServerDB_h__
#define GameServerDB_h__

#include "DBLayer.h"
#include "GSPlayer.h"
#include "SharedData.h"
using namespace SharedData;

class GameServerDB : public DBLayer, public Singleton<GameServerDB>
{
public:
	GameServerDB();
	~GameServerDB();

	int VerifyUser(QString& _user, QString& _pwd);

	int GetRoomInfo(int _roomID, RoomInfo& _info);

	int GetAccountID(QString& _user, quint32& _accountID);

	int GetPlayerInfo(GSPlayerPtr _player);

	int UpdatePlayerIp(int _accountID, QString& _ip);

protected:
private:
};

#define DB GameServerDB::GetSingleton()

#endif // GameServerDB_h__