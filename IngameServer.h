#pragma once
#include "stdafx.h"
#include "PlayerInfo.h"
#include "Player.h"

enum class NetworkWaitInfo {
	NICKNAME,
	READY,
	REJECT,
	START
};
class INGAME_SERVER
{
public:
	//인게임
	
	void inputManagement(); //[서버] 클라이언트 입력값 처리
	void objectInteract(); //[서버] 오브젝트 충돌처리
	void sendState(int connetPlayerNum); //[서버] 플레이어 정보, 오브젝트 정보를 송신하는 함수
	void Render(); //[서버, 클라이언트] 게임 화면 출력
	void sendEnd(); //[서버] 게임 종료 신호 송신
	void stringAnalysis(char*); //받은 문자열 구문을 분석하여 목적에 맞는 함수 실행
	void setPlayerCount(int);
	int getPlayerCount();
	PlayerInfo getPlayer(int num);
private:
	int playerCount{ 0 }; //대기방 안의 클라이언트 플레이어 수
	PlayerInfo player[3];
	SOCKET listen;
	Player serverPlayer;
	Player clientPlayer[3];
};