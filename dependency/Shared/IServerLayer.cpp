#include "IServerLayer.h"
#include "LogModule.h"
#include "Packet.h"

IServerLayer::IServerLayer()
: QTcpServer(0)
{
	connect(this, SIGNAL(SiInfo(const QString&)), &LOG, SLOT(StInfo(const QString&)));
	connect(this, SIGNAL(newConnection()), this, SLOT(stNewConnections()));
}

IServerLayer::~IServerLayer()
{

}

//------------------------------------------------------------------------------
void IServerLayer::StStart(QString _ip, uint _port)
{
	QHostAddress addr;
	if ( _ip.isEmpty() )
		addr = QHostAddress::Any;
	else
		addr = QHostAddress(_ip);

	if (!this->listen(addr, _port))
	{
		emit SiError(this->errorString());
		return;
	}
	emit SiStarted(_port);
	emit SiInfo("Started!");
}
//------------------------------------------------------------------------------
void IServerLayer::StStop()
{
	this->close();
	emit SiStoped();
	emit SiInfo("Stopped!");
}

void IServerLayer::stNewConnections()
{
	QTcpSocket* s = nextPendingConnection();
	if (s)
	{
		emit SiInfo(QString("incoming socket: %1:%2").arg(s->peerAddress().toString()).arg(s->peerPort()));

		// ��Ҫ����һ��ClientInstance���������Socket��ָ�룬���Բ�ͬ��server���в�ͬ��ʵ��
		mClientList.append(ISocketInstancePtr(new ISocketInstance(s)));

		// �������Ӵ���Client Socket�����к���
		connect(s, SIGNAL(disconnected()), this, SLOT(stClientDisconnect()));
		connect(s, SIGNAL(readyRead()), this, SLOT(stReadData()));

	}
}

void IServerLayer::stClientDisconnect()
{
	QTcpSocket* s = (QTcpSocket*)sender();
	ISocketInstancePtr sockIns = findInstance(s);
	if ( sockIns )
	{
		emit SiInfo(QString("socket disconnected: %1:%2").arg(sockIns->GetSocket()->peerAddress().toString()).arg(sockIns->GetSocket()->peerPort()));
		mClientList.removeOne(sockIns);
		//emit SiInfo(QString("ClientListSize:%1").arg(mClientList.size()));
	}
}

void IServerLayer::stReadData()
{
	// �����緢�͹������ֽ���ת��ΪPacket
	QTcpSocket* s = (QTcpSocket*)sender();
	ISocketInstancePtr sockIns = findInstance(s);
	if ( sockIns && sockIns->GetSocket()->bytesAvailable() )
	{
		char buff[MAX_PACKET_SIZE]={0};
		sockIns->GetSocket()->read(buff, MAX_PACKET_SIZE);
		qDebug()<<"Data Received:"<<buff;

		Packet p;
		p.SetData(buff);
		if ( !p.IsTokenValid() )
		{
			emit SiInfo(QString("Invalid Packet from %1:%2").arg(s->peerAddress().toString()).arg(s->peerPort()));
			return;
		}
		
		// ��ͬ��Serverӵ���Լ���PacketHandler()
		PakcetHandler(sockIns, &p);
	}
}

ISocketInstancePtr IServerLayer::findInstance( QTcpSocket* _socket )
{
	if (!_socket)
		return ISocketInstancePtr(NULL);

	QList<ISocketInstancePtr>::iterator itr = mClientList.begin();
	for (; itr != mClientList.end(); ++itr)
	{
		if ( (*itr)->GetSocket() == _socket )
		{
			return *itr;
		}
	}

	return ISocketInstancePtr(NULL);
}

