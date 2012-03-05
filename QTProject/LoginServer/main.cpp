#include <QtCore/QCoreApplication>
#include "AppFrame.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	AppFrame app;
	app.Run();

	return a.exec();
}
