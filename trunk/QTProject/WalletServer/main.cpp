#include <QtCore/QCoreApplication>
#include "AppFrame.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	QCoreApplication a(argc, argv);

	AppFrame app;
	app.Run();

	return a.exec();
}
