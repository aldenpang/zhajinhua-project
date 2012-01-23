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
class ZjhGameServer;
class AppFrame : public QObject
{
public:
	AppFrame();
	~AppFrame();

	void Run();
protected:
private:
	ZjhGameServer* mGameServer;
};

#endif //_APPFRAME_H_

/*
 *
 * [Revision 1.0   2011-12-31   14:58  Administrator] Created
 *
 */