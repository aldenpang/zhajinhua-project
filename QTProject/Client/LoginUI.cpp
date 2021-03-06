#include "stdafx.h"
#include "LoginUI.h"
#include "LoginServerNet.h"
#include "GameServerNet.h"
#include "Packet.h"
#include "Downloader.h"
#include "Setting.h"
#include "BaseDialog.h"

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

	mUserName = mMainWidget->findChild<QLineEdit*>("edit_username");
	mPassword = mMainWidget->findChild<QLineEdit*>("edit_password");

	// load urls
	QString url = SETTINGS.GetLoginADURL();
	//mMainWidget->findChild<QWebView*>("")

	initLoginServer();

	regConnections();
	//downloader.Start(QString("http://ww3.sinaimg.cn/large/97ed3185jw1duzuczkiaqj111.jpg"), QString("C:/1.jpg"));

	//mPatcher.Start(QString("PatchFileList.xml"));
}

void LoginUI::regConnections()
{
 	QPushButton* btn_quit = mMainWidget->findChild<QPushButton*>("btn_quit");
 	connect(btn_quit, SIGNAL(clicked()), this, SIGNAL(SiQuit()));

	QPushButton* btn_login = mMainWidget->findChild<QPushButton*>("btn_login");
	connect(btn_login, SIGNAL(clicked()), this, SLOT(stLogin()));

	QPushButton* btn_quickLogin = mMainWidget->findChild<QPushButton*>("btn_directEnter");
	connect(btn_quickLogin, SIGNAL(clicked()), this, SLOT(stQuickLogin()));

}

void LoginUI::stLogin()
{
	if ( !mLoginServer->IsValid() )
	{
		mLoginServer->Connect(SETTINGS.GetIP(), SETTINGS.GetPort());
	}
	QString userName = mUserName->text();
	QString password = mPassword->text();

	mLoginServer->SendLoginRequest(userName, password);
}

//------------------------------------------------------------------------------
void LoginUI::initLoginServer()
{
	mLoginServer = new LoginServerNet();
	mLoginServer->Init();
	
	connect(mLoginServer, SIGNAL(SiError(QString)), this, SLOT(stNetError(QString)));
	connect(mLoginServer, SIGNAL(SiLoginOK()), this, SLOT(stLoginOK()));
	connect(mLoginServer, SIGNAL(SiLoginFailed(quint32)), this, SLOT(stLoginFailed(quint32)));
	connect(mLoginServer, SIGNAL(SiGameList(QVector<RoomInfo>)), this, SLOT(stGameList(QVector<RoomInfo>)));
	connect(mLoginServer, SIGNAL(SiPlayerInfo(CommonPlayer)), this, SIGNAL(SiPlayerInfo(CommonPlayer)));
	connect(mLoginServer, SIGNAL(SiTempLogin()), this, SLOT(stTempLogin()));
}

void LoginUI::stLoginOK()
{
	mLoginServer->RequestGameList(ZJH);
	LOG_D_INFO("LoginOK, Request Game List");

	QString userName = mUserName->text();
	QString password = mPassword->text();

	mPassword->setText("");

	emit SiLoginOK(userName, password);
	return;
}

void LoginUI::stNetError( QString _err )
{
	//qDebug()<<__FUNCTION__<<":"<<_err;
	LOG_D_ERR(_err);
}
//
void LoginUI::stLoginFailed( quint32 _errorCode )
{
	//qDebug()<<__FUNCTION__<<":"<<_errorCode;
	LOG_D_ERR(QString("ErrorCode:%1").arg(_errorCode));
}

void LoginUI::stGameList( QVector<RoomInfo> _gameList )
{
	int size = _gameList.size();
	qDebug()<<__FUNCTION__<<"GameList size:"<<size;

	emit SiShowLobby(_gameList);
}

void LoginUI::stQuickLogin()
{
	if ( !mLoginServer->IsValid() )
	{
		mLoginServer->Connect(SETTINGS.GetIP(), SETTINGS.GetPort());
	}

	Packet p;
	p.SetMessage(MSG_CL_LS_LOGIN_ANONYMOUS);
	mLoginServer->Send(&p);
}

void LoginUI::stTempLogin()
{
	SETTINGS.SetTempLogin(true);
}
