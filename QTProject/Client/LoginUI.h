/********************************************************************
	Copyright (C) 2005 by Pang Shuo
	@date:	    2012-7-15   14:58
	@file: 		LoginUI.h
	@author:    Snake
	@desc:		Login����Ĳ��ֺ��߼�
*********************************************************************/

#ifndef LOGINUI_H
#define LOGINUI_H

#include "BaseUI.h"
#include "Patcher.h"
#include "CommonPlayer.h"

class LoginServerNet;
class LoginUI : public BaseUI
{
	Q_OBJECT
public:
	LoginUI();
	~LoginUI();

	void Init();

signals:
	void SiQuit();
	void SiShowLobby(QVector<RoomInfo> _gameList);
	void SiPlayerInfo(CommonPlayer _player);
	void SiLoginOK(QString _username, QString _pwd);

private slots:
	void stLoginOK();
	void stNetError(QString _err);
	void stLoginFailed(quint32 _errorCode);
	void stGameList(QVector<RoomInfo> _gameList);

	void stLogin();
protected:
private:
	Patcher mPatcher;
	LoginServerNet* mLoginServer;
	QLineEdit* mUserName;
	QLineEdit* mPassword;

	void regConnections();

	void initLoginServer();
};

#endif