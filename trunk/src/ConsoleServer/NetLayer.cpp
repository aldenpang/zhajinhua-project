#include "stdafx.h"
#include "NetLayer.h"

#define BACKLOG 10 /* ���ͬʱ���������� */

pthread_mutex_t mMutex;

void *NetLayer::ReceiveMessageMT(void* _arg)
{
	NetLayer* netLayer = (NetLayer*)_arg;

	while(1)
	{
		Sleep(LOOP_INTERVAL);
		if ( netLayer->mClientList.size() == 0 )
			continue;

		for ( size_t i = 0; i<netLayer->mClientList.size(); i++ )
		{
			pthread_mutex_lock(&mMutex);
			SOCKET socket = netLayer->mClientList[i];
			pthread_mutex_unlock(&mMutex);

			char bufffer[MAX_PACKET_SIZE]={0};
			int preReceivedLength = recv(socket, bufffer, MAX_PACKET_SIZE, 0);

			if (preReceivedLength < 0) 
			{
				std::cout << "recv() failed.\n";
				netLayer->removeSocket(socket);
				continue;
			} 
			else if (preReceivedLength == 0) 
			{
				std::cout << "Client has been disconnected.\n";
				netLayer->removeSocket(socket);
				continue;
			}

			printf("received data:%s data size:%d\n", bufffer, preReceivedLength);

			Packet pkt;
			pkt.SetData(bufffer);
			if( pkt.IsTokenValid() )
			{
				netLayer->parseMessages(pkt);		
			}
			else
			{
				//printf("packet from %s is invalid\n", inet_ntoa(remote_addr.sin_addr));
			}
		}
	}

	return NULL;
}

void *NetLayer::AcceptSocketMT(void* _arg)
{
	int sin_size = 0;
	SOCKET client_fd;
	SOCKET sockfd = *((SOCKET*)_arg);
	struct sockaddr_in remote_addr; /* �ͻ��˵�ַ��Ϣ */
	while(1) 
	{
		sin_size = sizeof(struct sockaddr_in);
		if ((client_fd = accept(sockfd, (struct sockaddr *)&remote_addr, &sin_size)) == -1) 
		{
			printf("accept����");
			continue;
		}
		printf("received a connection from %s:%d socket:%d\n", inet_ntoa(remote_addr.sin_addr), remote_addr.sin_port, client_fd);

		pthread_mutex_lock(&mMutex);
		NetLayer::mClientList.push_back(client_fd);
		printf("socket:%d\n", client_fd);
		pthread_mutex_unlock(&mMutex);

		Sleep(LOOP_INTERVAL);
	}
}

NetLayer::NetLayer()
{
	pthread_mutex_init(&mMutex, NULL);

	pthread_t thread;
	pthread_create(&thread, NULL, ReceiveMessageMT, this);
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
	SOCKET sockfd; /*sock_fd������socket */
	struct sockaddr_in my_addr; /* ������ַ��Ϣ */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		int error = WSAGetLastError();
		printf("socket��������WSAError:%d", error); 
		return -1;
	}

	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(_port);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	//my_addr.sin_addr.s_addr = inet_addr("192.168.2.2");
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
	printf("Initialize done(%s:%d), waiting for connections...\n", inet_ntoa(my_addr.sin_addr), _port);

	pthread_t thread;
	pthread_create(&thread, NULL, AcceptSocketMT, &sockfd);

	while(1) // to keep this program running
	{
		printf("idle looping...\n");
		Sleep(LOOP_INTERVAL);
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
		printf("Received MSG_TEST");
		break;
	case SharedData::MSG_TEST2:
		printf("Received MSG_TEST2");
		break;
	default:
		printf("unknow message is: %d\n", message);
		break;
	}

	return 0;	
}

void NetLayer::removeSocket( SOCKET _socket )
{
	std::vector<SOCKET>::iterator itr = mClientList.begin();
	for ( size_t i = 0; i<mClientList.size(); i++, itr++ )
	{
		if ( _socket == mClientList[i] )
		{
			mClientList.erase(itr);
			break;
		}
	}
	return;
}