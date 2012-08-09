/********************************************************************
	Copyright (C) 2012 by Pang Shuo
	@date:	    29:7:2012   15:04
	@file: 		PokerItem.h
	@author:    Pang Shuo
	@desc:		
*********************************************************************/

#ifndef PokerItem_h__
#define PokerItem_h__
#include "Poker.h"
#include "MoveItem.h"

class PokerItem : public Poker, public MoveItem
{
public:
	PokerItem(quint32 _id);
	~PokerItem();

	void toBack();

protected:
private:
};

#endif // PokerItem_h__