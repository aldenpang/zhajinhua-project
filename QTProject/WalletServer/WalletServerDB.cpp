#include "WalletServerDB.h"
#include "LogModule.h"

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
	int tableAmount = 0;
	while (q.next()) 
	{
		tableAmount = q.value(4).toInt();
		records++;
		if ( records >=2 )
			break;
	}
	if ( records == 1 )
	{
		if ( tableAmount == 0 )
		{
			return WS_NO_ERR;
		}
		else
		{
			LOG_D_ERR(QString("this table wallet exist and coin is [%1]").arg(tableAmount));
			return ERR_WS_TABLE_WALLET_EXIST;
		}
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

int WalletServerDB::InsertRake( quint32 _roomID, quint32 _rake )
{
	QString sql = QString("select * from RakeWallet where RoomID=%1 ").arg(_roomID);
	QSqlQuery q = mDb.exec(sql);

	int totalRake = 0;
	int records = 0;
	while (q.next()) 
	{
		totalRake = q.value(1).toInt();
		records++;
		if ( records >=2 )
			break;
	}
	if( records > 1 )
	{
		return ERR_WS_MULTI_RESULT;
	}
	else if( records == 0 )
	{
		sql = QString("insert into RakeWallet(RoomID, Rake) values(%1,%2)").arg(_roomID).arg(_rake);
		q = mDb.exec(sql);
		return WS_NO_ERR;
	}
	else		// record == 1, means already have a record, need query rake first, and add increment
	{
		sql = QString("update RakeWallet set Rake=%1 where RoomID=%2").arg(totalRake + _rake).arg(_roomID);
		q = mDb.exec(sql);

		return WS_NO_ERR;
	}


}

int WalletServerDB::InsertTransactionRecord( TransactionType _type, quint32 _amount, quint32 _fromID, quint32 _toID, quint32 _result )
{
	QString typeStr;
	switch ( _type )
	{
	case UserToTable:
		typeStr = "UserToTable";
		break;
	case TableToTable:
		typeStr = "TableToTable";
		break;
	case TableToUser:
		typeStr = "TableToUser";
		break;
	case TableToRake:
		typeStr = "TableToRake";
		break;
	case SilverToTable:
		typeStr = "SilverToTable";
		break;
	case TableToSilver:
		typeStr = "TableToSilver";
		break;
	default:
		typeStr = "UnknowTransactionType";
		break;
	}

	QString sql = QString("insert into TransactionRecord(Type, TypeStr, Amount, FromID, ToID, Result, Time) values(%1,\"%2\",%3,%4,%5,%6,\"%7\")")
		.arg(_type).arg(typeStr).arg(_amount).arg(_fromID).arg(_toID).arg(_result).arg(QDateTime::currentDateTime ().toString());
	QSqlQuery q = mDb.exec(sql);

	return WS_NO_ERR;
}

int WalletServerDB::QueryUserWalletID( quint32 _accountID, quint32& _id )
{
	QString sql = QString("select * from UserWallet where AccountID=%1").arg(_accountID);
	QSqlQuery q = mDb.exec(sql);

	int records = 0;
	while (q.next()) 
	{
		_id = q.value(0).toInt();
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

int WalletServerDB::QueryTableWalletID( quint32 _roomID, quint32 _tableID, quint32 _seatID, quint32& _id )
{
	QString sql = QString("select * from TableWallet where RoomID=%1 and TableID=%2 and SeatID=%3").arg(_roomID).arg(_tableID).arg(_seatID);
	QSqlQuery q = mDb.exec(sql);

	int records = 0;
	while (q.next()) 
	{
		_id = q.value(0).toInt();
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

int WalletServerDB::QuerySilverWallet( quint32 _accountID, quint32& _coinAmount )
{
	QString sql = QString("select * from SilverWallet where AccountID=%1").arg(_accountID);
	QSqlQuery q = mDb.exec(sql);

	int records = 0;
	int silverCoin = 0;
	while (q.next()) 
	{
		silverCoin = q.value(2).toInt();
		records++;
		if ( records >=2 )
			break;
	}

	if( records == 1 )
	{
		_coinAmount = silverCoin;
		return WS_NO_ERR;
	}
	else if(records < 1 )
	{
		LOG_D_ERR(QString("Exception when query silver coin! record=[%1]").arg(records));
		return ERR_WS_WALLET_NOT_FOUND;
	}
	else
	{
		LOG_D_ERR(QString("Exception when query silver coin! record=[%1]").arg(records));
		return ERR_WS_MULTI_RESULT;
	}

	return WS_NO_ERR;
}

int WalletServerDB::UpdateSilverWallet( quint32 _accountID, quint32 _coinAmount )
{
	QString sql = QString("update SilverWallet set Amount=%1 where AccountID=%2")
		.arg(_coinAmount).arg(_accountID);
	QSqlQuery q = mDb.exec(sql);

	return WS_NO_ERR;
}
