#include "stdafx.h"
#include "PokerItem.h"


PokerItem::PokerItem( quint32 _id )
: MoveItem(QString(":/Pokers/Media/Pokers/%1.png").arg(_id))
, Poker(_id)
{
	mImagePath = QString(":/Pokers/Media/Pokers/%1.png").arg(_id);
	//setScale(0.7);
}

PokerItem::~PokerItem()
{

}

void PokerItem::ToBack()
{
	setPixmap(":/Pokers/Media/Pokers/back.png");
}

void PokerItem::ToFront(quint32 _id)
{
	setPixmap(QString(":/Pokers/Media/Pokers/%1.png").arg(_id));
}