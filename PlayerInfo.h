#pragma once
#include "stdafx.h"
#include "Player.h"

class PlayerInfo
{
public:
	PlayerInfo();
	~PlayerInfo();
	char* GetNick();
	void SetNick(char* name);
	bool GetIsReady();
	void SetIsReady(bool ready);
	SOCKET playerSock; //플레이어 소켓
	SOCKET GetSock();
	void SetSock(SOCKET sock);
	void CloseSock();
	int GetNum();
	void SetNum(int num);
	char* temp;  // 클라이언트 입력값 임시 저장용
	Player p;
private:
	char* playerNickname; //플레이어 닉네임
	bool playerIsReady; //플레이어 대기방 준비 여부
	int playerNum; //플레이어 식별번호
};