#include "SettingModule.h"
#include <QSettings>

SettingModule::SettingModule()
{
	mSettings = new QSettings("config.ini", QSettings::IniFormat);

	mSettings->beginGroup("CommonSettings");
	mIP = mSettings->value("IP").toString();
	mPort = mSettings->value("Port").toUInt();
	mSettings->endGroup();
}

SettingModule::~SettingModule()
{
	SAFE_DELETE(mSettings);
}
