#include "INetLayer.h"
#include "LogModule.h"

//------------------------------------------------------------------------------
INetLayer::INetLayer()
{
	connect(this, SIGNAL(SiInfo(const QString&)), &LOG, SLOT(StInfo(const QString&)));
	connect(this, SIGNAL(SiWarn(const QString&)), &LOG, SLOT(StWarn(const QString&)));
	connect(this, SIGNAL(SiError(const QString&)), &LOG, SLOT(StError(const QString&)));

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

	connect(&mTcpSocket, SIGNAL(connected()), 
		this, SIGNAL(SiConnected()));
	connect(&mTcpSocket, SIGNAL(disconnected()),
		this, SIGNAL(SiDisconnected()));
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
	//char sendBuff[MAX_PACKET_SIZE] = {0};
	//memcpy(sendBuff, _packet->GetData(), MAX_PACKET_SIZE);

	//qint64 res = mTcpSocket.write(sendBuff);
	//int leng = _packet->GetDataLength();
	//qint64 res = mTcpSocket.write(QByteArray::fromRawData(sendBuff, leng));
	qint64 res = mTcpSocket.write(_packet->GetData());
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
	emit SiError(mTcpSocket.errorString());
}
//------------------------------------------------------------------------------
void INetLayer::stRead()
{
	// 将网络发送过来的字节流转化为Packet
	QTcpSocket* s = (QTcpSocket*)sender();
	if ( s && s->bytesAvailable() )
	{
		QByteArray buff = s->readAll();
		qDebug()<<QString("Receive Packet from %1:%2, DataSize:%3").arg(s->peerAddress().toString()).arg(s->peerPort()).arg(buff.length());
		Packet p;
		p.SetData(buff);
		if ( !p.IsTokenValid() )
		{
			//emit SiInfo(QString("Invalid Packet from %1:%2").arg(s->peerAddress().toString()).arg(s->peerPort()));
			qDebug()<<QString("Invalid Packet from %1:%2").arg(s->peerAddress().toString()).arg(s->peerPort());
			return;
		}

		PacketHandler(p);
	}
}
