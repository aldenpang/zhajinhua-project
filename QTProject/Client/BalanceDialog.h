/********************************************************************
	Copyright (C) 2013  by Alden Pang
	@date:	    2013-2-1 0:29
	@file: 		BalanceDialog.h
	@author:    Alden Pang
	@desc:		
*********************************************************************/

#ifndef BalanceDialog_h__
#define BalanceDialog_h__
#include "TableInfo.h"

class BalanceDialog : public QObject
{
	Q_OBJECT
public:
	
	BalanceDialog(QWidget* _base);
	~BalanceDialog();

	void SetAndShow( QMap<int, int>& _result, QMap<int, TablePlayer>& _players );
signals:
	void SiBack();
	void SiContinue();
public slots:
	void StHide();
protected:
private:
	QWidget* mMain;
	QMap<int, TablePlayer> mPlayers;

	void regConnections();
};

#endif // BalanceDialog_h__
