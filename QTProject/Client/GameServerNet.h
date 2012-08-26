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
	void SiTableJoinResult(quint32 _res, quint32 _tableID, quint32 _seatID, TablePlayer _player);
	void SiTableLeaveResult(quint32 _res, quint32 _tableID, TablePlayer _player);
	void SiStartGame(TableInfo _info);
	void SiBringMoneyRes(int _res);
	void SiDropBaseChip(int _baseChip);
	void SiDistribute(QVector<int> _pokers);
	void SiCurrentPlayer(int _currentPlayer);
	void SiTableEnd();
	void SiFollow(int _seatID, int _chip, int _currentPlayer, int _currentBid);
	void SiSyncStart();
	void SiUpdateMoney(quint32 _goldCoin, quint32 _silverCoin);

public:
	GameServerNet();
	~GameServerNet();

	virtual void PacketHandler(Packet& _packet);

	void SendLoginGS(QString& _userName, QString& _pwd, int _isTempLogin=false);
	void SendBringMoney( quint32 _tableID, quint32 _seatID, quint32 _money );
	void SendJoinTable(quint32 _tableID, quint32 _seatID);
	void SendLeaveTable(quint32 _tableID);
	void RequestTableInfo();
protected:
private:
	void processLogin(Packet& _packet);
	void processTableInfo(Packet& _packet);
	void processBringMoneyRes(Packet& _packet);
	void processTableJoin(Packet& _packet);
	void processTableLeave(Packet& _packet);
	void processStartGame(Packet& _packet);
	void processDropBaseChip(Packet& _packet);
	void processDistribute(Packet& _packet);
	void processCurrentPlayer(Packet& _packet);
	void processTableEnd(Packet& _packet);
	void processFollow(Packet& _packet);
	void processSyncStart(Packet& _packet);
	void processQueryMoney(Packet& _packet);
};

#endif // GameServerNet_h__