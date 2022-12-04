#include "Server.h"

DWORD WINAPI roomServerThread(LPVOID arg)
{
	struct SERVERINFO sv_info = *(struct SERVERINFO*)arg;
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;

	while (1) {
		addrlen = sizeof(clientaddr);
		client_sock = accept(sv_info.listenSock, (struct sockaddr*)&clientaddr, &addrlen);


	}
}

WAITING_ROOM::WAITING_ROOM()
{
	WSAStartup(MAKEWORD(2, 2), &wsa);
}

WAITING_ROOM::~WAITING_ROOM()
{
}

int WAITING_ROOM::MAKE_ROOM()
{
	int retval = 0;
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == SOCKET_ERROR)
		return -1;

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		return -1;

	listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		return -1;

	struct SERVERINFO sinfo = { listen_sock };
	CreateThread(NULL, 0, roomServerThread, (LPVOID)&sinfo, 0, NULL);
	return 0;
}
