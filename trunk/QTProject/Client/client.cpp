#include "stdafx.h"
#include "client.h"
#include "MouseEventFilter.h"
#include "MousePropagation.h"
#include "ClientNet.h"
#include "Packet.h"
#include "MD5.h"


int argcCount = 1;
Client::Client(int & argc, char ** argv)
	: QApplication(argcCount, argv)
	, mNetLayer(NULL)
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
		initNetLayer();
	}
}

Client::~Client()
{
	if ( mNetLayer )
	{
		delete mNetLayer;
		mNetLayer = NULL;
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
void Client::initNetLayer()
{
	mNetLayer = new ClientNet();
	mNetLayer->Init();
	mNetLayer->Connect("localhost", 5000);

	Packet p;
	p.SetMessage(MSG_CL_LS_LOGIN);
	QString userName("acc4");
	//p.Put(userName.size());
	//p.Put(const_cast<char*>(userName.toStdString().c_str()));
	QString md5pwd = ToMD5(QString("1234"));
	//p.Put(md5pwd.size());
	//p.Put(const_cast<char*>(md5pwd.toStdString().c_str()));
	p<<userName<<md5pwd;
	mNetLayer->Send(&p);
}