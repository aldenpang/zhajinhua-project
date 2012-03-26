#include "AppFrame.h"
#include "WalletServer.h"
#include "SettingModule.h"
#include "LogModule.h"
#include "SharedData.h"
using namespace SharedData;

AppFrame::AppFrame()
{
	mWalletServer = new WalletServer();

	SETTINGS.Init("WSConfig.ini");

	InitDatabase();
}

AppFrame::~AppFrame()
{

}

void AppFrame::InitDatabase()
{
// 	connect(&DB, SIGNAL(SiInfo(QString)), &LOG, SLOT(StInfo(QString)));
// 	connect(&DB, SIGNAL(SiError(QString)), &LOG, SLOT(StError(QString)));
// 
// 	DB.Connect("../../DB/db.db");
}

void AppFrame::Run()
{
	mWalletServer->StStart("", SETTINGS.GetPort());
}
