/********************************************************************
	created:	2012/03/05
	created:	5:3:2012   15:01
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\LoginServer\AppFrame.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\LoginServer
	file base:	AppFrame
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef AppFrame_h__
#define AppFrame_h__
#include <QObject>

class LoginServer;
class AppFrame : public QObject
{
public:
	AppFrame();
	~AppFrame();


	void Run();
protected:
private:
	LoginServer* mLoginServer;


	void initDatabase();
	void initWalletDatabase();

};

#endif // AppFrame_h__