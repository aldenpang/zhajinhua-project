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
#include "CommonPlayer.h"
#include "SharedData.h"
using namespace SharedData;

class LoginServerNet : public INetLayer
{
	Q_OBJECT
public:
	LoginServerNet();
	~LoginServerNet();

	void RequestGameList(int _gameType);

	virtual void PacketHandler(Packet& _packet);

	void SendLoginRequest(QString& _userName, QString& _pwd);

signals:
	void SiLoginOK();
	void SiLoginFailed(quint32 _errorCode);
	void SiGameList(QVector<RoomInfo> _gameList);
	void SiPlayerInfo(CommonPlayer _player);

protected:
private:
	void processLogin(Packet& _packet);
	void processGameList(Packet& _packet);
	void processPlayerInfo(Packet& _packet);

};
#endif // LoginServerNet_h__