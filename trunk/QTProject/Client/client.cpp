#include "stdafx.h"
#include "client.h"
#include "MouseEventFilter.h"
#include "MousePropagation.h"
#include "LoginUI.h"
#include "LobbyUI.h"
#include "ZJHGameUI.h"
#include "LogModule.h"
#include "Setting.h"

QString gUserName = "acc4";
QString gPassword = "1234";

int argcCount = 1;
Client::Client(int & argc, char ** argv)
	: QApplication(argcCount, argv)
{

	LOG_D_INFO("Client Start");

	QResource::registerResource(QString("media.dat"));

	SETTINGS.Init("ClientConfig.ini");

	mLoginUI = new LoginUI;
	mLoginUI->Init();
	mLoginUI->Show();
	initMouseEventFilter(mLoginUI->GetWidget());

	mLobbyUI = new LobbyUI;
	mLobbyUI->Init();
	//mLobbyUI->Show();
	initMouseEventFilter(mLobbyUI->GetWidget());

	mZjhUI = new ZJHGameUI;
	mZjhUI->Init();
	//mZjhUI->Show();
	initMouseEventFilter(mZjhUI->GetWidget());

	regConnections();
}

Client::~Client()
{
	SAFE_DELETE(mLoginUI);
	SAFE_DELETE(mLobbyUI);
}
//------------------------------------------------------------------------------
void Client::initMouseEventFilter(QWidget* _widget)
{
	mMouseEventFilter = new MouseEventFilter(_widget, _widget, MouseEventFilter::Enable_Drag | MouseEventFilter::Enable_Scale);
}
//------------------------------------------------------------------------------
void Client::regConnections()
{
	connect(mLoginUI, SIGNAL(SiQuit()), this, SLOT(stQuit()));
	connect(mLoginUI, SIGNAL(SiShowLobby(QVector<RoomInfo>)), mLobbyUI, SLOT(StShowLobby(QVector<RoomInfo>)));

	connect(mLobbyUI, SIGNAL(SiQuit()), this, SLOT(stQuit()));
	connect(mZjhUI, SIGNAL(SiQuit()), this, SLOT(stQuit()));

}
//------------------------------------------------------------------------------
void Client::stQuit()
{
	QApplication::quit();
}

//void Client::initGameServer()
//{
//	mGameServer = new GameServerNet();
//	mGameServer->Init();
//
//	//connect(this, SIGNAL(siConnectGS(QString&, quint32)), mGameServer, SLOT(stConnectGS(QString&, quint32)));
//	connect(mGameServer, SIGNAL(SiError(QString)), this, SLOT(stNetError(QString)));
//	connect(mGameServer, SIGNAL(SiConnected()), this, SLOT(stGSConnected()));
//	connect(mGameServer, SIGNAL(SiLoginOK()), this, SLOT(stGSLoginOK()));
//	connect(mGameServer, SIGNAL(SiLoginFailed(quint32)), this, SLOT(stGSLoginFailed(quint32)));
//}
//
//void Client::stConnectGS( QString& _ip, quint32 _port )
//{
//	if ( mGameServer )
//	{
//		mGameServer->Connect(_ip, _port);
//	}
//
//}
//
//void Client::stGSConnected()
//{
//	mGameServer->SendLoginGS(gUserName, gPassword);
//
//	return;
//}
//
//void Client::stGSLoginOK()
//{
//	mGameServer->SendJoinTable(0, 0);
//	return;
//}
//
//void Client::stGSLoginFailed( quint32 _errCode )
//{
//	qDebug()<<__FUNCTION__<<":"<<_errCode;
//}
