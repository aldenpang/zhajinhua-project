/********************************************************************
	created:	2012/03/06
	created:	6:3:2012   16:15
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\LoginServer\LoginServerDB.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\LoginServer
	file base:	LoginServerDB
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef LoginServerDB_h__
#define LoginServerDB_h__

#include "DBLayer.h"
class LoginServerDB : public DBLayer, public Singleton<LoginServerDB>
{
public:
	LoginServerDB();
	~LoginServerDB();

	int VerifyUser( QString& _user, QString& _pwd );
protected:
private:
};

#define DB LoginServerDB::GetSingleton()

#endif // LoginServerDB_h__