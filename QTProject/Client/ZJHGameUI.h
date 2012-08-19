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
#include "TableInfo.h"

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
public slots:
	void StShowGame(TableInfo _tableInfo);

signals:
	void SiQuit();
private slots:
	void stUpdate();
	void stMoveLeftPokers();
	void stDropBaseChip(int _baseChip);
	void stDistribute(QVector<int> _pokers);
	void stCurrentPlayer(int _currentPlayer);
	void stTableEnd();
	void stFollow(int _seatID, int _chip, int _currentPlayer, int _currentBid);
	void stSyncStart();

protected:
private:
	GameServerNet* mGameServer;
	QTimer mTimer;
	QGraphicsScene* mScene;
	void regConnections();
	QLabel* mShuffleLabel;
	QMovie* mShuffleAni;
	TableInfo mTableInfo;

	QVector<PokerItem*> mPokers;
	QVector<PokerItem*> mLeftPokers;


	void reset();
};

#endif