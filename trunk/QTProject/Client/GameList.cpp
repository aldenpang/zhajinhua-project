#include "stdafx.h"
#include "GameList.h"

GameList::GameList( QTreeWidget* _tree )
: mTree(_tree)
{
	//QTreeWidgetItem* item = new QTreeWidgetItem();
	//item->setText(0, "test");

	//QTreeWidgetItem* item2 = new QTreeWidgetItem(item);
	//item2->setIcon(0, QIcon(":/Images/Media/nobodyImage.png"));
	//item2->setText(0, "test2");

	//mTree->insertTopLevelItem(0, item);
	//mTree->insertTopLevelItem(0, item2);

	regConnections();
}

GameList::~GameList()
{

}

void GameList::Update( QVector<RoomInfo>& _roomVec )
{
	mTree->clear();

	mCurrentRoomInfos = _roomVec;

	QTreeWidgetItem* zjhRoot0 = new QTreeWidgetItem();
	zjhRoot0->setText(0, "zjhRootGold");

	QTreeWidgetItem* zjhRoot1 = new QTreeWidgetItem();
	zjhRoot1->setText(0, "zjhRootSilver");

	foreach ( RoomInfo r, mCurrentRoomInfos )
	{
		if ( r.mType == ZJH )
		{
			QVariant var;
			var.setValue(r);
			if ( r.mMoneyType == SILVER_COIN )
			{
				QTreeWidgetItem* room = new QTreeWidgetItem(zjhRoot1);
				room->setIcon(0, QIcon(":/Images/Media/silverCoin.png"));
				room->setText(0, r.mName);
				room->setData(0, Qt::UserRole, var);
			}
			else if ( r.mMoneyType == GOLD_COIN )
			{
				QTreeWidgetItem* room = new QTreeWidgetItem(zjhRoot0);
				room->setIcon(0, QIcon(":/Images/Media/goldCoin.png"));
				room->setText(0, r.mName);
				room->setData(0, Qt::UserRole, var);
			}
		}
	}


	mTree->insertTopLevelItem(0, zjhRoot0);
	mTree->insertTopLevelItem(1, zjhRoot1);

	mTree->expandAll();
}

void GameList::regConnections()
{
	connect(mTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(stRoomClicked(QTreeWidgetItem *, int)));
}

void GameList::stRoomClicked( QTreeWidgetItem *_roomItem, int _column )
{
	RoomInfo roomInfo = _roomItem->data(0, Qt::UserRole).value<RoomInfo>();
	//int sss = mCurrentRoomInfos.size();

	emit SiConnectGS(roomInfo);
	return;
}
