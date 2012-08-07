#include "stdafx.h"
#include "MoveItem.h"
#include <QPropertyAnimation>
#include <QPainter>

MoveItem::MoveItem()
: mDuration(0)
, mTo(QPoint(0, 0))
{
	mAnimation = new QPropertyAnimation(this, "pos");
	mAnimation->setEasingCurve(QEasingCurve::OutQuint);

	mDelayTimer = new QTimer;
	mDelayTimer->setSingleShot(true);
	connect(mDelayTimer, SIGNAL(timeout()), this, SLOT(stStart()));

}

MoveItem::~MoveItem()
{

}

void MoveItem::Move( quint32 _delay, quint32 _duration, QPoint _to )
{
	mDelayTimer->start(_delay);
	mDuration = _duration;
	mTo = _to;
}

void MoveItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	painter->drawPixmap(0, 0, mPixmap);
}

QRectF MoveItem::boundingRect() const
{
	return QRectF(0, 0, mPixmap.width(), mPixmap.height());
}

void MoveItem::stStart()
{
	mAnimation->stop();
	mAnimation->setDuration(mDuration);
	mAnimation->setEndValue(mTo);

	mAnimation->start();
}
