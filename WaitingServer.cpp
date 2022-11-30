#include "stdafx.h"
#include "WaitingServer.h"

DWORD WINAPI roomServerReceiveThread(LPVOID lpParam)
{
	/// <summary>
	/// ����->Ŭ���̾�Ʈ
	///	Ŭ���̾�Ʈ ���Ϲ��� ������ ����(�г���, ����, ����, ������� ��ȣ ���)
	///	����ź� ��ȣ, ���ӽ��� ��ȣ �۽�
	/// </summary>
	/// <param name="lpParam"></param>
	/// <returns></returns>

}
DWORD WINAPI roomServerSendThread(LPVOID lpParam)
{
	/// <summary>
	/// ����->Ŭ���̾�Ʈ
	///	Ŭ���̾�Ʈ ���Ϲ��� ������ ����(�г���, ����, ����, ������� ��ȣ ���)
	///	����ź� ��ȣ, ���ӽ��� ��ȣ �۽�
	/// </summary>
	/// <param name="lpParam"></param>
	/// <returns></returns>

}

DWORD WINAPI roomDataProcessingThread(LPVOID lpParam)
{
	/// <summary>
	/// ���� ��Ʈ��ũ �����͸� ó���ϴ� ������
	/// Ŭ���̾�Ʈ ���� ó��(�г���, ��������)
	/// ���� ���� ȭ�� ���
	/// ���� ���� ����
	/// </summary>
	/// <param name="lpParam"></param>
	/// <returns></returns>
}
DWORD WINAPI WaitFiveSecAndStart(LPVOID lpParam)
{
	/// <summary>
	/// ���� ���� �� 5�� ��ٸ��� �����ϴ� ������
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
	/// ���� Ŭ������ ����� �Լ�
	/// </summary>
	/// <param name="lpParam"></param>
	/// <returns></returns>
	for (int i{}; i < playerCount; ++i)
	{
		strcpy(player[i].GetNick(), "\0");
		player[i].SetIsReady(false);
		player[i].SetSock(NULL); // CreateThread() ������ ��쵵 NULL
		player[i].SetNum(0); // 0 - �÷��̾� ���� �������

		//sock[i] = (SOCKET)-1; // accept() ������ ��쵵 -1
	}

	listen = (SOCKET)-1; // listen() ������ ��쵵 -1
}

WATING_SERVER::~WATING_SERVER()
{
	/// <summary>
	/// ���� ����� ���� �� �ҷ��� ���������� ������ ��� �����ϰ� �ڵ��� �ݾ��ִ� ������ ����
	/// </summary>
	for (int i{}; i < playerCount; ++i)
	{
		player[i].CloseSock();
	}

}

bool WATING_SERVER::checkReduplication(char* name)
{
	/// <summary>
	/// �г��� �ߺ� üũ
	/// </summary>
	/// <param name="name"></param>
	/// <returns>���� �г��� �� �� true �ߺ��� false</returns>
	for (int i{}; i < playerCount; ++i) {
		if (strcmp(player[i].GetNick(), name) == 0)
			return false;
	}
	return true;
}

bool WATING_SERVER::checkAllReady()
{
	/// <summary>
	/// Ŭ���̾�Ʈ���� isReady ���� 
	/// </summary>
	/// <returns>üũ ��� Ŭ���̾�Ʈ�� Ready�� true �ƴϸ� false</returns>
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
	/// �������� ���ӽõ����� �˻�
	/// </summary>
	/// <param name="">�����Ϸ��� Ŭ���̾�Ʈ�� �ڵ鰪</param>
	/// <returns>�г��� �ߺ� �� false ��ȯ �� �� ���� ����ó�� �� true ��ȯ</returns>
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
	/// ������ ���� ���� �õ�
	/// </summary>
	if (this->checkAllReady() == true) {
		HANDLE hthr = CreateThread(NULL, 0, WaitFiveSecAndStart, NULL, 0, NULL);
		CloseHandle(hthr);
	}
}

void WATING_SERVER::receiveReady(int playerNumber)
{
	/// <summary>
	/// Ŭ���̾�Ʈ�� ready 
	/// ready ��ư ���� ������ isReady�� true, false�� ��ȯ
	/// </summary>
	/// <param name="playerNumber">�÷��̾� �ĺ���ȣ</param>
	player[playerNumber].SetIsReady(player[playerNumber].GetIsReady() ? false : true); //true�� false, false�� true ��ȯ.
}


void WATING_SERVER::receiveData()
{
	/// <summary>
	/// 
	/// Ŭ���̾�Ʈ ���Ϲ��� ������ �۽�(NetworkWaitInfo)
	/// </summary>
}

void WATING_SERVER::refuseEnter(SOCKET consock)
{
	/// <summary>
	/// ����ź� ��ȣ �۽�(������)
	/// </summary>
	int retval = send(consock, "RJ", sizeof("RJ"), 0);
	if (retval == SOCKET_ERROR) {
		// ���н� ���� �ݱ�
		return;
	}
}
void WATING_SERVER::sendStart()
{
	/// <summary>
	/// �÷��̾� ����, ������Ʈ ������ �۽�(������)
	/// </summary>
	//int retval = send(player[playerNum].GetSock(), "RJ", sizeof("RJ"), 0);
	int retval;
	for (int i = 0; i < playerCount; ++i)
	{
		retval = send(player[i].GetSock(), "ST", sizeof("ST"), 0);
		if (retval == SOCKET_ERROR) {
			printf("�������� ���� ���� ��ȣ �۽� ����");
			break;
		}
	}
}

void WATING_SERVER::stringAnalysis(char* command)
{
	/// <summary>
	/// ���� ���ڿ� ������ �м��Ͽ� ������ �´� �Լ� ����
	/// </summary>
	/// <param name="">�ؼ��� ���ڿ�</param>
	if (strncmp(command, "NN", 2) == 0)
	{
		//�÷��̾� �г��� ���ϱ�
		//NN0'�̸�': 0�� �÷��̾� �̸� ���ϱ� ...
	}
	else if (strncmp(command, "RD", 2) == 0)
	{
		//RD: ���� ��ȣ
		//RD0: 0�� �÷��̾� ����
		//RD1: 1�� �÷��̾� ���� ...
		receiveReady(command[2]);
	}
}
