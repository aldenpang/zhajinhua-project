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
, mTempAccountID(1000000)
{
	mTimer.setInterval(10000);
	mTimer.start();
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(stTimeout()));

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
	case MSG_CL_GS_TABLE_INFO:
		processRequestTableInfo(_incomeSocket, _packet);
		break;
	default:
		break;
	}
}

void ZjhGameServer::processLogin( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	QString userName;
	QString pwd;
	int isTempLogin = 0;
	_packet>>isTempLogin;
	_packet>>userName>>pwd;
	int res = GS_NO_ERR;
	if ( !isTempLogin )
	{
		LOG_INFO(QString("[%1]wants to login, pwd is[%2]").arg(userName).arg(pwd));

		res = DB.VerifyUser(userName, pwd);

		if ( res != GS_NO_ERR )
			LOG_ERR(QString("LoginFailed! Reason:[%1]").arg(res));
		else
			LOG_INFO("Login OK");
	}

	// send login result
	Packet p;
	p.SetMessage(MSG_GS_CL_LOGIN);
	p<<res;
	_incomeSocket->Send(&p);

	if ( res == GS_NO_ERR )
	{
		// get player info
		GSPlayerPtr player = GSPlayerPtr(new GSPlayer(_incomeSocket->GetSocket()));
		if ( isTempLogin )
		{
			player->SetAccountID(mTempAccountID++);
			player->SetNickName("Temp");
			player->SetGender(0);
			player->SetProtraitID(0);
			player->SetSilverCoin(TEMP_COIN);
			// send table list
			sendTableInfo(player);

			mPlayerList.push_back(player);
		}
		else
		{
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

				quint32 silverCoin = 0;
				res = WalletDB.QuerySilverWallet(player->GetAccountID(), silverCoin);
				if ( res == WS_NO_ERR )
				{
					LOG_INFO(QString("Player[%1] 's silver coin is [%2]").arg(player->GetAccountID()).arg(silverCoin));
					player->SetSilverCoin(silverCoin);
				}
				else
					LOG_ERR(QString("QuerySilverWallet Failed! Reason:[%1]").arg(res));


				// TODO: send player info
			}
			else
			{
				LOG_ERR(QString("GetAccountID Failed! Reason:[%1]").arg(res));
			}
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
		int res = 0;
		if ( DATACENTER.mRoomInfo.mMoneyType == GOLD_COIN )
		{
			res = bringMoney_tableToUser(player);
		}
		else if ( DATACENTER.mRoomInfo.mMoneyType == SILVER_COIN )
		{
			res = bringMoney_tableToSilver(player);
		}
		if ( res != WS_NO_ERR )
		{
			LOG_ERR(QString("Error when bringMoney_tableToUser, err[%1]").arg(res));
		}
		quint32 leaveFrom = 0;
		res = TABLE.StLeaveTable(player, leaveFrom);
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
			if ( res == WS_NO_ERR )
			{
				if ( DATACENTER.mRoomInfo.mMoneyType == GOLD_COIN )
				{
					res = bringMoney_userToTable(player, tableID, seatID, mRoomInfo.mMinMoney);
				}
				else if ( DATACENTER.mRoomInfo.mMoneyType == SILVER_COIN )
				{
					// query player's silver coin
					quint32 silverCoin = 0;
					WalletDB.QuerySilverWallet(player->GetAccountID(), silverCoin);
					player->SetSilverCoin(silverCoin);		// update momery for insurance
					res = bringMoney_silverToTable(player, tableID, seatID, silverCoin);
				}
				if ( res == WS_NO_ERR )
				{
					// send the result of join table
					Packet p;
					p.SetMessage(MSG_GS_BC_TABLE_JOIN);
					p<<res<<tableID<<seatID<<player->GetNickName()<<player->GetProtraitID()<<getPlayerMoney(player);
					//_incomeSocket->Send(&p);
					Broadcast(&p);
					return;

				}
			}
			else
			{
				LOG_ERR(QString("InsertTableWallet Error, roomID[%1] tableID[%2] seatID[%3] res[%4]")
					.arg(mRoomInfo.mRoomID).arg(tableID).arg(seatID).arg(res));
			}
		}
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
		handlePlayerLeave(player);
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
					p<<getPlayerMoney(player);
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
		handlePlayerLeave(player);

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
	quint32 money = 0;
	_packet>>tableID>>seatID>>money;
	GSPlayerPtr player = findPlayer(_incomeSocket);
	if ( player )
	{
		bringMoney_userToTable(player, tableID, seatID, money);
	}
	else
	{
		LOG_ERR(QString("Can not find player"));
	}
}


