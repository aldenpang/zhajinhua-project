#include "stdafx.h"
#include "ZJHGameUI.h"
#include "PokerItem.h"
#include "Setting.h"
#include "GameServerNet.h"
#include "BalanceDialog.h"

QPoint gTableCenter = QPoint(360, 220);
//QPoint gPokerPos[MAX_PLAYER][MAX_PLAYER] = {{QPoint(300, 360), QPoint(550, 220), QPoint(300, 60), QPoint(110, 220)}, 
											//{QPoint(550, 220), QPoint(300, 60), QPoint(110, 220), QPoint(300, 360)}, 
											//{QPoint(300, 60), QPoint(110, 220), QPoint(300, 360), QPoint(550, 220)}, 
											//{QPoint(110, 220), QPoint(300, 360), QPoint(550, 220), QPoint(300, 60)}};

QPoint gPokerPos[MAX_PLAYER] = {QPoint(300, 60), QPoint(550, 220), QPoint(300, 360), QPoint(110, 220)};
quint32 gPokerGap = 20;
QPoint gLeftPoker = QPoint(20, 20);


ZJHGameUI::ZJHGameUI(GameServerNet* _gameServerNet)
: mGameServer(_gameServerNet)
, mMyTableID(0)
, mMySeatID(0)
{
	mTimer.setInterval(50);
}

ZJHGameUI::~ZJHGameUI()
{
	SAFE_DELETE(mBalanceDlg);
}

void ZJHGameUI::Init()
{
	QUiLoader uiLoader;
	QString fileStr(":/Client/zjh.ui");
	QFile uiFile(fileStr);
	mMainWidget = uiLoader.load(&uiFile);
	if ( mMainWidget )
	{
		mMainWidget->setWindowFlags(Qt::FramelessWindowHint);
	}

	mTimer.setSingleShot(true);
	mTimer.start(5000);

	mBalanceDlg = new BalanceDialog(mMainWidget->findChild<QWidget*>("balance"));

	QGraphicsView* view = mMainWidget->findChild<QGraphicsView*>("graphicsView");
	mScene = new QGraphicsScene(0, 0, 
		view->size().width()-view->verticalScrollBar()->size().width(),
		view->size().height()-view->horizontalScrollBar()->size().height());
	//mScene->setBackgroundBrush(Qt::blue);
	view->setScene(mScene);

	for ( int i = 0; i<MAX_PLAYER; i++ )
	{
		mPortrait[i] = mMainWidget->findChild<QLabel*>(QString("playerImage_%1").arg(i));
		mNickName[i] = mMainWidget->findChild<QLabel*>(QString("nickNameText_%1").arg(i));
		mCoin[i] = mMainWidget->findChild<QLabel*>(QString("goldCoinText_%1").arg(i));
		mCoinLogo[i] = mMainWidget->findChild<QLabel*>(QString("goldCoin_%1").arg(i));
	}

	mShuffleLabel = mMainWidget->findChild<QLabel*>("shuffleAni");
	mShuffleAni = new QMovie(":/Images/Media/shuffle.gif");
	mShuffleLabel->setMovie(mShuffleAni);
	HideShuffleAni();
	

	int amount = MAX_HAND_POKER*MAX_PLAYER;
	int playerIdx = 0;
	QVector<PokerItem*> v;
	for ( int i = 0; i<amount; i++ )
	{
		PokerItem* item = new PokerItem(0);
		item->setZValue(i);
		item->setPos(gPokerPos[playerIdx].x()+gPokerGap*(i%MAX_HAND_POKER), gPokerPos[playerIdx].y());
		item->ToBack();
		item->hide();
		mScene->addItem(item);
		v.push_back(item);

		if ( v.size() == MAX_HAND_POKER )
		{
			mPokers.insert(playerIdx, v);
			v.clear();	// 存入mPokers时是值传递，这里clear了应该不影响下一个的值
			playerIdx++;
		}
	}

	hideAllClocks();
	hideGiveUps();
	mContinueTimer.setSingleShot(true);
	mContinueTimer.setInterval(5000);

	//for ( int i = 0; i<amount; i++ )
	//{
	//	PokerItem* item = new PokerItem(0);
	//	item->setZValue(i);
	//	item->setPos(gTableCenter);
	//	item->ToBack();
	//	mScene->addItem(item);
	//	mPokers.push_back(item);
	//}

	//for ( int i = 0; i<amount; i++ )
	//{
	//	PokerItem* last = new PokerItem(0);	// last poker as background
	//	last->setZValue(0);
	//	last->setPos(gTableCenter);
	//	last->ToBack();
	//	mScene->addItem(last);
	//	mLeftPokers.push_back(last);
	//}

	//ShowDistributeAni(0, 1);

	//addChip(1000);

	regConnections();
}

