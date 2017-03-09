#include <time.h>
#include "Winsock2.h"
#include "stdio.h"
#pragma comment (lib,"WS2_32.lib")

#define MAXLINE 4096
#define LISTENQ 1024
#define SERVER_PORT 13

int main(int argc, char* argv[])
{
	SOCKET ListenSocket = INVALID_SOCKET, ClientSocket = INVALID_SOCKET;
	int iResult;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	time_t ticks; 
	int iSendResult;
	//��ʼ��Windows Sockets DLL��Э�̰汾��
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	iResult = WSAStartup(wVersionRequested, &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup�������ô��󣬴���ţ�%d\n", WSAGetLastError());
		return -1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("�޷��ҵ����õ�Winsock.dll�汾\n");
		WSACleanup();
		return -1;
	}
	else
		printf("Winsock 2.2 dll��ʼ���ɹ�\n");
	if ((ListenSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket �������ô��󣬴���ţ�%d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);
	iResult = bind(ListenSocket, (struct sockaddr *) &servaddr, sizeof (servaddr));
	if (iResult == SOCKET_ERROR)
	{
		printf("bild �������ô��󣬴���ţ�%d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return -1;
	}
	iResult = listen(ListenSocket, LISTENQ);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen �������ô��󣬴���ţ�%d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return -1;
	}
	for (;;)
	{
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET)
		{
			printf("�������ô��󣬴���ţ�%d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return -1;
		}



		ticks = time(NULL);
		memset(buff, "%.24s\r\n", ctime(&ticks));
		printf("��ȡ��ǰϵͳʱ��: %s\n", buff);
		iSendResult = send(ClientSocket, buff, strlen(buff), 0);
		if (iSendResult == SOCKET_ERROR)
		{
			printf("send �������ô��󣬴����:%d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return -1;
		}
		printf("��ͻ��˷���ʱ��ɹ�\n");
		iResult = shutdown(ClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR)
		{
			printf("shutdown �������ô��󣬴���ţ�%d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return -1;
		}
		closesocket(ClientSocket);
		printf("�����ر�����\n");

	}
	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}
















