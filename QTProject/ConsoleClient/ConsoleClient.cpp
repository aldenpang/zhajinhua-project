#include "ConsoleClient.h"
#include "LoginServerNet.h"
#include "GameServerNet.h"
#include "LogModule.h"

ConsoleClient::ConsoleClient()
{
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

	mGameServer = new GameServerNet();
	mGameServer->Init();
	connect(mGameServer, SIGNAL(SiLoginOK()), this, SLOT(stGSLoginOK()));
	connect(mGameServer, SIGNAL(SiLoginFailed(quint32)), this, SLOT(stGSLoginFailed(quint32)));
	connect(mGameServer, SIGNAL(SiConnected()), this, SLOT(stGSConnected()));
	connect(mGameServer, SIGNAL(SiDisconnected()), this, SLOT(stGSDisconnected()));
	connect(mGameServer, SIGNAL(SiTableList(QMap<int, TableData>)), this, SLOT(stTableList(QMap<int, TableData>)));
	connect(mGameServer, SIGNAL(SiTableJoinResult(quint32)), this, SLOT(stTableJoinRes(quint32)));
	connect(mGameServer, SIGNAL(SiStartGame()), this, SLOT(stStartGame()));


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

	// TODO: ����Ӧ�ô������б���ѡ��һ�����˵Ľ��루Ϊ�˿���������
	int tableID = 0; 
	int seatID = 0;
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
				seatID = i;
				LOG_INFO(QString("Seat[%1] in Table[%2] is not occupied").arg(seatID).arg(itr.key()));
				goto End;
				//break;
			}

		}
	}
End:
	LOG_INFO(QString("Join to table[%1] seat[%2]").arg(tableID).arg(seatID));
	mGameServer->SendJoinTable(tableID, seatID);
	return;
}

void ConsoleClient::stTableJoinRes( quint32 _res )
{
	if ( _res == GS_NO_ERR )
	{
		LOG_INFO("Join Table OK");
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
		LOG_ERR(QString("Table Join Result:%1").arg(_res));


}

void ConsoleClient::stStartGame()
{
	LOG_INFO("Start Game, client should start loading");

	// ��ConsoleClient�ֱ�ӷ��Ϳ�ʼ��ϢMSG_CL_GS_SYNC_START
	Packet p;
	p.SetMessage(MSG_CL_GS_SYNC_START);
	mGameServer->Send(&p);

}
