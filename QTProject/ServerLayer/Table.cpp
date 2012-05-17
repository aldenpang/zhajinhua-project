#include "Table.h"
#include "SharedData.h"
#include "LogModule.h"
#include <QDateTime>
using namespace SharedData;

Table::Table()
: mState(TS_WAITING)
{
	initPokers();
}

Table::~Table()
{

}

int Table::Join( int _seatID, ISocketInstancePtr _player )
{
	if ( mPlayers.size() >= MAX_PLAYER )
		return ERR_GS_TABLE_FULL;

	if ( mPlayers[_seatID] != NULL )
		return ERR_GS_SEAT_OCCUPY;
	
	mPlayers.insert(_seatID, _player);

	return GS_NO_ERR;
}

int Table::Leave( int _seatID )
{
	QMap<int, ISocketInstancePtr>::iterator itr = mPlayers.find(_seatID);
	if ( itr != mPlayers.end() )
	{
		mPlayers.erase(itr);
		return GS_NO_ERR;
	}
	else
		return ERR_GS_PLAYER_NOT_FOUND;
}

int Table::Leave( ISocketInstancePtr _player )
{
	QMap<int, ISocketInstancePtr>::iterator itr;
	for ( itr = mPlayers.begin(); itr != mPlayers.end(); itr++ )
	{
		if ( itr.value() == _player )
		{
			Leave(itr.key());
			return GS_NO_ERR;
		}
	}

	return ERR_GS_PLAYER_NOT_FOUND;
}

bool Table::IsPlayerJoin( ISocketInstancePtr _player )
{
	QMap<int, ISocketInstancePtr>::iterator itr;
	for ( itr = mPlayers.begin(); itr != mPlayers.end(); itr++ )
	{
		if ( itr.value() == _player )
		{
			return true;
		}
	}
	return false;
}

void Table::initPokers()
{
	for ( int i = 0; i<MAX_POKERS_ZJH; i++ )
	{
		PokerPtr p = PokerPtr(new Poker);
		p->mID = i;				// ID：从2～K～A，0～51，按照黑红梅方顺序
		p->mValue = i % 13;		// 牌面值： 从2～K～A，0~12；做比大小用
		p->mFlower = (PokerFlower)(i / 13);	// 花色
		mPokers.push_back(p);
	}

	Shuffle();
}

void Table::Shuffle()
{
	qsrand(QDateTime::currentMSecsSinceEpoch());
	//LOG_D_INFO(QDateTime::currentMSecsSinceEpoch());
// 	QString buff;
// 	LOG_D_INFO("Before Shuffle");
// 	for ( int i = 0; i<MAX_POKERS_ZJH; i++ )
// 	{
// 		buff += QString("%1").arg(mPokers[i]->mID);
// 		buff += QString("/");
// 	}
// 	LOG_D_INFO(buff);

	for ( int i = 0; i<MAX_POKERS_ZJH; i++ )
	{
		int rand = qrand() % MAX_POKERS_ZJH;
		PokerPtr tempP = mPokers.front();
		mPokers.pop_front();
		mPokers.insert(rand, tempP);
	}
// 
// 	buff = "";
// 	LOG_D_INFO("After Shuffle");
// 	for ( int i = 0; i<MAX_POKERS_ZJH; i++ )
// 	{
// 		buff += QString("%1").arg(mPokers[i]->mID);
// 		buff += QString("/");
// 	}
// 	LOG_D_INFO(buff);
}
