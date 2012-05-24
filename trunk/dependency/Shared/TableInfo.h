/********************************************************************
	created:	2012/05/24
	created:	24:5:2012   15:11
	filename: 	d:\My Projects\ZJHClient&Server\dependency\Shared\TableInfo.h
	file path:	d:\My Projects\ZJHClient&Server\dependency\Shared
	file base:	TableInfo
	file ext:	h
	author:		PangShuo
	functionality: 用来在游戏开始时传递table设定
*********************************************************************/


#ifndef TableInfo_h__
#define TableInfo_h__

class TableInfo
{
public:
	int mBaseChip;
	int mTopChip;

	TableInfo()
	{
		mBaseChip = 0;
		mTopChip = 0;
	}
protected:
private:
};

#endif // TableInfo_h__