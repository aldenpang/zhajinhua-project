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
#include "TableInfo.h"

class GameServerNet : public INetLayer
{
	Q_OBJECT
signals:
	void SiLoginOK();
	void SiLoginFailed(quint32 _errCode);
	void SiTableList(QMap<int, TableData> _tables);
	void SiTableJoinResult(quint32 _res, quint32 _tableID, quint32 _seatID);
	void SiStartGame(TableInfo _info);
	void SiDropBaseChip(int _baseChip);
	void SiDistribute(QVector<int> _pokers);
	void SiCurrentPlayer(int _currentPlayer);
	void SiTableEnd();
	void SiFollow(int _seatID, int _chip, int _currentPlayer);

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
	void processTableJoin(Packet& _packet);
	void processStartGame(Packet& _packet);
	void processDropBaseChip(Packet& _packet);
	void processDistribute(Packet& _packet);
	void processCurrentPlayer(Packet& _packet);
	void processTableEnd(Packet& _packet);
	void processFollow(Packet& _packet);
	
};

#endif // GameServerNet_h__