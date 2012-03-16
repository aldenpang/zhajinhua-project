/********************************************************************
	created:	2012/03/02
	created:	2:3:2012   15:46
	filename: 	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer\SettingModule.h
	file path:	d:\My Projects\ZJHClient&Server\QTProject\ServerLayer
	file base:	SettingModule
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef SettingModule_h__
#define SettingModule_h__
#include <QString>
#include "MyToolkits.h"

class QSettings;
class SettingModule : public Singleton<SettingModule>
{
public:
	SettingModule();
	~SettingModule();
	void Init(QString _fileName);

	PROPERTY(QString, IP);
	PROPERTY(uint, Port);
	

protected:
private:
	QSettings* mSettings;
};

#define SETTINGS SettingModule::GetSingleton()

#endif // SettingModule_h__