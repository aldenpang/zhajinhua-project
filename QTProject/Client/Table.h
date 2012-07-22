/********************************************************************
	Copyright (C) 2012 by Pang Shuo
	@date:	    22:7:2012   9:57
	@file: 		Table.h
	@author:    Pang Shuo
	@desc:		
*********************************************************************/

#ifndef Table_h__
#define Table_h__

#include <QObject>
#include "MyToolkits.h"

class Table : public QObject
{
	Q_OBJECT
signals:
	void SiSit(quint32 _tableID, quint32 _seatID, quint32 _accountID);

public:
	Table();
	~Table();

	PROPERTY(quint32, ID);

	PROPERTY(QWidget*, Widget);

	void Init();

public slots:
	void StSit(quint32 _tableID, quint32 _seatID, quint32 _accountID);

protected:
private:

};

#endif // Table_h__