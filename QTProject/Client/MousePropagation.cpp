#include "stdafx.h"
#include "MousePropagation.h"
//------------------------------------------------------------------------------
MousePropagation::MousePropagation(QObject* _parent)
: mEnable(true)
{
	if (_parent)
	{
		_parent->installEventFilter(this);
	}
}
//------------------------------------------------------------------------------
void MousePropagation::Enable()
{
	mEnable = true;
}
//------------------------------------------------------------------------------
void MousePropagation::Disable()
{
	mEnable = false;
}
//------------------------------------------------------------------------------
bool MousePropagation::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonPress) 
	{
		if (mEnable)
			return false;
		else
			return true;
	} 
	else if (event->type() == QEvent::MouseMove)
	{
		if (mEnable)
			return false;
		else
			return true;
	}
	else if(event->type() == QEvent::HoverMove)
	{
		if (mEnable)
			return false;
		else
			return true;
	}
	else if (event->type() == QEvent::MouseButtonRelease)
	{
		if (mEnable)
			return false;
		else
			return true;
	}
	else if(event->type() == QEvent::HoverLeave)
	{
		if (mEnable)
			return false;
		else
			return true;
	}
	// standard event processing 
	return QObject::eventFilter(obj, event);
}
