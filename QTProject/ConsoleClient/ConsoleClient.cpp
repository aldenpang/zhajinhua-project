#include "ConsoleClient.h"
#include "LoginServerNet.h"
#include "GameServerNet.h"
#include "LogModule.h"
#include <QCoreApplication>

ConsoleClient::ConsoleClient()
: mCurrentPlayer(0)
, mMySeatID(0)
, mCurrentTableMinBringChip(0)
, mMyTableID(0)
, mIsEnd(false)
, mTempTableID(0)
, mTempSeatID(0)
{
	LOG.SetModuleName(QString("Console_%1").arg(__argv[1]));

	connect(this, SIGNAL(SiInfo(const QString&)), &LOG, SLOT(StInfo(const QString&)));
	connect(this, SIGNAL(SiWarn(const QString&)), &LOG, SLOT(StWarn(const QString&)));
	connect(this, SIGNAL(SiError(const QString&)), &LOG, SLOT(StError(const QString&)));

	LOG_INFO("ConsoleClient Start");


	mLoginServer = new LoginServerNet();
	mLoginServer->Init();
	mLoginServer->Connect("localhost", 5000);
	connect(mLoginServer, SIGNAL(SiError(QString)), this, SLOT(stNetError(QString)));
	connect(mLoginServer, SIGNAL(SiLoginOK()), this, SLOT(stLoginOK()));
	connect(mLoginServer, SIGNAL(SiLoginFailed(quint32)), this, SLOT(stLoginFailed(quint32)));
	connect(mLoginServer, SIGNAL(SiGameList(QVector<RoomInfo>)), this, SLOT(stGameList(QVector<RoomInfo>)));
	connect(mLoginServer, SIGNAL(SiConnected()), this, SLOT(stLSConnected()));
	connect(mLoginServer, SIGNAL(SiDisconnected()), this, SLOT(stLSDisconnected()));
	connect(mLoginServer, SIGNAL(SiPlayerInfo(CommonPlayer)), this, SLOT(stPlayerInfo(CommonPlayer)));

	mGameServer = new GameServerNet();
	mGameServer->Init();
	connect(mGameServer, SIGNAL(SiLoginOK()), this, SLOT(stGSLoginOK()));
	connect(mGameServer, SIGNAL(SiLoginFailed(quint32)), this, SLOT(stGSLoginFailed(quint32)));
	connect(mGameServer, SIGNAL(SiConnected()), this, SLOT(stGSConnected()));
	connect(mGameServer, SIGNAL(SiDisconnected()), this, SLOT(stGSDisconnected()));
	connect(mGameServer, SIGNAL(SiBringMoneyRes(int)), this, SLOT(stBringMoneyRes(int)));
	connect(mGameServer, SIGNAL(SiTableList(QMap<int, TableData>)), this, SLOT(stTableList(QMap<int, TableData>)));
	connect(mGameServer, SIGNAL(SiTableJoinResult(quint32, quint32, quint32, TablePlayer)), this, SLOT(stTableJoinRes(quint32, quint32, quint32, TablePlayer)));
	connect(mGameServer, SIGNAL(SiStartGame(TableInfo)), this, SLOT(stStartGame(TableInfo)));
	connect(mGameServer, SIGNAL(SiDropBaseChip(int)), this, SLOT(stDropBaseChip(int)));
	connect(mGameServer, SIGNAL(SiDistribute(QVector<int>)), this, SLOT(stDistribute(QVector<int>)));
	connect(mGameServer, SIGNAL(SiCurrentPlayer(int)), this, SLOT(stCurrentPlayer(int)));
	connect(mGameServer, SIGNAL(SiTableEnd(TableInfo, QMap<int, int>)), this, SLOT(stTableEnd(TableInfo, QMap<int, int>)));
	connect(mGameServer, SIGNAL(SiFollow(int, int, int, int)), this, SLOT(stFollow(int, int, int, int)));
	connect(mGameServer, SIGNAL(SiSyncStart()), this, SLOT(stSyncStart()));

	//connect(mTimer, SIGNAL(timeout()), this, SLOT(stFollowByTimer()));
	//mTimer.
}

ConsoleClient::~ConsoleClient()
{

}

void ConsoleClient::stLoginOK()
{
	LOG_INFO("Login OK");
	mLoginServer->RequestGameList(atoi(__argv[3]));

}

