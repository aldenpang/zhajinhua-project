#include "AppFrame.h"
#include "LoginServer.h"
#include "SettingModule.h"
#include "LoginServerDB.h"
#include "LogModule.h"
#include "MD5.h"

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
	DB.VerifyUser(QString("acc3"), ToMD5(QString("1234")));
	//DB.RegUser(QString("acc3"), QString(dd));

	connect(&DB, SIGNAL(SiInfo(QString)), &LOG, SLOT(StInfo(QString)));
	connect(&DB, SIGNAL(SiError(QString)), &LOG, SLOT(StError(QString)));
}
