/********************************************************************
	created:	2012/05/17
	created:	17:5:2012   15:51
	filename: 	d:\My Projects\ZJHClient&Server\dependency\Shared\Poker.h
	file path:	d:\My Projects\ZJHClient&Server\dependency\Shared
	file base:	Poker
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef Poker_h__
#define Poker_h__

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



enum PokerType
{
	BAOZI=0,	// 豹子 三张点数相同的牌，如AAA 222
	SHUNJIN,	// 顺金 花色相同的顺子，如果红桃234
	JINHUA,		// 金花 花色相同，非顺子，如红桃259
	SHUNZI,		// 顺子 花色不同的顺子，如红桃2黑桃3方片4
	DUIZI,		// 对子 两张相同的牌，如223
	SANPAI,		// 散牌 不是任何牌型的牌
	SANPAI235,	// 散牌235 任意花色的235 只能压豹子

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

	Poker(quint32 _id)
	{
		mID = _id;
		mValue = _id % 13 + 2;		// +2 是因为整除得到的值是从0开始，而牌面值是从2开始
		mFlower = (PokerFlower)(int)(_id/13);
	}

	// for fit hash table
	Poker &operator=(const Poker &other)
	{
		this->mFlower = other.mFlower;
		this->mID = other.mID;
		this->mValue = other.mValue;
		return *this;
	}

	//const Poker operator>(const Poker &other)
	//{
	//	return this->mValue > other.mValue?*this:other;
	//}

	//const Poker operator<(const Poker &other)
	//{
	//	return this->mValue < other.mValue?*this:other;
	//}

};

typedef QSharedPointer<Poker> PokerPtr;

#endif // Poker_h__