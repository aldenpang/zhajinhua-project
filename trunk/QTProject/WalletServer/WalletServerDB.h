/********************************************************************
	created:	2012/03/26
	created:	26:3:2012   16:18
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\WalletServer\WalletServerDB.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\WalletServer
	file base:	WalletServerDB
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef WalletServerDB_h__
#define WalletServerDB_h__
#include "DBLayer.h"
#include "SharedData.h"
using namespace SharedData;

class WalletServerDB : public DBLayer, public Singleton<WalletServerDB>
{
public:
	WalletServerDB();
	~WalletServerDB();


protected:
private:
};

#endif // WalletServerDB_h__