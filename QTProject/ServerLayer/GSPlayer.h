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
	BAOZI=0,	// ���� ���ŵ�����ͬ���ƣ���AAA 222
	SHUNJIN,	// ˳�� ��ɫ��ͬ��˳�ӣ��������234
	JINHUA,		// �� ��ɫ��ͬ����˳�ӣ������259
	SHUNZI,		// ˳�� ��ɫ��ͬ��˳�ӣ������2����3��Ƭ4
	DUIZI,		// ���� ������ͬ���ƣ���223
	SANPAI,		// ɢ�� �����κ����͵���
	SANPAI235,	// ɢ��235 ���⻨ɫ��235

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
	quint32		mID;		// ID����2��K��A��0��51�����պں�÷��˳��
	quint32		mValue;		// ����ֵ�� ��2��K��A��0~12�����ȴ�С��
	PokerFlower	mFlower;	// ��ɫ

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

	// �ǳ�
	PROPERTY(QString, NickName);

	// �Ա� 0-male 1-female
	PROPERTY(int, Gender);

	// ��ֵ
	PROPERTY(uint, Coin);

	// ����ֵ
	PROPERTY(uint, Exp);

	// ����ʱ��
	PROPERTY(uint, PlayTime);

	// �Ƿ��ڵȴ�������Ϸ
	PROPERTY(bool, IsWaiting);

	// ��ԴIP
	PROPERTY(QString, IP);

	void AddPoker(quint32 _id);

	void CleanPokers();

	// �жϱ��˵����Ƿ��ܹ�ѹ��
	// return value: 1=true 0=false -1=same
	int CanPush(QList<PokerPtr>& _other);
protected:
private:
	// ��������
	QList<PokerPtr>	mHandPoker;
	PokerType mHandPokerType;

private:
	// ���˿˴�С��������
	void reorderPokers2toA(QList<PokerPtr>& _input);

	// �������е�������������
	PokerType analysePokerType( QList<PokerPtr>& _input );

	static bool lessThan(const PokerPtr &s1, const PokerPtr &s2);

};

typedef QSharedPointer<GSPlayer> GSPlayerPtr;

#endif // GSPlayer_h__