/********************************************************************
	created:	2012/03/16
	created:	16:3:2012   14:34
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\Client\GameServerNet.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\Client
	file base:	GameServerNet
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef GameServerNet_h__
#define GameServerNet_h__

#include "INetLayer.h"
#include "TableData.h"

class GameServerNet : public INetLayer
{
	Q_OBJECT
signals:
	void SiLoginOK();
	void SiLoginFailed(quint32 _errCode);
	void SiTableList(QMap<int, TableData> _tables);
public:
	GameServerNet();
	~GameServerNet();

	virtual void PacketHandler(Packet& _packet);

	void SendLoginGS(QString& _userName, QString& _pwd);

	void SendJoinTable(quint32 _tableID, quint32 _seatID);

protected:
private:
	void processLogin(Packet& _packet);
	void processTableInfo(Packet& _packet);
};

#endif // GameServerNet_h__