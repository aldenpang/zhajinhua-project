/********************************************************************
	created:	2012/03/31
	created:	31:3:2012   14:13
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\ConsoleClient\ConsoleClient.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\ConsoleClient
	file base:	ConsoleClient
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef ConsoleClient_h__
#define ConsoleClient_h__
#include <QObject>
#include <QVector>
#include <QTimer>
#include "TableData.h"
#include "SharedData.h"
#include "TableInfo.h"
#include "Poker.h"
#include "CommonPlayer.h"
using namespace SharedData;

class LoginServerNet;
class GameServerNet;
class ConsoleClient : public QObject
{
Q_OBJECT
public:
	ConsoleClient();
	~ConsoleClient();

signals:
	void SiInfo(const QString& _text);
	void SiWarn(const QString& _text);
	void SiError(const QString& _text);

private slots:
	void stLSConnected();
	void stLSDisconnected();
	void stNetError(QString _errStr);
	void stLoginOK();
	void stLoginFailed(quint32 _errorCode);
	void stGameList(QVector<RoomInfo> _gameList);
	void stPlayerInfo(CommonPlayer _player);

	void stGSConnected();
	void stGSDisconnected();
	void stGSLoginOK();
	void stGSLoginFailed(quint32 _errorCode);
	void stBringMoneyRes(int _res);
	void stTableList(QMap<int, TableData> _tables);
	void stTableJoinRes(quint32 _res, quint32 _tableID, quint32 _seatID, TablePlayer _player);
	void stStartGame(TableInfo _info);
	void stDropBaseChip(int _baseChip);
	void stDistribute(QVector<int> _pokers);
	void stCurrentPlayer(int _currentPlayer);
	void stTableEnd(TableInfo _info, QMap<int, int> _res);
	void stFollow(int _seatID, int _chip, int _currentPlayer, int _currentBid);
	void stSyncStart();

	void stActionByTimer();	// 可能是跟，也可能是放弃
protected:
private:
	LoginServerNet* mLoginServer;
	GameServerNet* mGameServer;

	CommonPlayer mMyInfo;
	QTimer mTimer;		// pause before follow
	TableInfo mCurrentTableInfo;
	QVector<PokerPtr> mPokers;
	int mCurrentPlayer;
	int mMySeatID;				// table id which is confirmed
	int mMyTableID;				// seat id which is confirmed
	int mTempTableID;			// table id which is maybe not confirmed;
	int mTempSeatID;			// table id which is maybe not confirmed;
	int mCurrentTableMinBringChip;
	bool mIsEnd;
};

#endif // ConsoleClient_h__