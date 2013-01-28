#include "Table.h"
#include "SharedData.h"
#include "LogModule.h"
#include <QDateTime>
#include "Packet.h"
#include "../WalletServer/WalletServerDB.h"
#include "GSPlayer.h"
#include "DataCenter.h"
using namespace SharedData;

Table::Table()
: mState(TS_WAITING)
, mReadyAmount(0)
, mDealerSeat(0)
, mCurrentPlayer(0)
, mCurrentBid(0)
, mMinBringChip(0)
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
			setAllPlaying();

			startTable();

		}
		else if ( mState == TS_PLAYING )
		{
			// other player who join after table start to play, will let them to waiting for next time of starting
			LOG_D_INFO(QString("Player[%1] join, waiting for table restart").arg(gsp->GetNickName()));
		}
	}

	return GS_NO_ERR;
}

int Table::Leave( int _seatID )
{
	QMap<int, ISocketInstancePtr>::iterator itr = mPlayers.find(_seatID);
	if ( itr != mPlayers.end() )
	{
		mPlayers.erase(itr);

		if ( mPlayers.size() <= 1 )
		{
			mState = TS_WAITING;
			reset();
		}
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


void Table::setAllPlaying()
{
	int amount = 0;
	QMap<int, ISocketInstancePtr>::iterator itr;
	for ( itr = mPlayers.begin(); itr != mPlayers.end(); itr++ )
	{
		GSPlayerPtr p = itr.value().staticCast<GSPlayer>();
		p->SetIsWaiting(false);
		amount++;
	}
	LOG_D_INFO(QString("CurrentPlayingPlayer[%1]").arg(amount));
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

	// 洗牌算法：从原有序列中取出第一个的指针，再pop掉第一个，然后把刚才取出的指针再插入一个随机位置，理论上可以洗任意次
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
	// 广播开始消息
	Packet p;
	p.SetMessage(MSG_GS_CL_START_GAME);
	p<<BASE_CHIP<<TOP_CHIP<<mDealerSeat;
	QMap<int, ISocketInstancePtr> players = getPlayingPlayers();
	p<<(quint32)players.size();
	QMap<int, ISocketInstancePtr>::iterator itr;
	for (itr = players.begin(); itr != players.end(); itr++)
	{
		GSPlayerPtr ppp = itr.value().staticCast<GSPlayer>();
		p<<(quint32)itr.key()<<ppp->GetNickName()<<ppp->GetProtraitID()<<getPlayerMoney(ppp);
	}
	broadcastToAll(&p);
	LOG_D_INFO("Broadcast MSG_GS_CL_START_GAME");

	// send sync message
	Packet ppp;
	ppp.SetMessage(MSG_GS_CL_SYNC_START);
	broadcastToAll(&ppp);

	// 重置已经ready的玩家
	mReadyAmount = 0;
}

quint32 Table::getPlayerMoney( GSPlayerPtr _player )
{
	//if ( DATACENTER.mRoomInfo.mMoneyType == GOLD_COIN )
	//{
		return _player->GetTableWalletMoney();
	//}else if ( DATACENTER.mRoomInfo.mMoneyType == SILVER_COIN )
	//{
	//	return _player->GetSilverCoin();
	//}else
	//	return 0;
}

void Table::broadcastToPlaying( Packet* _p )
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

void Table::broadcastToAll( Packet* _p )
{
	QMap<int, ISocketInstancePtr>::iterator itr;
	for ( itr = mPlayers.begin(); itr != mPlayers.end(); itr++ )
	{
		itr.value()->Send(_p);
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

	setAllPlaying();
	QMap<int, ISocketInstancePtr> players = getPlayingPlayers();
	if ( mReadyAmount >= players.size() )
	{
		// 先投底注
		int baseChip = BASE_CHIP;
		Packet p;
		p.SetMessage(MSG_GS_CL_BASE_CHIP);
		p<<baseChip;
		broadcastToPlaying(&p);
		QMap<int, ISocketInstancePtr>::iterator itr;
		for ( itr = players.begin(); itr != players.end(); itr++ )
		{
			GSPlayerPtr player = itr.value().staticCast<GSPlayer>();
			player->SetTableWalletMoney(player->GetTableWalletMoney()-baseChip);
			mCurrentBid += baseChip;
			LOG_D_INFO(QString("After base chip, [%1] has [%2] left").arg(player->GetNickName()).arg(player->GetTableWalletMoney()));
		}
		// 每人发三张牌
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
		// 指定当前玩家
		Packet ppp;
		ppp.SetMessage(MSG_GS_CL_CURRENT_PLAYER);
		ppp<<mCurrentPlayer;
		LOG_D_INFO(QString("CurrentPlayer[%1]").arg(mCurrentPlayer));
		broadcastToPlaying(&ppp);

		mState = TS_PLAYING;
	}
}

void Table::Follow( int _seatID, int _chip )
{
	if ( mState != TS_PLAYING )
		return;

	if ( _seatID != mCurrentPlayer )
	{
		LOG_D_WARN(QString("SeatID[%1] not match with mCurrentPlayer[%2]").arg(_seatID).arg(mCurrentPlayer));
		return;
	}
	GSPlayerPtr currentPlayer = mPlayers[mCurrentPlayer].staticCast<GSPlayer>();
	if ( _chip > currentPlayer->GetTableWalletMoney() )
	{
		LOG_D_WARN(QString("SeatID[%1] has not enough money to follow").arg(_seatID));
		// TODO:Send deny message
		return;
	}
	if ( currentPlayer->GetIsGiveUp() )
	{
		LOG_D_WARN(QString("SeatID[%1] already give up").arg(_seatID));
		return;
	}

	LOG_D_INFO(QString("[%1] follow by [%2] chips").arg(_seatID).arg(_chip));
	QMap<int, ISocketInstancePtr> players = getPlayingPlayers();
	QMap<int, ISocketInstancePtr>::iterator itr = players.find(mCurrentPlayer);
	itr++;
	if(itr == players.end())
		itr = players.begin();
	mCurrentPlayer = itr.key();

	// update chips
	currentPlayer->SetTableWalletMoney(currentPlayer->GetTableWalletMoney()-_chip);
	currentPlayer->AlreadyFollow(_chip);

	//if mCurrentBid is higher than TOP_CHIP, then turn to game end
	mCurrentBid += _chip;
	LOG_D_INFO(QString("CurrentBid[%1] CurrentPlayer[%2]").arg(mCurrentBid).arg(mCurrentPlayer));
	if ( mCurrentBid >= TOP_CHIP )
	{
		gameEnd();

		startTable();
	}
	else
	{
		// broadcast who follow how much
		Packet p;
		p.SetMessage(MSG_GS_CL_FOLLOW);
		p<<_seatID<<_chip<<mCurrentPlayer<<mCurrentBid;
		broadcastToPlaying(&p);
	}

}

void Table::reset()
{
	mState = TS_WAITING;
	mReadyAmount = 0;
	mDealerSeat = 0;
	mCurrentPlayer = 0;
	mCurrentBid = 0;

	// clean all player's poker
	QMap<int, ISocketInstancePtr>::iterator itr;
	for ( itr = mPlayers.begin(); itr != mPlayers.end(); itr++ )
	{
		itr.value().staticCast<GSPlayer>()->CleanPokers();
		itr.value().staticCast<GSPlayer>()->CleanAlreadyFollow();
		itr.value().staticCast<GSPlayer>()->SetIsGiveUp(false);
	}

	initPokers();
}

GSPlayerPtr Table::WhoWin()
{
	GSPlayerPtr winPlayer;
	QMap<int, ISocketInstancePtr>::iterator itr;
	for ( itr = mPlayers.begin(); itr != mPlayers.end(); itr++ )
	{
		GSPlayerPtr player = itr.value().staticCast<GSPlayer>();
		int pushCounter = 0;
		QMap<int, ISocketInstancePtr>::iterator otherItr;
		for ( otherItr = mPlayers.begin(); otherItr != mPlayers.end(); otherItr++ )
		{
			GSPlayerPtr otherPlayer = otherItr.value().staticCast<GSPlayer>();
			if ( player == otherPlayer )		// if this player is same player, continue
				continue;

			PokerType playerType = player->GetPokerType();
			QList<PokerPtr> playerPokers = player->GetPokers();
			PokerType otherType = otherPlayer->GetPokerType();
			QList<PokerPtr> otherPokers = otherPlayer->GetPokers();
			if ( player->CanPush(otherPlayer->GetPokers()) )
				pushCounter++;
		}
		if ( pushCounter == mPlayers.size()-1 )	// if this player' poker is bigger than other player's, then this player is winner(except himself)
		{
			winPlayer = player;
			break;
		}
	}
	return winPlayer;
}

void Table::GiveUp( int _seatID )
{
	// check player amount, if less than 2, game will end, otherwise this player will end himself
	GSPlayerPtr player = mPlayers[_seatID].staticCast<GSPlayer>();
	player->SetIsGiveUp(true);

	int notGiveUpAmount = 0;
	QMap<int, ISocketInstancePtr>::iterator itr;
	for ( itr = mPlayers.begin(); itr != mPlayers.end(); itr++ )
	{
		if ( !itr.value().staticCast<GSPlayer>()->GetIsGiveUp() )
		{
			notGiveUpAmount++;
		}
	}

	if( notGiveUpAmount == 1 )
	{
		gameEnd();
		startTable();
	}
	// broadcast
	Packet p;
	p.SetMessage(MSG_GS_CL_GIVEUP);
	p<<_seatID;
	broadcastToPlaying(&p);
}

void Table::calculateBalance(QMap<int, int>& _result)
{
	GSPlayerPtr winner = WhoWin();
	// transfer money from loser's table wallet to winner's table wallet, and transfer rake
	int rake = mCurrentBid*(DATACENTER.mRoomInfo.mMoneyType == GOLD_COIN?RAKE:0);
	int afterRake = mCurrentBid-rake;
	winner->SetTableWalletMoney(winner->GetTableWalletMoney()+afterRake);
	LOG_D_INFO(QString("[%1] wins [%2] coins(after rake)").arg(winner->GetNickName()).arg(afterRake));
	// write player's current money to wallet db
	QMap<int, ISocketInstancePtr>::iterator itr;
	for ( itr = mPlayers.begin(); itr != mPlayers.end(); itr++ )
	{
		int res = WalletDB.UpdateTableWallet(DATACENTER.mRoomInfo.mRoomID, mTableID, itr.key(), itr.value().staticCast<GSPlayer>()->GetTableWalletMoney());
		if ( res != WS_NO_ERR )
			LOG_D_ERR(QString("Player[%1] desposit to table wallet error[%2]").arg(itr.value().staticCast<GSPlayer>()->GetAccountID()).arg(res));

		if ( itr.value().staticCast<GSPlayer>() != winner )
		{
			res = WalletDB.InsertTransactionRecord(TableToTable, itr.value().staticCast<GSPlayer>()->GetAlreadyFollow()+BASE_CHIP, itr.value().staticCast<GSPlayer>()->GetTableWalletID()
				, winner->GetTableWalletID(), res);
			if ( res != WS_NO_ERR )
				LOG_D_ERR(QString("InsertTransactionRecord for Player[%1] error[%2]").arg(itr.value().staticCast<GSPlayer>()->GetAccountID()).arg(res));
		}
	}

	// write rake to wallet db
	int res = WalletDB.InsertRake(DATACENTER.mRoomInfo.mRoomID, rake);
	if ( res != WS_NO_ERR )
		LOG_D_ERR(QString("InsertRake error[%1]").arg(res));

	// record
	res = WalletDB.InsertTransactionRecord(TableToRake, rake, winner->GetTableWalletID(), DATACENTER.mRoomInfo.mRoomID, res);
	if ( res != WS_NO_ERR )
		LOG_D_ERR(QString("InsertTransactionRecord error[%1]").arg(res));

	// make result
	QMap<int, ISocketInstancePtr>::iterator itr2;
	for (itr2 = mPlayers.begin(); itr2 != mPlayers.end(); itr2++)
	{
		if (*itr2 == winner)
		{
			_result.insert(itr2.key(), afterRake);
		}
		else
		{
			_result.insert(itr2.key(), (itr2.value().staticCast<GSPlayer>()->GetAlreadyFollow()+BASE_CHIP)*-1);
		}
	}

}

void Table::gameEnd()
{
	LOG_D_INFO("###### GameEnd ######");
	mState = TS_BALANCE;

	QMap<int, int> result;
	calculateBalance(result);

	Packet p;
	p.SetMessage(MSG_GS_BC_TABLE_END);
	p<<BASE_CHIP<<TOP_CHIP<<mDealerSeat;
	QMap<int, ISocketInstancePtr> players = getPlayingPlayers();
	p<<(quint32)players.size();
	QMap<int, ISocketInstancePtr>::iterator itr;
	for (itr = players.begin(); itr != players.end(); itr++)
	{
		GSPlayerPtr ppp = itr.value().staticCast<GSPlayer>();
		p<<(quint32)itr.key()<<ppp->GetNickName()<<ppp->GetProtraitID()<<getPlayerMoney(ppp);
		LOG_D_INFO(QString("///////////// CurrentPlayerMoney: [%1] has [%2]").arg(ppp->GetNickName()).arg(getPlayerMoney(ppp)));
	}
	for (int i = 0; i<MAX_PLAYER; i++)
	{
		p<<i<<result[i];
	}
	broadcastToPlaying(&p);

	reset();

}
