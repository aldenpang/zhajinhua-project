#include "stdafx.h"
#include "client.h"
#include "MouseEventFilter.h"
#include "MousePropagation.h"
#include "INetLayer.h"


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
	mNetLayer = new INetLayer();
	mNetLayer->Init();
	mNetLayer->Connect("localhost", 5000);
}