/********************************************************************
	created:	 2012/08/09
	created:	 9:8:2012   16:10
	file name:	 CommonPlayer.h
	author:		 PangShuo
	description:
*********************************************************************/


#ifndef CommonPlayer_h__
#define CommonPlayer_h__

#include "MyToolkits.h"
#include "Packet.h"

class CommonPlayer
{
public:
	// Account ID
	PROPERTY(quint32, AccountID);

	// �ǳ�
	PROPERTY(QString, NickName);

	// �Ա� 0-male 1-female
	PROPERTY(quint32, Gender);

	// User wallet
	PROPERTY(quint32, UserWalletMoney);

	// User wallet id
	PROPERTY(quint32, UserWalletID);

	// Table wallet
	PROPERTY(quint32, TableWalletMoney);

	// Table wallet id
	PROPERTY(quint32, TableWalletID);

	// ����ֵ
	PROPERTY(quint32, Exp);

	// ����ʱ��
	PROPERTY(quint32, PlayTime);

	// ��ԴIP
	PROPERTY(QString, IP);

	// ͷ��ID
	PROPERTY(quint32, ProtraitID);

	CommonPlayer()
	{
		mAccountID = 0;
		mNickName = "";
		mGender = 0;
		mUserWalletID = 0;
		mTableWalletID = 0;
		mUserWalletMoney = 0;
		mTableWalletMoney = 0;
		mExp = 0;
		mPlayTime = 0;
		mProtraitID = 0;
		mIP = "";
	}

	void ToPacket(Packet& _p)
	{
		_p<<mAccountID<<mNickName<<mGender<<mUserWalletMoney<<mExp<<mPlayTime<<mProtraitID;
	}

	void FromPacket(Packet& _p)
	{
		_p>>mAccountID>>mNickName>>mGender>>mUserWalletMoney>>mExp>>mPlayTime>>mProtraitID;
	}
protected:
private:
};

#endif // CommonPlayer_h__