#include "stdafx.h"
#include "LobbyUI.h"
#include "GameList.h"

LobbyUI::LobbyUI()
{

}

LobbyUI::~LobbyUI()
{
	QMap<quint32, Table*>::iterator itr;
	for ( itr = mTableList.begin(); itr != mTableList.end(); itr++ )
	{
		SAFE_DELETE(itr.value());
	}
	mTableList.clear();
}

void LobbyUI::Init()
{
	QUiLoader uiLoader;
	QString fileStr(":/Client/lobby.ui");
	QFile uiFile(fileStr);
	mMainWidget = uiLoader.load(&uiFile);
	if ( mMainWidget )
	{
		mMainWidget->setWindowFlags(Qt::FramelessWindowHint);
	}

	QGraphicsView* tableList = mMainWidget->findChild<QGraphicsView*>("tableList");
	mScene = new QGraphicsScene(0, 0, tableList->size().width()-100, 2000);
	tableList->setScene(mScene);

	QTreeWidget* gameList = mMainWidget->findChild<QTreeWidget*>("gameList");
	mGameList = new GameList(gameList);

	//QVector<RoomInfo> rooms;
	//RoomInfo info;
	//info.mName = "silverroom0";
	//info.mMoneyType = SILVER_COIN;
	//info.mType = ZJH;
	//rooms.push_back(info);

	//RoomInfo info1;
	//info1.mName = "silverroom1";
	//info1.mMoneyType = SILVER_COIN;
	//info1.mType = ZJH;
	//rooms.push_back(info1);

	//RoomInfo info2;
	//info2.mName = "goldroom0";
	//info2.mMoneyType = GOLD_COIN;
	//info2.mType = ZJH;
	//rooms.push_back(info2);

	//RoomInfo info3;
	//info3.mName = "goldroom1";
	//info3.mMoneyType = GOLD_COIN;
	//info3.mType = ZJH;
	//rooms.push_back(info3);

	//mGameList->Update(rooms);

	initTables(50);

	regConnections();
}

void LobbyUI::regConnections()
{
	connect(mMainWidget->findChild<QPushButton*>("btn_quit"), SIGNAL(clicked()), this, SIGNAL(SiQuit()));
}

void LobbyUI::initTables(quint32 _amount)
{
	QUiLoader uiLoader;
	QString fileStr(":/Client/table.ui");

	int xGap = 145;
	int yGap = 150;
	int tablesPreRow = 5;

	int tempY = 0;int tempX = 0;
	for ( int i = 0; i<_amount; i++ )
	{
		tempX = (i%tablesPreRow)*xGap;
		if ( i != 0 && (i % tablesPreRow == 0) )
			tempY += yGap;
		
		QFile uiFile(fileStr);
		QWidget* tableWidget = uiLoader.load(&uiFile);
		if ( tableWidget )
		{
			//mScene->addWidget(tableWidget);
			tableWidget->move(tempX, tempY);

			// create table instance
			Table* t = new Table;
			t->SetWidget(tableWidget);
			t->SetID(i);
			t->Init();
			mTableList.insert(i, t);
		}
	}
	//QGraphicsView* tableList = mMainWidget->findChild<QGraphicsView*>("tableList");
	//tableList->verticalScrollBar()->setMinimum(0);
	//tableList->verticalScrollBar()->setMaximum(100);
	//tableList->verticalScrollBar()->setSliderPosition(0);
	//int ss = tableList->horizontalScrollBar()->sliderPosition();
	//tableList->verticalScrollBar()->setRange(0, 1000);
	//int sds = tableList->verticalScrollBar()->sliderPosition();

	return;
}
