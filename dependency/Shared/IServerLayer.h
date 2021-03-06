/********************************************************************
	Copyright (C) 2013 by Alden Pang
	@date:	    2011-12-31   14:43
	@file: 		IServerLayer.h
	@author:    Alden Pang
	@desc:		
*********************************************************************/
#ifndef _ISERVERLAYER_H_
#define _ISERVERLAYER_H_
#include <QtNetwork>
#include "LogModule.h"
#include "ISocketInstance.h"

class Packet;
/// \class IServer
class IServerLayer : public QTcpServer
{
	Q_OBJECT

signals: 
	void SiInfo(const QString& _text);
	void SiWarn(const QString& _text);
	void SiError(const QString& _text);

	void SiStarted(quint16 _port);
	void SiStoped();
	//void SiClientConnected(const QString& _info);
	//void SiClientDisconnected(ISocketInstancePtr _player);
public slots:
	virtual void StStart(QString _ip, uint _port);
	virtual void StStop();
	//virtual void StRestart() = 0;
public:
	IServerLayer();
	virtual ~IServerLayer();

	virtual void PacketHandler(ISocketInstancePtr _incomeSocket, Packet& _packet)=0;

	virtual void ClientDisconnected(ISocketInstancePtr _clientSocket){};

	virtual void Broadcast(Packet* _packet);
private slots:
	void stNewConnections();
	void stClientDisconnect();	
	void stReadData();
protected:
private:
	// 通过传入的Socket找SocketInstance
	ISocketInstancePtr findInstance(QTcpSocket* _socket);
	QList<ISocketInstancePtr>	mClientList;

};//class IServerLayer

//typedef boost::shared_ptr<ITcpServer> ITcpServerPtr;

#endif //_ISERVERLAYER_H_

/*
 *
 * [Revision 1.0   2011-12-31   14:43  Administrator] Created
 *
 */