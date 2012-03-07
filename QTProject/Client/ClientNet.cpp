#include "stdafx.h"
#include "ClientNet.h"

ClientNet::ClientNet()
{

}

ClientNet::~ClientNet()
{

}

void ClientNet::PakcetHandler( Packet* _packet )
{
	int msg = _packet->GetMessage();
	qDebug()<<"Client - Msg:"<<msg;

}
