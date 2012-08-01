#include "stdafx.h"
#include "LobbyUI.h"
#include "GameList.h"
#include "GameServerNet.h"

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

	initTables(50);

	initGameServer();

	regConnections();
}

void LobbyUI::regConnections()
{
	connect(mMainWidget->findChild<QPushButton*>("btn_quit"), SIGNAL(clicked()), this, SIGNAL(SiQuit()));
	QMap<quint32, Table*>::iterator itr;
	for ( itr = mTableList.begin(); itr != mTableList.end(); itr++)
	{
		connect(*itr, SIGNAL(SiSit(quint32, quint32)), this, SLOT(stTableJoin(quint32, quint32)));
	}

	connect(mGameList, SIGNAL(SiConnectGS(QString, quint32)), this, SLOT(stConnectGS(QString, quint32)));

	connect(mGameServer, SIGNAL(SiError(QString)), this, SLOT(stNetError(QString)));
	connect(mGameServer, SIGNAL(SiConnected()), this, SLOT(stGSConnected()));
	connect(mGameServer, SIGNAL(SiLoginOK()), this, SLOT(stGSLoginOK()));
	connect(mGameServer, SIGNAL(SiLoginFailed(quint32)), this, SLOT(stGSLoginFailed(quint32)));
	connect(mGameServer, SIGNAL(SiTableList(QMap<int, TableData>)), this, SLOT(stTableList(QMap<int, TableData>)));
	connect(mGameServer, SIGNAL(SiTableJoinResult(quint32, quint32, quint32)), this, SLOT(stTableJoinResult(quint32, quint32, quint32)));

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

	return;
}

void LobbyUI::StShowLobby( QVector<RoomInfo> _gameList )
{
	Show();
	mGameList->Update(_gameList);
}

void LobbyUI::stTableJoin( quint32 _tableID, quint32 _seatID )
{
	mGameServer->SendJoinTable(_tableID, _seatID);
}

void LobbyUI::stConnectGS( QString _ip, quint32 _port )
{
	mGameServer->Connect(_ip, _port);
}

void LobbyUI::initGameServer()
{
	mGameServer = new GameServerNet();
	mGameServer->Init();
}

void LobbyUI::stNetError( QString _err )
{
	LOG_D_ERR(_err);
}

void LobbyUI::stGSConnected()
{
	LOG_D_INFO("Game Server Connected");
	//mGameServer->SendLoginGS(gUserName, gPassword);
}

void LobbyUI::stGSLoginOK()
{
	LOG_D_INFO("Game Server Logined");
}

void LobbyUI::stGSLoginFailed( quint32 _errCode )
{
	LOG_D_ERR(QString("ErrorCode[%1]").arg(_errCode));
}

void LobbyUI::stTableJoinResult( quint32 _res, quint32 _tableID, quint32 _seatID )
{
	LOG_D_INFO(QString("Player joined table[%1], seat[%2], res[%3]").arg(_tableID).arg(_seatID).arg(_res));
}

void LobbyUI::stTableList( QMap<int, TableData> _tableData )
{
	// update tables
}
