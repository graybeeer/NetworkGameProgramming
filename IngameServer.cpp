#include "stdafx.h"
#include "IngameServer.h"
#include "GameFrame.h"
DWORD WINAPI inGameServerReceiveThread(LPVOID lpParam)
{
	/// <summary>
	/// �ΰ��ӿ��� ������ Ŭ���̾�Ʈ�κ��� �޽��� �޴� ������
	/// </summary>
	/// <param name="lpParam"></param>
	/// <returns></returns>
	INGAME_SERVER IS;
	int sockPlayerNum = (int)lpParam; //�� �����忡�� �������� Ŭ���̾�Ʈ�� ��ȣ
	int retval; //�ޱ� ���� ��
	char rcrBuf[2048]{}; //��������
	int rcrLen; // ���� ���� ������
	while (1)
	{
		//�������� �ޱ�
		retval = recv(IS.getPlayer(sockPlayerNum).playerSock, (char*)&rcrLen, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			// ���н� ���� ����
			return 0;
		}
		else if (retval == 0)
			break;
		/*
		//���� ������ �ޱ�
		retval = recv(IS.getPlayer(sockPlayerNum).playerSock, rcrBuf, rcrLen, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			// ���н� ��������
			return 0;
		}
		else if (retval == 0)
			break;
		*/
		//IS.stringAnalysis(rcrBuf);
		strcpy(IS.getPlayer(sockPlayerNum).temp, rcrBuf); // �� Ŭ���̾�Ʈ�� �Է°� �ӽ�����.
		
	}
}

DWORD WINAPI inGameDataProcessingThread(LPVOID lpParam) // Ŭ���̾�Ʈ���� ���޹��� ������ �����ϴ� ������
{
	int sockPlayerNum = (int)lpParam; //�� �����忡�� �������� Ŭ���̾�Ʈ�� ��ȣ
	int retval; //�ޱ� ���� ��
	char rcrBuf[2048]; //��������
	int rcrLen; // ���� ���� ������

	INGAME_SERVER* ppNetInfo = (INGAME_SERVER*)lpParam;

	
	ppNetInfo->inputManagement();
	ppNetInfo->objectInteract();
}

DWORD WINAPI inGameServerSendThread(LPVOID lpParam) // ������ ó���� ������ Ŭ���̾�Ʈ�� �����ϴ� ������
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
	/// [����] �÷��̾� ����, ������Ʈ ������ �۽��ϴ� �Լ�
	/// </summary>
	/// <param name="connetPlayerNum">����� �÷��̾� ��ȣ</param>
	POINT pt; //����Ʈ �ӽ� ��
	char rcrBuf[2048]{}; //��������
	int rcrLen; // ���� ���� ������
	int retval;

	//���� �÷��̾� ��ġ ����
	pt = serverPlayer.GetPlayerPt(); //�÷��̾� ��ġ ��������
	//+int 4�ڸ��� ������ ǥ��
	sprintf(rcrBuf, "P0SCO%d%d", pt.x, pt.y);
	retval = send(player[connetPlayerNum].playerSock, rcrBuf, 10, 0);
	if (retval == SOCKET_ERROR) {
		printf("�������� ���������ȣ �۽� ����");
		exit(-1);
	}

	//Ŭ���̾�Ʈ �÷��̾� ����
	for (int i = 0; i < playerCount; ++i) 
	{
		//+�����ϴ� Ŭ���̾�Ʈ ĳ���� �����ϱ�
		pt = clientPlayer[i].GetPlayerPt(); //�÷��̾� ��ġ ��������
		sprintf(rcrBuf, "P0%dCO%d%d", i, pt.x, pt.y);
	}
	
}


void INGAME_SERVER::Render()
{

}

void INGAME_SERVER::sendEnd()
{
	/// <summary>
	/// ���� ���� ��ȣ �۽�
	/// </summary>
	int retval;
	for (int i = 0; i < playerCount; ++i)
	{
		retval = send(player[i].playerSock, "END", sizeof("END"), 0);
		if (retval == SOCKET_ERROR) {
			printf("�������� ���������ȣ �۽� ����");
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
	/// �ش� ��ȣ�� �÷��̾� ������ ��� �Լ�
	/// </summary>
	/// <param name="num">�� �÷��̾��� ��ȣ</param>
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