int ZjhGameServer::bringMoney_userToTable( GSPlayerPtr _player, quint32 _tableID, quint32 _seatID, quint32 _money )
{
	// check user wallet to ensure money is enough
	quint32 userWalletMoney = 0;
	int res = WalletDB.QueryUserWallet(_player->GetAccountID(), userWalletMoney);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("Player[%1] query wallet error[%2]").arg(_player->GetAccountID()).arg(res));
		goto SEND_RESULT;
	}

	if ( _money > userWalletMoney )
	{
		LOG_ERR(QString("Player[%1]'s user wallet has[%1], but want bring[%2] money").arg(userWalletMoney).arg(_money));
		res = ERR_WS_USERWALLET_NOT_ENOUGH;
		goto SEND_RESULT;
	}

	// withdraw money from user wallet to table wallet
	res = WalletDB.UpdateTableWallet(mRoomInfo.mRoomID, _tableID, _seatID, _money);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("Player[%1] withdraw from user wallet error[%2]").arg(_player->GetAccountID()).arg(res));
		goto SEND_RESULT;
	}
	res = WalletDB.UpdateUserWallet(_player->GetAccountID(), userWalletMoney-_money);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("Player[%1] desposit to table wallet error[%2]").arg(_player->GetAccountID()).arg(res));
		goto SEND_RESULT;
	}

	_player->SetTableWalletMoney(_money);

	// query table wallet id
	quint32 tableWalletID = 0;
	res = WalletDB.QueryTableWalletID(mRoomInfo.mRoomID, _tableID, _seatID, tableWalletID);
	if ( res == WS_NO_ERR )
	{
		_player->SetTableWalletID(tableWalletID);
	}
	else
	{
		LOG_ERR(QString("Can not find Player[%1]'s table wallet id, err[%2]").arg(_player->GetAccountID()).arg(res));
	}

	// record transaction
	res = WalletDB.InsertTransactionRecord(UserToTable, _money, _player->GetUserWalletID(), _player->GetTableWalletID(), res);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("InsertTransactionRecord Error[%1]").arg(res));
		return res;
	}

	res = WS_NO_ERR;
SEND_RESULT:
	// send result
	Packet p;
	p.SetMessage(MSG_GS_CL_BRING_MONEY);
	p<<res;
	_player->Send(&p);

	return res;
}

int ZjhGameServer::bringMoney_silverToTable( GSPlayerPtr _player, quint32 _tableID, quint32 _seatID, quint32 _money )
{
	// check user wallet to ensure money is enough
	quint32 userWalletMoney = 0;
	int res = WalletDB.QuerySilverWallet(_player->GetAccountID(), userWalletMoney);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("Player[%1] query silver wallet error[%2]").arg(_player->GetAccountID()).arg(res));
		goto SEND_RESULT;
	}

	if ( _money > userWalletMoney )
	{
		LOG_ERR(QString("Player[%1]'s user wallet has[%2], BUT want bring[%3] money").arg(_player->GetAccountID()).arg(userWalletMoney).arg(_money));
		res = ERR_WS_USERWALLET_NOT_ENOUGH;
		goto SEND_RESULT;
	}

	// withdraw money from user wallet to table wallet
	res = WalletDB.UpdateTableWallet(mRoomInfo.mRoomID, _tableID, _seatID, _money);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("Player[%1] withdraw from user wallet error[%2]").arg(_player->GetAccountID()).arg(res));
		goto SEND_RESULT;
	}
	res = WalletDB.UpdateSilverWallet(_player->GetAccountID(), userWalletMoney-_money);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("Player[%1] desposit to table wallet error[%2]").arg(_player->GetAccountID()).arg(res));
		goto SEND_RESULT;
	}

	_player->SetTableWalletMoney(_money);

	// query table wallet id
	quint32 tableWalletID = 0;
	res = WalletDB.QueryTableWalletID(mRoomInfo.mRoomID, _tableID, _seatID, tableWalletID);
	if ( res == WS_NO_ERR )
	{
		_player->SetTableWalletID(tableWalletID);
	}
	else
	{
		LOG_ERR(QString("Can not find Player[%1]'s table wallet id, err[%2]").arg(_player->GetAccountID()).arg(res));
	}

	// record transaction
	res = WalletDB.InsertTransactionRecord(SilverToTable, _money, _player->GetUserWalletID(), _player->GetTableWalletID(), res);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("InsertTransactionRecord Error[%1]").arg(res));
		return res;
	}

	res = WS_NO_ERR;
