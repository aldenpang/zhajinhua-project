/********************************************************************
	Copyright (C) 2005 by Bele Group
	@date:	    2011-12-31   14:48
	@file: 		ZJHGameServer.h
	@author:    Snake
	@desc:		
*********************************************************************/
#ifndef _ZJHGAMESERVER_H_
#define _ZJHGAMESERVER_H_

#include "PublicHeader.h"
#include "IServerLayer.h"


class ZjhGameServer : public IServerLayer
{
	Q_OBJECT
public slots:
	virtual void StStart();
	virtual void StStop();
	virtual void StRestart();

	void StNewConnections();

private slots:
	void stClientDisconnect();			
public:
	ZjhGameServer();
	~ZjhGameServer();
protected:
	
private:
};

#endif //_ZJHGAMESERVER_H_

/*
 *
 * [Revision 1.0   2011-12-31   14:48  Administrator] Created
 *
 */