#include "Winsock2.h"
#include "stdio.h"
#pragma comment (lib,"WS2_32.lib")
#define MAXLINE 4096
#define SERVER_PORT 13
int main(int argc, char* argv[])
{
	SOCKET ConnectSocket = INVALID_SOCKET;
	int iResult;
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;
	if (argc != 2)
	{
		printf("usage: DayTime <IPaddress>");
		return 0;
	}
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	iResult = WSAStartup(wVersionRequested, &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup 函数调用错误，错误号： %d\n", WSAGetLastError());
		return -1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("无法找到可用的Winsock.dll版本\n");
		WSACleanup();
		return -1;

	}
	else
		printf("Winsock 2.2 dll初始化成功\n");
	if ((ConnectSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket 函数调用错误，错误号：%d\n", WSAGetLastError());
		WSACleanup();
		return -1;

	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	iResult = connect(ConnectSocket, (LPSOCKADDR)&servaddr, sizeof(servaddr));
	if (iResult == SOCKET_ERROR)
	{
		printf("connect 函数调用错误，错误号：%d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return -1;

	}
	memset(&recvline, 0, sizeof(recvline));
	printf("当前时间是：");
	do
	{
		iResult = recv(ConnectSocket, recvline, MAXLINE, 0);
		if (iResult > 0)
			printf("%s", recvline);
		else
		{
			if (iResult == 0)
				printf("对方连接关闭，退出\n");
			else
				printf("recv 函数调用错误，错误号： %d\n", WSAGetLastError());

		}
	} while (iResult > 0);
	closesocket(ConnectSocket);
	WSACleanup();
	return 0;


}

