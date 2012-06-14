#include "WalletServerDB.h"

WalletServerDB::WalletServerDB()
{

}

WalletServerDB::~WalletServerDB()
{

}

int WalletServerDB::QueryUserWallet( quint32 _accountID, quint32& _coinAmount )
{
	QString sql = QString("select * from UserWallet where AccountID=%1").arg(_accountID);
	QSqlQuery q = mDb.exec(sql);

	int records = 0;
	while (q.next()) 
	{
		_coinAmount = q.value(2).toInt();
		records++;
		if ( records >=2 )
			break;
	}

	if( records == 1 )
		return WS_NO_ERR;
	else if(records < 1 )
		return ERR_WS_WALLET_NOT_FOUND;
	else
		return ERR_WS_MULTI_RESULT;
}

int WalletServerDB::QueryTableWallet( quint32 _roomID, quint32 _tableID, quint32 _seatID, quint32& _coinAmount )
{
	QString sql = QString("select * from TableWallet where RoomID=%1 and TableID=%2 and SeatID=%3").arg(_roomID).arg(_tableID).arg(_seatID);
	QSqlQuery q = mDb.exec(sql);

	int records = 0;
	while (q.next()) 
	{
		_coinAmount = q.value(4).toInt();
		records++;
		if ( records >=2 )
			break;
	}

	if( records == 1 )
		return WS_NO_ERR;
	else if(records < 1 )
		return ERR_WS_WALLET_NOT_FOUND;
	else
		return ERR_WS_MULTI_RESULT;

	return WS_NO_ERR;
}

int WalletServerDB::InsertTableWallet( quint32 _roomID, quint32 _tableID, quint32 _seatID)
{
	QString sql = QString("select * from TableWallet where RoomID=%1 and TableID=%2 and SeatID=%3").arg(_roomID).arg(_tableID).arg(_seatID);
	QSqlQuery q = mDb.exec(sql);

	int records = 0;
	while (q.next()) 
	{
		records++;
		if ( records >=2 )
			break;
	}
	if ( records == 1 )
	{
		return ERR_WS_TABLE_WALLET_EXIST;
	}
	else if( records > 1 )
	{
		return ERR_WS_MULTI_RESULT;
	}
	else 
	{
		sql = QString("insert into TableWallet(RoomID, TableID, SeatID) values(%1,%2,%3)").arg(_roomID).arg(_tableID).arg(_seatID);
		q = mDb.exec(sql);
	}

	return WS_NO_ERR;

}

int WalletServerDB::UpdateTableWallet( quint32 _roomID, quint32 _tableID, quint32 _seatID, quint32 _coinAmount )
{
	QString sql = QString("update TableWallet set Amount=%1 where RoomID=%2 and TableID=%3 and SeatID=%4")
			.arg(_coinAmount).arg(_roomID).arg(_tableID).arg(_seatID);
	QSqlQuery q = mDb.exec(sql);

	return WS_NO_ERR;
}

int WalletServerDB::UpdateUserWallet( quint32 _accountID, quint32 _coinAmount )
{
	QString sql = QString("update UserWallet set Amount=%1 where AccountID=%2")
		.arg(_coinAmount).arg(_accountID);
	QSqlQuery q = mDb.exec(sql);

	return WS_NO_ERR;
}
