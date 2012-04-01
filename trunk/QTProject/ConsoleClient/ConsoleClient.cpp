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
	mGameServer->SendJoinTable(0, 0);
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
