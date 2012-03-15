#include "stdafx.h"
#include "ClientNet.h"
#include "SharedData.h"
using namespace SharedData;

ClientNet::ClientNet()
{

}

ClientNet::~ClientNet()
{

}

void ClientNet::PacketHandler( Packet& _packet )
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

void ClientNet::processLogin( Packet& _packet )
{
	quint32 res = 0;
	_packet>>res;

	if ( res == LOGIN_OK )
	{
		Packet p;
		p.SetMessage(MSG_CL_LS_GAMELIST);
		quint32 gameType = 0;
		p<<gameType;
		Send(&p);
	}

	return;
}

void ClientNet::processGameList( Packet& _packet )
{
	QVector<RoomInfo> gameList;
	quint32 size = 0;
	_packet>>size;
	for(int i = 0; i<size; i++)
	{
		RoomInfo info;
		_packet>>info.mName>>info.mType>>info.mIP>>info.mPort>>info.mScore>>info.mUnit;
	}
}
