/********************************************************************
	Copyright (C) 2005 by Pang Shuo
	@date:	    2012-7-15   15:12
	@file: 		LobbyUI.h
	@author:    Snake
	@desc:		Lobby����Ĳ��ֺ��߼�
*********************************************************************/

#ifndef LOBBYUI_H
#define LOBBYUI_H

#include "BaseUI.h"

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

	void regConnections();

};

#endif