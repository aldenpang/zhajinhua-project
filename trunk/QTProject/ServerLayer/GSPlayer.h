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

	// �ǳ�
	PROPERTY(QString, NickName);

	// �Ա� 0-male 1-female
	PROPERTY(int, Gender);

	// User wallet
	PROPERTY(uint, UserWalletMoney);

	// Table wallet
	PROPERTY(uint, TableWalletMoney);

	// ����ֵ
	PROPERTY(uint, Exp);

	// ����ʱ��
	PROPERTY(uint, PlayTime);

	// �Ƿ��ڵȴ�������Ϸ
	PROPERTY(bool, IsWaiting);

	// ��ԴIP
	PROPERTY(QString, IP);

	void AddPoker(quint32 _id);

	void AddPoker(PokerPtr _poker);

	void CleanPokers();

	// �жϱ��˵����Ƿ��ܹ�ѹ��
	// return value: 1=true 0=false -1=same
	int CanPush(QList<PokerPtr>& _other);

	PokerType GetPokerType(){ return mHandPokerType; }
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