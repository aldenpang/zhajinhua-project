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
#include "SharedData.h"
#include "Poker.h"

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

	// User wallet
	PROPERTY(uint, UserWalletMoney);

	// Table wallet
	PROPERTY(uint, TableWalletMoney);

	// 经验值
	PROPERTY(uint, Exp);

	// 在线时长
	PROPERTY(uint, PlayTime);

	// 是否在等待进入游戏
	PROPERTY(bool, IsWaiting);

	// 来源IP
	PROPERTY(QString, IP);

	void AddPoker(quint32 _id);

	void AddPoker(PokerPtr _poker);

	void CleanPokers();

	// 判断别人的牌是否能够压上
	// return value: 1=true 0=false -1=same
	int CanPush(QList<PokerPtr>& _other);

	PokerType GetPokerType(){ return mHandPokerType; }
protected:
private:
	// 保存手牌
	QList<PokerPtr>	mHandPoker;
	PokerType mHandPokerType;

private:
	// 让扑克从小往大排序
	void reorderPokers2toA(QList<PokerPtr>& _input);

	// 分析手中的牌是哪种牌型
	PokerType analysePokerType( QList<PokerPtr>& _input );

	static bool lessThan(const PokerPtr &s1, const PokerPtr &s2);

};

typedef QSharedPointer<GSPlayer> GSPlayerPtr;

#endif // GSPlayer_h__