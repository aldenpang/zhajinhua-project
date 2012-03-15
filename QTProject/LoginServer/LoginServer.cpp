#include "LoginServer.h"
#include "LoginServerDB.h"
#include "Packet.h"
#include "SharedData.h"
#include "LoginServerDB.h"
#include "LogModule.h"
using namespace SharedData;

void LoginServer::PacketHandler( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	int msg = _packet.GetMessage();

	LOG_INFO(QString("Rev msg[%1] from %2:%3").arg(msg).arg(_incomeSocket->GetSocket()->peerAddress().toString()).arg(_incomeSocket->GetSocket()->peerPort()));
	switch (msg)
	{
	case MSG_CL_LS_LOGIN:
		processClientLogin(_incomeSocket, _packet);
		break;
	case MSG_CL_LS_GAMELIST:
		processClientReqGameList(_incomeSocket, _packet);
		break;
	default:
		LOG_ERR(QString("Wrong Message[%1] from %2:%3").arg(msg).arg(_incomeSocket->GetSocket()->peerAddress().toString()).arg(_incomeSocket->GetSocket()->peerPort()));
		break;
	}
	
}

void LoginServer::processClientLogin( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	//int length = 0;
	//_packet->Get(&length);
	QString userName;
	QString pwd;
	_packet>>userName>>pwd;
	//_packet->Get(userName, length);
	//_packet->Get(&length);
	//_packet->Get(pwd, length);

	LOG_INFO(QString("[%1]wants to login, pwd is[%2]").arg(userName).arg(pwd));

	int res = DB.VerifyUser(userName, pwd);

	if ( res != LOGIN_OK )
		LOG_ERR(QString("LoginFailed! Reason:[%1]").arg(res));
	else
		LOG_INFO("Login OK");

	// send login result
	Packet p;
	p.SetMessage(MSG_LS_CL_LOGIN);
	p<<(quint32)res;
	//p.Put(res);
	_incomeSocket->Send(&p);

	return;
}

void LoginServer::processClientReqGameList( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	quint32 gameType = 0;
	//_packet->Get(&gameType);
	_packet>>gameType;
a
	// send game list
	sendGameList(gameType, _incomeSocket);
}

void LoginServer::sendGameList( int _gameType, ISocketInstancePtr _toSocket )
{
	QVector<RoomInfo> info;
	DB.GetRoomConfig(_gameType, info);

	Packet p;
	p.SetMessage(MSG_LS_CL_GAMELIST);
	//p.Put((int)info.size());
	p<<(quint32)info.size();
	for ( int i = 0; i<info.size(); i++ )
	{
		p<<info[i].mName<<info[i].mType<<info[i].mIP<<info[i].mPort<<info[i].mScore<<info[i].mUnit;

		//p.Put(info[i].mName.size());
		//p.Put(const_cast<char*>(info[i].mName.toStdString().c_str()));
		////p.Put(info[i].mType);
		//p.Put(info[i].mIP.size());
		//p.Put(const_cast<char*>(info[i].mIP.toStdString().c_str()));
		//p.Put(info[i].mPort);
		//p.Put(info[i].mScore);
		//p.Put(info[i].mUnit);
	}
	_toSocket->Send(&p);
}
