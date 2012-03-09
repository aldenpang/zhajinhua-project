#include "AppFrame.h"
#include "ZJHGameServer.h"
#include "SettingModule.h"
#include "TableManager.h"

//------------------------------------------------------------------------------
AppFrame::AppFrame()
{
	mGameServer = new ZjhGameServer();

	SETTINGS.Init();

	TABLE.AssignTables();
}
//------------------------------------------------------------------------------
AppFrame::~AppFrame()
{

}
//------------------------------------------------------------------------------
void AppFrame::Run()
{
	mGameServer->StStart("", SETTINGS.GetPort());
}