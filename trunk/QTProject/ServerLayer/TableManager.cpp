#include "TableManager.h"
#include "Table.h"
#include "GSPlayer.h"
#include "SharedData.h"
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
		int res = table->Join(_player);
		if(res != GS_NO_ERR)
			return res;

		if ( table->PlayerAmount() >= MIN_PLAYER )
		{
			//emit SiStartGame
		}
		else
		{
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