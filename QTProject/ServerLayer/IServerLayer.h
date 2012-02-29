/********************************************************************
	Copyright (C) 2005 by Bele Group
	@date:	    2011-12-31   14:43
	@file: 		IServerLayer.h
	@author:    Snake
	@desc:		
*********************************************************************/
#ifndef _ISERVERLAYER_H_
#define _ISERVERLAYER_H_
#include "PublicHeader.h"

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
	void SiClientConnected(const QString& _info);
	void SiClientDisconnected(const QString& _info);
public slots:
	virtual void StStart();
	virtual void StStop();
	//virtual void StRestart() = 0;
public:
	IServerLayer();
	virtual ~IServerLayer();

	virtual void PakcetHandler(Packet* _packet)=0;
private slots:
	void stNewConnections();
	void stClientDisconnect();	
	void stReadData();
protected:
	QList<QTcpSocket*>	mClientList;
private:
};//class IServerLayer

//typedef boost::shared_ptr<ITcpServer> ITcpServerPtr;

#endif //_ISERVERLAYER_H_

/*
 *
 * [Revision 1.0   2011-12-31   14:43  Administrator] Created
 *
 */