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
signals:
	void SiConnectGS(QString _ip, quint32 _port);

private slots:
	void stRoomClicked(QTreeWidgetItem *_roomItem, int _column);
private:
	QTreeWidget* mTree;
	QVector<RoomInfo> mCurrentRoomInfos;

	void regConnections();
};

#endif // GameList_h__