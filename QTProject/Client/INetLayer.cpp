#include "stdafx.h"
#include "INetLayer.h"

//------------------------------------------------------------------------------
INetLayer::INetLayer()
{

}
//------------------------------------------------------------------------------
INetLayer::~INetLayer()
{
	mTcpSocket.close();
}
//------------------------------------------------------------------------------
void INetLayer::Init()
{
	mTcpSocket.setSocketOption(QAbstractSocket::KeepAliveOption, 1);

	//connect(&mTcpSocket, SIGNAL(connected()), 
	//	this, SIGNAL(SiOutConnected()));
	//connect(&mTcpSocket, SIGNAL(connected()), 
	//	&mTimer, SLOT(stop()));
	//connect(&mTcpSocket, SIGNAL(disconnected()),
	//	this, SIGNAL(SiOutDisconnected()));
	connect(&mTcpSocket, SIGNAL(readyRead()),
		this, SLOT(stRead()));
	connect(&mTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(stError(QAbstractSocket::SocketError)));
	//connect(&mTcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
	//	this, SLOT(stPrintSocketState(QAbstractSocket::SocketState)));
}
//------------------------------------------------------------------------------
void INetLayer::Connect(const QString& _ip, const int _port)
{
	if (QAbstractSocket::UnconnectedState == mTcpSocket.state())
	{
		mTcpSocket.connectToHost(_ip, _port);
	}
}
//------------------------------------------------------------------------------
void INetLayer::Send()
{
	//mTcpSocket.write(_packet.GetBlock());
}
//------------------------------------------------------------------------------
void INetLayer::Disconnect()
{
	mTcpSocket.disconnectFromHost();
}
//------------------------------------------------------------------------------
void INetLayer::stError(QAbstractSocket::SocketError _socketError)
{

}
//------------------------------------------------------------------------------
void INetLayer::stRead()
{

}