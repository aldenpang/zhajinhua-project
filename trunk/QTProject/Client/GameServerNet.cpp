#include "GameServerNet.h"
#include "MD5.h"
#include "SharedData.h"
using namespace SharedData;


GameServerNet::GameServerNet()
{

}

GameServerNet::~GameServerNet()
{

}

void GameServerNet::PacketHandler( Packet& _packet )
{
	int msg = _packet.GetMessage();
	qDebug()<<"From Game Server - Recv Msg:"<<msg;
	switch (msg)
	{
	case MSG_GS_CL_LOGIN:
		processLogin(_packet);
		break;
	case MSG_GS_CL_TABLE_INFO:
		processTableInfo(_packet);
		break;
	case MSG_GS_CL_TABLE_JOIN:
		processTableJoin(_packet);
	default:
		break;
	}
}

void GameServerNet::processLogin( Packet& _packet )
{
	int res = 0;
	_packet>>res;

	if ( res != GS_NO_ERR )
		emit SiLoginFailed(res);
	else
		emit SiLoginOK();
}

void GameServerNet::SendLoginGS( QString& _userName, QString& _pwd )
{
	// login gs
	Packet p;
	p.SetMessage(MSG_CL_GS_LOGIN);
	QString userName = _userName;
	QString md5pwd = ToMD5(_pwd);
	p<<userName<<md5pwd;
	Send(&p);
}

void GameServerNet::SendJoinTable( quint32 _tableID, quint32 _seatID )
{
	// join table
	Packet p;
	p.SetMessage(MSG_CL_GS_TABLE_JOIN);
	p<<_tableID<<_seatID;
	Send(&p);
}

void GameServerNet::processTableInfo( Packet& _packet )
{
	QMap<int, TableData> tables;
	quint32 tableAmount = 0;
	quint32 tableID = 0;
	_packet>>tableAmount;
	
	for ( int i = 0; i<tableAmount; i++ )
	{
		TableData table;
		_packet>>tableID;
		table.SetID(tableID);
		for ( int j = 0; j < 4; j++ )
		{
			int seatID = 0;
			_packet>>seatID;
			QString nickName;
			_packet>>nickName;
			table.AddPlayer(seatID, nickName);
		}
		tables.insert(i, table);
	}

	emit SiTableList(tables);
}

void GameServerNet::processTableJoin( Packet& _packet )
{
	quint32 res = 0;
	_packet>>res;

	emit SiTableJoinResult(res);
}