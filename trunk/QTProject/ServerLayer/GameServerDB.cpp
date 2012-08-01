#include "GameServerDB.h"

GameServerDB::GameServerDB()
{

}

GameServerDB::~GameServerDB()
{

}

int GameServerDB::VerifyUser( QString& _user, QString& _pwd )
{
	QString sql = QString("select * from Accounts where UserName like \"%1\" and Password like \"%2\"").arg(_user).arg(_pwd);
	//QString sql = QString("insert into Accounts(UserName, Password) values(\"acc3\", \"1111\")");
	QSqlQuery q = mDb.exec(sql);

	int records = 0;
	while (q.next()) 
	{
		//QString country = q.value(0).toString();
		records++;
		if ( records >=2 )
			break;
	}

	if( records == 0)
	{
		LOG_INFO(QString("Can not find record for player[%1] pwd[%2]").arg(_user).arg(_pwd));
		return ERR_GS_PLAYER_NOT_FOUND;
	}
	else if (records == 1)
		return GS_NO_ERR;
	else if ( records > 1 )
	{
		LOG_ERR("Query result is more than 1");
		return ERR_GS_MULTI_RESULT;
	}
	else 
	{
		QString err = q.lastError().text();
		LOG_ERR(err);
		return -1;
	}
}

int GameServerDB::GetRoomInfo( int _roomID, RoomInfo& _info )
{
	QString sql = QString("select * from Config where RoomID=\"%1\"").arg(_roomID);
	QSqlQuery q = mDb.exec(sql);

	int records = 0;
	while (q.next()) 
	{
		_info.mName = q.value(0).toString();
		_info.mType = (RoomType)(q.value(1).toInt());
		_info.mIP = q.value(2).toString();
		_info.mPort = q.value(3).toInt();
		_info.mScore = q.value(4).toInt();
		_info.mMoneyType = (MoneyType)(q.value(5).toInt());
		_info.mRoomID = q.value(6).toInt();

		records++;
		if ( records >=2 )
			break;
	}
	if ( records  == 0 )
		return ERR_GS_ROOM_NOT_FOUND;
	else if ( records >= 2)
		return ERR_GS_MULTI_RESULT;
	else
		return GS_NO_ERR;
}

int GameServerDB::GetPlayerInfo( GSPlayerPtr _player )
{
	QString sql = QString("select * from Accounts where AccountID=%1").arg(_player->GetAccountID());
	QSqlQuery q = mDb.exec(sql);

	int records = 0;
	while (q.next()) 
	{
		_player->SetNickName(q.value(3).toString());
		_player->SetGender(q.value(4).toInt());
		_player->SetExp(q.value(5).toInt());
		_player->SetPlayTime(q.value(6).toInt());
		_player->SetProtraitID(q.value(6).toInt());
		records++;
		if ( records >=2 )
			break;
	}

	return GS_NO_ERR;
}

int GameServerDB::GetAccountID( QString& _user, quint32& _accountID )
{
	QString sql = QString("select * from Accounts where UserName like \"%1\"").arg(_user);
	QSqlQuery q = mDb.exec(sql);

	int records = 0;
	while (q.next()) 
	{
		_accountID = q.value(2).toInt();
		records++;
		if ( records >=2 )
			break;
	}
	if ( records  == 0 )
		return ERR_GS_PLAYER_NOT_FOUND;
	else if ( records >= 2)
		return ERR_GS_MULTI_RESULT;
	else
		return GS_NO_ERR;
}

int GameServerDB::UpdatePlayerIp( int _accountID, QString& _ip )
{
	QString sql = QString("update Accounts set IP=\"%1\" where AccountID=%2").arg(_ip).arg(_accountID);
	QSqlQuery q = mDb.exec(sql);

	return GS_NO_ERR;
}
