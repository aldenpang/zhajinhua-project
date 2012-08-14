#include "ZJHGameServer.h"
#include "LogModule.h"
#include "Packet.h"
#include "GameServerDB.h"
#include "SharedData.h"
#include "HardcodeConfig.h"
#include "GSPlayer.h"
#include "TableManager.h"
#include "Table.h"
#include "../WalletServer/WalletServerDB.h"
#include "DataCenter.h"
using namespace SharedData;

//------------------------------------------------------------------------------
ZjhGameServer::ZjhGameServer()
: IServerLayer()
{
	mTimer.setInterval(REFRESH_INTERVAL*1000);
	LOG.SetModuleName("GS");
	//mTimer.start();
	//connect(&mTimer, SIGNAL(timeout()), this, SLOT(stRefershTables()));

	//GSPlayer player(NULL);
	//QString name = "adsf";
	//player.SetNickName(name);
	//player.AddPoker(27);
	//player.AddPoker(0);
	//player.AddPoker(42);



}
//------------------------------------------------------------------------------
ZjhGameServer::~ZjhGameServer()
{

}

void ZjhGameServer::PacketHandler( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	int msg = _packet.GetMessage();
	qDebug()<<"ZjhGameServer - Recv Msg:"<<msg;

	switch(msg)
	{
	case MSG_CL_GS_LOGIN:
		processLogin(_incomeSocket, _packet);
		break;
	case MSG_CL_GS_TABLE_JOIN:
		processTableJoin(_incomeSocket, _packet);
		break;
	case MSG_CL_GS_TABLE_LEAVE:
		processTableLeave(_incomeSocket, _packet);
		break;
	case MSG_CL_GS_SYNC_START:
		processSyncStart(_incomeSocket, _packet);
		break;
	case MSG_CL_GS_FOLLOW:
		processFollow(_incomeSocket, _packet);
		break;
	case MSG_CL_GS_BRING_MONEY:
		processBringMoney(_incomeSocket, _packet);
		break;
	case MSG_CL_GS_GIVEUP:
		processGiveUp(_incomeSocket, _packet);
		break;
	case MSG_CL_GS_QUERY_MONEY:
		processQueryMoney(_incomeSocket, _packet);
		break;
	default:
		break;
	}
}

void ZjhGameServer::processLogin( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	QString userName;
	QString pwd;
	_packet>>userName>>pwd;

	LOG_INFO(QString("[%1]wants to login, pwd is[%2]").arg(userName).arg(pwd));

	int res = DB.VerifyUser(userName, pwd);

	if ( res != GS_NO_ERR )
		LOG_ERR(QString("LoginFailed! Reason:[%1]").arg(res));
	else
		LOG_INFO("Login OK");

	// send login result
	Packet p;
	p.SetMessage(MSG_GS_CL_LOGIN);
	p<<res;
	_incomeSocket->Send(&p);

	if ( res == GS_NO_ERR )
	{
		// get player info
		GSPlayerPtr player = GSPlayerPtr(new GSPlayer(_incomeSocket->GetSocket()));
		quint32 accID = 0;
		res = DB.GetAccountID(userName, accID);
		if ( res == GS_NO_ERR )
		{
			foreach(GSPlayerPtr p, mPlayerList)
			{
				if ( p->GetAccountID() == accID )
				{
					LOG_ERR(QString("Player[AccountID:%1] is already exist, deny login").arg(accID));
					return;
				}
			}

			// save player ip
			DB.UpdatePlayerIp(accID, player->GetSocket()->peerAddress().toString());
			player->SetAccountID(accID);

			res = DB.GetPlayerInfo(player);
			if ( res == GS_NO_ERR )
			{
				LOG_INFO(QString("Player[AccountID:%1] is login").arg(player->GetAccountID()));
				// send room config
				
				// send table list
				sendTableInfo(player);

				mPlayerList.push_back(player);
			}
			else
				LOG_ERR(QString("GetPlayerInfo Failed! Reason:[%1]").arg(res));

			// query user wallet
			quint32 chipcoin = 0;
			res = WalletDB.QueryUserWallet(player->GetAccountID(), chipcoin);
			if ( res == WS_NO_ERR )
			{
				LOG_INFO(QString("Player[%1] has [%2] chipcoin now").arg(player->GetAccountID()).arg(chipcoin));
				player->SetUserWalletMoney(chipcoin);
			}
			else
				LOG_ERR(QString("QueryUserWallet Failed! Reason:[%1]").arg(res));

			// query user wallet id
			quint32 userwalletID = 0;
			res = WalletDB.QueryUserWalletID(player->GetAccountID(), userwalletID);
			if ( res == WS_NO_ERR )
			{
				LOG_INFO(QString("Player[%1] 's user wallet id is [%2]").arg(player->GetAccountID()).arg(userwalletID));
				player->SetUserWalletID(userwalletID);
			}
			else
				LOG_ERR(QString("QueryUserWalletID Failed! Reason:[%1]").arg(res));

			player->SetSilverCoin(1000);

			// TODO: send player info
		}
		else
		{
			LOG_ERR(QString("GetAccountID Failed! Reason:[%1]").arg(res));
		}
	}

}

