#include "stdafx.h"
#include "Setting.h"

Setting::Setting()
{

}

Setting::~Setting()
{
	SAFE_DELETE(mSettings);
}

void Setting::Init( QString _fileName )
{
	mSettings = new QSettings(_fileName, QSettings::IniFormat);

	mSettings->beginGroup("CommonSettings");
	mIP = mSettings->value("IP").toString();
	mPort = mSettings->value("Port").toUInt();
	mSettings->endGroup();

	mSettings->beginGroup("URLs");
	mLoginADURL = mSettings->value("Login_Ad").toString();
	mLoginNewsURL = mSettings->value("Login_News").toString();
	mSettings->endGroup();
}
