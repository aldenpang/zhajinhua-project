/********************************************************************
	Copyright (C) 2005 by Bele Group
	@date:	    2011-12-19   15:27
	@file: 		INetLayer.h
	@author:    Snake
	@desc:		
*********************************************************************/
#ifndef _INETLAYER_H_
#define _INETLAYER_H_

#include "Packet.h"
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QHostAddress>

class INetLayer:public QObject
{
Q_OBJECT
public:
	INetLayer();
	~INetLayer();
	void Init();
	void Connect(const QString& _ip, const int _port);
	void Send(const char* _data);
	void Send(Packet* _packet);
	void Disconnect();

	virtual void PacketHandler(Packet& _packet)=0;
signals:
	void SiConnected();
	void SiDisconnected();

	void SiInfo(const QString& _text);
	void SiWarn(const QString& _text);
	void SiError(const QString& _text);

private slots:
	void stError(QAbstractSocket::SocketError _socketError);
	void stRead();

protected:
	QTcpSocket mTcpSocket;

private:
};

#endif //_INETLAYER_H_

/*
 *
 * [Revision 1.0   2011-12-19   15:27  Administrator] Created
 *
 */