SEND_RESULT:
	// send result
	Packet p;
	p.SetMessage(MSG_GS_CL_BRING_MONEY);
	p<<res;
	_player->Send(&p);

	return res;
}


void ZjhGameServer::SetRoomInfo( RoomInfo _info )
{
	DATACENTER.mRoomInfo = _info;
	mRoomInfo = _info; 

	LOG_INFO(QString("///////////////////////////////////"));
	LOG_INFO(QString("RoomInfo:MoneyType[%1] MinMoney[%2] RoomName[%3] RoomID[%4] GameType[%5]").arg(mRoomInfo.mMoneyType==GOLD_COIN?QString("Gold Coin"):QString("Silver Coin"))
		.arg(mRoomInfo.mMinMoney).arg(mRoomInfo.mName).arg(mRoomInfo.mRoomID).arg(mRoomInfo.mType==ZJH?QString("ZJH"):QString("OtherGame")));
	LOG_INFO(QString("///////////////////////////////////"));
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
		quint32 goldCoin = 0;
		int res = WalletDB.QueryUserWallet(player->GetAccountID(), goldCoin);
		if ( res == WS_NO_ERR )
		{
			player->SetUserWalletMoney(goldCoin);
		}
		else
			LOG_ERR(QString("Player[%1] query money error[%2]").arg(player->GetAccountID()).arg(res));
		quint32 silverCoin = player->GetSilverCoin();

		Packet p;
		p.SetMessage(MSG_GS_CL_QUERY_MONEY);
		p<<goldCoin<<silverCoin;
		_incomeSocket->Send(&p);
	}


}

