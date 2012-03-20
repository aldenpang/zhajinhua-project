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

protected:
private:
	// ��������
	QSet<int>	mHandPoker;
};

typedef QSharedPointer<GSPlayer> GSPlayerPtr;

#endif // GSPlayer_h__