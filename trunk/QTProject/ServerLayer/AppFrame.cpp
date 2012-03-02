#include "AppFrame.h"
#include "ZJHGameServer.h"
#include "SettingModule.h"

//------------------------------------------------------------------------------
AppFrame::AppFrame()
{
	mGameServer = new ZjhGameServer();

	SETTINGS.Init();
}
//------------------------------------------------------------------------------
AppFrame::~AppFrame()
{

}
//------------------------------------------------------------------------------
void AppFrame::Run()
{
	mGameServer->StStart();
}