int ZjhGameServer::bringMoney_tableToUser( GSPlayerPtr _player )
{
	quint32 tableID = 0;
	quint32 seatID = 0;
	int res = TABLE.GetPlayerCurrentTableInfo(_player, tableID, seatID);
	if ( res != GS_NO_ERR )
	{
		LOG_WARN(QString("GetPlayerCurrentTableInfo Error[%1]").arg(res));
		return res;
	}
	quint32 tableWalletMoney = 0;
	res = WalletDB.QueryTableWallet(mRoomInfo.mRoomID, tableID, seatID, tableWalletMoney);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("QueryTableWallet Error[%1]").arg(res));
		return res;
	}
	quint32 userWalletMoney = 0;
	res = WalletDB.QueryUserWallet(_player->GetAccountID(), userWalletMoney);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("QueryUserWallet Error[%1]").arg(res));
		return res;
	}
	res = WalletDB.UpdateUserWallet(_player->GetAccountID(), tableWalletMoney+userWalletMoney);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("UpdateUserWallet Error[%1]").arg(res));
		return res;
	}
	res = WalletDB.UpdateTableWallet(mRoomInfo.mRoomID, tableID, seatID, 0);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("QueryTableWallet Error[%1]").arg(res));
		return res;
	}
	LOG_INFO(QString("Player[%1] transfer money from tableWallet to userWallet successful, userWallet[%2]")
		.arg(_player->GetNickName()).arg(tableWalletMoney+userWalletMoney));

	res = WalletDB.InsertTransactionRecord(TableToUser, tableWalletMoney, _player->GetUserWalletID(), _player->GetTableWalletID(), res);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("InsertTransactionRecord Error[%1]").arg(res));
		return res;
	}

	return res;
}
int ZjhGameServer::bringMoney_tableToSilver( GSPlayerPtr _player )
{
	quint32 tableID = 0;
	quint32 seatID = 0;
	int res = TABLE.GetPlayerCurrentTableInfo(_player, tableID, seatID);
	if ( res != GS_NO_ERR )
	{
		LOG_WARN(QString("GetPlayerCurrentTableInfo Error[%1]").arg(res));
		return res;
	}
	quint32 tableWalletMoney = 0;
	res = WalletDB.QueryTableWallet(mRoomInfo.mRoomID, tableID, seatID, tableWalletMoney);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("QueryTableWallet Error[%1]").arg(res));
		return res;
	}
	quint32 userWalletMoney = 0;
	res = WalletDB.QuerySilverWallet(_player->GetAccountID(), userWalletMoney);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("QuerySilverWallet Error[%1]").arg(res));
		return res;
	}
	res = WalletDB.UpdateSilverWallet(_player->GetAccountID(), tableWalletMoney+userWalletMoney);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("UpdateUserWallet Error[%1]").arg(res));
		return res;
	}
	res = WalletDB.UpdateTableWallet(mRoomInfo.mRoomID, tableID, seatID, 0);
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("QueryTableWallet Error[%1]").arg(res));
		return res;
	}
	LOG_INFO(QString("Player[%1] transfer money[%2] from tableWallet to silverWallet successful, userWallet[%3]")
		.arg(_player->GetNickName()).arg(tableWalletMoney).arg(tableWalletMoney+userWalletMoney));

	//res = WalletDB.InsertTransactionRecord(TableToSilver, tableWalletMoney, _player->GetUserWalletID(), _player->GetTableWalletID(), res);
	//if ( res != WS_NO_ERR )
	//{
	//	LOG_ERR(QString("InsertTransactionRecord Error[%1]").arg(res));
	//	return res;
	//}

	return res;
}

quint32 ZjhGameServer::getPlayerMoney( GSPlayerPtr _player )
{
	if ( DATACENTER.mRoomInfo.mMoneyType == GOLD_COIN )
	{
		return _player->GetTableWalletMoney();
	}else if ( DATACENTER.mRoomInfo.mMoneyType == SILVER_COIN )
	{
		return _player->GetSilverCoin();
	}else
		return 0;
}

void ZjhGameServer::handlePlayerLeave( GSPlayerPtr _player )
{
	int res = 0;
	if ( DATACENTER.mRoomInfo.mMoneyType == GOLD_COIN )
	{
		res = bringMoney_tableToUser(_player);
	}
	else if ( DATACENTER.mRoomInfo.mMoneyType == SILVER_COIN )
	{
		res = bringMoney_tableToSilver(_player);
	}
	if ( res != WS_NO_ERR )
	{
		LOG_ERR(QString("Error when bringMoney_tableToUser, err[%1]").arg(res));
	}
	quint32 from;
	// 检查是否可离开桌子
	res = TABLE.StLeaveTable(_player, from);
	if ( res != GS_NO_ERR )
	{
		LOG_WARN(QString("player[%1] can not join this table[%2], res[%3]").arg(_player->GetAccountID()).arg(from).arg(res));

		// 不能离开桌子
		Packet p;
		p.SetMessage(MSG_GS_CL_TABLE_LEAVE);
		p<<res;
		_player->Send(&p);
		return;
	}
	LOG_INFO("player can join this table");

	//广播此玩家离开桌子的消息
	Packet p;
	p.SetMessage(MSG_GS_BC_TABLE_LEAVE);
	p<<res<<from<<_player->GetNickName()<<_player->GetProtraitID();
	Broadcast(&p);
}

void ZjhGameServer::processRequestTableInfo( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	GSPlayerPtr p = findPlayer(_incomeSocket);
	if ( p )
	{
		sendTableInfo(p);
	}
}

void ZjhGameServer::stTimeout()
{
	LOG_INFO(QString("GameServer is Running, [%1] players online").arg(mPlayerList.size()));
}