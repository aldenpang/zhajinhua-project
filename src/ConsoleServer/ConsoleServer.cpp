#include "stdafx.h"
#include "NetLayer.h"

#define SERVPORT 5000 /*�����������˿ں� */

int main()
{
	Packet p;

	NetLayer net;
	net.Initialize();

	net.Start(SERVPORT);

	net.Uninitialize();

	return 0;
}