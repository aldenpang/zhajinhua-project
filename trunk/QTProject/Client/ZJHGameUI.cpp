#include "stdafx.h"
#include "ZJHGameUI.h"
#include "PokerItem.h"

QPoint gTableCenter = QPoint(360, 220);
QPoint gPokerPos0 = QPoint(420, 510);
QPoint gPokerPos1 = QPoint(620, 350);
QPoint gPokerPos2 = QPoint(440, 180);
QPoint gPokerPos3 = QPoint(300, 350);
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

	mTimer.start();

	QGraphicsView* view = mMainWidget->findChild<QGraphicsView*>("graphicsView");
	mScene = new QGraphicsScene(0, 0, 
		view->size().width()-view->verticalScrollBar()->size().width(),
		view->size().height()-view->horizontalScrollBar()->size().height());
	view->setScene(mScene);

	PokerItem* item = new PokerItem(0);
	item->toBack();
	//item->moveBy(gTableCenter.x(), gTableCenter.y());
	mScene->addItem(item);

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
}