void ConsoleClient::stLoginFailed( quint32 _errorCode )
{
	LOG_INFO(QString("Login Failed:%1").arg(_errorCode));
}

void ConsoleClient::stGameList( QVector<RoomInfo> _gameList )
{
	LOG_INFO(QString("Game list received, size:%1").arg(_gameList.size()));

	RoomInfo join;
	foreach(RoomInfo info, _gameList)
	{
		if ( info.mRoomID == atoi(__argv[4]) )
		{
			join = info;
			break;
		}
	}
	
	LOG_INFO(QString("ConsoleClient will going to login game server:ip:%1 port:%2 roomID:%3").arg(join.mIP).arg(join.mPort).arg(join.mRoomID));
	mGameServer->Connect(join.mIP, join.mPort);
}

void ConsoleClient::stNetError( QString _errStr )
{

}

void ConsoleClient::stLSConnected()
{
	LOG_INFO("Login Server Connected");

	// do login
	mLoginServer->SendLoginRequest(QString(__argv[1]), QString(__argv[2]));
}

void ConsoleClient::stLSDisconnected()
{
	LOG_ERR("Login Server Disconnected");
}

void ConsoleClient::stGSLoginOK()
{
	LOG_INFO("Game Server Logined");
}

void ConsoleClient::stGSLoginFailed( quint32 _errorCode )
{
	LOG_ERR(QString("Game Server Login Failed:%1").arg(_errorCode));
}

void ConsoleClient::stGSConnected()
{
	LOG_INFO("Game Server Connected");
	mGameServer->SendLoginGS(QString(__argv[1]), QString(__argv[2]));
}

void ConsoleClient::stGSDisconnected()
{
	LOG_ERR("Game Server Disconnected");
	QCoreApplication::quit();
}

void ConsoleClient::stTableList( QMap<int, TableData> _tables )
{
	LOG_INFO(QString("Received Table Size:[%1]").arg(_tables.size()));
	//QMap<int, QString> playerTable0 = _tables[0].GetPlayers();
	//QMap<int, QString>::iterator playerTable0Itr;
	//for ( playerTable0Itr = playerTable0.begin(); playerTable0Itr != playerTable0.end(); playerTable0Itr++ )
	//{
	//	LOG_INFO(QString("##### [%1] seat on [%2]").arg(playerTable0Itr.value()).arg(playerTable0Itr.key()));
	//}

	//return;

	// TODO: 这里应该从桌子列表中选择一个有人的进入（为了快速配桌）
	mTempTableID = 0; 
	mTempSeatID = 0;
	QMap<int, TableData>::iterator itr;
	for ( itr = _tables.begin(); itr != _tables.end(); itr++)
	{
		quint32 playerNum = itr.value().GetPlayers().size();
		for ( int i = 0; i<playerNum; i++)
		{
			if ( itr.value().isSeatOccupied(i) )
			{
				LOG_INFO(QString("Seat[%1] in Table[%2] is occupied").arg(i).arg(itr.key()));
			}
			else 
			{
				mTempTableID = itr.key();
				mTempSeatID = i;
				mCurrentTableMinBringChip = itr.value().GetMinBringChip();
				LOG_INFO(QString("Seat[%1] in Table[%2] is not occupied").arg(mTempSeatID).arg(itr.key()));
				goto End;
				//break;
			}

		}
	}
End:
	//LOG_INFO(QString("BringMoney to table[%1] seat[%2]").arg(mTempTableID).arg(mTempSeatID));
	mGameServer->SendJoinTable(mTempTableID, mTempSeatID);
	//mGameServer->SendBringMoney(mTempTableID, mTempSeatID, mCurrentTableMinBringChip);
	return;
}

void ConsoleClient::stTableJoinRes( quint32 _res, quint32 _tableID, quint32 _seatID, TablePlayer _player )
{
	if ( _res == GS_NO_ERR || _res == WS_NO_ERR )
	{
		LOG_INFO(QString("Player[%1] Join Table[%2] seat[%3] OK").arg(_player.mNickName).arg(_tableID).arg(_seatID));
		if ( mMyInfo.GetNickName() == _player.mNickName )
		{
			mMySeatID = _seatID;
			mMyTableID = _tableID;
		}
		//mGameServer->SendBringMoney(mMyTableID, mMySeatID, mCurrentTableMinBringChip);
	}
	else if ( _res == ERR_GS_TABLE_NOT_FOUND )
	{
		LOG_ERR(QString("Table Join Result:ERR_GS_TABLE_NOT_FOUND"));
	}
	else if ( _res == ERR_GS_TABLE_FULL )
	{
		LOG_ERR(QString("Table Join Result:ERR_GS_TABLE_FULL"));
	}
	else if ( _res == ERR_GS_SEAT_OCCUPY )
	{
		LOG_ERR(QString("Table Join Result:ERR_GS_SEAT_OCCUPY"));
	}
	else
		LOG_ERR(QString("Table Join Unknown Result:%1").arg(_res));


}

