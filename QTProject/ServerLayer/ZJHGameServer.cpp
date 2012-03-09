#include "ZJHGameServer.h"
#include "LogModule.h"
#include "Packet.h"
#include "GameServerDB.h"
#include "SharedData.h"
using namespace SharedData;

//------------------------------------------------------------------------------
ZjhGameServer::ZjhGameServer()
: IServerLayer()
{
}
//------------------------------------------------------------------------------
ZjhGameServer::~ZjhGameServer()
{

}

void ZjhGameServer::PacketHandler( ISocketInstancePtr _incomeSocket, Packet* _packet )
{
	int msg = _packet->GetMessage();
	qDebug()<<"ZjhGameServer - Msg:"<<msg;

	switch(msg)
	{
	case MSG_CL_GS_LOGIN:
		processLogin(_incomeSocket, _packet);
		break;
	case MSG_CL_GS_TABLE_JOIN:
		processTableJoin(_incomeSocket, _packet);
		break;
	default:
		break;
	}
}

void ZjhGameServer::processLogin( ISocketInstancePtr _incomeSocket, Packet* _packet )
{
	int length = 0;
	_packet->Get(&length);
	char userName[32]={0};
	char pwd[32]={0};
	_packet->Get(userName, length);
	_packet->Get(&length);
	_packet->Get(pwd, length);

	LOG_INFO(QString("[%1]wants to login, pwd is[%2]").arg(userName).arg(pwd));

	int res = DB.VerifyUser(QString(userName), QString(pwd));

	if ( res != LOGIN_OK )
		LOG_ERR(QString("LoginFailed! Reason:[%1]").arg(res));
	else
		LOG_INFO("Login OK");

	Packet p;
	p.SetMessage(MSG_GS_CL_LOGIN);
	p.Put(res);
	_incomeSocket->Send(&p);
}

void ZjhGameServer::processTableJoin( ISocketInstancePtr _incomeSocket, Packet* _packet )
{

}
