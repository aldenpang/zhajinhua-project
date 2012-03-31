#include <QtCore/QCoreApplication>
#include "ConsoleClient.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	if ( argc == 1 )
	{
		printf("Wrong parameter!");
		return 0;
	}
	QCoreApplication a(argc, argv);

	ConsoleClient client;

	return a.exec();
}
