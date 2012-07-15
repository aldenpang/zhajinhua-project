#include "stdafx.h"
#include "client.h"
#include "MouseEventFilter.h"
#include "MousePropagation.h"
#include "LoginServerNet.h"
#include "GameServerNet.h"
#include "Packet.h"

QString gUserName = "acc4";
QString gPassword = "1234";

int argcCount = 1;
Client::Client(int & argc, char ** argv)
	: QApplication(argcCount, argv)
	, mLoginServer(NULL)
{
	QResource::registerResource(QString("media.dat"));

	//ui.setupUi(this);

	QUiLoader uiLoader;
	QString fileStr(":/Client/client.ui");
	QFile uiFile(fileStr);
	mMainWidget = uiLoader.load(&uiFile);
	if ( mMainWidget )
	{
		mMainWidget->setWindowFlags(Qt::FramelessWindowHint);
		mMainWidget->show();

		initMouseEventFilter();
		regConnections();
		initLoginServer();
		initGameServer();
	}
}

Client::~Client()
{
	if ( mLoginServer )
	{
		delete mLoginServer;
		mLoginServer = NULL;
	}
}
//------------------------------------------------------------------------------
void Client::initMouseEventFilter()
{
	mMouseEventFilter = new MouseEventFilter(mMainWidget, mMainWidget, MouseEventFilter::Enable_Drag | MouseEventFilter::Enable_Scale);
}
//------------------------------------------------------------------------------
void Client::regConnections()
{
	QPushButton* quitBtn = mMainWidget->findChild<QPushButton*>("btn_quit");
	if ( quitBtn )
		connect(quitBtn, SIGNAL(clicked()), this, SLOT(stQuit()));
}
//------------------------------------------------------------------------------
void Client::stQuit()
{
	QApplication::quit();
}
//------------------------------------------------------------------------------
void Client::initLoginServer()
{
	mLoginServer = new LoginServerNet();
	mLoginServer->Init();
	//mLoginServer->Connect("localhost", 5000);
	connect(mLoginServer, SIGNAL(SiError(QString)), this, SLOT(stNetError(QString)));
	connect(mLoginServer, SIGNAL(SiLoginOK()), this, SLOT(stLoginOK()));
	connect(mLoginServer, SIGNAL(SiLoginFailed(quint32)), this, SLOT(stLoginFailed(quint32)));
	connect(mLoginServer, SIGNAL(SiGameList(QVector<RoomInfo>)), this, SLOT(stGameList(QVector<RoomInfo>)));

	//mLoginServer->SendLoginRequest(gUserName, gPassword);

}

void Client::stLoginOK()
{
	mLoginServer->RequestGameList(0);
}

void Client::stNetError( QString _err )
{
	qDebug()<<__FUNCTION__<<":"<<_err;
}

void Client::stLoginFailed( quint32 _errorCode )
{
	qDebug()<<__FUNCTION__<<":"<<_errorCode;
}

void Client::stGameList( QVector<RoomInfo> _gameList )
{
	int size = _gameList.size();
	qDebug()<<__FUNCTION__<<"GameList size:"<<size;

	//emit siConnectGS(_gameList[0].mIP, _gameList[0].mPort);
	stConnectGS(_gameList[0].mIP, _gameList[0].mPort);
}

void Client::initGameServer()
{
	mGameServer = new GameServerNet();
	mGameServer->Init();

	//connect(this, SIGNAL(siConnectGS(QString&, quint32)), mGameServer, SLOT(stConnectGS(QString&, quint32)));
	connect(mGameServer, SIGNAL(SiError(QString)), this, SLOT(stNetError(QString)));
	connect(mGameServer, SIGNAL(SiConnected()), this, SLOT(stGSConnected()));
	connect(mGameServer, SIGNAL(SiLoginOK()), this, SLOT(stGSLoginOK()));
	connect(mGameServer, SIGNAL(SiLoginFailed(quint32)), this, SLOT(stGSLoginFailed(quint32)));
}

void Client::stConnectGS( QString& _ip, quint32 _port )
{
	if ( mGameServer )
	{
		mGameServer->Connect(_ip, _port);
	}

}

void Client::stGSConnected()
{
	mGameServer->SendLoginGS(gUserName, gPassword);

	return;
}

void Client::stGSLoginOK()
{
	mGameServer->SendJoinTable(0, 0);
	return;
}

void Client::stGSLoginFailed( quint32 _errCode )
{
	qDebug()<<__FUNCTION__<<":"<<_errCode;
}
