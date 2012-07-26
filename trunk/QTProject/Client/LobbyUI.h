/********************************************************************
	Copyright (C) 2005 by Pang Shuo
	@date:	    2012-7-15   15:12
	@file: 		LobbyUI.h
	@author:    Snake
	@desc:		Lobby界面的布局和逻辑
*********************************************************************/

#ifndef LOBBYUI_H
#define LOBBYUI_H

#include "BaseUI.h"
#include "Table.h"

class GameList;
class LobbyUI : public BaseUI
{
	Q_OBJECT
public:
	LobbyUI();
	~LobbyUI();

	void Init();


signals:
	void SiQuit();
private slots:


protected:
private:
	QMap<quint32, Table*> mTableList;
	QGraphicsScene* mScene;
	GameList* mGameList;

	void regConnections();

	void initTables(quint32 _amount);

};

#endif