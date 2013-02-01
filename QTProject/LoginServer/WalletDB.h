/********************************************************************
	Copyright (C) 2012 by Alden Pang
	@date:	    18:8:2012   13:43
	@file: 		WalletDB.h
	@author:    Alden Pang
	@desc:		
*********************************************************************/

#ifndef WalletDB_h__
#define WalletDB_h__

#include "DBLayer.h"
#include "SharedData.h"
using namespace SharedData;

class CommonPlayer;
class WalletDB : public DBLayer, public Singleton<WalletDB>
{
public:
	void QueryPlayerMoney(quint32 _accountID, CommonPlayer *_player);
protected:
private:
};

#define WDB WalletDB::GetSingleton()

#endif // WalletDB_h__