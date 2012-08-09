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
#include "CommonPlayer.h"

class GSPlayer : public CommonPlayer, public ISocketInstance
{
public:
	GSPlayer(QTcpSocket* _socket);
	~GSPlayer();

	// 是否在等待进入游戏
	PROPERTY(bool, IsWaiting);

	// 是否已经放弃
	PROPERTY(bool, IsGiveUp);


	// 已经跟的钱数
	void AlreadyFollow(quint32 _alreadyFollow);
	quint32 GetAlreadyFollow(){ return mAlreadyFollow; }
	// 清除跟的钱数
	void CleanAlreadyFollow();

	void AddPoker(quint32 _id);

	void AddPoker(PokerPtr _poker);

	void CleanPokers();

	// 判断别人的牌是否能够压上
	// return value: 1=true 0=false -1=same
	bool CanPush(QList<PokerPtr>& _other);

	PokerType GetPokerType(){ return mHandPokerType; }

	QList<PokerPtr>& GetPokers(){ return mHandPoker;	 }

protected:
private:
	// 保存手牌
	QList<PokerPtr>	mHandPoker;
	PokerType mHandPokerType;
	quint32 mAlreadyFollow;
private:
	// 让扑克从小往大排序
	void reorderPokers2toA(QList<PokerPtr>& _input);

	// 分析手中的牌是哪种牌型
	PokerType analysePokerType( QList<PokerPtr>& _input );

	static bool lessThan(const PokerPtr &s1, const PokerPtr &s2);

};

typedef QSharedPointer<GSPlayer> GSPlayerPtr;

#endif // GSPlayer_h__