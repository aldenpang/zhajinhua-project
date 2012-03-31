#include "ConsoleClient.h"
#include "LoginServerNet.h"
#include "GameServerNet.h"

ConsoleClient::ConsoleClient()
{
	mLoginServer = new LoginServerNet();
	mLoginServer->Init();
	mLoginServer->Connect("localhost", 5000);

	mGameServer = new GameServerNet();
	mGameServer->Init();
}

ConsoleClient::~ConsoleClient()
{

}
