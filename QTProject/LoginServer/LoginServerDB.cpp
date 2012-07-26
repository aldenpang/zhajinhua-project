#include "LoginServerDB.h"


LoginServerDB::LoginServerDB()
{

}

LoginServerDB::~LoginServerDB()
{

}

int LoginServerDB::VerifyUser( QString& _user, QString& _pwd )
{
	QString sql = QString("select * from Accounts where UserName like \"%1\" and Password like \"%2\"").arg(_user).arg(_pwd);
	QSqlQuery q = mDb.exec(sql);

	int status = 0;
	int records = 0;
 	while (q.next()) 
 	{
 		status = q.value(8).toInt();		// get player's status
 		records++;
		if ( records >=2 )
			break;
 	}

	if( records == 0)
	{
		LOG_INFO(QString("Can not find record for player[%1] pwd[%2]").arg(_user).arg(_pwd));
		return 1;
	}
	else if (records == 1)
	{
		if ( status == 0 )
		{
			return LOGIN_OK;
		}
		else if ( status == 1 )		// 1 is frezze
		{
			return ERR_LS_USER_FREEZED;
		}
		else
			return -1;
	}
	else if ( records > 1 )
	{
		LOG_ERR("Query result is more than 1");
		return -1;
	}
	else 
	{
		QString err = q.lastError().text();
		LOG_ERR(err);
		return -2;
	}
		

// 	while (q.next()) 
// 	{
// 		QString country = q.value(0).toString();
// 		int ii = 0;
// 	}

//	return 0;
}

int LoginServerDB::RegUser( QString& _user, QString& _pwd )
{
	// 根据username的记录数判断是否此username已经被注册
	QString sql = QString("select * from Accounts where UserName like \"%1\"").arg(_user);
	QSqlQuery q = mDb.exec(sql);
	int records = 0;
	while (q.next()) 
	{
		records++;
		if ( records >= 1 )
		{
			LOG_INFO(QString("UserName[%1] is already exist").arg(_user));
			return 1;
		}
	}

	sql = QString("insert into Accounts(UserName, Password) values(\"%1\", \"%2\")").arg(_user).arg(_pwd);
	q = mDb.exec(sql);
	return 0;
}

int LoginServerDB::GetRoomConfig( int _gameType, QVector<RoomInfo>& _info )
{
	QString sql = QString("select * from Config where GameType=\"%1\"").arg(_gameType);
	QSqlQuery q = mDb.exec(sql);

	while (q.next()) 
	{
		RoomInfo info;
		info.mName = q.value(0).toString();
		info.mType = (RoomType)(q.value(1).toInt());
		info.mIP = q.value(2).toString();
		info.mPort  = q.value(3).toInt();
		info.mScore = q.value(4).toInt();
		info.mMoneyType = (MoneyType)(q.value(5).toInt());
		info.mRoomID = q.value(6).toInt();
		_info.push_back(info);
	}

	if ( _info.isEmpty() )
	{
		LOG_ERR("Can not load any room config!");
		return 1;
	}
	else
		return 0;
}
