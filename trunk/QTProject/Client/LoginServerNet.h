/********************************************************************
	created:	2012/03/07
	created:	7:3:2012   11:20
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\Client\LoginServerNet.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\Client
	file base:	LoginServerNet
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef LoginServerNet_h__
#define LoginServerNet_h__
#include "INetLayer.h"

class LoginServerNet : public INetLayer
{
	Q_OBJECT
public:
	LoginServerNet();
	~LoginServerNet();

	void RequestGameList(int _gameType);

	virtual void PacketHandler(Packet& _packet);

signals:
	void SiLoginOK();
	void SiLoginFailed(quint32 _errorCode);
	void SiGameList(QVector<RoomInfo> _gameList);

protected:
private:
	void processLogin(Packet& _packet);
	void processGameList(Packet& _packet);

};
#endif // LoginServerNet_h__