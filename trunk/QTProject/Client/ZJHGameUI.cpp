#include "stdafx.h"
#include "ZJHGameUI.h"
#include "PokerItem.h"

QPoint gTableCenter = QPoint(360, 220);
QPoint gPokerPos[MAX_PLAYER][MAX_PLAYER] = {{QPoint(300, 360), QPoint(550, 220), QPoint(300, 60), QPoint(110, 220)}, 
											{QPoint(550, 220), QPoint(300, 60), QPoint(110, 220), QPoint(300, 360)}, 
											{QPoint(300, 60), QPoint(110, 220), QPoint(300, 360), QPoint(550, 220)}, 
											{QPoint(110, 220), QPoint(300, 360), QPoint(550, 220), QPoint(300, 60)}};
quint32 gPokerGap = 20;

ZJHGameUI::ZJHGameUI()
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

	mTimer.start(5000);

	QGraphicsView* view = mMainWidget->findChild<QGraphicsView*>("graphicsView");
	mScene = new QGraphicsScene(0, 0, 
		view->size().width()-view->verticalScrollBar()->size().width(),
		view->size().height()-view->horizontalScrollBar()->size().height());
	//mScene->setBackgroundBrush(Qt::blue);
	view->setScene(mScene);

	mShuffleLabel = new QLabel(mMainWidget);
	mShuffleLabel->move(gTableCenter+QPoint(50, 100));
	mShuffleAni = new QMovie(":/Images/Media/shuffle.gif");
	mShuffleLabel->setMovie(mShuffleAni);
	HideShuffleAni();

	int amount = MAX_HAND_POKER*MAX_PLAYER;

	for ( int i = 0; i<amount; i++ )
	{
		PokerItem* item = new PokerItem(0);
		item->setZValue(i);
		item->setPos(gTableCenter);
		item->ToBack();
		mScene->addItem(item);
		mPokers.push_back(item);
	}

	ShowDistributeAni(0);
}

void ZJHGameUI::regConnections()
{
	connect(mMainWidget->findChild<QPushButton*>("btn_quit"), SIGNAL(clicked()), this, SIGNAL(SiQuit()));
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(stUpdate()));
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

	QPoint* pos = gPokerPos[_dealerIdx];
	int posIdx = 0;
	int gap = 0;
	for ( int i = 0; i<mPokers.size(); i++, posIdx++ )
	{
		if ( i != 0 && i %  MAX_PLAYER == 0 )
		{
			gap++;
			posIdx = 0;
		}

		mPokers[i]->Move(150*i, 1000, QPoint(pos[posIdx].x()+gPokerGap*gap, pos[posIdx].y()));
	}

}

void ZJHGameUI::ShowPreShuffleAni()
{
	for ( int i = 0; i<mPokers.size(); i++ )
	{
		mPokers[i]->Move(50*i, 1000, gTableCenter);
	}
}