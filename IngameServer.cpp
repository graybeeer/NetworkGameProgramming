#include "stdafx.h"
#include "IngameServer.h"
#include "GameFrame.h"
DWORD WINAPI inGameServerReceiveThread(LPVOID lpParam)
{
	/// <summary>
	/// 인게임에서 서버가 클라이언트로부터 메시지 받는 쓰레드
	/// </summary>
	/// <param name="lpParam"></param>
	/// <returns></returns>
	INGAME_SERVER IS;
	int sockPlayerNum = (int)lpParam; //이 스레드에서 연결중인 클라이언트의 번호
	int retval; //받기 리턴 값
	char rcrBuf[2048]{}; //고정길이
	int rcrLen; // 고정 길이 데이터
	while (1)
	{
		//고정길이 받기
		retval = recv(IS.getPlayer(sockPlayerNum).playerSock, (char*)&rcrLen, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			// 실패시 게임 종료
			return 0;
		}
		else if (retval == 0)
			break;
		/*
		//가변 데이터 받기
		retval = recv(IS.getPlayer(sockPlayerNum).playerSock, rcrBuf, rcrLen, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			// 실패시 게임종료
			return 0;
		}
		else if (retval == 0)
			break;
		*/
		//IS.stringAnalysis(rcrBuf);
		strcpy(IS.getPlayer(sockPlayerNum).temp, rcrBuf); // 각 클라이언트의 입력값 임시저장.
		
	}
}

DWORD WINAPI inGameDataProcessingThread(LPVOID lpParam) // 클라이언트에게 전달받은 정보를 적용하는 쓰레드
{
	int sockPlayerNum = (int)lpParam; //이 스레드에서 연결중인 클라이언트의 번호
	int retval; //받기 리턴 값
	char rcrBuf[2048]; //고정길이
	int rcrLen; // 고정 길이 데이터

	INGAME_SERVER* ppNetInfo = (INGAME_SERVER*)lpParam;

	
	ppNetInfo->inputManagement();
	ppNetInfo->objectInteract();
}

DWORD WINAPI inGameServerSendThread(LPVOID lpParam) // 데이터 처리를 끝낸후 클라이언트로 전달하는 쓰레드
{
	
}



void INGAME_SERVER::inputManagement()
{
	
}

void INGAME_SERVER::objectInteract()
{
}

void INGAME_SERVER::sendState(int connetPlayerNum)
{
	
	/// <summary>
	/// [서버] 플레이어 정보, 오브젝트 정보를 송신하는 함수
	/// </summary>
	/// <param name="connetPlayerNum">연결된 플레이어 번호</param>
	POINT pt; //포인트 임시 값
	char rcrBuf[2048]{}; //고정길이
	int rcrLen; // 고정 길이 데이터
	int retval;

	//서버 플레이어 위치 전달
	pt = serverPlayer.GetPlayerPt(); //플레이어 위치 가져오기
	//+int 4자리수 까지만 표현
	sprintf(rcrBuf, "P0SCO%d%d", pt.x, pt.y);
	retval = send(player[connetPlayerNum].playerSock, rcrBuf, 10, 0);
	if (retval == SOCKET_ERROR) {
		printf("서버에서 게임종료신호 송신 오류");
		exit(-1);
	}

	//클라이언트 플레이어 전달
	for (int i = 0; i < playerCount; ++i) 
	{
		//+전달하는 클라이언트 캐릭터 제외하기
		pt = clientPlayer[i].GetPlayerPt(); //플레이어 위치 가져오기
		sprintf(rcrBuf, "P0%dCO%d%d", i, pt.x, pt.y);
	}
	
}


void INGAME_SERVER::Render()
{

}

void INGAME_SERVER::sendEnd()
{
	/// <summary>
	/// 게임 종료 신호 송신
	/// </summary>
	int retval;
	for (int i = 0; i < playerCount; ++i)
	{
		retval = send(player[i].playerSock, "END", sizeof("END"), 0);
		if (retval == SOCKET_ERROR) {
			printf("서버에서 게임종료신호 송신 오류");
			break;
		}
	}
	
}

void INGAME_SERVER::stringAnalysis(char*)
{

}

PlayerInfo INGAME_SERVER::getPlayer(int num)
{
	/// <summary>
	/// 해당 번호의 플레이어 정보를 얻는 함수
	/// </summary>
	/// <param name="num">그 플레이어의 번호</param>
	/// <returns></returns>
	return player[num];
}

void INGAME_SERVER::setPlayerCount(int n)
{
	playerCount = n;
}

int INGAME_SERVER::getPlayerCount()
{
	return this->playerCount;
}