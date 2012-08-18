#include "AppFrame.h"
#include "LoginServer.h"
#include "SettingModule.h"
#include "LoginServerDB.h"
#include "WalletDB.h"
#include "LogModule.h"
#include "MD5.h"
#include "SharedData.h"
using namespace SharedData;

AppFrame::AppFrame()
{
	mLoginServer = new LoginServer();

	SETTINGS.Init("LSConfig.ini");

	initDatabase();

	initWalletDatabase();

	LOG.SetModuleName("LS");
}

AppFrame::~AppFrame()
{

}

void AppFrame::Run()
{
	mLoginServer->StStart("", SETTINGS.GetPort());
}

void AppFrame::initDatabase()
{
	connect(&DB, SIGNAL(SiInfo(QString)), &LOG, SLOT(StInfo(QString)));
	connect(&DB, SIGNAL(SiError(QString)), &LOG, SLOT(StError(QString)));

	DB.Connect("../../DB/db.db", "LoginServerDB");
	//DB.VerifyUser(QString("acc3"), ToMD5(QString("1234")));
	//DB.RegUser(QString("acc4"), ToMD5(QString("1234")));

}

void AppFrame::initWalletDatabase()
{
	connect(&WDB, SIGNAL(SiInfo(QString)), &LOG, SLOT(StInfo(QString)));
	connect(&WDB, SIGNAL(SiError(QString)), &LOG, SLOT(StError(QString)));

	WDB.Connect("../../DB/walletdb.db", "WalletDB");
}