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
	//�ΰ���
	
	void inputManagement(); //[����] Ŭ���̾�Ʈ �Է°� ó��
	void objectInteract(); //[����] ������Ʈ �浹ó��
	void sendState(int connetPlayerNum); //[����] �÷��̾� ����, ������Ʈ ������ �۽��ϴ� �Լ�
	void Render(); //[����, Ŭ���̾�Ʈ] ���� ȭ�� ���
	void sendEnd(); //[����] ���� ���� ��ȣ �۽�
	void stringAnalysis(char*); //���� ���ڿ� ������ �м��Ͽ� ������ �´� �Լ� ����
	void setPlayerCount(int);
	int getPlayerCount();
	PlayerInfo getPlayer(int num);
private:
	int playerCount{ 0 }; //���� ���� Ŭ���̾�Ʈ �÷��̾� ��
	PlayerInfo player[3];
	SOCKET listen;
	Player serverPlayer;
	Player clientPlayer[3];
};