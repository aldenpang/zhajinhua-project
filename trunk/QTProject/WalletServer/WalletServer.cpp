#include "WalletServer.h"
#include "WalletServerDB.h"
#include "SharedData.h"
#include "Packet.h"
using namespace SharedData;

WalletServer::WalletServer()
{

}

WalletServer::~WalletServer()
{

}

void WalletServer::PacketHandler( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	quint32 msg = _packet.GetMessage();
	switch(msg)
	{
	case MSG_LS_WS_QUERY_USER_WALLET:
		processQueryUserWallet(_incomeSocket, _packet);
		break;
	case MSG_GS_WS_QUERY_TABLE_WALLET:
		processQueryTableWallet(_incomeSocket, _packet);
		break;
	case MSG_GS_WS_INSERT_TABLE_WALLET:
		processInserTableWallet(_incomeSocket, _packet);
		break;
	default:
		LOG_WARN(QString("incorrect message:%1").arg(msg));
		break;
	}
}

void WalletServer::processQueryUserWallet( ISocketInstancePtr _incomeSocket, Packet& _p )
{
	quint32 accID = 0;
	_p>>accID;

	quint32 coin = 0;
	int res = WalletDB.QueryUserWallet(accID, coin);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("QueryUserWalletError:%1").arg(res));
	}

	Packet p;
	p.SetMessage(MSG_WS_LS_QUERY_USER_WALLET);
	p<<coin;
	_incomeSocket->Send(&p);

}

void WalletServer::processQueryTableWallet( ISocketInstancePtr _incomeSocket, Packet& _p )
{
	quint32 roomID, tableID, seatID;
	roomID=tableID=seatID=0;

	_p>>roomID>>tableID>>seatID;

	quint32 coin = 0;
	int res = WalletDB.QueryTableWallet(roomID, tableID, seatID, coin);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("QueryTableWalletError:%1").arg(res));
	}

	Packet p;
	p.SetMessage(MSG_WS_GS_QUERY_TABLE_WALLET);
	p<<coin;
	_incomeSocket->Send(&p);

}

void WalletServer::processInserTableWallet( ISocketInstancePtr _incomeSocket, Packet& _p )
{
	quint32 roomID, tableID, seatID, coin;
	roomID=tableID=seatID=coin=0;

	_p>>roomID>>tableID>>seatID>>coin;

	int res = WalletDB.InsertTableWallet(roomID, tableID, seatID);
	if ( res != ERR_WS_MULTI_RESULT )
	{
		res = WalletDB.UpdateTableWallet(roomID, tableID, seatID, coin);
	}
	else
		LOG_ERR(QString("InsertTableWalletError:%1").arg(res));

	Packet p;
	p.SetMessage(MSG_WS_GS_INSERT_TABLE_WALLET);
	p<<res;
	_incomeSocket->Send(&p);

}
