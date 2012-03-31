#include "GameServerNet.h"
#include "SharedData.h"
using namespace SharedData;


GameServerNet::GameServerNet()
{

}

GameServerNet::~GameServerNet()
{

}

void GameServerNet::PacketHandler( Packet& _packet )
{
	int msg = _packet.GetMessage();
	switch (msg)
	{
	case MSG_GS_CL_LOGIN:
		processLogin(_packet);
		break;
	default:
		break;
	}
}

void GameServerNet::processLogin( Packet& _packet )
{
	int res = 0;
	_packet>>res;

	if ( res )
		emit SiLoginFailed(res);
	else
		emit SiLoginOK();
}
