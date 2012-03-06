#include "AppFrame.h"
#include "LoginServer.h"
#include "SettingModule.h"
#include "LoginServerDB.h"
#include "LogModule.h"

AppFrame::AppFrame()
{
	mLoginServer = new LoginServer();

	SETTINGS.Init();

	InitDatabase();
}

AppFrame::~AppFrame()
{

}

void AppFrame::Run()
{
	mLoginServer->StStart("", SETTINGS.GetPort());
}

void AppFrame::InitDatabase()
{
	DB.Connect("../../DB/db.db");
	DB.VerifyUser(QString("acc1"), QString("1234"));

	connect(&DB, SIGNAL(SiInfo(QString)), &LOG, SLOT(StInfo(QString)));
	connect(&DB, SIGNAL(SiError(QString)), &LOG, SLOT(StError(QString)));
}