void ZjhGameServer::processTableJoin( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	quint32 tableID = 0;
	quint32 seatID = 0;
	_packet>>tableID>>seatID;
	GSPlayerPtr player = findPlayer(_incomeSocket);
	if ( player )
	{
		// leave player if he sit in other table
		quint32 leaveFrom = 0;
		int res = TABLE.StLeaveTable(player, leaveFrom);
		if ( res == GS_NO_ERR )
		{
			Packet p;
			p.SetMessage(MSG_GS_BC_TABLE_LEAVE);
			p<<res<<leaveFrom<<player->GetNickName()<<player->GetProtraitID();
			Broadcast(&p);
		}

		// check if this table is availble, then sit
		res = TABLE.StJoinTable(player, tableID, seatID);
		LOG_INFO(QString("JoinTable result[%1]").arg(res));

		if ( res == GS_NO_ERR )
		{
			int res = WalletDB.InsertTableWallet(mRoomInfo.mRoomID, tableID, seatID);
			if ( res != WS_NO_ERR )
			{
				LOG_ERR(QString("InsertTableWallet Error, roomID[%1] tableID[%2] seatID[%3] res[%4]")
					.arg(mRoomInfo.mRoomID).arg(tableID).arg(seatID).arg(res));
			}
		}

		// send the result of join table
		Packet p;
		p.SetMessage(MSG_GS_BC_TABLE_JOIN);
		p<<res<<tableID<<seatID<<player->GetNickName()<<player->GetProtraitID();
		//_incomeSocket->Send(&p);
		Broadcast(&p);
		return;
	}
	else
	{
		LOG_ERR(QString("Not Find Player from [%1:%2]").arg(_incomeSocket->IP()).arg(_incomeSocket->Port()));
	}
}

void ZjhGameServer::stRefershTables()
{
	LOG_INFO("Refersh Tables");
}

void ZjhGameServer::processTableLeave( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	quint32 tableID = 0;
	_packet>>tableID;
	GSPlayerPtr player = findPlayer(_incomeSocket);
	if ( player )
	{
		// 检查是否可离开桌子
		int res = TABLE.StLeaveTable(player, tableID);
		if ( res != GS_NO_ERR )
		{
			LOG_WARN("player can not join this table");

			// 不能离开桌子
			Packet p;
			p.SetMessage(MSG_GS_CL_TABLE_LEAVE);
			p<<res;
			_incomeSocket->Send(&p);
			return;
		}
		LOG_INFO("player can join this table");

		//广播此玩家离开桌子的消息
		Packet p;
		p.SetMessage(MSG_GS_BC_TABLE_LEAVE);
		p<<res<<tableID<<player->GetNickName()<<player->GetProtraitID();
		Broadcast(&p);
	}
	else
	{
		LOG_ERR(QString("Not Find Player from [%1:%2]").arg(_incomeSocket->IP()).arg(_incomeSocket->Port()));
	}
}

