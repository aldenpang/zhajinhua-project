#include "TableManager.h"
#include "Table.h"
#include "GSPlayer.h"
#include "SharedData.h"
#include "LogModule.h"
using namespace SharedData;

TableManager::TableManager()
{

}

TableManager::~TableManager()
{
	QMap<int, Table*>::iterator itr;
	for ( itr = mTables.begin(); itr!= mTables.end(); itr++ )
	{
		SAFE_DELETE(itr.value());
	}
	mTables.clear();
}

void TableManager::AssignTables( int _amount/*=MAX_TABLE*/ )
{
	for ( int i = 0; i<_amount; i++ )
	{
		Table* table = new Table();
		mTables.insert(i, table);
	}
}

int TableManager::StJoinTable( ISocketInstancePtr _player, uint _tableID, uint _seatID )
{
	Table* table = mTables.value(_tableID);
	if ( table )
	{
		int res = table->Join(_seatID, _player);
		if(res != GS_NO_ERR)
			return res;

		LOG_D_INFO(QString("Player[%1:%2], join seat[%3] in table[%4] successful, table has [%5] player now.")
			.arg(_player->IP()).arg(_player->Port()).arg(_seatID).arg(_tableID).arg(table->PlayerAmount()));
		if ( table->PlayerAmount() >= MIN_PLAYER )
		{
			LOG_D_INFO(QString("Going to start game"));
			//emit SiStartGame
		}
		else
		{
			LOG_D_INFO(QString("Player not enought to start game"));

			// �ȴ���ǰ�ֽ���
			GSPlayerPtr gsp = _player.staticCast<GSPlayer>();
			gsp->SetIsWaiting(true);
		}
	}
	else
		return ERR_GS_TABLE_NOT_FOUND;

	return GS_NO_ERR;
}

int TableManager::StLeaveTable( ISocketInstancePtr _player, uint _tableID )
{
	Table* table = mTables.value(_tableID);
	if ( table )
	{
		int res = table->Leave(_player);

		return res;
	}

	return ERR_GS_TABLE_NOT_FOUND;
}

int TableManager::StLeaveTable( ISocketInstancePtr _player )
{
	QMap<int, Table*>::iterator itr;
	for ( itr = mTables.begin();itr != mTables.end(); itr++ )
	{
		if ( itr.value()->IsPlayerJoin(_player) )
		{
			int res = StLeaveTable(_player, itr.key());
			if ( res == GS_NO_ERR )
				LOG_D_INFO(QString("Player[%1:%2] leave from table[%3]").arg(_player->IP()).arg(_player->Port()).arg(itr.key()));
			else
				LOG_D_ERR(QString("Player[%1:%2] leave from table[%3], errCode:%4").arg(_player->IP()).arg(_player->Port()).arg(itr.key()).arg(res));
			return res;
		}
	}

	LOG_D_ERR(QString("NOT Found Player[%1:%2]").arg(_player->IP()).arg(_player->Port()));
	return ERR_GS_TABLE_NOT_FOUND;
}