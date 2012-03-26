/********************************************************************
	created:	2012/03/26
	created:	26:3:2012   16:12
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\WalletServer\AppFrame.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\WalletServer
	file base:	AppFrame
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef AppFrame_h__
#define AppFrame_h__

#include <QObject>

class WalletServer;
class AppFrame : public QObject
{
public:
	AppFrame();
	~AppFrame();

	void InitDatabase();

	void Run();
protected:
private:
	WalletServer* mWalletServer;
};

#endif // AppFrame_h__