#include "stdafx.h"
#include "IngameClient.h"

void INGAME_CLIENT::sendPress()
{
	/// <summary>
	/// �Է��� Ű ������
	/// </summary>
}

void INGAME_CLIENT::receiveState()
{
	/// <summary>
	/// ��ü ���� �ޱ�(��ü ��ġ ��)
	/// </summary>
}

void INGAME_CLIENT::Render()
{

}

void INGAME_CLIENT::receiveEnd()
{
	/// <summary>
	/// ���� ���� ��ɾ� ����
	/// </summary>
	//���� ���� �Լ�
	exit(0);
}

void INGAME_CLIENT::stringAnalysis(char* command)
{
	//��ɾ� ���ڿ� �ؼ�
	if (strcmp(command, "END") == 0)
	{
		receiveEnd();
	}
	else if (strncmp(command, "CO",2) == 0)
	{
		//CO:
		receiveEnd();
	}
}
