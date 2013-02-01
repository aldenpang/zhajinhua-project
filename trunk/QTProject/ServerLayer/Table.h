/********************************************************************
	created:	2012/03/07
	created:	7:3:2012   15:57
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer\Table.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer
	file base:	Table
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef Table_h__
#define Table_h__

#include <QMap>
#include <QVector>
#include "ISocketInstance.h"
#include "HardcodeConfig.h"
#include "Poker.h"
#include "MyToolkits.h"
#include "GSPlayer.h"

enum TableState
{
	TS_WAITING=0,
	TS_PLAYING,
	TS_BALANCE,

	TS_TOTAL,
};

enum SeatState
{
	SS_EMPTY=0,
	SS_OCCUPY,

	SS_TOTAL,
};

class Table
{
public:
	Table();
	~Table();

	// returns: 0-successful 1-table full
	int Join(int _seatID, ISocketInstancePtr _player);

	// returns: 0-successful 1-player not exist
	int Leave(int _seatID);

	int Leave(ISocketInstancePtr _player);

	int PlayerAmount(){return mPlayers.size();}

	QMap<int, ISocketInstancePtr>& GetSeatInfo(){ return mPlayers; }

	bool IsPlayerJoin(ISocketInstancePtr _player);

	void UpdateReadyState( int _seatID );

	void Follow( int _seatID, int _chip );

	void GiveUp( int _seatID );

	PROPERTY(int, MinBringChip);
	PROPERTY(int, TableID);

	GSPlayerPtr WhoWin();

	void Continue(int _seatID);

protected:
private:
	QMap<int, ISocketInstancePtr>		mPlayers;	//<SeatID, PlayerInfo>
	TableState							mState;
	QVector<PokerPtr>					mPokers;
	int									mReadyAmount;	// 记录有多少个玩家已经准备好开始了，如果跟NotWaiting的玩家数相等，则开始发牌
	int									mDealerSeat;
	int									mCurrentPlayer;
	int									mCurrentBid;
	int									mContinueAmount;	// 记录有多少个玩家要继续，如果等于当前正在玩的玩家，则继续

private:
	void initPokers();

	void shuffle(bool _print=false);

	void startTable();

	void broadcastToPlaying( Packet* _p );
	void broadcastToAll( Packet* _p );

	PokerPtr distribute();

	void reset();

	void setAllPlaying();

	QMap<int, ISocketInstancePtr> getPlayingPlayers();

	void gameEnd();

	void calculateBalance(QMap<int, int>& _result);

	quint32 getPlayerMoney( GSPlayerPtr _player );

	void broadcastCurrentPlayer();
};

#endif // Table_h__