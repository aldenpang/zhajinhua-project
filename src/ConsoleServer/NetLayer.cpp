#include "stdafx.h"
#include "NetLayer.h"

#define BACKLOG 10 /* ���ͬʱ���������� */

NetLayer::NetLayer()
{
	memset(mBuffer, 0, MAX_PACKET_SIZE);
}

NetLayer::~NetLayer()
{
}

int NetLayer::Initialize()
{
	WSAData wsadata;
	if ( WSAStartup(MAKEWORD(2, 2), &wsadata) == SOCKET_ERROR )
	{
		int error = WSAGetLastError();
		printf("socket��ʼ������WSAError:%d", error); 
		return -1;
	}

	return 0;
}

int NetLayer::Start( const int _port )
{
	SOCKET sockfd,client_fd; /*sock_fd������socket��client_fd�����ݴ���socket */
	struct sockaddr_in my_addr; /* ������ַ��Ϣ */
	struct sockaddr_in remote_addr; /* �ͻ��˵�ַ��Ϣ */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		int error = WSAGetLastError();
		printf("socket��������WSAError:%d", error); 
		return -1;
	}

	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(_port);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(my_addr.sin_zero, 0, 8);
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) 
	{
		printf("bind����");
		return -1;
	}
	if (listen(sockfd, BACKLOG) == -1) 
	{
		printf("listen����");
		return -1;
	}
	int sin_size = 0;
	while(1) 
	{
		sin_size = sizeof(struct sockaddr_in);
		if ((client_fd = accept(sockfd, (struct sockaddr *)&remote_addr, &sin_size)) == -1) 
		{
			printf("accept����");
			continue;
		}
		printf("received a connection from %s ", inet_ntoa(remote_addr.sin_addr));

		int preReceivedLength = recv(client_fd, mBuffer, MAX_PACKET_SIZE, 0);

		if (preReceivedLength < 0) 
		{
			std::cout << "recv() failed.";
		} 
		else if (preReceivedLength == 0) 
		{
			std::cout << "Client has been disconnected.\n";
		}

		printf("received data:%s data size:%d\n", mBuffer, preReceivedLength);

		Packet pkt;
		pkt.SetData(mBuffer);
		if( pkt.IsTokenValid() )
		{
			parseMessages(pkt);		
		}
		else
		{
			printf("packet from %s is invalid", inet_ntoa(remote_addr.sin_addr));
		}

		//if (send(client_fd, "Hello, you are connected! ", 26, 0) == -1)
		//{
		//	printf("send����");
		//	closesocket(client_fd);
		//	return -1;
		//}
		closesocket(client_fd);

	}

}

int NetLayer::Uninitialize()
{
	// release wsa
	if ( WSACleanup() == SOCKET_ERROR )
	{
		int error = WSAGetLastError();
		printf("socket�ͷų���WSAError:%d", error); 
		return -1;
	}
	return 0;
}

int NetLayer::parseMessages(Packet& _packet)
{
	int message = _packet.GetMessage();
	switch(message)
	{
	case SharedData::MSG_TEST:
		break;
	case SharedData::MSG_TEST2:
		break;
	default:
		printf("unknow message");
		break;
	}

	return 0;	
}
