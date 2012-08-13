#include "stdafx.h"
#include "LobbyUI.h"
#include "GameList.h"
#include "GameServerNet.h"
#include "Setting.h"
#include "BaseDialog.h"

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

	mTableListView = mMainWidget->findChild<QGraphicsView*>("tableList");
	mScene = new QGraphicsScene(0, 0, mTableListView->size().width()-mTableListView->verticalScrollBar()->size().width(), 2000);
	mTableListView->setScene(mScene);
	int min = mTableListView->verticalScrollBar()->minimum();
	int max = mTableListView->verticalScrollBar()->maximum();
	mTableListView->verticalScrollBar()->setValue(min+1);		// if set 0, thumb will stay at middle of scrollbar...WTF
	

	QTreeWidget* gameList = mMainWidget->findChild<QTreeWidget*>("gameList");
	mGameList = new GameList(gameList);

	mSelectNotice = new QLabel(mMainWidget);
	mSelectNotice->move(270, 200);
	mSelectNotice->setPixmap(QPixmap(":/Images/Media/selectRoom.png"));

#ifdef NDEBUG
	mSelectNotice->show();
	mTableListView->hide();
#else
	mSelectNotice->hide();
	mTableListView->show();
#endif

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

	connect(mGameList, SIGNAL(SiConnectGS(RoomInfo)), this, SLOT(stConnectGS(RoomInfo)));

	connect(mGameServer, SIGNAL(SiError(QString)), this, SLOT(stNetError(QString)));
	connect(mGameServer, SIGNAL(SiConnected()), this, SLOT(stGSConnected()));
	connect(mGameServer, SIGNAL(SiLoginOK()), this, SLOT(stGSLoginOK()));
	connect(mGameServer, SIGNAL(SiLoginFailed(quint32)), this, SLOT(stGSLoginFailed(quint32)));
	connect(mGameServer, SIGNAL(SiTableList(QMap<int, TableData>)), this, SLOT(stTableList(QMap<int, TableData>)));
	connect(mGameServer, SIGNAL(SiTableJoinResult(quint32, quint32, quint32, TablePlayer)), this, SLOT(stTableJoinResult(quint32, quint32, quint32, TablePlayer)));
	connect(mGameServer, SIGNAL(SiTableLeaveResult(quint32, quint32, TablePlayer)), this, SLOT(stTableLeaveResult(quint32, quint32, TablePlayer)));
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

void LobbyUI::stConnectGS( RoomInfo _roomInfo )
{
	mGameServer->Connect(_roomInfo.mIP, _roomInfo.mPort);
	SETTINGS.SetRoomInfo(_roomInfo);
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
	mGameServer->SendLoginGS(mUserName, mPassword);
}

void LobbyUI::stGSLoginOK()
{
	LOG_D_INFO("Game Server Logined");
	mTableListView->show();
	mSelectNotice->hide();

	QString roomName = SETTINGS.GetRoomInfo().mName;
}

void LobbyUI::stGSLoginFailed( quint32 _errCode )
{
	LOG_D_ERR(QString("ErrorCode[%1]").arg(_errCode));
}

void LobbyUI::stTableJoinResult( quint32 _res, quint32 _tableID, quint32 _seatID, TablePlayer _player )
{
	if ( _res == GS_NO_ERR )
	{
		LOG_D_INFO(QString("Player joined table[%1], seat[%2], res[%3]").arg(_tableID).arg(_seatID).arg(_res));
		QMap<quint32, Table*>::iterator itr = mTableList.find(_tableID);
		if ( itr != mTableList.end() )
		{
			itr.value()->UpdatePlayer(_seatID, _player);
		}
		else
			LOG_D_ERR(QString("Can not find table id[%1]").arg(_tableID));
	}
	else
	{
		LOG_D_WARN(QString("Player join table[%1], seat[%2], res[%3]").arg(_tableID).arg(_seatID).arg(_res));

	}
}

void LobbyUI::stTableList( QMap<int, TableData> _tableData )
{
	// update tables
	QMap<int, TableData>::iterator itr;
	int index = 0;
	for ( itr = _tableData.begin(); itr != _tableData.end(); itr++, index++)
	{
		if ( index >= mTableList.size() )
			break;
		
		mTableList[index]->UpdateTableInfo(itr.key(), *itr);
	}

	return;
}

void LobbyUI::stTableLeaveResult( quint32 _res, quint32 _tableID, TablePlayer _player )
{
	if ( _res == GS_NO_ERR )
	{
		LOG_D_INFO(QString("Player leave table[%1], seat[%2], res[%3]").arg(_tableID).arg(_res));
		QMap<quint32, Table*>::iterator itr = mTableList.find(_tableID);
		if ( itr != mTableList.end() )
		{
			itr.value()->PlayerLeave(_player);
		}
		else
			LOG_D_ERR(QString("Can not find table id[%1]").arg(_tableID));
	}
	else
	{
		LOG_D_WARN(QString("Player leave table[%1], seat[%2], res[%3]").arg(_tableID).arg(_res));

	}
}

void LobbyUI::StUpdatePlayerInfo( CommonPlayer _player )
{
	QLabel* nickNameLabel = mMainWidget->findChild<QLabel*>("nickNameText");
	nickNameLabel->setText(_player.GetNickName());

	QLabel* imageLabel = mMainWidget->findChild<QLabel*>("playerImage");
	imageLabel->setPixmap(QPixmap(QString(":/Portraits/Media/Portrait/%1.png").arg(_player.GetProtraitID())));

}

void LobbyUI::StGetLoginInfo( QString _username, QString _pwd )
{
	mUserName = _username;
	mPassword = _pwd;
}