void ZJHGameUI::regConnections()
{
	connect(mMainWidget->findChild<QPushButton*>("btn_quit"), SIGNAL(clicked()), this, SIGNAL(SiQuit()));
	connect(mMainWidget->findChild<QPushButton*>("btn_back"), SIGNAL(clicked()), this, SLOT(stBtn_Back()));
	connect(mMainWidget->findChild<QPushButton*>("btn_back"), SIGNAL(clicked()), mBalanceDlg, SLOT(StHide()));
	connect(mMainWidget->findChild<QPushButton*>("btn_oper_1"), SIGNAL(clicked()), this, SLOT(stBtn_Follow()));
	connect(mMainWidget->findChild<QPushButton*>("btn_oper_2"), SIGNAL(clicked()), this, SLOT(stBtn_test()));
	connect(mMainWidget->findChild<QPushButton*>("btn_oper_7"), SIGNAL(clicked()), this, SLOT(stBtn_Giveup()));
	connect(mBalanceDlg, SIGNAL(SiBack()), this, SLOT(stBtn_Back()));
	connect(mBalanceDlg, SIGNAL(SiContinue()), this, SLOT(stContinue()));
	connect(&mContinueTimer, SIGNAL(timeout()), this, SLOT(stContinue()));
	connect(&mContinueTimer, SIGNAL(timeout()), mBalanceDlg, SLOT(StHide()));
	
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(stUpdate()));

	connect(mGameServer, SIGNAL(SiTableJoinResult(quint32, quint32, quint32, TablePlayer)), this, SLOT(stTableJoinResult(quint32, quint32, quint32, TablePlayer)));
	connect(mGameServer, SIGNAL(SiTableLeaveResult(quint32, quint32, TablePlayer)), this, SLOT(stTableLeaveResult(quint32, quint32, TablePlayer)));
	connect(mGameServer, SIGNAL(SiStartGame(TableInfo)), this, SLOT(StStartGame(TableInfo)));
	connect(mGameServer, SIGNAL(SiDropBaseChip(int)), this, SLOT(stDropBaseChip(int)));
	connect(mGameServer, SIGNAL(SiDistribute(QVector<int>)), this, SLOT(stDistribute(QVector<int>)));
	connect(mGameServer, SIGNAL(SiCurrentPlayer(int)), this, SLOT(stCurrentPlayer(int)));
	connect(mGameServer, SIGNAL(SiTableEnd(TableInfo, QMap<int, int>)), this, SLOT(stTableEnd(TableInfo, QMap<int, int>)));
	connect(mGameServer, SIGNAL(SiFollow(int, int, int, int)), this, SLOT(stFollow(int, int, int, int)));
	connect(mGameServer, SIGNAL(SiGiveUp(int)), this, SLOT(stGiveUp(int)));
	connect(mGameServer, SIGNAL(SiSyncStart()), this, SLOT(stSyncStart()));
	connect(mGameServer, SIGNAL(SiUpdateMoney(quint32, quint32)), this, SLOT(stUpdateMoney(quint32, quint32)));
	connect(mGameServer, SIGNAL(SiTableList(QMap<int, TableData>)), this, SLOT(stTableList(QMap<int, TableData>)));

}

void ZJHGameUI::stUpdate()
{
	//QLabel* poker = mMainWidget->findChild<QLabel*>("poker2_2");
	//if ( poker )
	//{
	//	poker->move(poker->pos().x()+1, poker->pos().y());
	//}

	//ShowPreShuffleAni();
}

void ZJHGameUI::ShowShuffleAni()
{
	mShuffleLabel->show();
	mShuffleAni->start();
}

