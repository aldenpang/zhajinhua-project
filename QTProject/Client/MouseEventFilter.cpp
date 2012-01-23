#include "stdafx.h"
#include "MouseEventFilter.h"
const int gSpacing = 2;
//------------------------------------------------------------------------------
MouseEventFilter::MouseEventFilter( QWidget* _wacthedWidget /*= 0*/, QWidget* _disWidget/* = 0*/, int _style /*= 0*/ )
: QObject(_disWidget)
, mIsMouseDown(false)
, mStyle(_style)
, mIsArrowInLeft(false)
, mIsArrowInRight(false)
, mIsArrowInBottom(false)
{
	if (_wacthedWidget)
	{
		_wacthedWidget->installEventFilter(this);
	}
}
//------------------------------------------------------------------------------
bool MouseEventFilter::eventFilter( QObject *obj, QEvent *event )
{
	QWidget* disWidget = qobject_cast<QWidget *>(parent());
	if (!disWidget)
		return false;

	QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

	if (event->type() == QEvent::MouseButtonPress) 
	{
		if ( mouseEvent->button() == Qt::LeftButton )
		{
			mIsMouseDown = true;
			mOffsetPos = mouseEvent->pos();
			return true;
		}
		return false;
	} 
	else if (event->type() == QEvent::MouseMove)
	{
		if (mIsMouseDown)
		{	
			if (mStyle & Enable_Scale)
			{
				int x = mouseEvent->x();
				int y = mouseEvent->y();
				if (mIsMouseDown && (mIsArrowInLeft || mIsArrowInRight || mIsArrowInBottom))
				{
					// 缩放窗口
					int disX = mouseEvent->x() - mOffsetPos.x();
					int disY = mouseEvent->y() - mOffsetPos.y();
					QRect rect = disWidget->geometry();
					if (mIsArrowInLeft)
						rect.setLeft(rect.left() + disX);
					if(mIsArrowInRight)
						rect.setRight(rect.right() + disX);
					if(mIsArrowInBottom)
						rect.setBottom(rect.bottom() + disY);

					disWidget->setGeometry(rect);
					mOffsetPos = QPoint(!mIsArrowInLeft ? mouseEvent->x() : mOffsetPos.x(),mouseEvent->y());
				}
			}
			if ((mStyle & Enable_Drag) && !(mIsArrowInLeft || mIsArrowInRight || mIsArrowInBottom))
			{
				disWidget->move(mouseEvent->globalPos() - mOffsetPos);
			}
			return true;
		}
		return false;
	}
	else if(event->type() == QEvent::HoverMove)
	{
		if(mIsMouseDown)
			return false;
		if (mStyle & Enable_Scale)
		{
			int x = mouseEvent->x();
			int y = mouseEvent->y();
			if (!mIsMouseDown)
			{
				//设置鼠标指针
				QRect rect = disWidget->rect();
				//mIsArrowInLeft = (qAbs(y - rect.left()) <= 5);
				mIsArrowInLeft = false;
				mIsArrowInRight = (qAbs(y - rect.right()) <= gSpacing);
				mIsArrowInBottom = (qAbs(x - rect.bottom()) <= gSpacing);

				bool isHor = mIsArrowInLeft | mIsArrowInRight;
				if (isHor && mIsArrowInBottom)
				{
					if (mIsArrowInLeft)
						disWidget->setCursor(Qt::SizeBDiagCursor);
					else
						disWidget->setCursor(Qt::SizeFDiagCursor);
				} 
				else if(isHor)
				{
					disWidget->setCursor(Qt::SizeHorCursor);
				}
				else if(mIsArrowInBottom)
				{
					disWidget->setCursor(Qt::SizeVerCursor);
				}
				else
				{
					if(disWidget->cursor().shape ()  != Qt::PointingHandCursor)
						disWidget->setCursor(Qt::ArrowCursor);
				}
			}
			return true;
		}
		return false;
	}
	else if (event->type() == QEvent::MouseButtonRelease)
	{
		if (mStyle & Enable_Scale)
		{
			mIsArrowInLeft = mIsArrowInRight = mIsArrowInBottom = false;
			disWidget->setCursor(Qt::ArrowCursor);
		}
		mIsMouseDown = false;
		return true;
	}
	else if(event->type() == QEvent::HoverLeave)
	{
		mIsMouseDown = false;
		return true;
	}
	// standard event processing 
	return QObject::eventFilter(obj, event);
}
//------------------------------------------------------------------------------
void MouseEventFilter::SetStyle(int _style)
{
	mStyle = _style;
}
//------------------------------------------------------------------------------
int MouseEventFilter::GetStyle()
{
	return mStyle;
}
