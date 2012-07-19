/********************************************************************
	Copyright (C) 2005 by Pang Shuo
	@date:	    2012-7-15   14:58
	@file: 		LoginUI.h
	@author:    Snake
	@desc:		Login界面的布局和逻辑
*********************************************************************/

#ifndef LOGINUI_H
#define LOGINUI_H

#include "BaseUI.h"
#include "Patcher.h"
class LoginUI : public BaseUI
{
	Q_OBJECT
public:
	LoginUI();
	~LoginUI();

	void Init();

signals:
	void SiQuit();
private slots:
	//void stLoginOK();
	//void stNetError(QString _err);
	//void stLoginFailed(quint32 _errorCode);
	//void stGameList(QVector<RoomInfo> _gameList);

protected:
private:
	Patcher mPatcher;

	void regConnections();

	//void initLoginServer();
};

#endif