void ZJHGameUI::HideShuffleAni()
{
	mShuffleLabel->hide();
}

void ZJHGameUI::ShowDistributeAni( quint32 _dealerIdx, quint32 _absentIdx1/*=-1*/, quint32 _absentIdx2/*=-1*/ )
{
	//if ( _dealerIdx >= MAX_PLAYER || _dealerIdx < 0 )
	//	return;

	//if ( _absentIdx1 == 0 || _absentIdx2 == 0 )
	//	return;
	//
	//QPoint* pos = gPokerPos[_dealerIdx];
	//int posIdx = 0;
	//int gap = 0;
	//for ( int i = 0; i<mPokers.size(); i++, posIdx++ )
	//{
	//	// 如果当前发牌的player不在，就跳到下一家发牌，本来发给他的牌要push到mLeftPokers中等待收走
	//	if ( posIdx == _absentIdx1 || posIdx == _absentIdx2 )
	//	{
	//		mLeftPokers.push_back(mPokers[i]);
	//		continue;
	//	}
	//	
	//	if ( i != 0 && i %  MAX_PLAYER == 0 )
	//	{
	//		gap++;
	//		posIdx = 0;
	//	}

	//	mPokers[i]->Move(150*i, 1000, QPoint(pos[posIdx].x()+gPokerGap*gap, pos[posIdx].y()));
	//}
	//
	//QTimer::singleShot(150*MAX_PLAYER*MAX_HAND_POKER, this, SLOT(stMoveLeftPokers()));
}

void ZJHGameUI::ShowPreShuffleAni()
{
	//for ( int i = 0; i<mPokers.size(); i++ )
	//{
	//	mPokers[i]->Move(50*i, 1000, gTableCenter);
	//}
	//for ( int i = 0; i<mLeftPokers.size(); i++ )
	//{
	//	mLeftPokers[i]->Move(50*i, 1000, gTableCenter);
	//}
}

void ZJHGameUI::stMoveLeftPokers()
{
	//for ( int i = 0; i<mLeftPokers.size(); i++ )
	//{
	//	mLeftPokers[i]->Move(50*i, 1000, gLeftPoker);
	//}
}

void ZJHGameUI::StStartGame( TableInfo _tableInfo )
{
	reset();
	
	ShowShuffleAni();
	// update table info
	mTableInfo = _tableInfo;

	QMap<int, TablePlayer>::iterator itr;
	for ( itr = mTableInfo.mPlayers.begin(); itr != mTableInfo.mPlayers.end(); itr++ )
	{
		if ( SETTINGS.GetPlayer().GetNickName() == itr.value().mNickName )
			// 在这里重新设置一次
			mMySeatID = itr.key();
	}

	for ( itr = mTableInfo.mPlayers.begin(); itr != mTableInfo.mPlayers.end(); itr++ )
	{
		updatePlayerInfo((Seat)itr.key(), itr.value());
	}
}

void ZJHGameUI::reset()
{
	// update money type
	for ( int i = 0; i<MAX_PLAYER; i++ )
	{
		if ( SETTINGS.GetRoomInfo().mMoneyType == GOLD_COIN )
		{
			mCoinLogo[i]->setPixmap(QPixmap(QString(":/Images/Media/goldCoin.png")));
		}
		else if( SETTINGS.GetRoomInfo().mMoneyType == SILVER_COIN )
		{
			mCoinLogo[i]->setPixmap(QPixmap(QString(":/Images/Media/silverCoin.png")));
		}
		else
		{
			mCoinLogo[i]->setPixmap(QPixmap(QString(":/Images/Media/goldCoin.png")));
		}
	}

	hidePokers();
	hideChips();
	hideGiveUps();
	mPlayers.clear();

	for ( int i = 0; i<MAX_PLAYER; i++ )
	{
		resetPlayerInfo((Seat)i);
	}
}

void ZJHGameUI::stSyncStart()
{
	ShowShuffleAni();
	LOG_D_INFO("<-= MSG_CL_GS_SYNC_START =->");
	// start a timer to show shuffle ani for seconds, then send sync start
	QTimer::singleShot(5000, this, SLOT(stRespSyncStart()));
}

