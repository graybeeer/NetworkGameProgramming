#pragma once
#include "stdafx.h"
#include "Player.h"

class PlayerInfo
{
public:
	PlayerInfo();
	~PlayerInfo();
	
	SOCKET playerSock; //플레이어 소켓
	char playerNickname[15]; //플레이어 닉네임
	bool playerIsReady; //플레이어 대기방 준비 여부
	int playerNum; //플레이어 식별번호
	char* temp;  // 클라이언트 입력값 임시 저장용



	char* GetNick();
	void SetNick(char* name);
	bool GetIsReady();
	void SetIsReady(bool ready);
	SOCKET GetSock();
	void SetSock(SOCKET sock);
	void CloseSock();
	int GetNum();
	void SetNum(int num);
	
private:
	
};