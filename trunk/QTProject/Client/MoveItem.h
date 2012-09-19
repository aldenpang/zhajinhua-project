/********************************************************************
	created:	 2012/08/06
	created:	 6:8:2012   15:10
	file name:	 MoveItem.h
	author:		 PangSHuo
	description:
*********************************************************************/


#ifndef MoveItem_h__
#define MoveItem_h__

#include <QGraphicsObject>
#include <QTimer>

class QPropertyAnimation;
class MoveItem : public QGraphicsObject
{
Q_OBJECT
signals:
public:
	MoveItem(const QString &_filename);
	~MoveItem();

	void Move(quint32 _delay, quint32 _duration, QPoint _to);

	 virtual QRectF boundingRect() const;

protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	
	void setPixmap(const QString &_filename){ mPixmap = QPixmap(_filename); }

private slots:
	void stStart();
private:
	QPixmap mPixmap;
	QTimer* mDelayTimer;
	quint32 mDuration;
	QPoint mTo;
	QPropertyAnimation *mAnimation;

};

#endif // MoveItem_h__