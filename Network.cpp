#include "Network.h"

DWORD WINAPI WaitFiveSecAndStart(LPVOID lpParam)
{
	WAITING_ROOM* ppNetInfo = (WAITING_ROOM*)lpParam;
	Sleep(5000);
	if (ppNetInfo->checkAllReady() == true) {
		ppNetInfo->sendStart();
	}
}

DWORD WINAPI NetworkServerInit(LPVOID lpParam)
{
	WAITING_ROOM* ppNetInfo = (WAITING_ROOM*)lpParam;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	
	SOCKET* listenSockP = ppNetInfo->getListenP();
	*listenSockP = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	bind(*listenSockP, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

	listen(*listenSockP, MAX_PLAYER);
}

DWORD WINAPI NetworkClientInit(LPVOID lpParam)
{
	WAITING_ROOM* ppNetInfo = (WAITING_ROOM*)lpParam;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET* listenSockP = ppNetInfo->getListenP();
	*listenSockP = socket(AF_INET, SOCK_STREAM, 0);
}

DWORD WINAPI roomServerThread(LPVOID lpParam)
{
	return 0;
}

DWORD WINAPI inGameServerThread(LPVOID lpParam)
{
	return 0;
}

WAITING_ROOM::WAITING_ROOM(bool isHost)
{
	for (int i{}; i < 3; ++i) {
		player[i].nickname[0] = "\0";
		player[i].isReady = false;
		player[i].sock = NULL; // CreateThread() 실패한 경우도 NULL
		player[i].num = 0; // 0 - 플레이어 슬롯 비어있음

		sock[i] = (SOCKET)-1; // accept() 실패한 경우도 -1
	}

	listen = (SOCKET)-1; // listen() 실패한 경우도 -1

	if (isHost) {
		HANDLE hthr = CreateThread(NULL, 0, NetworkServerInit, this, 0, NULL);
		CloseHandle(hthr);
	}
	else {
		HANDLE hthr = CreateThread(NULL, 0, NetworkClientInit, this, 0, NULL);
		CloseHandle(hthr);
	}
}

bool WAITING_ROOM::checkReduplication(char* name)
{
	for (int i{}; i < 3; ++i) {
		if (strcmp(player[i].nickname, name) == 0)
			return false;
	}
	return true;
}

void WAITING_ROOM::pressStart()
{
	if (this->checkAllReady() == true) {
		HANDLE hthr = CreateThread(NULL, 0, WaitFiveSecAndStart, this, 0, NULL);
		CloseHandle(hthr);
	}
}

void WAITING_ROOM::receiveData()
{
	char buf[6];
	recv(*getListenP(), buf, 6, 0);
	stringAnalysis(buf);
}

void WAITING_ROOM::stringAnalysis(char* data)
{
	// 문자열 검사
	if (strlen(data) < 3) {
		return;
	}

	char* tmpstr;
	int playerNum = atoi((char*)data[0]);
	char checkEnum[3];

	strncpy(checkEnum, &data[1], 2);

	if (checkEnum == "NN") {
		strncpy(tmpstr, &data[3], 2);
		int namelen = atoi(tmpstr);

		// 닉네임 글자 수 제한
		if (namelen > 14)
			namelen = 14; 

		strncpy(tmpstr, &data[5], namelen);
		strcpy(player[playerNum].nickname, tmpstr);
	}
	else if (checkEnum == "RD") {
		pressReady(atoi((char*)data[3]));
	}
	else if (checkEnum == "RJ") {

	}
	else if (checkEnum == "ST") {

	}
}

SOCKET* WAITING_ROOM::getListenP()
{
	return &listen;
}
//-------------------------------------------------------------------------
INGAME::INGAME()
{

}

void INGAME::sendPress()
{
	char tmpstr[11] = "PU";

	for (int i = 0; i < MAX_BUFFER; ++i) {
		if (keyInfo[VK_LEFT].pressed) {
			strcpy(tmpstr, "PUL");
			send(sock[0], tmpstr, 3, 0);
		}
		if (keyInfo[VK_RIGHT].pressed) {
			strcpy(tmpstr, "PUR");
			send(sock[0], tmpstr, 3, 0);
		}
		if (keyInfo[VK_DOWN].pressed) {
			strcpy(tmpstr, "PUD");
			send(sock[0], tmpstr, 3, 0);
		}		
		if (keyInfo[VK_SPACE].down) {
			strcpy(tmpstr, "PUJ");
			send(sock[0], tmpstr, 3, 0);
		}
		if (keyInfo['Z'].down) {
			strcpy(tmpstr, "PUZ");
			send(sock[0], tmpstr, 3, 0);
		}		
		if (keyInfo['X'].down) {
			strcpy(tmpstr, "PUX");
			send(sock[0], tmpstr, 3, 0);
		}
	}
}

void INGAME::objectInteract()
{
	//Stage.cpp CollisionCheck 손보기
}

void INGAME::sendEnd()
{
	// 수정 필요?
	send(sock[0], "GAMEEND", 8, 0); 
	send(sock[1], "GAMEEND", 8, 0);
	send(sock[2], "GAMEEND", 8, 0);
}
