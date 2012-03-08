#include "GSPlayer.h"

GSPlayer::GSPlayer(QTcpSocket* _socket)
: ISocketInstance(_socket)
{
	mNickName = "";
	mGender = 0;
	mCoin = 0;
	mExp = 0;
	mPlayTime = 0;
}

GSPlayer::~GSPlayer()
{

}