void ZJHGameUI::stDropBaseChip( int _baseChip )
{
	// _baseChip是一个人投的底注数量
	//ShowDistributeAni(mTableInfo.mDealerSeat);

	int base = mTableInfo.mBaseChip*mPlayers.size();
	addChip(base);

	QLabel* totalCoin = mMainWidget->findChild<QLabel*>("totalCoin");
	totalCoin->setText(QString("%1").arg(base));
}

void ZJHGameUI::stDistribute( QVector<int> _pokers )
{
	//TODO: 发牌时会无故增加内存
	HideShuffleAni();
	// 设置自己的手牌
	mPlayers[mMySeatID].SetPokers(_pokers);

	// 显示所有人的手牌（背面）
	QMap<int, TablePlayer>::iterator itr;
	for ( itr = mPlayers.begin(); itr != mPlayers.end(); itr++ )
	{
		showPokers(itr.key(), true);
	}

	// 显示自己的手牌
	showPokers(mMySeatID, false);
	//for ( int i = 0; i<MAX_HAND_POKER; i++ )
	//{
	//	quint32 dd = convertSeatID(mMySeatID);
	//	mPokers[convertSeatID(mMySeatID)].value(i)->ToFront(_pokers[i]);
	//}
}

void ZJHGameUI::stCurrentPlayer( int _currentPlayer )
{
	showClock(_currentPlayer);
	if (_currentPlayer == mMySeatID)
		unlockOperBtn();
	else
		lockOperBtn();
}

void ZJHGameUI::stTableEnd(TableInfo _tableInfo, QMap<int, int> _res)
{
	// update table info
	mTableInfo = _tableInfo;

	QMap<int, TablePlayer>::iterator itr;
	for ( itr = mTableInfo.mPlayers.begin(); itr != mTableInfo.mPlayers.end(); itr++ )
	{
		if ( SETTINGS.GetPlayer().GetNickName() == itr.value().mNickName )
			// 在这里重新设置一次
			mMySeatID = itr.key();
		else
		{
			mPlayers[itr.key()].SetPokers(itr.value().GetPokers());
			showPokers(itr.key(), false);
		}

	}

	for ( itr = mTableInfo.mPlayers.begin(); itr != mTableInfo.mPlayers.end(); itr++ )
	{
		updatePlayerInfo((Seat)itr.key(), itr.value());
	}

	mBalanceDlg->SetAndShow(_res, mPlayers);
	mContinueTimer.start();
	hideAllClocks();
	lockOperBtn();
}

void ZJHGameUI::stFollow( int _seatID, int _chip, int _currentPlayer, int _currentBid )
{
	addChip(_chip);

	QLabel* totalCoin = mMainWidget->findChild<QLabel*>("totalCoin");
	totalCoin->setText(QString("%1").arg(_currentBid));

}

void ZJHGameUI::StUpdatePlayerInfo( CommonPlayer _player )
{
	QLabel* nickNameLabel = mMainWidget->findChild<QLabel*>("nickNameText");
	nickNameLabel->setText(_player.GetNickName());

	QLabel* imageLabel = mMainWidget->findChild<QLabel*>("playerImage");
	imageLabel->setPixmap(QPixmap(QString(":/Portraits/Media/Portrait/%1.png").arg(_player.GetProtraitID())));

	QLabel* levelLabel = mMainWidget->findChild<QLabel*>("levelText");
	levelLabel->setText(QString("Lv.%1").arg(SETTINGS.GetLevel(_player.GetExp())));

	stUpdateMoney(_player.GetUserWalletMoney(), _player.GetSilverCoin());

}

void ZJHGameUI::stUpdateMoney( quint32 _goldCoin, quint32 _silverCoin )
{
	mMainWidget->findChild<QLabel*>("goldCoinText")->setText(QString("%1").arg(_goldCoin));
	mMainWidget->findChild<QLabel*>("silverCoinText")->setText(QString("%1").arg(_silverCoin));
}

void ZJHGameUI::StMyTable( quint32 _tableID, quint32 _seatID )
{
	mMyTableID = _tableID;
	mMySeatID = _seatID;
}

