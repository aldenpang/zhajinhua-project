#include "GameServerNet.h"
#include "MD5.h"
#include "SharedData.h"

using namespace SharedData;


GameServerNet::GameServerNet()
{

}

GameServerNet::~GameServerNet()
{

}

void GameServerNet::PacketHandler( Packet& _packet )
{
	int msg = _packet.GetMessage();
	qDebug()<<"From Game Server - Recv Msg:"<<msg;
	switch (msg)
	{
	case MSG_GS_CL_LOGIN:
		processLogin(_packet);
		break;
	case MSG_GS_CL_TABLE_INFO:
		processTableInfo(_packet);
		break;
	case MSG_GS_CL_BRING_MONEY:
		processBringMoneyRes(_packet);
		break;
	case MSG_GS_BC_TABLE_JOIN:
		processTableJoin(_packet);
		break;
	case MSG_GS_BC_TABLE_LEAVE:
		processTableLeave(_packet);
		break;
	case MSG_GS_CL_START_GAME:
		processStartGame(_packet);
		break;
	case MSG_GS_CL_BASE_CHIP:
		processDropBaseChip(_packet);
		break;
	case MSG_GS_CL_DISTRIBUTE:
		processDistribute(_packet);
		break;
	case MSG_GS_CL_CURRENT_PLAYER:
		processCurrentPlayer(_packet);
		break;
	case MSG_GS_BC_TABLE_END:
		processTableEnd(_packet);
		break;
	case MSG_GS_CL_FOLLOW:
		processFollow(_packet);
		break;
	case MSG_GS_CL_SYNC_START:
		processSyncStart(_packet);
		break;
	case MSG_GS_CL_QUERY_MONEY:
		processQueryMoney(_packet);
		break;
	default:
		break;
	}
}

void GameServerNet::processLogin( Packet& _packet )
{
	int res = 0;
	_packet>>res;

	if ( res != GS_NO_ERR )
		emit SiLoginFailed(res);
	else
		emit SiLoginOK();
}

void GameServerNet::SendLoginGS( QString& _userName, QString& _pwd, int _isTempLogin )
{
	// login gs
	Packet p;
	p.SetMessage(MSG_CL_GS_LOGIN);
	QString userName = _userName;
	QString md5pwd = ToMD5(_pwd);
	p<<_isTempLogin<<userName<<md5pwd;
	Send(&p);
}

void GameServerNet::SendJoinTable( quint32 _tableID, quint32 _seatID )
{
	// join table
	Packet p;
	p.SetMessage(MSG_CL_GS_TABLE_JOIN);
	p<<_tableID<<_seatID;
	Send(&p);
}

void GameServerNet::SendBringMoney( quint32 _tableID, quint32 _seatID, quint32 _money )
{
	Packet p;
	p.SetMessage(MSG_CL_GS_BRING_MONEY);
	p<<_tableID<<_seatID<<_money;
	Send(&p);
}

void GameServerNet::processTableInfo( Packet& _packet )
{
	QMap<int, TableData> tables;
	quint32 tableAmount = 0;
	quint32 tableID = 0;
	int playerAmount = 0;
	int minBringChip = 0;
	_packet>>tableAmount;
	
	for ( int i = 0; i<tableAmount; i++ )
	{
		TableData table;
		_packet>>tableID;
		_packet>>playerAmount;
		_packet>>minBringChip;
		table.SetID(tableID);
		table.SetMinBringChip(minBringChip);
		for ( int j = 0; j < playerAmount; j++ )
		{
			int seatID = 0;
			_packet>>seatID;
			TablePlayer p;
			_packet>>p.mNickName>>p.mProtraitID;
			table.AddPlayer(seatID, p);
		}
		tables.insert(i, table);
	}

	emit SiTableList(tables);
}

void GameServerNet::processTableJoin( Packet& _packet )
{
	quint32 res = 0;
	quint32 seatID = 0;
	quint32 tableID = 0;
	TablePlayer tablePlayer;
	_packet>>res>>tableID>>seatID>>tablePlayer.mNickName>>tablePlayer.mProtraitID;

	emit SiTableJoinResult(res, tableID, seatID, tablePlayer);
}

void GameServerNet::processTableLeave( Packet& _packet )
{
	quint32 res = 0;
	quint32 tableID = 0;
	TablePlayer tablePlayer;
	_packet>>res>>tableID>>tablePlayer.mNickName>>tablePlayer.mProtraitID;

	emit SiTableLeaveResult(res, tableID, tablePlayer);
}

void GameServerNet::processStartGame( Packet& _packet )
{
	TableInfo info;
	_packet>>info.mBaseChip>>info.mTopChip>>info.mDealerSeat;

	emit SiStartGame(info);
}

void GameServerNet::processDropBaseChip( Packet& _packet )
{
	int baseChip = 0;
	_packet>>baseChip;

	emit SiDropBaseChip(baseChip);
}

void GameServerNet::processDistribute( Packet& _packet )
{
	QVector<int> pokers;
	for ( int i = 0; i<MAX_HAND_POKER; i++ )
	{
		int poker = 0;
		_packet>>poker;
		pokers.push_back(poker);
	}
	emit SiDistribute(pokers);
}

void GameServerNet::processCurrentPlayer( Packet& _packet )
{
	int currentPlayer = 0;
	_packet>>currentPlayer;

	emit SiCurrentPlayer(currentPlayer);
}

void GameServerNet::processTableEnd( Packet& _packet )
{
	emit SiTableEnd();
}

void GameServerNet::processFollow( Packet& _packet )
{
	int seat = 0;
	int chip = 0;
	int currentPlayer = 0;
	int currentBid = 0;
	_packet>>seat>>chip>>currentPlayer>>currentBid;

	emit SiFollow(seat, chip, currentPlayer, currentBid);
}

void GameServerNet::processSyncStart( Packet& _packet )
{
	emit SiSyncStart();
}

void GameServerNet::processBringMoneyRes( Packet& _packet )
{
	int res = 0;
	_packet>>res;

	emit SiBringMoneyRes(res);
}

void GameServerNet::processQueryMoney( Packet& _packet )
{
	quint32 goldCoin = 0;
	quint32 silverCoin = 0;
	_packet>>goldCoin>>silverCoin;
	emit SiUpdateMoney(goldCoin, silverCoin);
}

void GameServerNet::SendLeaveTable( quint32 _tableID )
{
	Packet p;
	p.SetMessage(MSG_CL_GS_TABLE_LEAVE);
	p<<_tableID;
	Send(&p);
}
