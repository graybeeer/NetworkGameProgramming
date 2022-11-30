#pragma once
#include "stdafx.h"
#include "Player.h"

class PlayerInfo
{
public:
	PlayerInfo();
	~PlayerInfo();
	
	SOCKET playerSock; //�÷��̾� ����
	char playerNickname[15]; //�÷��̾� �г���
	bool playerIsReady; //�÷��̾� ���� �غ� ����
	int playerNum; //�÷��̾� �ĺ���ȣ
	char* temp;  // Ŭ���̾�Ʈ �Է°� �ӽ� �����



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