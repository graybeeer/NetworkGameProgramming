#include "framework.h"
#include "player.h"
#define SERVERPORT 9000
#define MAX_PLAYER 4

enum class NetworkWaitInfo {
	NICKNAME,
	READY,
	REJECT,
	START
};

enum class NetworkIngameInfo {
	COORD,
	PUSH
};

DWORD WINAPI WaitFiveSecAndStart(LPVOID lpParam);
DWORD WINAPI NetworkServerInit(LPVOID lpParam);
DWORD WINAPI NetworkClientInit(LPVOID lpParam);

DWORD WINAPI roomServerThread(LPVOID lpParam);
DWORD WINAPI inGameServerThread(LPVOID lpParam);

class WAITING_ROOM
{
public:
	WAITING_ROOM(bool isHost);
	~WAITING_ROOM(); // need ¼Ò¸êÀÚ
	bool checkReduplication(char* name);
	bool checkAllReady(); // need checkAllReady
	void pressStart();
	void sendStart(); // need sendStart
	void receiveData();
	void stringAnalysis(char* data);
	SOCKET* getListenP(); // need to talk

	
private:
	SOCKET listen;  // need to talk(11.09)
	SOCKET sock[3]; // need to talk(11.08)

protected:
	PlayerInfo player[3]; // need PayerInfo, need getter setter

};

class INGAME
{
public:
	INGAME();
	~INGAME();

	void sendPress();
	void objectInteract();
	void sendEnd();

private:
	Player player[3];

protected:
	SOCKET sock[3];




};