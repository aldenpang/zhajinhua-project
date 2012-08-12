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

class PokerItem;
class GameServerNet;
class ZJHGameUI : public BaseUI
{
	Q_OBJECT
public:
	ZJHGameUI(GameServerNet* _gameServerNet);
	~ZJHGameUI();

	void Init();

	void ShowShuffleAni();
	void HideShuffleAni();

	void ShowDistributeAni(quint32 _dealerIdx, quint32 _absentIdx1=-1, quint32 _absentIdx2=-1);

	void ShowPreShuffleAni();

signals:
	void SiQuit();
private slots:
	void stUpdate();
	void stMoveLeftPokers();

protected:
private:
	GameServerNet* mGameServer;
	QTimer mTimer;
	QGraphicsScene* mScene;
	void regConnections();
	QLabel* mShuffleLabel;
	QMovie* mShuffleAni;

	QVector<PokerItem*> mPokers;
	QVector<PokerItem*> mLeftPokers;
};

#endif