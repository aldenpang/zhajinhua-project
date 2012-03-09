#include "stdafx.h"
#include "ClientNet.h"

ClientNet::ClientNet()
{

}

ClientNet::~ClientNet()
{

}

void ClientNet::PacketHandler( Packet* _packet )
{
	int msg = _packet->GetMessage();
	qDebug()<<"Client - Msg:"<<msg;

	switch(msg)
	{
	case MSG_LS_CL_LOGIN:
		processLogin(_packet);
		break;
	default:
		break;
	}

}

void ClientNet::processLogin( Packet* _packet )
{
	int res = 0;
	_packet->Get(&res);

	return;
}
