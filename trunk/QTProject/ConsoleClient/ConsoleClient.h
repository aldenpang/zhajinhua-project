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

class LoginServerNet;
class GameServerNet;
class ConsoleClient : public QObject
{
Q_OBJECT
public:
	ConsoleClient();
	~ConsoleClient();

private slots:

protected:
private:

	LoginServerNet* mLoginServer;
	GameServerNet* mGameServer;
};

#endif // ConsoleClient_h__