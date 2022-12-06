#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더
#include <stdio.h> // printf(), ...
#include "resource.h"

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#define SERVERPORT 4444
#define NICKBUFSIZE 20

DWORD WINAPI roomServerThread(LPVOID arg);
DWORD WINAPI roomClientThread(LPVOID arg);
DWORD WINAPI roomDataProcessingThread(LPVOID arg);
DWORD WINAPI roomDataResendThread(LPVOID arg);


typedef struct PLAYERINFO {
	char nickname[NICKBUFSIZE] = "";
	bool isReady = false;
	SOCKET sock = INVALID_SOCKET;
}PlayerInfo;

class WAITING_ROOM
{
public:
	WAITING_ROOM();
	~WAITING_ROOM();
	WAITING_ROOM(const WAITING_ROOM& wr);
	int MAKE_ROOM(HWND dlg);
	int CONNECT_ROOM(HWND dlg, char* serverip, char* name);

	int FindBlankPlayer();

	PlayerInfo	GetPlayer(int n);
	void		SetPlayerSock(int n, SOCKET sock);
	void		SetPlayerNickname(int n, char* name);
	SOCKET		GetMySock();
	HWND		GetDlgHandle();


private:
	PlayerInfo player[3];
	WSADATA wsa;
	SOCKET my_sock;
	struct sockaddr_in serveraddr;
	HWND DlgHandle;
	
};

struct CLIENTINFO {
	SOCKET sock;
	HWND dlg;
	short num;
};