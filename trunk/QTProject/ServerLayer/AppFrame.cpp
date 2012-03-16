#include "AppFrame.h"
#include "ZJHGameServer.h"
#include "SettingModule.h"
#include "TableManager.h"
#include "GameServerDB.h"
#include "LogModule.h"


//------------------------------------------------------------------------------
AppFrame::AppFrame()
{
	mGameServer = new ZjhGameServer();

	TABLE.AssignTables();

	InitDatabase();

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

	DB.Connect("../../DB/db.db");

	int res = DB.GetRoomInfo(atoi(__argv[1]), mRoomInfo);
	if( res != GS_NO_ERR )
	{
		LOG_ERR(QString("Can not find room config in db! res[%1]").arg(res));
	}
}
