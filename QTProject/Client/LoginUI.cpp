#include "stdafx.h"
#include "LoginUI.h"
#include "LoginServerNet.h"
#include "GameServerNet.h"
#include "Packet.h"
#include "Downloader.h"

LoginUI::LoginUI()
{

}

LoginUI::~LoginUI()
{

}
//Downloader downloader;

void LoginUI::Init()
{
	QUiLoader uiLoader;
	QString fileStr(":/Client/client.ui");
	QFile uiFile(fileStr);
	mMainWidget = uiLoader.load(&uiFile);
	if ( mMainWidget )
	{
		mMainWidget->setWindowFlags(Qt::FramelessWindowHint);
	}

	regConnections();
	//downloader.Start(QString("http://ww3.sinaimg.cn/large/97ed3185jw1duzuczkiaqj111.jpg"), QString("C:/1.jpg"));
}

void LoginUI::regConnections()
{
 	QPushButton* quitBtn = mMainWidget->findChild<QPushButton*>("btn_quit");
	if ( quitBtn )
 		connect(quitBtn, SIGNAL(clicked()), this, SIGNAL(SiQuit()));
}

////------------------------------------------------------------------------------
//void Client::initLoginServer()
//{
//	mLoginServer = new LoginServerNet();
//	mLoginServer->Init();
//	//mLoginServer->Connect("localhost", 5000);
//	connect(mLoginServer, SIGNAL(SiError(QString)), this, SLOT(stNetError(QString)));
//	connect(mLoginServer, SIGNAL(SiLoginOK()), this, SLOT(stLoginOK()));
//	connect(mLoginServer, SIGNAL(SiLoginFailed(quint32)), this, SLOT(stLoginFailed(quint32)));
//	connect(mLoginServer, SIGNAL(SiGameList(QVector<RoomInfo>)), this, SLOT(stGameList(QVector<RoomInfo>)));
//
//	//mLoginServer->SendLoginRequest(gUserName, gPassword);
//
//}

//void Client::stLoginOK()
//{
//	mLoginServer->RequestGameList(0);
//}
//
//void Client::stNetError( QString _err )
//{
//	qDebug()<<__FUNCTION__<<":"<<_err;
//}
//
//void Client::stLoginFailed( quint32 _errorCode )
//{
//	qDebug()<<__FUNCTION__<<":"<<_errorCode;
//}
//
//void Client::stGameList( QVector<RoomInfo> _gameList )
//{
//	int size = _gameList.size();
//	qDebug()<<__FUNCTION__<<"GameList size:"<<size;
//
//	//emit siConnectGS(_gameList[0].mIP, _gameList[0].mPort);
//	stConnectGS(_gameList[0].mIP, _gameList[0].mPort);
//}
//