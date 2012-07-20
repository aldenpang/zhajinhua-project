#include "stdafx.h"
#include "client.h"
#include <QtGui/QApplication>
#include "LogModule.h"

bool copyAndRun()
{
	// copy self to a temp exe file, then run it. In order to patch self
	QString tempPath = QDir::tempPath() + QString("/Client_XL.exe");
	if ( QFile::exists(tempPath) )
	{
		QFile::remove(tempPath);
	}
	if(!QFile::copy("Client.exe", tempPath))
	{
		LOG_D_WARN("copy exe file failed!");
	}
	QStringList arg;
	arg<<"XL";		// used for distinguish user clicked exe file or auto run exe file
	QString workDir = QDir::currentPath();
	if ( !QProcess::startDetached(tempPath, arg, workDir) )
	{
		LOG_D_ERR("run copied exe file failed!");
		return false;
	}
	if ( __argc == 1 )	// this is user clicked exe file, need quit immediately
	{
		return false;
	}

	return true;
}
int main(int argc, char *argv[])
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	// init log module
	LOG.SetModuleName(QString("Client"));

#ifdef NDEBUG		// only run in release mode
	if ( !copyAndRun() )
		return 0;
#endif

	Client w(argc, argv);
	return w.exec();
}