void ZJHGameUI::updatePlayerInfo( Seat _seat, TablePlayer& _player )
{
	if ( _player.mNickName == EMPTY_SEAT )
	{
		return;
	}
	quint32 dd = convertSeatID(_seat);
	mPortrait[convertSeatID(_seat)]->setPixmap(QPixmap(QString(":/Portraits/Media/Portrait/%1.png").arg(_player.mProtraitID)));
	mNickName[convertSeatID(_seat)]->setText(_player.mNickName);
	mCoin[convertSeatID(_seat)]->setText(QString("%1").arg(_player.mTableMoney));

	mPlayers.insert(_seat, _player);
}

void ZJHGameUI::stTableJoinResult( quint32 _res, quint32 _tableID, quint32 _seatID, TablePlayer _player )
{
	//if ( _res != GS_NO_ERR  )
	//	return;
	if ( _res != WS_NO_ERR )
		return;
	if ( _tableID != mMyTableID )
		return;

	updatePlayerInfo((Seat)_seatID, _player);
}

void ZJHGameUI::stTableLeaveResult( quint32 _res, quint32 _tableID, TablePlayer _player )
{
	if ( _res != GS_NO_ERR  )
		return;
	//if ( _res != WS_NO_ERR )
	//	return;
	if ( _tableID != mMyTableID )
		return;

	for ( int i = 0; i<MAX_PLAYER; i++ )
	{
		if ( mNickName[i]->text() == _player.mNickName )
		{
			resetPlayerInfo((Seat)i);

			// if player is self, hide game
			//if ( _player.mNickName == SETTINGS.GetPlayer().GetNickName() )
			//{
			//	Hide();
			//}
			QMap<int, TablePlayer>::iterator itr = mPlayers.find(i);
			if ( itr != mPlayers.end() )
			{
				mPlayers.erase(itr);
			}
			return;
		}
	}
}

quint32 ZJHGameUI::convertSeatID( quint32 _serverID )
{
	// 客户端的id顺序跟server端一致，只是显示的时候将自己固定显示在最下面，其他人依次顺时针旋转
	// 顺序是按照顺时针方向，上0，右1，下2，左3
	//	   [0]
	//[3]		[1]
	//	   [2]
	return (_serverID - mMySeatID + 6) % MAX_PLAYER;
}

void ZJHGameUI::stBtn_Back()
{
	mGameServer->SendLeaveTable(mMyTableID);

	reset();

	Hide();
}

void ZJHGameUI::StShow()
{
	Show();

	refreshPlayerMoney();
	//mGameServer->RequestTableInfo();
}

void ZJHGameUI::refreshPlayerMoney()
{
	// query current user wallet
	Packet p;
	p.SetMessage(MSG_CL_GS_QUERY_MONEY);
	mGameServer->Send(&p);
}

void ZJHGameUI::stTableList( QMap<int, TableData> _tableList )
{
	//QMap<int, TableData>::iterator itr = _tableList.find(mMyTableID);
	//if ( itr != _tableList.end() )
	//{
	//	QMap<int, TablePlayer> players = itr.value().GetPlayers();
	//	QMap<int, TablePlayer>::iterator pItr;
	//	for ( pItr = players.begin(); pItr != players.end(); pItr++)
	//	{
	//		updatePlayerInfo((Seat)pItr.key(), pItr.value());
	//	}		
	//}
}

void ZJHGameUI::addChip( quint32 _money )
{
	int chipAmount = _money / 5;
	for ( int i = 0; i<chipAmount; i++ )
	{
		MoveItem* chip = new MoveItem(":/Images/Media/chip.png");
		// 分布在围绕中心+-100个像素的范围内，z轴随机
		chip->setPos(gTableCenter.x()+(qrand()%2?1:-1)*(qrand()%100), gTableCenter.y()+(qrand()%2?1:-1)*(qrand()%100));
		chip->setZValue(qrand()%100);
		mScene->addItem(chip);
		mChips.push_back(chip);
	}
	
}

void ZJHGameUI::showPokers( quint32 _seatID, bool _isBackUp )
{
	// show pokers
	QMap<int, QVector<PokerItem*>>::iterator itr = mPokers.find(convertSeatID(_seatID));
	QVector<int> pokers = mPlayers[_seatID].GetPokers();
	if ( itr != mPokers.end() )
	{
		for ( int i = 0; i<MAX_HAND_POKER; i++ )
		{
			if(_isBackUp)
				itr.value()[i]->ToBack();
			else
			{
				if(pokers.size() != 0)
					itr.value()[i]->ToFront(pokers[i]);
			}
			itr.value()[i]->show();
		}
	}
}

