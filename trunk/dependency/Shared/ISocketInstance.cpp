#include "ISocketInstance.h"
#include "Packet.h"


ISocketInstance::ISocketInstance(QTcpSocket* _socket)
: mSocket(_socket)
{

}

ISocketInstance::~ISocketInstance()
{

}

void ISocketInstance::Send( Packet* _p )
{
	_p->End();
	char sendBuff[MAX_PACKET_SIZE] = {0};
	memcpy(sendBuff, _p->GetData(), MAX_PACKET_SIZE);

	//qint64 res = mTcpSocket.write(sendBuff);
	int leng = _p->GetDataLength();
	qint64 res = mSocket->write(QByteArray::fromRawData(sendBuff, leng));

}
