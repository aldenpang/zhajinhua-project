/********************************************************************
	Copyright (C) 2013 by Alden Pang
	@date:	    2011-12-31   14:58
	@file: 		AppFrame.h
	@author:    Alden Pang
	@desc:		
*********************************************************************/
#ifndef _APPFRAME_H_
#define _APPFRAME_H_

#include "PublicHeader.h"
#include "WalletServerLayer.h"
#include "SharedData.h"
using namespace SharedData;

class ZjhGameServer;
class AppFrame : public QObject
{
	Q_OBJECT
signals:
	void SiInfo(const QString& _text);
	void SiWarn(const QString& _text);
	void SiError(const QString& _text);
private slots:
	void stWalletConnected();
	void stWalletDisconnected();
	void stWalletError(QString _err);

public:
	AppFrame();
	~AppFrame();

	void Run();

	void InitDatabase();

	void InitWalletServer();

	void InitWalletDB();
protected:
private:
	ZjhGameServer* mGameServer;
	RoomInfo mRoomInfo;
	WalletServerLayer mWalletServer;
};

#endif //_APPFRAME_H_

/*
 *
 * [Revision 1.0   2011-12-31   14:58  Administrator] Created
 *
 */