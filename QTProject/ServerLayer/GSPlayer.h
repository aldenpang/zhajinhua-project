/********************************************************************
	created:	2012/03/07
	created:	7:3:2012   14:28
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer\GSPlayer.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer
	file base:	GSPlayer
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef GSPlayer_h__
#define GSPlayer_h__

#include "ISocketInstance.h"
#include "MyToolkits.h"

class GSPlayer : public ISocketInstance
{
public:
	GSPlayer(QTcpSocket* _socket);
	~GSPlayer();

	// Account ID
	PROPERTY(quint32, AccountID);

	// 昵称
	PROPERTY(QString, NickName);

	// 性别 0-male 1-female
	PROPERTY(int, Gender);

	// 币值
	PROPERTY(uint, Coin);

	// 经验值
	PROPERTY(uint, Exp);

	// 在线时长
	PROPERTY(uint, PlayTime);

	// 是否在等待进入游戏
	PROPERTY(bool, IsWaiting);

	// 来源IP
	PROPERTY(QString, IP);

protected:
private:
	// 保存手牌
	QSet<int>	mHandPoker;
};

typedef QSharedPointer<GSPlayer> GSPlayerPtr;

#endif // GSPlayer_h__