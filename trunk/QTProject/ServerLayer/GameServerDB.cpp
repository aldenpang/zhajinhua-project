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
		return 1;
	}
	else if (records == 1)
		return 0;
	else if ( records > 1 )
	{
		LOG_ERR("Query result is more than 1");
		return 2;
	}
	else 
	{
		QString err = q.lastError().text();
		LOG_ERR(err);
		return -1;
	}
}
