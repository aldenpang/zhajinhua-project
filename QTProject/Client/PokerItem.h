/********************************************************************
	Copyright (C) 2012 by Alden Pang
	@date:	    29:7:2012   15:04
	@file: 		PokerItem.h
	@author:    Alden Pang
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

	void ToBack();

	void ToFront(quint32 _id);

protected:
private:
	QString mImagePath;
};

#endif // PokerItem_h__