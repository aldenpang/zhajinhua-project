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

	int QueryUserWallet(quint32 _accountID, quint32& _coinAmount);

	int QueryTableWallet(quint32 _roomID, quint32 _tableID, quint32 _seatID, quint32& _coinAmount);

	int UpdateUserWallet(quint32 _accountID, quint32 _coinAmount);

	// 先查询有没有此TableWallet，如果有就不插入，如果没有就插入，但是不刷新CoinAmount
	int InsertTableWallet(quint32 _roomID, quint32 _tableID, quint32 _seatID);

	int UpdateTableWallet(quint32 _roomID, quint32 _tableID, quint32 _seatID, quint32 _coinAmount);

	// Check if has room-relatived rake record in wallet db, if not, insert a new recored, if has, insert the increment of rake amount
	int InsertRake( quint32 _roomID, quint32 _rake );

protected:
private:
};

#define WalletDB WalletServerDB::GetSingleton()

#endif // WalletServerDB_h__