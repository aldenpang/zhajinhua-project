#include "stdafx.h"
#include "NetLayer.h"

#define SERVPORT 5000 /*·şÎñÆ÷¼àÌı¶Ë¿ÚºÅ */

int main()
{
	NetLayer net;
	net.Initialize();

	net.Start(SERVPORT);

	net.Uninitialize();

	return 0;
}