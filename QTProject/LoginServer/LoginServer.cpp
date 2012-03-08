#include "LoginServer.h"
#include "LoginServerDB.h"
#include "Packet.h"
#include "SharedData.h"
using namespace SharedData;

void LoginServer::PakcetHandler( ISocketInstancePtr _incomeSocket, Packet* _packet )
{
	int msg = _packet->GetMessage();

	emit SiInfo(QString("Class:%1 rev msg[%2]").arg(metaObject()->className()).arg(msg));
	switch (msg)
	{
	case MSG_CL_LS_LOGIN:
		processClientLogin(_incomeSocket, _packet);
		break;
	default:
		emit SiError(QString("Wrong Message[%1] from %2:%3").arg(msg).arg(_incomeSocket->GetSocket()->peerAddress().toString()).arg(_incomeSocket->GetSocket()->peerPort()));
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

	emit SiInfo(QString("[%1]wants to login, pwd is[%2]").arg(userName).arg(pwd));

	int res = DB.VerifyUser(QString(userName), QString(pwd));

	if ( res != LOGIN_OK )
	{
		emit SiError(QString("LoginFailed! Reason:[%1]").arg(res));
	}

	Packet p;
	p.SetMessage(MSG_LS_CL_LOGIN);
	p.Put(res);
	_incomeSocket->Send(&p);

	return;
}
