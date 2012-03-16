#include "SettingModule.h"
#include <QSettings>

SettingModule::SettingModule()
{
}

SettingModule::~SettingModule()
{
	SAFE_DELETE(mSettings);
}

void SettingModule::Init( QString _fileName )
{
	mSettings = new QSettings(_fileName, QSettings::IniFormat);

	mSettings->beginGroup("CommonSettings");
	mIP = mSettings->value("IP").toString();
	mPort = mSettings->value("Port").toUInt();
	mSettings->endGroup();
}
