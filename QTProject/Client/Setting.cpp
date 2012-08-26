#include "stdafx.h"
#include "Setting.h"

Setting::Setting()
{
	mTempLogin = false;

	initLevels();
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

quint32 Setting::GetLevel( quint32 _exp )
{
	quint32 level = 0;
	for ( int i = 0; i<mLevels.size(); i++ )
	{
		if ( _exp < mLevels[i] )
		{
			level = i+1;
			break;
		}
	}
	return level;
}

void Setting::initLevels()
{
	// the maxmimum level is 60
	for ( int i = 0; i<60; i++ )
	{
		mLevels.push_back(i*i*100+100);
	}
}