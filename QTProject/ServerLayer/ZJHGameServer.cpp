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

void ZjhGameServer::PakcetHandler( ISocketInstancePtr _incomeSocket, Packet* _packet )
{
	int msg = _packet->GetMessage();
	qDebug()<<"ZjhGameServer - Msg:"<<msg;

	Packet p;
	p.SetMessage(7777);
	_incomeSocket->Send(&p);
}
