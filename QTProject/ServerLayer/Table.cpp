#include "Table.h"
#include "SharedData.h"
using namespace SharedData;

Table::Table()
{

}

Table::~Table()
{

}

int Table::Join( ISocketInstancePtr _player )
{
	if ( mPlayers.size() >= MAX_PLAYER )
		return ERR_GS_TABLE_FULL;
	
	mPlayers.insert(_player);

	return GS_NO_ERR;
}

int Table::Leave( ISocketInstancePtr _player )
{
	QSet<ISocketInstancePtr>::iterator itr = mPlayers.find(_player);
	if ( itr != mPlayers.end() )
	{
		mPlayers.erase(itr);
		return GS_NO_ERR;
	}
	else
		return ERR_GS_PLAYER_NOT_FOUND;
}
