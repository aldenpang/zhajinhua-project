/********************************************************************
	Copyright (C) 2005 by Bele Group
	@date:	    2011-12-31   14:58
	@file: 		AppFrame.h
	@author:    Snake
	@desc:		
*********************************************************************/
#ifndef _APPFRAME_H_
#define _APPFRAME_H_

#include "PublicHeader.h"
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
public:
	AppFrame();
	~AppFrame();

	void Run();

	void InitDatabase();
protected:
private:
	ZjhGameServer* mGameServer;
	RoomInfo mRoomInfo;

};

#endif //_APPFRAME_H_

/*
 *
 * [Revision 1.0   2011-12-31   14:58  Administrator] Created
 *
 */