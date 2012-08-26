#include "stdafx.h"
#include "ZJHGameUI.h"
#include "PokerItem.h"
#include "Setting.h"
#include "GameServerNet.h"

QPoint gTableCenter = QPoint(360, 220);
QPoint gPokerPos[MAX_PLAYER][MAX_PLAYER] = {{QPoint(300, 360), QPoint(550, 220), QPoint(300, 60), QPoint(110, 220)}, 
											{QPoint(550, 220), QPoint(300, 60), QPoint(110, 220), QPoint(300, 360)}, 
											{QPoint(300, 60), QPoint(110, 220), QPoint(300, 360), QPoint(550, 220)}, 
											{QPoint(110, 220), QPoint(300, 360), QPoint(550, 220), QPoint(300, 60)}};
quint32 gPokerGap = 20;
QPoint gLeftPoker = QPoint(20, 20);


ZJHGameUI::ZJHGameUI(GameServerNet* _gameServerNet)
: mGameServer(_gameServerNet)
, mMySeatID(0)
{
	mTimer.setInterval(50);
}

ZJHGameUI::~ZJHGameUI()
{

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

	regConnections();

	mTimer.setSingleShot(true);
	mTimer.start(5000);

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

	mShuffleLabel = new QLabel(mMainWidget);
	mShuffleLabel->move(gTableCenter+QPoint(50, 100));
	mShuffleAni = new QMovie(":/Images/Media/shuffle.gif");
	mShuffleLabel->setMovie(mShuffleAni);
	HideShuffleAni();
	//ShowShuffleAni();
	
	//int amount = MAX_HAND_POKER*MAX_PLAYER;

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
}

void ZJHGameUI::regConnections()
{
	connect(mMainWidget->findChild<QPushButton*>("btn_quit"), SIGNAL(clicked()), this, SIGNAL(SiQuit()));
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(stUpdate()));

	connect(mGameServer, SIGNAL(SiStartGame(TableInfo)), this, SLOT(StStartGame(TableInfo)));
	connect(mGameServer, SIGNAL(SiDropBaseChip(int)), this, SLOT(stDropBaseChip(int)));
	connect(mGameServer, SIGNAL(SiDistribute(QVector<int>)), this, SLOT(stDistribute(QVector<int>)));
	connect(mGameServer, SIGNAL(SiCurrentPlayer(int)), this, SLOT(stCurrentPlayer(int)));
	connect(mGameServer, SIGNAL(SiTableEnd()), this, SLOT(stTableEnd()));
	connect(mGameServer, SIGNAL(SiFollow(int, int, int, int)), this, SLOT(stFollow(int, int, int, int)));
	connect(mGameServer, SIGNAL(SiSyncStart()), this, SLOT(stSyncStart()));
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
	mShuffleAni->stop();
}

void ZJHGameUI::ShowDistributeAni( quint32 _dealerIdx, quint32 _absentIdx1/*=-1*/, quint32 _absentIdx2/*=-1*/ )
{
	if ( _dealerIdx >= MAX_PLAYER || _dealerIdx < 0 )
		return;

	if ( _absentIdx1 == 0 || _absentIdx2 == 0 )
		return;
	
	QPoint* pos = gPokerPos[_dealerIdx];
	int posIdx = 0;
	int gap = 0;
	for ( int i = 0; i<mPokers.size(); i++, posIdx++ )
	{
		// 如果当前发牌的player不在，就跳到下一家发牌，本来发给他的牌要push到mLeftPokers中等待收走
		if ( posIdx == _absentIdx1 || posIdx == _absentIdx2 )
		{
			mLeftPokers.push_back(mPokers[i]);
			continue;
		}
		
		if ( i != 0 && i %  MAX_PLAYER == 0 )
		{
			gap++;
			posIdx = 0;
		}

		mPokers[i]->Move(150*i, 1000, QPoint(pos[posIdx].x()+gPokerGap*gap, pos[posIdx].y()));
	}
	
	QTimer::singleShot(150*MAX_PLAYER*MAX_HAND_POKER, this, SLOT(stMoveLeftPokers()));
}

void ZJHGameUI::ShowPreShuffleAni()
{
	for ( int i = 0; i<mPokers.size(); i++ )
	{
		mPokers[i]->Move(50*i, 1000, gTableCenter);
	}
	for ( int i = 0; i<mLeftPokers.size(); i++ )
	{
		mLeftPokers[i]->Move(50*i, 1000, gTableCenter);
	}
}

void ZJHGameUI::stMoveLeftPokers()
{
	for ( int i = 0; i<mLeftPokers.size(); i++ )
	{
		mLeftPokers[i]->Move(50*i, 1000, gLeftPoker);
	}
}

void ZJHGameUI::StStartGame( TableInfo _tableInfo )
{
	reset();

	ShowShuffleAni();
	// update table info
	mTableInfo = _tableInfo;

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
}

void ZJHGameUI::stSyncStart()
{
	//LOG_INFO("<-= MSG_CL_GS_SYNC_START =->");
	//Packet p;
	//p.SetMessage(MSG_CL_GS_SYNC_START);
	//mGameServer->Send(&p);
}

void ZJHGameUI::stDropBaseChip( int _baseChip )
{

}

void ZJHGameUI::stDistribute( QVector<int> _pokers )
{

}

void ZJHGameUI::stCurrentPlayer( int _currentPlayer )
{

}

void ZJHGameUI::stTableEnd()
{

}

void ZJHGameUI::stFollow( int _seatID, int _chip, int _currentPlayer, int _currentBid )
{

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

	updatePlayerInfo(BOTTOM, _player.GetProtraitID(), _player.GetNickName(), _player.GetTableWalletMoney());
}

void ZJHGameUI::stUpdateMoney( quint32 _goldCoin, quint32 _silverCoin )
{
	mMainWidget->findChild<QLabel*>("goldCoinText")->setText(QString("%1").arg(_goldCoin));
	mMainWidget->findChild<QLabel*>("silverCoinText")->setText(QString("%1").arg(_silverCoin));
}

void ZJHGameUI::StMySeat( quint32 _seatID )
{
	mMySeatID = _seatID;
}

void ZJHGameUI::updatePlayerInfo( Seat _seat, quint32 _protraitID, QString& _nickName, quint32 _money )
{
	mPortrait[_seat]->setPixmap(QPixmap(QString(":/Portraits/Media/Portrait/%1.png").arg(_protraitID)));
	mNickName[_seat]->setText(_nickName);
	mCoin[_seat]->setText(QString("%1").arg(_money));
}