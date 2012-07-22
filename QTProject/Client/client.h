#ifndef CLIENT_H
#define CLIENT_H

#include <QtGui/QWidget>

class MouseEventFilter;
class LoginServerNet;
class GameServerNet;
class LoginUI;
class LobbyUI;
class ZJHGameUI;
class Client : public QApplication
{
	Q_OBJECT

public:
	Client(int & argc, char ** argv);
	~Client();

signals:

private slots:
	void stQuit();

	//void stConnectGS(QString& _ip, quint32 _port);
	//void stGSConnected();
	//void stGSLoginOK();
	//void stGSLoginFailed(quint32 _errCode);
private:

	LoginUI* mLoginUI;
	LobbyUI* mLobbyUI;
	ZJHGameUI* mZjhUI;
	MouseEventFilter* mMouseEventFilter;

	void initMouseEventFilter(QWidget* _widget);
	void regConnections();

	//void initGameServer();

};

#endif // CLIENT_H
