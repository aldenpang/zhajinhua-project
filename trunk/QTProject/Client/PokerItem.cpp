#include "stdafx.h"
#include "PokerItem.h"


PokerItem::PokerItem( quint32 _id )
: Poker(_id)
{
	QString imagePath = QString(":/Pokers/Media/Pokers/%1.png").arg(_id);
	setPixmap(imagePath);
	//setScale(0.7);
}

PokerItem::~PokerItem()
{

}

void PokerItem::toBack()
{
	setPixmap(":/Pokers/Media/Pokers/back.png");
}