void ZjhGameServer::sendTableInfo( GSPlayerPtr _to )
{
	Packet p;
	p.SetMessage(MSG_GS_CL_TABLE_INFO);

	QMap<int, Table*> tables = TABLE.GetTables();
	p<<(int)tables.size();
	QMap<int, Table*>::iterator itr;
	for ( itr = tables.begin(); itr != tables.end(); itr++ )
	{
		p<<itr.key();
		p<<itr.value()->PlayerAmount();
		p<<itr.value()->GetMinBringChip();
		LOG_INFO(QString("[%1] players in table [%2]").arg(itr.value()->PlayerAmount()).arg(itr.key()));
		QMap<int, ISocketInstancePtr> players = itr.value()->GetSeatInfo();
		QMap<int, ISocketInstancePtr>::iterator pItr;
		for ( pItr = players.begin(); pItr  != players.end(); pItr++)
		{
			if ( pItr.value()->GetSocket() != NULL )
			{
				p<<pItr.key();
				//const GSPlayerPtr player = pItr.value().staticCast<GSPlayer>();
				GSPlayerPtr player = findPlayer(pItr.value());
				if ( player )
				{
					p<<player->GetNickName();
					p<<player->GetProtraitID();
					LOG_INFO(QString("Player[%1] seat at [%2]").arg(player->GetNickName()).arg(pItr.key()));
				}
			}		
		}
	}

	LOG_INFO(QString("Send table list to player[%1:%2]").arg(_to->IP()).arg(_to->Port()));
	_to->Send(&p);
}

void ZjhGameServer::ClientDisconnected( ISocketInstancePtr _clientSocket )
{
	// need remove this player
	GSPlayerPtr player = findPlayer(_clientSocket);
	if ( player != NULL )
	{
		quint32 tableID = 0;
		quint32 seatID = 0;
		int res = TABLE.GetPlayerCurrentTableInfo(player, tableID, seatID);
		if ( res != GS_NO_ERR )
		{
			LOG_WARN(QString("GetPlayerCurrentTableInfo Error[%1]").arg(res));
		}
		quint32 tableWalletMoney = 0;
		res = WalletDB.QueryTableWallet(mRoomInfo.mRoomID, tableID, seatID, tableWalletMoney);
		if ( res != WS_NO_ERR )
		{
			LOG_ERR(QString("QueryTableWallet Error[%1]").arg(res));
			return;
		}
		quint32 userWalletMoney = 0;
		res = WalletDB.QueryUserWallet(player->GetAccountID(), userWalletMoney);
		if ( res != WS_NO_ERR )
		{
			LOG_ERR(QString("QueryUserWallet Error[%1]").arg(res));
			return;
		}
		res = WalletDB.UpdateUserWallet(player->GetAccountID(), tableWalletMoney+userWalletMoney);
		if ( res != WS_NO_ERR )
		{
			LOG_ERR(QString("UpdateUserWallet Error[%1]").arg(res));
			return;
		}
		res = WalletDB.UpdateTableWallet(mRoomInfo.mRoomID, tableID, seatID, 0);
		if ( res != WS_NO_ERR )
		{
			LOG_ERR(QString("QueryTableWallet Error[%1]").arg(res));
			return;
		}
		LOG_INFO(QString("Player[%1] transfer money from tableWallet to userWallet successful, userWallet[%2]")
			.arg(player->GetNickName()).arg(tableWalletMoney+userWalletMoney));

		res = WalletDB.InsertTransactionRecord(TableToUser, tableWalletMoney, player->GetUserWalletID(), player->GetTableWalletID(), res);
		if ( res != WS_NO_ERR )
		{
			LOG_ERR(QString("InsertTransactionRecord Error[%1]").arg(res));
			return;
		}

		// leave table first
		quint32 from = 0;
		res = TABLE.StLeaveTable(player, from);
		LOG_INFO(QString("Player Leave from table[%1], res[%2]").arg(from).arg(res));
		if ( res != GS_NO_ERR )
		{
			LOG_WARN(QString("StLeaveTable[%1] Error[%2]").arg(from).arg(res));
		}
		// remove player from memory
		deletePlayer(player);

		
	}
}

GSPlayerPtr ZjhGameServer::findPlayer( ISocketInstancePtr _input )
{
	if ( mPlayerList.isEmpty() )
		return GSPlayerPtr(NULL);
	if ( _input->GetSocket()  == NULL )
		return GSPlayerPtr(NULL);
	
	QList<GSPlayerPtr>::iterator itr;
	for ( itr = mPlayerList.begin(); itr != mPlayerList.end(); itr++ )
	{
		if ( (*itr)->GetSocket() == _input->GetSocket() )
		{
			return *itr;
		}
	}

	LOG_ERR(QString("Can not find player from [%1:%2]").arg(_input->IP()).arg(_input->Port()));
	return GSPlayerPtr(NULL);
}

