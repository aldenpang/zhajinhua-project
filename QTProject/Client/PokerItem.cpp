#include "stdafx.h"
#include "PokerItem.h"


PokerItem::PokerItem( quint32 _id )
: Poker(_id)
{
	mImagePath = QString(":/Pokers/Media/Pokers/%1.png").arg(_id);
	setPixmap(mImagePath);
	//setScale(0.7);
}

PokerItem::~PokerItem()
{

}

void PokerItem::ToBack()
{
	setPixmap(":/Pokers/Media/Pokers/back.png");
}

void PokerItem::ToFront()
{
	setPixmap(mImagePath);
}