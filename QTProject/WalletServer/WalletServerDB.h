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

	// �Ȳ�ѯ��û�д�TableWallet������оͲ����룬���û�оͲ��룬���ǲ�ˢ��CoinAmount
	int InsertTableWallet(quint32 _roomID, quint32 _tableID, quint32 _seatID);

	// ˢ��CoinAmount
	int UpdateTableWallet(quint32 _roomID, quint32 _tableID, quint32 _seatID, quint32 _coinAmount);

protected:
private:
};

#define WalletDB WalletServerDB::GetSingleton()

#endif // WalletServerDB_h__