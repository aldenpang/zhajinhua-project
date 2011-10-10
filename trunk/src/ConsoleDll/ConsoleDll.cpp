// ConsoleDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ConsoleDll.h"


SOCKET sockClient;
_declspec(dllexport) int ClientNetLayer_Initialize()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return -1;
	}

	if ( LOBYTE( wsaData.wVersion ) != 1 ||
		HIBYTE( wsaData.wVersion ) != 1 ) {
			WSACleanup( );
			return -1;
	}
	sockClient=socket(AF_INET,SOCK_STREAM,0);

	return 0;
}

//_declspec(dllexport) int ClientNetLayer_Connect_For_Unity3D()
//{
//	ClientNetLayer_Connect("192.168.1.101", 5000);
//
//	return 0;
//}

_declspec(dllexport) int ClientNetLayer_Connect(std::string& _ip, int _port)
{
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(_ip.c_str());
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(_port);

	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	return 0;
}

_declspec(dllexport) int ClientNetLayer_SendPacket(Packet* _pk)
{
	_pk->SetHeader();
	ClientNetLayer_SendData(_pk->GetData(), _pk->GetDataLength());

	return 0;
}


_declspec(dllexport) int ClientNetLayer_SendData(const char* _buffer, int _bufferSize)
{
	if (_buffer == NULL)
		return -1;

	int res = send(sockClient,_buffer, _bufferSize,0);

	return 0;
}

_declspec(dllexport) int ClientNetLayer_Uninitialize()
{
	closesocket(sockClient);
	WSACleanup();

	return 0;
}