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

private slots:
	void stQuit();

	// for testing
	void stLoginOK();
	void stNetError(QString _err);
	void stLoginFailed(quint32 _errorCode);
	void stGameList(QVector<RoomInfo> _gameList);

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
