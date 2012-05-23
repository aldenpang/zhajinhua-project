#include "Table.h"
#include "SharedData.h"
#include "LogModule.h"
#include <QDateTime>
#include "Packet.h"
#include "GSPlayer.h"
using namespace SharedData;

Table::Table()
: mState(TS_WAITING)
, mReadyAmount(0)
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

	GSPlayerPtr gsp = _player.staticCast<GSPlayer>();
	gsp->SetIsWaiting(true);

	if ( PlayerAmount() >= MIN_PLAYER )
	{
		if ( mState == TS_WAITING )
		{
			// switch to TS_PLAYING
			mState = TS_PLAYING;

			// set all player's state to not waiting
			QMap<int, ISocketInstancePtr>::iterator itr;
			for ( itr = mPlayers.begin(); itr != mPlayers.end(); itr++ )
			{
				GSPlayerPtr p = itr.value().staticCast<GSPlayer>();
				p->SetIsWaiting(false);
			}

			startTable();

		}
		//else if ( mState == TS_PLAYING )
		//{
		//	// other player who join after table start to play, will let them to waiting for next time of starting
		//	// 等待当前局结束

		//}
	}

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
	mPokers.clear();

	for ( int i = 0; i<MAX_POKERS_ZJH; i++ )
	{
		PokerPtr p = PokerPtr(new Poker(i));
		mPokers.push_back(p);
	}

	shuffle();
}

void Table::shuffle(bool _print)
{
	qsrand(QDateTime::currentMSecsSinceEpoch());

	QString buff;
	if ( _print )
	{
		LOG_D_INFO(QDateTime::currentMSecsSinceEpoch());
	 	LOG_D_INFO("Before Shuffle");
	 	for ( int i = 0; i<MAX_POKERS_ZJH; i++ )
	 	{
	 		buff += QString("%1").arg(mPokers[i]->mID);
	 		buff += QString("/");
	 	}
	 	LOG_D_INFO(buff);
	}

	for ( int i = 0; i<MAX_POKERS_ZJH; i++ )
	{
		int rand = qrand() % MAX_POKERS_ZJH;
		PokerPtr tempP = mPokers.front();
		mPokers.pop_front();
		mPokers.insert(rand, tempP);
	}

	if ( _print )
	{
	 	buff = "";
	 	LOG_D_INFO("After Shuffle");
	 	for ( int i = 0; i<MAX_POKERS_ZJH; i++ )
	 	{
	 		buff += QString("%1").arg(mPokers[i]->mID);
	 		buff += QString("/");
	 	}
	 	LOG_D_INFO(buff);
	}
}

void Table::startTable()
{
	// 洗牌
	shuffle(true);

	// 广播开始消息
	Packet p;
	p.SetMessage(MSG_GS_CL_START_GAME);
	broadcast(&p);
}

void Table::broadcast( Packet* _p )
{
	QMap<int, ISocketInstancePtr>::iterator itr;
	for ( itr = mPlayers.begin(); itr != mPlayers.end(); itr++ )
	{
		GSPlayerPtr p = itr.value().staticCast<GSPlayer>();
		if ( !p->GetIsWaiting() )
		{
			itr.value()->Send(_p);
		}
	}
}

PokerPtr Table::distribute()
{
	PokerPtr p = mPokers.front();
	mPokers.pop_front();
	return p;
}

QMap<int, ISocketInstancePtr> Table::getPlayingPlayers()
{
	QMap<int, ISocketInstancePtr> out;
	QMap<int, ISocketInstancePtr>::iterator itr;
	for ( itr = mPlayers.begin(); itr != mPlayers.end(); itr++ )
	{
		GSPlayerPtr p = itr.value().staticCast<GSPlayer>();
		if ( !p->GetIsWaiting() )
		{
			out.insert(itr.key(), itr.value());
		}
	}

	return out;
}

void Table::UpdateReadyState( int _seatID )
{
	// 先只记录有多少玩家已经ready，暂时忽略是哪个座位的玩家ready
	mReadyAmount++;

	QMap<int, ISocketInstancePtr> players = getPlayingPlayers();
	if ( mReadyAmount >= players.size() )
	{
		// 每人发三张牌
		QMap<int, ISocketInstancePtr>::iterator itr;
		for ( itr = players.begin(); itr != players.end(); itr++ )
		{
			Packet pp;
			pp.SetMessage(MSG_GS_CL_DISTRIBUTE);
			QString log;
			for ( int i = 0; i<MAX_HAND_POKER; i++ )
			{
				PokerPtr p = distribute();
				GSPlayerPtr player = itr.value().staticCast<GSPlayer>();
				player->AddPoker(p);
				pp<<p->mID;
				log += QString("%1/").arg(p->mID);
			}
			itr.value()->Send(&pp);
			LOG_D_INFO(QString("Send Poker [%1] to Player[%2:%3]").arg(log).arg(itr.value()->IP()).arg(itr.value()->Port()));
		}
	}
}
