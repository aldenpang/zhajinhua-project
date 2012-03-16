#include "stdafx.h"
#include "GameServerNet.h"


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
	case 0:
		break;
	default:
		break;
	}
}
