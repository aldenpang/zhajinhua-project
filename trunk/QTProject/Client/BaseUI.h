/********************************************************************
	Copyright (C) 2013 by Alden Pang
	@date:	    2012-7-15   15:16
	@file: 		BaseUI.h
	@author:    Alden Pang
	@desc:		Login\Lobby\GameµÄ³éÏó²ã
*********************************************************************/

#ifndef BASEUI_H
#define BASEUI_H

#include <QObject>

class BaseUI : public QObject
{
	Q_OBJECT
public:
	BaseUI(){};
	~BaseUI(){};

	virtual void Init()=0;

	QWidget* GetWidget(){ return mMainWidget; }

public slots:
	void Show(){mMainWidget->show();}
	void Hide(){mMainWidget->hide();}

private slots:


protected:
	QWidget*	mMainWidget;

	virtual void regConnections()=0;


};

#endif