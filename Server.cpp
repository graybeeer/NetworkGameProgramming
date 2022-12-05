#include "Server.h"

DWORD WINAPI roomServerThread(LPVOID arg)
{
	WAITING_ROOM wr_server = *(WAITING_ROOM*)arg;
	SOCKET client_sock;
	SOCKET server_sock = wr_server.GetMySock();
	struct sockaddr_in clientaddr;
	int addrlen = sizeof(clientaddr);


	while (1) {
		client_sock = accept(server_sock, (struct sockaddr*)&clientaddr, &addrlen);
		CLIENTINFO cinfo;
		cinfo.sock = client_sock;
		cinfo.wrp = wr_server;
		HANDLE hnd = CreateThread(NULL, 0, roomDataProcessingThread, (LPVOID)&cinfo, 0, NULL);
		CloseHandle(hnd);
	}

	return 0;
}

DWORD WINAPI roomClientThread(LPVOID arg)
{
	WAITING_ROOM wr_server = *(WAITING_ROOM*)arg;

	SOCKET sv_sock = wr_server.GetMySock();
	HWND hDlg = wr_server.GetDlgHandle();
	char recvcode[30];


	int retval = recv(sv_sock, recvcode, NICKBUFSIZE, MSG_WAITALL);
	SetDlgItemTextA(hDlg, IDC_HOSTNAME, recvcode); // IDC_P1NAME + n = IDC_P(n+1)NAME


	while (1) {
		retval = recv(sv_sock, recvcode, 3, MSG_WAITALL);
		if (strcmp(recvcode, "NN") == 0) {
			recv(sv_sock, recvcode, 2, MSG_WAITALL);
			int editnum = atoi(recvcode);
			recv(sv_sock, recvcode, NICKBUFSIZE, MSG_WAITALL);
			wr_server.SetPlayerNickname(editnum, recvcode);
			SetDlgItemTextA(hDlg, IDC_P1NAME+editnum, recvcode); // IDC_P1NAME + n = IDC_P(n+1)NAME
		}
	}

	return 0;
}

DWORD WINAPI roomDataProcessingThread(LPVOID arg)
{
	struct CLIENTINFO cl_info = *(struct CLIENTINFO*)arg;
	short cl_num = cl_info.wrp.FindBlankPlayer();
	if (cl_num == -1) return 0;

	cl_info.wrp.SetPlayerSock(cl_num, cl_info.sock);
	SOCKET cl_sock = cl_info.wrp.GetPlayer(cl_num).sock;
	HWND hDlg = cl_info.wrp.GetDlgHandle();
	char recvcode[30];

	while (1) {
		int retval = recv(cl_sock, recvcode, 4, MSG_WAITALL);
		if (strcmp(recvcode, "NNN") == 0) {
			recv(cl_sock, recvcode, NICKBUFSIZE, MSG_WAITALL);
			cl_info.wrp.SetPlayerNickname(cl_num, recvcode);
			SetDlgItemTextA(hDlg, IDC_P1NAME, recvcode); // IDC_P1NAME + n = IDC_P(n+1)NAME

			// ÀçºÐ¹è
			char nickbuf[NICKBUFSIZE];
			GetDlgItemTextA(hDlg, IDC_EDITNICKNAME, nickbuf, NICKBUFSIZE);
			send(cl_sock, nickbuf, NICKBUFSIZE, 0);
			for (int i{}; i < 3; ++i) { 
				if (strcmp(cl_info.wrp.GetPlayer(i).nickname, "") != 0) {
					char tmpstr[2] = "";
					SOCKET tmpsock = cl_info.wrp.GetPlayer(i).sock;
					_itoa(cl_num, tmpstr, 10);
					send(tmpsock, (char*)"NN", 3, 0);
					send(tmpsock, tmpstr, 2, 0);
					send(tmpsock, recvcode, NICKBUFSIZE, 0);
				}
			}
		}
	}
	closesocket(cl_sock);
	return 0;
}

WAITING_ROOM::WAITING_ROOM()
{
	WSAStartup(MAKEWORD(2, 2), &wsa);
}

WAITING_ROOM::~WAITING_ROOM()
{
}

WAITING_ROOM::WAITING_ROOM(const WAITING_ROOM& wr)
{
	wsa = wr.wsa;
	my_sock = wr.my_sock;
	serveraddr = wr.serveraddr;
	DlgHandle = wr.DlgHandle;
	for (int i{}; i < 3; ++i) {
		player[i].isReady = wr.player[i].isReady;
		strcpy(player[i].nickname, wr.player[i].nickname);
		player[i].sock = wr.player[i].sock;
	}
}

int WAITING_ROOM::MAKE_ROOM(HWND dlg)
{
	DlgHandle = dlg;
	int retval = 0;
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock == SOCKET_ERROR)
		return -1;

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(my_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		return -1;

	listen(my_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		return -1;

	HANDLE hnd = CreateThread(NULL, 0, roomServerThread, (LPVOID)this, 0, NULL);
	CloseHandle(hnd);
	return 0;
}

int WAITING_ROOM::CONNECT_ROOM(HWND dlg, char* serverip, char* name)
{
	DlgHandle = dlg;
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock == INVALID_SOCKET)
		return -2;

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, serverip, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	if (connect(my_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
		return -2;

	send(my_sock, (char*)"NNN", 4, 0);
	send(my_sock, name, NICKBUFSIZE, 0);

	HANDLE hnd = CreateThread(NULL, 0, roomClientThread, (LPVOID)this, 0, NULL);
	CloseHandle(hnd);

	return 0;
}

int WAITING_ROOM::FindBlankPlayer()
{
	for (int i{}; i < 3; ++i) {
		if (strcmp(player[i].nickname, "") == 0)
			return i;
	}
	return -1;
}

PlayerInfo WAITING_ROOM::GetPlayer(int n)
{
	if (n > -1 && n < 3 && player[n].sock != INVALID_SOCKET)
		return player[n];
	return PlayerInfo();
}

void WAITING_ROOM::SetPlayerSock(int n, SOCKET sock)
{
	if (n > -1 && n < 3) {
		player[n].sock = sock;
	}
}

void WAITING_ROOM::SetPlayerNickname(int n, char* name)
{
	if (n > -1 && n < 3) {
		strcpy(player[n].nickname, name);
	}
}

SOCKET WAITING_ROOM::GetMySock()
{
	return my_sock;
}

HWND WAITING_ROOM::GetDlgHandle()
{
	return DlgHandle;
}
