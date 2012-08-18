#include "WalletDB.h"
#include "CommonPlayer.h"


void WalletDB::QueryPlayerMoney( quint32 _accountID, CommonPlayer *_player )
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

	if( records = 1 )
	{
		_player->SetSilverCoin(silverCoin);
	}
	else
	{
		LOG_ERR(QString("Exception when query silver coin! record=[%1]").arg(records));
		return;
	}

	sql = QString("select * from UserWallet where AccountID=%1").arg(_accountID);
	q = mDb.exec(sql);

	records = 0;
	int goldCoin = 0;
	while (q.next()) 
	{
		goldCoin = q.value(2).toInt();
		records++;
		if ( records >=2 )
			break;
	}

	if( records = 1 )
	{
		_player->SetUserWalletMoney(goldCoin);
	}
	else
	{
		LOG_ERR(QString("Exception when query gold coin! record=[%1]").arg(records));
		return;
	}
}