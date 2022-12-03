#pragma once
#include "stdafx.h"

char* SERVERIP = (char*)"175.211.236.143";
#define SERVERPORT 9000
#define BUFSIZE    50

int clientConnect()
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		return 1;

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		return 1;
	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE];
	const char* testdata[] = {
		"안녕하세요",
		"반가워요",
		"오늘따라 할 이야기가 많을 것 같네요",
		"저도 그렇네요",
	};

	// 서버와 데이터 통신
	for (int i = 0; i < 4; i++) {
		// 데이터 입력(시뮬레이션)
		memset(buf, '#', sizeof(buf));
		strncpy(buf, testdata[i], strlen(testdata[i]));

		// 데이터 보내기
		retval = send(sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			return 1;
			break;
		}
		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);
	}

	// 소켓 닫기
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}