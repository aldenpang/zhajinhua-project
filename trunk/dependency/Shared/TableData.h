/********************************************************************
	created:	2012/04/16
	created:	16:4:2012   15:44
	filename: 	d:\My Projects\ZJHClient&Server\dependency\Shared\TableData.h
	file path:	d:\My Projects\ZJHClient&Server\dependency\Shared
	file base:	TableData
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef TableData_h__
#define TableData_h__
#include <QMap>
#include "MyToolkits.h"
#include "LogModule.h"
#include "SharedData.h"

#define EMPTY_SEAT ""

struct TablePlayer
{
	PROPERTY(QVector<int>, Pokers);

	quint32 mProtraitID;
	QString mNickName;
	quint32 mTableMoney;
	TablePlayer()
	{
		mTableMoney = 0;
		mProtraitID = 0;
		mNickName = EMPTY_SEAT;
	}
};

class TableData
{
public:
	TableData()
	{
		for ( int i = 0; i <MAX_PLAYER; i++ )
		{
			TablePlayer emptyPlayer;
			mPlayers.insert(i, emptyPlayer);
		}
	}

	PROPERTY(quint32,ID);

	PROPERTY(int, MinBringChip);

	void AddPlayer(quint32 _seatID, TablePlayer _player)
	{
		mPlayers.insert(_seatID, _player);
	}

	bool isSeatOccupied( quint32 _seatID )
	{
		QMap<int, TablePlayer>::iterator itr = mPlayers.find(_seatID);
		if ( itr != mPlayers.end() )
		{
			LOG_D_INFO(QString("[%1] is on seatID[%2]").arg(itr.value().mNickName).arg(itr.key()));
			if ( itr.value().mNickName == QString(EMPTY_SEAT) )
				return false;
			else
				return true;
		}

		LOG_D_ERR(QString("Can't find _seatID[%1] in talble[%2]").arg(_seatID).arg(mID));
		return false;
	}

	QMap<int, TablePlayer>& GetPlayers(){return mPlayers;}
protected:
private:
	QMap<int, TablePlayer> mPlayers;

};

#endif // TableData_h__
