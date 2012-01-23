#include "ZJHGameServer.h"
#include "LogModule.h"


//------------------------------------------------------------------------------
ZjhGameServer::ZjhGameServer()
: IServerLayer()
{
	connect(this, SIGNAL(newConnection()), this, SLOT(StNewConnections()));

	connect(this, SIGNAL(SiInfo(const QString&)), &LOG, SLOT(StInfo(const QString&)));
}
//------------------------------------------------------------------------------
ZjhGameServer::~ZjhGameServer()
{

}
//------------------------------------------------------------------------------
void ZjhGameServer::StStart()
{
	int port = 5000;
	if (!this->listen(QHostAddress::Any, port))
	{
		emit SiError(this->errorString());
		return;
	}
	emit SiStarted(port);
	//emit SiInfo("Started!");
	emit SiInfo("Started!");
}
//------------------------------------------------------------------------------
void ZjhGameServer::StStop()
{
	this->close();
	emit SiStoped();
	//emit SiInfo("Stop!");
	emit SiInfo("Stopped!");
}
//------------------------------------------------------------------------------
void ZjhGameServer::StRestart()
{

}
//------------------------------------------------------------------------------
void ZjhGameServer::StNewConnections()
{
	QTcpSocket* s = nextPendingConnection();
	if (s)
	{
		emit SiInfo(QString("incoming socket: %1:%2").arg(s->peerAddress().toString()).arg(s->peerPort()));

		mClientList.append(s);

		// 这里连接处理Client Socket的所有函数
		connect(s, SIGNAL(disconnected()), this, SLOT(stClientDisconnect()));
		
	}
}
//------------------------------------------------------------------------------
void ZjhGameServer::stClientDisconnect()
{
	QTcpSocket* s = (QTcpSocket*)sender();
	if ( mClientList.contains(s) )
	{
		emit SiInfo(QString("socket disconnected: %1:%2").arg(s->peerAddress().toString()).arg(s->peerPort()));
		mClientList.removeOne(s);
		//emit SiInfo(QString("ClientListSize:%1").arg(mClientList.size()));
	}
}