void ZjhGameServer::deletePlayer( GSPlayerPtr _player )
{
	QTcpSocket* socket = _player->GetSocket();
	QList<GSPlayerPtr>::iterator itr = qFind(mPlayerList.begin(), mPlayerList.end(), _player);
	if ( itr != mPlayerList.end() )
	{
		LOG_INFO(QString("Remove player[AccountID:%1]").arg(_player->GetAccountID()));
		mPlayerList.erase(itr);
		LOG_INFO(QString("%1 player in mPlayerList now").arg(mPlayerList.size()));
	}
}

void ZjhGameServer::processSyncStart( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	// 当玩家loading完成以后进入游戏时接收此消息
	quint32 tableID=0;
	quint32 seatID=0;
	_packet>>tableID>>seatID;

	TABLE.SetReadyToStart(tableID, seatID);

}

void ZjhGameServer::processFollow( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	int tableID = 0;
	int seatID = 0;
	int chip = 0;

	_packet>>tableID>>seatID>>chip;

	TABLE.Follow(tableID, seatID, chip);

}

void ZjhGameServer::processBringMoney( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	quint32 tableID = 0;
	quint32 seatID = 0;
	quint32 bringMoney = 0;
	_packet>>tableID>>seatID>>bringMoney;
	GSPlayerPtr player = findPlayer(_incomeSocket);
	if ( player )
	{
		// check user wallet to ensure money is enough
		quint32 userWalletMoney = 0;
		int res = WalletDB.QueryUserWallet(player->GetAccountID(), userWalletMoney);
		if ( res != WS_NO_ERR )
		{
			LOG_ERR(QString("Player[%1] query wallet error[%2]").arg(player->GetAccountID()).arg(res));
			goto SEND_RESULT;
		}

		if ( bringMoney > userWalletMoney )
		{
			LOG_ERR(QString("Player[%1]'s user wallet has[%1], but want bring[%2] money").arg(userWalletMoney).arg(bringMoney));
			res = ERR_WS_USERWALLET_NOT_ENOUGH;
			goto SEND_RESULT;
		}

		// withdraw money from user wallet to table wallet
		res = WalletDB.UpdateTableWallet(mRoomInfo.mRoomID, tableID, seatID, bringMoney);
		if ( res != WS_NO_ERR )
		{
			LOG_ERR(QString("Player[%1] withdraw from user wallet error[%2]").arg(player->GetAccountID()).arg(res));
			goto SEND_RESULT;
		}
		res = WalletDB.UpdateUserWallet(player->GetAccountID(), userWalletMoney-bringMoney);
		if ( res != WS_NO_ERR )
		{
			LOG_ERR(QString("Player[%1] desposit to table wallet error[%2]").arg(player->GetAccountID()).arg(res));
			goto SEND_RESULT;
		}

		player->SetTableWalletMoney(bringMoney);

		// query table wallet id
		quint32 tableWalletID = 0;
		res = WalletDB.QueryTableWalletID(mRoomInfo.mRoomID, tableID, seatID, tableWalletID);
		if ( res == WS_NO_ERR )
		{
			player->SetTableWalletID(tableWalletID);
		}
		else
		{
			LOG_ERR(QString("Can not find Player[%1]'s table wallet id, err[%2]").arg(player->GetAccountID()).arg(res));
		}

		// record transaction
		res = WalletDB.InsertTransactionRecord(UserToTable, bringMoney, player->GetUserWalletID(), player->GetTableWalletID(), res);
		if ( res != WS_NO_ERR )
		{
			LOG_ERR(QString("InsertTransactionRecord Error[%1]").arg(res));
			return;
		}

		res = WS_NO_ERR;
SEND_RESULT:
		// send result
		Packet p;
		p.SetMessage(MSG_GS_CL_BRING_MONEY);
		p<<res;
		_incomeSocket->Send(&p);

	}
	else
	{
		
	}
}

void ZjhGameServer::SetRoomInfo( RoomInfo _info )
{
	DATACENTER.mRoomInfo = _info;
	mRoomInfo = _info; 
}

void ZjhGameServer::processGiveUp( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	int tableID = 0;
	int seatID = 0;

	_packet>>tableID>>seatID;

	TABLE.GiveUp(tableID, seatID);

}

void ZjhGameServer::processQueryMoney( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	GSPlayerPtr player = findPlayer(_incomeSocket);
	if ( player )
	{
		quint32 goldCoin = player->GetUserWalletMoney();
		quint32 silverCoin = player->GetSilverCoin();

		Packet p;
		p.SetMessage(MSG_GS_CL_QUERY_MONEY);
		p<<goldCoin<<silverCoin;
		_incomeSocket->Send(&p);
	}


}
