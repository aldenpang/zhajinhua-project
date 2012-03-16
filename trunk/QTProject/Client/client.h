#ifndef CLIENT_H
#define CLIENT_H

#include <QtGui/QWidget>
//#include "ui_client.h"

class MouseEventFilter;
class LoginServerNet;
class GameServerNet;
class Client : public QApplication
{
	Q_OBJECT

public:
	Client(int & argc, char ** argv);
	~Client();

signals:
	void siConnectGS(QString& _ip, quint32 _port);
private slots:
	void stQuit();

	// for testing
	void stLoginOK();
	void stNetError(QString _err);
	void stLoginFailed(quint32 _errorCode);
	void stGameList(QVector<RoomInfo> _gameList);

	void stConnectGS(QString& _ip, quint32 _port);
	void stGSConnected();
	void stGSLoginOK();
	void stGSLoginFailed(quint32 _errCode);
private:
	//Ui::ClientClass ui;
	QWidget*	mMainWidget;
	MouseEventFilter* mMouseEventFilter;
	LoginServerNet* mLoginServer;
	GameServerNet* mGameServer;

	void initMouseEventFilter();
	void regConnections();

	void initLoginServer();
	void initGameServer();

};

#endif // CLIENT_H
