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

class TableData
{
public:
	TableData()
	{

	}

	PROPERTY(quint32,ID);

	void AddPlayer(quint32 _seatID, QString& _nickName)
	{
		mPlayers.insert(_seatID, _nickName);
	}

	bool isSeatOccupied( quint32 _seatID )
	{
		QMap<int, QString>::iterator itr = mPlayers.find(_seatID);
		if ( itr != mPlayers.end() )
		{
			if ( itr.value() == QString("EmptySeat") )
				return false;
			else
				return true;
		}

		LOG_D_ERR(QString("Can't find _seatID[%1] in talble[%2]").arg(_seatID).arg(mID));
		return false;
	}

	QMap<int, QString>& GetPlayers(){return mPlayers;}
protected:
private:
	QMap<int, QString> mPlayers;

};

#endif // TableData_h__