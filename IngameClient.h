#pragma once
#include "stdafx.h"
#include "PlayerInfo.h"

class INGAME_CLIENT
{
public:
	//인게임 클라이언트
	void sendPress(); //[클라이언트] 입력 값 송신
	void receiveState(); //[클라이언트] 플레이어 정보, 오브젝트 정보를 수신하는 함수
	void Render(); //[서버, 클라이언트] 게임 화면 출력
	void receiveEnd(); //[클라이언트] 게임 종료 신호 수신
	void stringAnalysis(char* command); //받은 문자열 구문을 분석하여 목적에 맞는 함수 실행
	SOCKET serverSock; //서버 연결 소켓

private:
	int playerCount{ 0 }; //대기방 안의 클라이언트 플레이어 수
	int myNumber{ 0 }; //내 클라이언트 번호
	PlayerInfo player[3];
	Player serverPlayer;
	Player clientPlayer[3];
};