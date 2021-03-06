#include "LoginServerNet.h"
#include "MD5.h"
#include "CommonPlayer.h"
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
	qDebug()<<"From Login Server - Recv Msg:"<<msg;

	switch(msg)
	{
	case MSG_LS_CL_LOGIN:
		processLogin(_packet);
		break;
	case MSG_LS_CL_GAMELIST:
		processGameList(_packet);
		break;
	case MSG_LS_CL_PLAYERINFO:
		processPlayerInfo(_packet);
		break;
	default:
		break;
	}

}

void LoginServerNet::processLogin( Packet& _packet )
{
	quint32 res = 0;
	int isTempLogin = 0;
	_packet>>res>>isTempLogin;

	if ( res == LOGIN_OK )
		emit SiLoginOK();
	else
		emit SiLoginFailed(res);

	if ( isTempLogin == 1 )
	{
		emit SiTempLogin();
	}
}

void LoginServerNet::processGameList( Packet& _packet )
{
	QVector<RoomInfo> gameList;
	quint32 size = 0;
	_packet>>size;
	for(int i = 0; i<size; i++)
	{
		RoomInfo info;
		int roomType = 0;
		int moneyType = 0;
		_packet>>info.mRoomID>>info.mName>>roomType>>info.mIP>>info.mPort>>info.mMinMoney>>moneyType;
		info.mType = (RoomType)roomType;
		info.mMoneyType = (MoneyType)moneyType;
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

void LoginServerNet::SendLoginRequest( QString& _userName, QString& _pwd )
{
	Packet p;
	p.SetMessage(MSG_CL_LS_LOGIN);
	QString userName = _userName;
	QString md5pwd = ToMD5(_pwd);
	p<<userName<<md5pwd;
	Send(&p);
}

void LoginServerNet::processPlayerInfo( Packet& _packet )
{
	CommonPlayer player;
	player.FromPacket(_packet);

	emit SiPlayerInfo(player);
	return;
}
