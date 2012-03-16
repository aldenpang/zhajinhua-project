#include "stdafx.h"
#include "LoginServerNet.h"
#include "SharedData.h"
using namespace SharedData;

LoginServerNet::LoginServerNet()
{

}

LoginServerNet::~LoginServerNet()
{

}

void LoginServerNet::PacketHandler( Packet& _packet )
{
	int msg = _packet.GetMessage();
	qDebug()<<"Client - Msg:"<<msg;

	switch(msg)
	{
	case MSG_LS_CL_LOGIN:
		processLogin(_packet);
		break;
	case MSG_LS_CL_GAMELIST:
		processGameList(_packet);
		break;
	default:
		break;
	}

}

void LoginServerNet::processLogin( Packet& _packet )
{
	quint32 res = 0;
	_packet>>res;

	if ( res == LOGIN_OK )
		emit SiLoginOK();
	else
		emit SiLoginFailed(res);
}

void LoginServerNet::processGameList( Packet& _packet )
{
	QVector<RoomInfo> gameList;
	quint32 size = 0;
	_packet>>size;
	for(int i = 0; i<size; i++)
	{
		RoomInfo info;
		_packet>>info.mName>>info.mType>>info.mIP>>info.mPort>>info.mScore>>info.mUnit;
		gameList.push_back(info);
	}

	emit SiGameList(gameList);
}

void LoginServerNet::RequestGameList( int _gameType )
{
	Packet p;
	p.SetMessage(MSG_CL_LS_GAMELIST);
	quint32 gameType = _gameType;
	p<<gameType;
	Send(&p);
}
