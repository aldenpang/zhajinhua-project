#include "AppFrame.h"
#include "ZJHGameServer.h"
#include "SettingModule.h"
#include "TableManager.h"
#include "GameServerDB.h"
#include "LogModule.h"
#include "../WalletServer/WalletServerDB.h"

//------------------------------------------------------------------------------
AppFrame::AppFrame()
{
	mGameServer = new ZjhGameServer();

	TABLE.AssignTables();

	InitDatabase();

	//InitWalletServer();

	// use sigle-thread db
	InitWalletDB();

	connect(this, SIGNAL(SiInfo(const QString&)), &LOG, SLOT(StInfo(const QString&)));
	connect(this, SIGNAL(SiWarn(const QString&)), &LOG, SLOT(StWarn(const QString&)));
	connect(this, SIGNAL(SiError(const QString&)), &LOG, SLOT(StError(const QString&)));
}
//------------------------------------------------------------------------------
AppFrame::~AppFrame()
{

}
//------------------------------------------------------------------------------
void AppFrame::Run()
{
	mGameServer->StStart("", mRoomInfo.mPort);
}

void AppFrame::InitDatabase()
{
	connect(&DB, SIGNAL(SiInfo(QString)), &LOG, SLOT(StInfo(QString)));
	connect(&DB, SIGNAL(SiError(QString)), &LOG, SLOT(StError(QString)));

	DB.Connect("../../DB/db.db", "GameServerDB");

	int res = DB.GetRoomInfo(atoi(__argv[1]), mRoomInfo);
	if( res != GS_NO_ERR )
	{
		LOG_ERR(QString("Can not find room config in db! res[%1]").arg(res));
	}

	//quint32 acc = 0;
	//res = DB.GetAccountID(QString("acc2"), acc);

	//GSPlayer p(NULL);
	//p.SetAccountID(acc);
	//res = DB.GetPlayerInfo(p);
	//DB.UpdatePlayerIp(4, QString("dbc"));

	return;
}

void AppFrame::InitWalletServer()
{
	connect(&mWalletServer, SIGNAL(SiConnected()), this, SLOT(stWalletConnected()));
	connect(&mWalletServer, SIGNAL(SiDisconnected()), this, SLOT(stWalletDisconnected()));
	connect(&mWalletServer, SIGNAL(SiError(QString)), this, SLOT(stWalletError(QString)));

	mWalletServer.Init();
	mWalletServer.Connect("192.168.2.2", 4000);
}

void AppFrame::stWalletConnected()
{
	LOG_INFO("Wallet Connected");
}

void AppFrame::stWalletDisconnected()
{
	LOG_WARN("Wallet Disonnected");
}

void AppFrame::stWalletError(QString _err)
{
	LOG_ERR(QString("Wallet Connect Error:%1").arg(_err));
}

void AppFrame::InitWalletDB()
{
	connect(&WalletDB, SIGNAL(SiInfo(QString)), &LOG, SLOT(StInfo(QString)));
	connect(&WalletDB, SIGNAL(SiError(QString)), &LOG, SLOT(StError(QString)));

	WalletDB.Connect("../../DB/walletdb.db", "WalletDB");

}
