#include "stdafx.h"
#include "LobbyUI.h"

LobbyUI::LobbyUI()
{

}

LobbyUI::~LobbyUI()
{

}

void LobbyUI::Init()
{
	QUiLoader uiLoader;
	QString fileStr(":/Client/lobby.ui");
	QFile uiFile(fileStr);
	mMainWidget = uiLoader.load(&uiFile);
	if ( mMainWidget )
	{
		mMainWidget->setWindowFlags(Qt::FramelessWindowHint);
	}

	initTables();

	regConnections();
}

void LobbyUI::regConnections()
{
	connect(mMainWidget->findChild<QPushButton*>("btn_quit"), SIGNAL(clicked()), this, SIGNAL(SiQuit()));
}

void LobbyUI::initTables()
{
	QUiLoader uiLoader;
	QString fileStr(":/Client/table.ui");
	QFile uiFile(fileStr);
	QWidget* tableWidget = uiLoader.load(&uiFile);
	if ( tableWidget )
	{
		QScrollArea* tableScroll = mMainWidget->findChild<QScrollArea*>("tableList");
		tableWidget->setParent(tableScroll->widget());
	}

	QFile uiFile2(fileStr);
	QWidget* tableWidget2 = uiLoader.load(&uiFile2);
	if ( tableWidget2 )
	{
		QScrollArea* tableScroll = mMainWidget->findChild<QScrollArea*>("tableList");
		tableWidget2->setParent(tableScroll->widget());
		tableWidget2->move(100, 0);
	}
}
