#include "stdafx.h"
#include "NetLayer.h"

#define SERVPORT 5000 /*服务器监听端口号 */

int main()
{
	//// init wsa
	//WSAData wsadata;
	//if ( WSAStartup(MAKEWORD(2, 2), &wsadata) == SOCKET_ERROR )
	//{
	//	int error = WSAGetLastError();
	//	printf("socket初始化出错！WSAError:%d", error); 
	//	exit(1);
	//}

	//SOCKET sockfd,client_fd; /*sock_fd：监听socket；client_fd：数据传输socket */
	//struct sockaddr_in my_addr; /* 本机地址信息 */
	//struct sockaddr_in remote_addr; /* 客户端地址信息 */
	//if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	//{
	//	int error = WSAGetLastError();
	//	printf("socket创建出错！WSAError:%d", error); 
	//	exit(1);
	//}

	//my_addr.sin_family=AF_INET;
	//my_addr.sin_port=htons(SERVPORT);
	//my_addr.sin_addr.s_addr = INADDR_ANY;
	//memset(my_addr.sin_zero, 0, 8);
	//if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) 
	//{
	//	printf("bind出错！");
	//	exit(1);
	//}
	//if (listen(sockfd, BACKLOG) == -1) 
	//{
	//	printf("listen出错！");
	//	exit(1);
	//}
	//int sin_size = 0;
	//while(1) 
	//{
	//	sin_size = sizeof(struct sockaddr_in);
	//	if ((client_fd = accept(sockfd, (struct sockaddr *)&remote_addr, &sin_size)) == -1) 
	//	{
	//		printf("accept出错");
	//		continue;
	//	}
	//	printf("received a connection from %s ", inet_ntoa(remote_addr.sin_addr));

	//	if (send(client_fd, "Hello, you are connected! ", 26, 0) == -1)
	//	{
	//		printf("send出错！");
	//		closesocket(client_fd);
	//		exit(0);
	//	}
	//	closesocket(client_fd);

	//}

	//// release wsa
	//if ( WSACleanup() == SOCKET_ERROR )
	//{
	//	int error = WSAGetLastError();
	//	printf("socket释放出错！WSAError:%d", error); 
	//	exit(1);
	//}

	NetLayer net;
	net.Initialize();

	net.Start(SERVPORT);

	net.Uninitialize();

	return 0;
}