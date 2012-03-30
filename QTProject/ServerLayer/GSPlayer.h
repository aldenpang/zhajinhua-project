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

#define MAX_HAND_POKER 3

enum PokerType
{
	BAOZI=0,	// 豹子 三张点数相同的牌，如AAA 222
	SHUNJIN,	// 顺金 花色相同的顺子，如果红桃234
	JINHUA,		// 金花 花色相同，非顺子，如红桃259
	SHUNZI,		// 顺子 花色不同的顺子，如红桃2黑桃3方片4
	DUIZI,		// 对子 两张相同的牌，如223
	SANPAI,		// 散牌 不是任何牌型的牌
	SANPAI235,	// 散牌235 任意花色的235

	NONE,
};

enum PokerFlower
{
	HEITAO=0,
	HONGTAO,
	MEIHUA,
	FANGPIAN,
};

class Poker
{
public:
	quint32		mID;		// ID：从2～K～A，0～51，按照黑红梅方顺序
	quint32		mValue;		// 牌面值： 从2～K～A，0~12；做比大小用
	PokerFlower	mFlower;	// 花色

	Poker()
	{
		mID=mValue=0;
		mFlower = HEITAO;
	}

	// for fit hash table
	Poker &operator=(const Poker &other)
	{
		this->mFlower = other.mFlower;
		this->mID = other.mID;
		this->mValue = other.mValue;
		return *this;
	}

};

typedef QSharedPointer<Poker> PokerPtr;

//////////////////////////////////////////////////////////////////
/////////////////////// Poker value table //////////////////////
//////////////////////////////////////////////////////////////////
//ID		0  1  2  3  4  5  6  7  8   9  10  11  12    Heitao
//Value     2  3  4  5  6  7  8  9  10  J  Q   K   A
//ID		13 14 15 16 17 18 19 20 21  22 23  24  25	Hongtao
//Value     2  3  4  5  6  7  8  9  10  J  Q   K   A
//ID		26 27 28 29 30 31 32 33 34  35 36  37  38	Meihua
//Value     2  3  4  5  6  7  8  9  10  J  Q   K   A
//ID		39 40 41 42 43 44 45 46 47  48 49  50  51	Fangpian
//Value     2  3  4  5  6  7  8  9  10  J  Q   K   A
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


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

	void AddPoker(quint32 _id);

	void CleanPokers();

	// 判断别人的牌是否能够压上
	// return value: 1=true 0=false -1=same
	int CanPush(QList<PokerPtr>& _other);
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