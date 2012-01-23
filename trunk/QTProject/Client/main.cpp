#include "stdafx.h"
#include "client.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	Client w(argc, argv);
	return w.exec();
}
