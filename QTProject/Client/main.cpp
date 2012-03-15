#include "stdafx.h"
#include "client.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	Client w(argc, argv);
	return w.exec();
}
