#include "stdafx.h"
#include "IngameClient.h"

void INGAME_CLIENT::sendPress()
{
	/// <summary>
	/// 입력한 키 보내기
	/// </summary>
}

void INGAME_CLIENT::receiveState()
{
	/// <summary>
	/// 객체 상태 받기(객체 위치 등)
	/// </summary>
}

void INGAME_CLIENT::Render()
{

}

void INGAME_CLIENT::receiveEnd()
{
	/// <summary>
	/// 게임 종료 명령어 받음
	/// </summary>
	//게임 종료 함수
	exit(0);
}

void INGAME_CLIENT::stringAnalysis(char* command)
{
	//명령어 문자열 해석
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
