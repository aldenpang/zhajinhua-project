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
#include "TableData.h"
#include "SharedData.h"
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

	void stGSConnected();
	void stGSDisconnected();
	void stGSLoginOK();
	void stGSLoginFailed(quint32 _errorCode);
	void stTableList(QMap<int, TableData> _tables);
	void stTableJoinRes(quint32 _res);

protected:
private:

	LoginServerNet* mLoginServer;
	GameServerNet* mGameServer;
};

#endif // ConsoleClient_h__