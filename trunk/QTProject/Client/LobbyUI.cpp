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

	regConnections();
}

void LobbyUI::regConnections()
{

}