/********************************************************************
	created:	2012/03/07
	created:	7:3:2012   15:01
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer\TableManager.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer
	file base:	TableManager
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef TableManager_h__
#define TableManager_h__

#include "MyToolkits.h"
#include "ISocketInstance.h"
#include "HardcodeConfig.h"

class Table;
class TableManager : public Singleton<TableManager>
{
public:
	TableManager();
	~TableManager();
signals:
	void SiStartGame(QVector<ISocketInstancePtr>& _players);

public slots:
	// 加入桌子后自动ready，如果达到最低人数，游戏自动开始；如果加入时已经开始，则下一局自动进入
	// returns: 0-successful 1-table full 2-table not exist
	int StJoinTable(ISocketInstancePtr _player, uint _tableID, uint _seatID);

	int StLeaveTable( ISocketInstancePtr _player );
	// returns: 0-successful 1-player not exist 2-table not exist
	int StLeaveTable(ISocketInstancePtr _player, uint _tableID);

	QMap<int, Table*>& GetTables(){ return mTables; }
public:
	void AssignTables(int _amount=MAX_TABLE);

	void SetReadyToStart(quint32 _tableID, quint32 _seatID);
protected:
private:
	QMap<int, Table*> mTables;
};
#define TABLE TableManager::GetSingleton()

#endif // TableManager_h__