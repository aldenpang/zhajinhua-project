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

}

GameList::~GameList()
{

}

void GameList::Update( QVector<RoomInfo>& _roomVec )
{
	mTree->clear();

	QTreeWidgetItem* zjhRoot0 = new QTreeWidgetItem();
	zjhRoot0->setText(0, "zjhRootGold");

	QTreeWidgetItem* zjhRoot1 = new QTreeWidgetItem();
	zjhRoot1->setText(0, "zjhRootSilver");

	foreach ( RoomInfo r, _roomVec )
	{
		if ( r.mType == ZJH )
		{
			if ( r.mMoneyType == SILVER_COIN )
			{
				QTreeWidgetItem* room = new QTreeWidgetItem(zjhRoot1);
				room->setText(0, r.mName);
			}
			else if ( r.mMoneyType == GOLD_COIN )
			{
				QTreeWidgetItem* room = new QTreeWidgetItem(zjhRoot0);
				room->setText(0, r.mName);
			}
		}
	}


	mTree->insertTopLevelItem(0, zjhRoot0);
	mTree->insertTopLevelItem(1, zjhRoot1);

	mTree->expandAll();
}
