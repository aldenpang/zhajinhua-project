#include "GSPlayer.h"
#include <QtAlgorithms>

GSPlayer::GSPlayer(QTcpSocket* _socket)
: ISocketInstance(_socket)
{
	mNickName = "";
	mGender = 0;
	mUserWalletMoney = 0;
	mTableWalletMoney = 0;
	mExp = 0;
	mPlayTime = 0;
	mIsWaiting = false;
	mHandPokerType = NONE;
}

GSPlayer::~GSPlayer()
{

}

bool GSPlayer::CanPush( QList<PokerPtr>& _other )
{
	PokerType otherType = analysePokerType(_other);

	if ( otherType == mHandPokerType )
	{
		QList<PokerPtr>::iterator itr;
		int otherIdx = 0;
		for( itr = mHandPoker.begin(); itr != mHandPoker.end(); itr++, otherIdx++ )
		{
			if ( (*itr)->mValue > _other[otherIdx]->mValue)
			{
				return true;
			}
		}
	}
	else
	{
		if ( mHandPokerType < otherType )
		{
			return true;
		}
		else if ( (mHandPokerType == SANPAI235) && (otherType == BAOZI) )
		{
			return true;
		}
		else
			return false;
	}

	return false;
}

void GSPlayer::reorderPokers2toA( QList<PokerPtr>& _input )
{
	if ( _input.isEmpty() )
		return;

	//QList<PokerPtr> temp = _input.toList();
	//qSort(temp.begin(), temp.end(), lessThan);
	//QSet<PokerPtr> res = QSet<PokerPtr>::fromList(temp);

	qSort(_input.begin(), _input.end(), lessThan);
}

PokerType GSPlayer::analysePokerType( QList<PokerPtr>& _input )
{
	if ( _input.isEmpty() )
		return NONE;

	reorderPokers2toA(_input);

	if ( (_input[0]->mValue == _input[1]->mValue) && (_input[1]->mValue  == _input[2]->mValue) )
	{
		return BAOZI;
	}
	
	if ( (_input[0]->mFlower == _input[1]->mFlower) && (_input[1]->mFlower == _input[2]->mFlower) )
	{
		if ( (_input[2]->mValue-_input[1]->mValue == 1) && (_input[1]->mValue-_input[0]->mValue == 1) )
		{
			return SHUNJIN;
		}
		else
			return JINHUA;
	}
	else
	{
		if ( (_input[2]->mValue-_input[1]->mValue == 1) && (_input[1]->mValue-_input[0]->mValue == 1) )
		{
			return SHUNZI;
		}
	}

	if ( _input[2]->mValue == _input[1]->mValue ||
		_input[2]->mValue==_input[0]->mValue ||
		_input[0]->mValue==_input[1]->mValue)
	{
		return DUIZI;
	}

	if ( _input[0]->mValue == 2 && _input[1]->mValue == 3 && _input[2]->mValue == 5 )
	{
		return SANPAI235;
	}

	return SANPAI;
}

bool GSPlayer::lessThan( const PokerPtr &s1, const PokerPtr &s2 )
{
	return s1->mValue < s2->mValue;
}

void GSPlayer::AddPoker( quint32 _id )
{
	if ( mHandPoker.size() >= MAX_HAND_POKER )
		return;

	if ( _id > 51 || _id < 0 )
		return;

	PokerPtr p = PokerPtr(new Poker(_id));

	AddPoker(p);
}

void GSPlayer::AddPoker( PokerPtr _poker )
{
	if ( mHandPoker.size() >= MAX_HAND_POKER )
		return;

	mHandPoker.append(_poker);

	if ( mHandPoker.size() == MAX_HAND_POKER )
	{
		mHandPokerType = analysePokerType(mHandPoker);
	}
}

void GSPlayer::CleanPokers()
{
	mHandPoker.clear();
}
