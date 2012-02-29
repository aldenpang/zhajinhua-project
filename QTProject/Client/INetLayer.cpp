#include "stdafx.h"
#include "INetLayer.h"
#include "Packet.h"

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
void INetLayer::Send(const char* _data)
{
	mTcpSocket.write(_data);
}

void INetLayer::Send( Packet* _packet )
{
	_packet->End();
	char sendBuff[MAX_PACKET_SIZE] = {0};
	memcpy(sendBuff, _packet->GetData(), MAX_PACKET_SIZE);

	qint64 res = mTcpSocket.write(sendBuff);
	return;
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