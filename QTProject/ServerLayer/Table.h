/********************************************************************
	created:	2012/03/07
	created:	7:3:2012   15:57
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer\Table.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer
	file base:	Table
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef Table_h__
#define Table_h__

#include <QMap>
#include <QVector>
#include "GSPlayer.h"
#include "ISocketInstance.h"
#include "HardcodeConfig.h"

enum TableState
{
	TS_WAITING=0,
	TS_PLAYING,
	TS_BALANCE,

	TS_TOTAL,
};

enum SeatState
{
	SS_EMPTY=0,
	SS_OCCUPY,

	SS_TOTAL,
};

class Table
{
public:
	Table();
	~Table();

	// returns: 0-successful 1-table full
	int Join(int seatID, GSPlayerPtr _player);

	// returns: 0-successful 1-player not exist
	int Leave(GSPlayerPtr _player);

	int PlayerAmount(){return mPlayers.size();}
protected:
private:
	QMap<int, GSPlayerPtr> mPlayers;
	TableState mState;
};

#endif // Table_h__