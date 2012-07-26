/********************************************************************
	created:	 2012/07/26
	created:	 26:7:2012   14:56
	file name:	 GameList.h
	author:		 PangShuo
	description:
*********************************************************************/


#ifndef GameList_h__
#define GameList_h__
#include "SharedData.h"
using namespace SharedData;

class GameList : public QObject
{
	Q_OBJECT
public:
	GameList(QTreeWidget* _tree);
	~GameList();

	void Update(QVector<RoomInfo>& _roomVec);

protected:
private:
	QTreeWidget* mTree;
};

#endif // GameList_h__