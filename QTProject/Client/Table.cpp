#include "stdafx.h"
#include "Table.h"
#include "LogModule.h"

Table::Table()
// : QObject(NULL)
: mID(0)
{

}

Table::~Table()
{

}

void Table::Init()
{
	QLabel* tableID = mWidget->findChild<QLabel*>("tableID");
	tableID->setText(QString("%1").arg(mID+1));

	mSeat[0] = mWidget->findChild<QPushButton*>("seat0");
	mSeat[1] = mWidget->findChild<QPushButton*>("seat1");
	mSeat[2] = mWidget->findChild<QPushButton*>("seat2");
	mSeat[3] = mWidget->findChild<QPushButton*>("seat3");

	mNameLabel[0] = mWidget->findChild<QTextEdit*>("name0");
	mNameLabel[1] = mWidget->findChild<QTextEdit*>("name1");
	mNameLabel[2] = mWidget->findChild<QTextEdit*>("name2");
	mNameLabel[3] = mWidget->findChild<QTextEdit*>("name3");


	regConnection();
}

void Table::StSit( quint32 _tableID, TableData _data)
{
	if ( _tableID != mID )
	{
		LOG_D_ERR(QString("Wrong TableID[%1], self id is[%2]").arg(_tableID).arg(mID));
		return;
	}

	QMap<int, TablePlayer> players = _data.GetPlayers();
	QMap<int, TablePlayer>::iterator itr;
	for( itr = players.begin(); itr != players.end(); itr++ )
	{
		int seatID = itr.key();
		if ( seatID < 0 || seatID >3 )
		{
			LOG_D_ERR(QString("Invaild seatID[%1]").arg(seatID));
			break;
		}
		if ( itr.value().mNickName != EMPTY_SEAT )
		{
			mNameLabel[seatID]->setText(itr.value().mNickName);
		}
	}

}

void Table::regConnection()
{
	for ( int i = 0; i<MAX_PLAYER; i++ )
		connect(mSeat[i], SIGNAL(clicked()), this, SLOT(stClickSeat()));
}

void Table::stClickSeat()
{
	int seatID = -1;
	for ( int i = 0; i<MAX_PLAYER; i++)
	{
		if ( sender() == mSeat[i] )
		{
			seatID = i;
			break;
		}
	}
	if ( seatID == -1 )
	{
		LOG_D_ERR(QString("SeatID[%1] is not found").arg(seatID));
		return;
	}
	else
	{
		LOG_D_INFO(QString("Player click on seatID[%1]").arg(seatID));
		emit SiSit(mID, seatID);
	}
}
