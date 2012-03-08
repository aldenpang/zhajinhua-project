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

#include <QVector>
#include "ISocketInstance.h"
#include "HardcodeConfig.h"

enum TableState
{
	TS_WAITING=0,
	TS_PLAYING,
	TS_BALANCE,

	TS_TOTAL,
};

class Table
{
public:
	Table();
	~Table();

	// returns: 0-successful 1-table full
	int Join(ISocketInstancePtr _player);

	// returns: 0-successful 1-player not exist
	int Leave(ISocketInstancePtr _player);

	int PlayerAmount(){return mPlayers.size();}
protected:
private:
	QSet<ISocketInstancePtr> mPlayers;
};

#endif // Table_h__