void ConsoleClient::stStartGame(TableInfo _info)
{
	LOG_INFO(QString("Start Game, BaseCoin[%1]TopCoin[%2]DealerSeat[%3] client should start loading").arg(_info.mBaseChip).arg(_info.mTopChip).arg(_info.mDealerSeat));

	mCurrentTableInfo = _info;

}

void ConsoleClient::stDropBaseChip( int _baseChip )
{
	LOG_INFO(QString("Drop [%1] for base chip").arg(_baseChip));
	mIsEnd = false;
}

void ConsoleClient::stDistribute( QVector<int> _pokers )
{
	mPokers.clear();

	QString log;
	foreach( int p, _pokers )
	{
		log += QString("%1/").arg(p);
		PokerPtr poker = PokerPtr(new Poker(p));
		mPokers.push_back(poker);
	}
	LOG_INFO(QString("Distribute Pokers[%1]").arg(log));
}

void ConsoleClient::stCurrentPlayer( int _currentPlayer )
{
	mCurrentPlayer = _currentPlayer;
	LOG_INFO(QString("CurrentPlayer[%1] mMySeatID[%2] mIsEnd[%3]").arg(mCurrentPlayer).arg(mMySeatID).arg(mIsEnd));
	// if current player is self, he can follow, drop, or add chips
	if ( mCurrentPlayer == mMySeatID && !mIsEnd )
	{
		// follow after 1 second, can be a randomly number
		//int ran = qrand() % 3 + 1;
		int ran = 1;
		QTimer::singleShot(ran * 1000, this, SLOT(stFollowByTimer()));
	}
}

void ConsoleClient::stTableEnd(TableInfo _info, QMap<int, int> _res)
{
	LOG_INFO("######################");
	LOG_INFO("########Game End######");
	LOG_INFO("######################");
	
	mCurrentPlayer = 0;
	mIsEnd = true;

	Packet p;
	p.SetMessage(MSG_CL_GS_CONTINUE);
	p<<mMyTableID<<mMySeatID;
	mGameServer->Send(&p);
}

void ConsoleClient::stFollowByTimer()
{
	int chip = 5;
	Packet p;
	p.SetMessage(MSG_CL_GS_FOLLOW);
	p<<mMyTableID<<mMySeatID<<chip;
	mGameServer->Send(&p);
	LOG_INFO("#####Send [MSG_CL_GS_FOLLOW]########");
}

void ConsoleClient::stFollow( int _seatID, int _chip, int _currentPlayer, int _currentBid )
{
	LOG_INFO(QString("Seat[%1]follow[%2], currentPlayer[%3] currentBid[%4]")
		.arg(_seatID).arg(_chip).arg(_currentPlayer).arg(_currentBid));
	if ( _currentBid >= mCurrentTableInfo.mTopChip || mIsEnd )
	{
		LOG_INFO("End");
		return;
	}
	stCurrentPlayer(_currentPlayer);
}

void ConsoleClient::stSyncStart()
{
	// 在ConsoleClient里，直接发送开始消息MSG_CL_GS_SYNC_START
	LOG_INFO("<-= MSG_CL_GS_SYNC_START =->");
	Packet p;
	p.SetMessage(MSG_CL_GS_SYNC_START);
	mGameServer->Send(&p);
}

void ConsoleClient::stBringMoneyRes( int _res )
{
	if ( _res == WS_NO_ERR )
	{
		LOG_INFO("Bring money successful");
		//mGameServer->SendJoinTable(mTempTableID, mTempSeatID);
	}
	else
		LOG_ERR(QString("BringMoneyFailed[%1]").arg(_res));

}

void ConsoleClient::stPlayerInfo( CommonPlayer _player )
{
	mMyInfo = _player;
}