#ifndef CLIENT_H
#define CLIENT_H

#include <QtGui/QWidget>
//#include "ui_client.h"

class MouseEventFilter;
class ClientNet;
class Client : public QApplication
{
	Q_OBJECT

public:
	Client(int & argc, char ** argv);
	~Client();

private slots:
	void stQuit();

private:
	//Ui::ClientClass ui;
	QWidget*	mMainWidget;
	MouseEventFilter* mMouseEventFilter;
	ClientNet* mNetLayer;

	void initMouseEventFilter();
	void regConnections();

	void initNetLayer();
	
};

#endif // CLIENT_H
