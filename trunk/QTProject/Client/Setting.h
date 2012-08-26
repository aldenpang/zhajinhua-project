/********************************************************************
	created:	 2012/07/20
	created:	 20:7:2012   14:54
	file name:	 Setting.h
	author:		 PangShuo
	description:
*********************************************************************/


#ifndef Setting_h__
#define Setting_h__

#include <QString>
#include "SharedData.h"
#include "MyToolkits.h"
#include "CommonPlayer.h"

class QSettings;
class Setting : public Singleton<Setting>
{
public:
	Setting();
	~Setting();
	void Init(QString _fileName);

	PROPERTY(QString, IP);
	PROPERTY(uint, Port);
	PROPERTY(QString, LoginADURL);
	PROPERTY(QString, LoginNewsURL);
	PROPERTY(RoomInfo, RoomInfo);
	PROPERTY(CommonPlayer, Player);
	PROPERTY(bool, TempLogin);

	quint32 GetLevel( quint32 _exp );

protected:
private:
	QSettings* mSettings;
	QVector<quint32> mLevels;

	void initLevels();
};

#define SETTINGS Setting::GetSingleton()

#endif // Setting_h__
