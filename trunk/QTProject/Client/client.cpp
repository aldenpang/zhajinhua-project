#include "stdafx.h"
#include "client.h"
#include "MouseEventFilter.h"
#include "MousePropagation.h"
#include "LoginServerNet.h"
#include "GameServerNet.h"
#include "Packet.h"
#include "MD5.h"


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
	mLoginServer->Connect("localhost", 5000);
	connect(mLoginServer, SIGNAL(SiError(QString)), this, SLOT(stNetError(QString)));
	connect(mLoginServer, SIGNAL(SiLoginOK()), this, SLOT(stLoginOK()));
	connect(mLoginServer, SIGNAL(SiLoginFailed(quint32)), this, SLOT(stLoginFailed(quint32)));
	connect(mLoginServer, SIGNAL(SiGameList(QVector<RoomInfo>)), this, SLOT(stGameList(QVector<RoomInfo>)));

	Packet p;
	p.SetMessage(MSG_CL_LS_LOGIN);
	QString userName("acc4");
	QString md5pwd = ToMD5(QString("1234"));
	p<<userName<<md5pwd;
	mLoginServer->Send(&p);
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
}

void Client::initGameServer()
{
	mGameServer = new GameServerNet();
	mGameServer->Init();
}