void ZJHGameUI::hidePokers()
{
	QMap<int, QVector<PokerItem*>>::iterator itr;
	for ( itr = mPokers.begin(); itr != mPokers.end(); itr++ )
	{
		for ( int i = 0; i < MAX_HAND_POKER; i++ )
		{
			itr.value()[i]->hide();
		}
	}
}

void ZJHGameUI::hideChips()
{
	for ( int i = 0; i<mChips.size(); i++ )
	{
		mScene->removeItem(mChips[i]);
	}
}

void ZJHGameUI::resetPlayerInfo( Seat _seat )
{
	mPortrait[_seat]->setPixmap(QPixmap(QString(":/Images/Media/nobodyImage.png")));
	mNickName[_seat]->setText("Unknown");
	mCoin[_seat]->setText(QString("0"));

}

void ZJHGameUI::stBtn_Follow()
{
	int chip = mTableInfo.mBaseChip;
	Packet p;
	p.SetMessage(MSG_CL_GS_FOLLOW);
	p<<mMyTableID<<mMySeatID<<chip;
	mGameServer->Send(&p);
}

void ZJHGameUI::hideAllClocks()
{
	QLabel* clocks[MAX_PLAYER];
	
	for (int i= 0; i<MAX_PLAYER; i++)
	{
		clocks[i] = mMainWidget->findChild<QLabel*>(QString("clock_%1").arg(i));
		clocks[i]->hide();
	}
	
}

void ZJHGameUI::showClock( int _seatID )
{
	hideAllClocks();
	QLabel* clock = mMainWidget->findChild<QLabel*>(QString("clock_%1").arg(convertSeatID(_seatID)));
	clock->show();
	QMovie* movie = new QMovie(":/Images/Media/clock.gif");
	clock->setMovie(movie);
	movie->start();
}

void ZJHGameUI::stRespSyncStart()
{
	Packet p;
	p.SetMessage(MSG_CL_GS_SYNC_START);
	mGameServer->Send(&p);
}

void ZJHGameUI::stContinue()
{
	mContinueTimer.stop();

	Packet p;
	p.SetMessage(MSG_CL_GS_CONTINUE);
	p<<mMyTableID<<mMySeatID;
	mGameServer->Send(&p);
}

void ZJHGameUI::lockOperBtn()
{
	for (int i = 1; i<8; i++)
	{
		QPushButton* btn = mMainWidget->findChild<QPushButton*>(QString("btn_oper_%1").arg(i));
		if (btn)
		{
			btn->setEnabled(false);
		}
	}
}

void ZJHGameUI::unlockOperBtn()
{
	for (int i = 1; i<8; i++)
	{
		QPushButton* btn = mMainWidget->findChild<QPushButton*>(QString("btn_oper_%1").arg(i));
		if (btn)
		{
			btn->setEnabled(true);
		}
	}
}

void ZJHGameUI::stBtn_test()
{
	QVector<int> testP;
	testP.push_back(1);
	testP.push_back(2);
	testP.push_back(3);

	mPlayers[1].SetPokers(testP);
	showPokers(1, rand()%2==0?true:false);
}

void ZJHGameUI::stBtn_Giveup()
{
	Packet p;
	p.SetMessage(MSG_CL_GS_GIVEUP);
	p<<mMyTableID<<mMySeatID;
	mGameServer->Send(&p);
}

void ZJHGameUI::stGiveUp( int _seatID )
{
	showGiveUp(_seatID);
}

void ZJHGameUI::hideGiveUps()
{
	for (int i = 0; i<MAX_PLAYER; i++)
	{
		QLabel* giveup = mMainWidget->findChild<QLabel*>(QString("giveup_%1").arg(i));
		if(giveup)
			giveup->hide();
	}
}

void ZJHGameUI::showGiveUp( int _seatID )
{
	QLabel* giveup = mMainWidget->findChild<QLabel*>(QString("giveup_%1").arg(_seatID));
	if(giveup)
		giveup->show();
}
