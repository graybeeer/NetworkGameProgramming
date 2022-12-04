#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더
#include <stdio.h> // printf(), ...

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#define SERVERPORT 9000
#define NICKBUFSIZE 20

DWORD WINAPI roomServerThread(LPVOID arg);

struct SERVERINFO {
	SOCKET listenSock;
};

class WAITING_ROOM
{
private:

protected:
	WSADATA wsa;
	SOCKET listen_sock;
	struct sockaddr_in serveraddr;


public:
	WAITING_ROOM();
	~WAITING_ROOM();
	int MAKE_ROOM();


};