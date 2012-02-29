#include "ZJHGameServer.h"
#include "LogModule.h"
#include "Packet.h"


//------------------------------------------------------------------------------
ZjhGameServer::ZjhGameServer()
: IServerLayer()
{
}
//------------------------------------------------------------------------------
ZjhGameServer::~ZjhGameServer()
{

}

void ZjhGameServer::PakcetHandler( Packet* _packet )
{
	int msg = _packet->GetMessage();
	qDebug()<<"ZjhGameServer - Msg:"<<msg;

}
