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
#include "CommonPlayer.h"
#include "TableData.h"

enum Seat
{
	TOP=0,
	RIGHT,
	BOTTOM,
	LEFT,

	TOTAL,
};
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
	void StShow();
	void StStartGame(TableInfo _tableInfo);
	void StUpdatePlayerInfo(CommonPlayer _player);
	void StMyTable(quint32 _tableID, quint32 _seatID);

signals:
	void SiQuit();
private slots:
	void stBtn_Back();

	void stUpdate();
	void stMoveLeftPokers();
	void stDropBaseChip(int _baseChip);
	void stDistribute(QVector<int> _pokers);
	void stCurrentPlayer(int _currentPlayer);
	void stTableEnd();
	void stFollow(int _seatID, int _chip, int _currentPlayer, int _currentBid);
	void stSyncStart();
	void stUpdateMoney( quint32 _goldCoin, quint32 _silverCoin );
	void stTableJoinResult(quint32 _res, quint32 _tableID, quint32 _seatID, TablePlayer _player);
	void stTableLeaveResult(quint32 _res, quint32 _tableID, TablePlayer _player);

protected:
private:
	GameServerNet* mGameServer;
	QTimer mTimer;
	QGraphicsScene* mScene;
	void regConnections();
	QLabel* mShuffleLabel;
	QMovie* mShuffleAni;
	TableInfo mTableInfo;
	quint32 mMyTableID;
	quint32 mMySeatID;
	QLabel* mPortrait[MAX_PLAYER];
	QLabel* mNickName[MAX_PLAYER];
	QLabel* mCoin[MAX_PLAYER];
	QLabel* mCoinLogo[MAX_PLAYER];

	QVector<PokerItem*> mPokers;
	QVector<PokerItem*> mLeftPokers;

private:
	void reset();
	void updatePlayerInfo(Seat _seat, quint32 _protraitID, QString& _nickName, quint32 _money);
	quint32 convertSeatID(quint32 _serverID);
};

#endif