/********************************************************************
	Copyright (C) 2005 by Bele Group
	@date:	    2011-12-19   15:27
	@file: 		INetLayer.h
	@author:    Snake
	@desc:		
*********************************************************************/
#ifndef _INETLAYER_H_
#define _INETLAYER_H_

class INetLayer:public QObject
{
Q_OBJECT
public:
	INetLayer();
	~INetLayer();
	void Init();
	void Connect(const QString& _ip, const int _port);
	void Send();
	void Disconnect();

	//virtual void packetHandle(GCPacket& _packet) = 0;

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