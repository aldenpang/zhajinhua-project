#include "stdafx.h"
#include "ZJHGameUI.h"

ZJHGameUI::ZJHGameUI()
{

}

ZJHGameUI::~ZJHGameUI()
{

}

void ZJHGameUI::Init()
{
	QUiLoader uiLoader;
	QString fileStr(":/Client/zjh.ui");
	QFile uiFile(fileStr);
	mMainWidget = uiLoader.load(&uiFile);
	if ( mMainWidget )
	{
		mMainWidget->setWindowFlags(Qt::FramelessWindowHint);
	}

	regConnections();
}

void ZJHGameUI::regConnections()
{
	connect(mMainWidget->findChild<QPushButton*>("btn_quit"), SIGNAL(clicked()), this, SIGNAL(SiQuit()));
}