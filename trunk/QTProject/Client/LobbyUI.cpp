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
	mScene = new QGraphicsScene(0, 0, tableList->size().width()-tableList->verticalScrollBar()->size().width(), 2000);
	tableList->setScene(mScene);
	int min = tableList->verticalScrollBar()->minimum();
	int max = tableList->verticalScrollBar()->maximum();
	tableList->verticalScrollBar()->setValue(min+1);		// if set 0, thumb will stay at middle of scrollbar...WTF

	QTreeWidget* gameList = mMainWidget->findChild<QTreeWidget*>("gameList");
	mGameList = new GameList(gameList);

	//initTables(50);

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

	int xGap = 140;
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
			mScene->addWidget(tableWidget);
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

void LobbyUI::StShowLobby( QVector<RoomInfo> _gameList )
{
	Show();
	mGameList->Update(_gameList);
}
