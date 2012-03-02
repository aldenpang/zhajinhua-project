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

	//qint64 res = mTcpSocket.write(sendBuff);
	int leng = _packet->GetDataLength();
	qint64 res = mTcpSocket.write(QByteArray::fromRawData(sendBuff, leng));
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
	// 将网络发送过来的字节流转化为Packet
	QTcpSocket* s = (QTcpSocket*)sender();
	if ( s && s->bytesAvailable() )
	{
		char buff[MAX_PACKET_SIZE]={0};
		s->read(buff, MAX_PACKET_SIZE);
		qDebug()<<"Data Received:"<<buff;

		Packet p;
		p.SetData(buff);
		if ( !p.IsTokenValid() )
		{
			//emit SiInfo(QString("Invalid Packet from %1:%2").arg(s->peerAddress().toString()).arg(s->peerPort()));
			qDebug()<<QString("Invalid Packet from %1:%2").arg(s->peerAddress().toString()).arg(s->peerPort());
			return;
		}

		PakcetHandler(&p);
	}
}

void INetLayer::PakcetHandler( Packet* _packet )
{
	int msg = _packet->GetMessage();
	qDebug()<<"Client - Msg:"<<msg;

}
