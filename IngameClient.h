#pragma once
#include "stdafx.h"
#include "PlayerInfo.h"

class INGAME_CLIENT
{
public:
	//�ΰ��� Ŭ���̾�Ʈ
	void sendPress(); //[Ŭ���̾�Ʈ] �Է� �� �۽�
	void receiveState(); //[Ŭ���̾�Ʈ] �÷��̾� ����, ������Ʈ ������ �����ϴ� �Լ�
	void Render(); //[����, Ŭ���̾�Ʈ] ���� ȭ�� ���
	void receiveEnd(); //[Ŭ���̾�Ʈ] ���� ���� ��ȣ ����
	void stringAnalysis(char* command); //���� ���ڿ� ������ �м��Ͽ� ������ �´� �Լ� ����
	SOCKET serverSock; //���� ���� ����

private:
	int playerCount{ 0 }; //���� ���� Ŭ���̾�Ʈ �÷��̾� ��
	int myNumber{ 0 }; //�� Ŭ���̾�Ʈ ��ȣ
	PlayerInfo player[3];
	Player serverPlayer;
	Player clientPlayer[3];
};