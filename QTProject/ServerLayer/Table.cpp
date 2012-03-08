#include "Table.h"

Table::Table()
{

}

Table::~Table()
{

}

int Table::Join( ISocketInstancePtr _player )
{
	if ( mPlayers.size() >= MAX_PLAYER )
		return 1;
	
	mPlayers.insert(_player);

	return 0;
}

int Table::Leave( ISocketInstancePtr _player )
{
	QSet<ISocketInstancePtr>::iterator itr = mPlayers.find(_player);
	if ( itr != mPlayers.end() )
	{
		mPlayers.erase(itr);
		return 0;
	}
	else
		return 1;
}
