/********************************************************************
	created:	2012-06-27
	created:	16:30
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer\Table.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer
	author:		PangShuo
	description: Global data center
*********************************************************************/

#ifndef DataCenter_h__
#define DataCenter_h__

#include "MyToolkits.h"
#include "SharedData.h"
using namespace SharedData;

class DataCenter : public Singleton<DataCenter>
{
public:
	RoomInfo mRoomInfo;
protected:
private:
};

#define DATACENTER DataCenter::GetSingleton()

#endif // DataCenter_h__