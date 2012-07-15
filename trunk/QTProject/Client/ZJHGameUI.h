/********************************************************************
	Copyright (C) 2005 by Pang Shuo
	@date:	    2012-7-15   15:30
	@file: 		ZJHGameUI.h
	@author:    Snake
	@desc:		ZJH界面的布局和逻辑
*********************************************************************/

#ifndef ZJHGAMEUI_H
#define ZJHGAMEUI_H

#include "BaseUI.h"

class ZJHGameUI : public BaseUI
{
	Q_OBJECT
public:
	ZJHGameUI();
	~ZJHGameUI();

	void Init();


signals:
	void SiQuit();
private slots:


protected:
private:

	void regConnections();

};

#endif