#include <QtCore/QCoreApplication>
#include "AppFrame.h"
#include <QTextCodec>
#include "LogModule.h"

int main(int argc, char *argv[])
{
	LOG.SetModuleName("GS");
	if ( __argc == 1 )
	{
		LOG_D_ERR("wrong input parameters!");
		return 0;
	}

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	QCoreApplication a(argc, argv);

	AppFrame app;
	app.Run();

	return a.exec();
}
