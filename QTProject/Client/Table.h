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
#include "TableData.h"

class Table : public QObject
{
	Q_OBJECT
signals:
	void SiSit(quint32 _tableID, quint32 _seatID);

public:
	Table();
	~Table();

	PROPERTY(quint32, ID);

	PROPERTY(QWidget*, Widget);

	void Init();

	void UpdatePlayer(quint32 _seatID, TablePlayer _player);
	void UpdateTableInfo(quint32 _tableID, TableData _data);

	void PlayerLeave(TablePlayer _player);

public slots:
	
private slots:
	void stClickSeat();

protected:
private:
	QPushButton*	mSeat[MAX_PLAYER];
	QTextEdit*		mNameLabel[MAX_PLAYER];

	void regConnection();

};

#endif // Table_h__