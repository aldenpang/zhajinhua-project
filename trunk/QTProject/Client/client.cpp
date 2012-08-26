#include "stdafx.h"
#include "client.h"
#include "MouseEventFilter.h"
#include "MousePropagation.h"
#include "LoginUI.h"
#include "LobbyUI.h"
#include "ZJHGameUI.h"
#include "LogModule.h"
#include "Setting.h"
#include "MessageLoader.h"


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

	mZjhUI = new ZJHGameUI(mLobbyUI->GetGameServerNet());
	mZjhUI->Init();
	//mZjhUI->Show();
	initMouseEventFilter(mZjhUI->GetWidget());

	MSGLOADER.Load(QString("ClientMessages.txt"));

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
	connect(mLoginUI, SIGNAL(SiPlayerInfo(CommonPlayer)), mLobbyUI, SLOT(StUpdatePlayerInfo(CommonPlayer)));
	connect(mLoginUI, SIGNAL(SiPlayerInfo(CommonPlayer)), mZjhUI, SLOT(StUpdatePlayerInfo(CommonPlayer)));
	connect(mLoginUI, SIGNAL(SiLoginOK(QString, QString)), mLobbyUI, SLOT(StGetLoginInfo(QString, QString)));

	connect(mLobbyUI, SIGNAL(SiQuit()), this, SLOT(stQuit()));
	connect(mLobbyUI, SIGNAL(SiShowGame()), mZjhUI, SLOT(StShow()));
	connect(mLobbyUI, SIGNAL(SiMyTable(quint32, quint32)), mZjhUI, SLOT(StMyTable(quint32, quint32)));


	connect(mZjhUI, SIGNAL(SiQuit()), this, SLOT(stQuit()));

}
//------------------------------------------------------------------------------
void Client::stQuit()
{
	QApplication::quit();
}


