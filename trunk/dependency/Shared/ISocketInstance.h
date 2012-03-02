/********************************************************************
	created:	2012/03/02
	created:	2:3:2012   11:32
	filename: 	d:\My Projects\ZJHClient&Server\dependency\Shared\ISocketInstance.h
	file path:	d:\My Projects\ZJHClient&Server\dependency\Shared
	file base:	ISocketInstance
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef ISocketInstance_h__
#define ISocketInstance_h__

#include <QSharedPointer>
#include <QTcpSocket>

class Packet;
class QTcpSocket;
class ISocketInstance
{
public:
	ISocketInstance(QTcpSocket* _socket);
	~ISocketInstance();

	void Send(Packet* _p);

	QTcpSocket* GetSocket(){return mSocket;}

protected:
private:
	QTcpSocket* mSocket;
};

typedef QSharedPointer<ISocketInstance> ISocketInstancePtr;

#endif // ISocketInstance_h__