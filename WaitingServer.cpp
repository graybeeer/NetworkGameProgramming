#include "stdafx.h"
#include "WaitingServer.h"

DWORD WINAPI roomServerReceiveThread(LPVOID lpParam)
{
	/// <summary>
	/// 서버->클라이언트
	///	클라이언트 소켓버퍼 데이터 수신(닉네임, 입장, 레디, 레디취소 신호 등등)
	///	입장거부 신호, 게임시작 신호 송신
	/// </summary>
	/// <param name="lpParam"></param>
	/// <returns></returns>

}
DWORD WINAPI roomServerSendThread(LPVOID lpParam)
{
	/// <summary>
	/// 서버->클라이언트
	///	클라이언트 소켓버퍼 데이터 수신(닉네임, 입장, 레디, 레디취소 신호 등등)
	///	입장거부 신호, 게임시작 신호 송신
	/// </summary>
	/// <param name="lpParam"></param>
	/// <returns></returns>

}

DWORD WINAPI roomDataProcessingThread(LPVOID lpParam)
{
	/// <summary>
	/// 받은 네트워크 데이터를 처리하는 스레드
	/// 클라이언트 연결 처리(닉네임, 소켓정보)
	/// 대기실 정보 화면 출력
	/// 게임 시작 관리
	/// </summary>
	/// <param name="lpParam"></param>
	/// <returns></returns>
}
DWORD WINAPI WaitFiveSecAndStart(LPVOID lpParam)
{
	/// <summary>
	/// 게임 시작 전 5초 기다리고 시작하는 쓰레드
	/// </summary>
	/// <param name="lpParam"></param>
	/// <returns></returns>
	WATING_SERVER* ppNetInfo = (WATING_SERVER*)lpParam;
	Sleep(5000);
	if (ppNetInfo->checkAllReady() == true) {
		ppNetInfo->sendStart();
	}
}

//*************************************THREAD*************************************//

WATING_SERVER::WATING_SERVER()
{
	/// <summary>
	/// 대기실 클래스를 만드는 함수
	/// </summary>
	/// <param name="lpParam"></param>
	/// <returns></returns>
	for (int i{}; i < playerCount; ++i)
	{
		strcpy(player[i].GetNick(), "\0");
		player[i].SetIsReady(false);
		player[i].SetSock(NULL); // CreateThread() 실패한 경우도 NULL
		player[i].SetNum(0); // 0 - 플레이어 슬롯 비어있음

		//sock[i] = (SOCKET)-1; // accept() 실패한 경우도 -1
	}

	listen = (SOCKET)-1; // listen() 실패한 경우도 -1
}

WATING_SERVER::~WATING_SERVER()
{
	/// <summary>
	/// 대기실 사용을 멈출 때 불러짐 정상적으로 연결을 모두 종료하고 핸들을 닫아주는 내용을 포함
	/// </summary>
	for (int i{}; i < playerCount; ++i)
	{
		player[i].CloseSock();
	}

}

bool WATING_SERVER::checkReduplication(char* name)
{
	/// <summary>
	/// 닉네임 중복 체크
	/// </summary>
	/// <param name="name"></param>
	/// <returns>없는 닉네임 일 때 true 중복시 false</returns>
	for (int i{}; i < playerCount; ++i) {
		if (strcmp(player[i].GetNick(), name) == 0)
			return false;
	}
	return true;
}

bool WATING_SERVER::checkAllReady()
{
	/// <summary>
	/// 클라이언트들의 isReady 여부 
	/// </summary>
	/// <returns>체크 모든 클라이언트가 Ready면 true 아니면 false</returns>
	for (int i = 0; i < playerCount; ++i)
	{
		if (player[i].GetIsReady() == false)
			return false;
	}
	return true;

}

bool WATING_SERVER::checkJoin(HANDLE handle)
{
	/// <summary>
	/// 정상적인 접속시도인지 검사
	/// </summary>
	/// <param name="">접속하려는 클라이언트의 핸들값</param>
	/// <returns>닉네임 중복 시 false 반환 그 외 경우는 접속처리 후 true 반환</returns>
	for (int i = 0; i < playerCount; ++i)
	{
		if (strcmp(player[i].GetNick(), (char*)handle) == 0)
		{
			return false;
		}
	}
	return true;
}

void WATING_SERVER::pressStart()
{
	/// <summary>
	/// 방장이 게임 시작 시도
	/// </summary>
	if (this->checkAllReady() == true) {
		HANDLE hthr = CreateThread(NULL, 0, WaitFiveSecAndStart, NULL, 0, NULL);
		CloseHandle(hthr);
	}
}

void WATING_SERVER::receiveReady(int playerNumber)
{
	/// <summary>
	/// 클라이언트가 ready 
	/// ready 버튼 누를 때마다 isReady에 true, false값 전환
	/// </summary>
	/// <param name="playerNumber">플레이어 식별번호</param>
	player[playerNumber].SetIsReady(player[playerNumber].GetIsReady() ? false : true); //true면 false, false면 true 변환.
}


void WATING_SERVER::receiveData()
{
	/// <summary>
	/// 
	/// 클라이언트 소켓버퍼 데이터 송신(NetworkWaitInfo)
	/// </summary>
}

void WATING_SERVER::refuseEnter(SOCKET consock)
{
	/// <summary>
	/// 입장거부 신호 송신(보내기)
	/// </summary>
	int retval = send(consock, "RJ", sizeof("RJ"), 0);
	if (retval == SOCKET_ERROR) {
		// 실패시 소켓 닫기
		return;
	}
}
void WATING_SERVER::sendStart()
{
	/// <summary>
	/// 플레이어 정보, 오브젝트 정보를 송신(보내기)
	/// </summary>
	//int retval = send(player[playerNum].GetSock(), "RJ", sizeof("RJ"), 0);
	int retval;
	for (int i = 0; i < playerCount; ++i)
	{
		retval = send(player[i].GetSock(), "ST", sizeof("ST"), 0);
		if (retval == SOCKET_ERROR) {
			printf("서버에서 게임 시작 신호 송신 오류");
			break;
		}
	}
}

void WATING_SERVER::stringAnalysis(char* command)
{
	/// <summary>
	/// 받은 문자열 구문을 분석하여 목적에 맞는 함수 실행
	/// </summary>
	/// <param name="">해석할 문자열</param>
	if (strncmp(command, "NN", 2) == 0)
	{
		//플레이어 닉네임 정하기
		//NN0'이름': 0번 플레이어 이름 정하기 ...
	}
	else if (strncmp(command, "RD", 2) == 0)
	{
		//RD: 레디 신호
		//RD0: 0번 플레이어 레디
		//RD1: 1번 플레이어 레디 ...
		receiveReady(command[2]);
	}
}
