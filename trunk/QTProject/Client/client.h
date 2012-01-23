#ifndef CLIENT_H
#define CLIENT_H

#include <QtGui/QWidget>
//#include "ui_client.h"

class MouseEventFilter;
class INetLayer;
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
	INetLayer* mNetLayer;

	void initMouseEventFilter();
	void regConnections();

	void initNetLayer();
	
};

#endif // CLIENT_H
