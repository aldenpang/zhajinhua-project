#include "ZJHGameServer.h"
#include "LogModule.h"
#include "Packet.h"
#include "GameServerDB.h"
#include "SharedData.h"
#include "HardcodeConfig.h"
#include "GSPlayer.h"
#include "TableManager.h"
using namespace SharedData;

//------------------------------------------------------------------------------
ZjhGameServer::ZjhGameServer()
: IServerLayer()
{
	mTimer.setInterval(REFRESH_INTERVAL*1000);
	mTimer.start();
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(stRefershTables()));
}
//------------------------------------------------------------------------------
ZjhGameServer::~ZjhGameServer()
{

}

void ZjhGameServer::PacketHandler( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	int msg = _packet.GetMessage();
	qDebug()<<"ZjhGameServer - Msg:"<<msg;

	switch(msg)
	{
	case MSG_CL_GS_LOGIN:
		processLogin(_incomeSocket, _packet);
		break;
	case MSG_CL_GS_TABLE_JOIN:
		processTableJoin(_incomeSocket, _packet);
		break;
	case MSG_CL_GS_TABLE_LEAVE:
		processTableLeave(_incomeSocket, _packet);
		break;
	default:
		break;
	}
}

void ZjhGameServer::processLogin( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	int length = 0;
	//_packet.Get(&length);
	char userName[32]={0};
	char pwd[32]={0};
	//_packet->Get(userName, length);
	//_packet->Get(&length);
	//_packet->Get(pwd, length);

	LOG_INFO(QString("[%1]wants to login, pwd is[%2]").arg(userName).arg(pwd));

	int res = DB.VerifyUser(QString(userName), QString(pwd));

	if ( res != LOGIN_OK )
		LOG_ERR(QString("LoginFailed! Reason:[%1]").arg(res));
	else
		LOG_INFO("Login OK");

	// send login result
	Packet p;
	p.SetMessage(MSG_GS_CL_LOGIN);
	//p.Put(res);
	_incomeSocket->Send(&p);

	// send room config

	// send table list

}

void ZjhGameServer::processTableJoin( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	quint32 tableID = 0;
	quint32 seatID = 0;
	//_packet->Get(&tableID);
	//_packet->Get(&seatID);
	_packet>>tableID>>seatID;
	GSPlayerPtr player = _incomeSocket.staticCast<GSPlayer>();

	// 检查桌子是否可坐 & 加入桌子
	int res = TABLE.StJoinTable(player, tableID, seatID);
	if ( res != GS_NO_ERR )
	{
		// 不能加入桌子
		Packet p;
		p.SetMessage(MSG_GS_CL_TABLE_JOIN);
		//p.Put(res);		// reason
		p<<(quint32)res;
		_incomeSocket->Send(&p);
		return;
	}

	//广播此玩家加入桌子的消息
	Packet p;
	p.SetMessage(MSG_GS_BC_TABLE_JOIN);
	//p.Put(who)
	Broadcast(&p);
}

void ZjhGameServer::stRefershTables()
{
	LOG_INFO("Refersh Tables");
}

void ZjhGameServer::processTableLeave( ISocketInstancePtr _incomeSocket, Packet& _packet )
{
	quint32 tableID = 0;
	_packet>>tableID;
	GSPlayerPtr player = _incomeSocket.staticCast<GSPlayer>();

	// 检查是否可离开桌子
	int res = TABLE.StLeaveTable(player, tableID);
	if ( res != GS_NO_ERR )
	{
		// 不能离开桌子
		Packet p;
		p.SetMessage(MSG_GS_CL_TABLE_LEAVE);
		p<<res;
		_incomeSocket->Send(&p);
		return;
	}

	//广播此玩家离开桌子的消息
	Packet p;
	p.SetMessage(MSG_GS_BC_TABLE_LEAVE);
	//p.Put(who)
	Broadcast(&p);
}
