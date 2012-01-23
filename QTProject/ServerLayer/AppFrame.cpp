#include "AppFrame.h"
#include "ZJHGameServer.h"

//------------------------------------------------------------------------------
AppFrame::AppFrame()
{
	mGameServer = new ZjhGameServer();
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