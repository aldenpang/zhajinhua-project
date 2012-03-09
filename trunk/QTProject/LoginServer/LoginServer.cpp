#include "LoginServer.h"
#include "LoginServerDB.h"
#include "Packet.h"
#include "SharedData.h"
#include "LogModule.h"
using namespace SharedData;

void LoginServer::PacketHandler( ISocketInstancePtr _incomeSocket, Packet* _packet )
{
	int msg = _packet->GetMessage();

	LOG_INFO(QString("Rev msg[%1] from %2:%3").arg(msg).arg(_incomeSocket->GetSocket()->peerAddress().toString()).arg(_incomeSocket->GetSocket()->peerPort()));
	switch (msg)
	{
	case MSG_CL_LS_LOGIN:
		processClientLogin(_incomeSocket, _packet);
		break;
	default:
		LOG_ERR(QString("Wrong Message[%1] from %2:%3").arg(msg).arg(_incomeSocket->GetSocket()->peerAddress().toString()).arg(_incomeSocket->GetSocket()->peerPort()));
		break;
	}
	
}

void LoginServer::processClientLogin( ISocketInstancePtr _incomeSocket, Packet* _packet )
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
	p.SetMessage(MSG_LS_CL_LOGIN);
	p.Put(res);
	_incomeSocket->Send(&p);

	return;
}
