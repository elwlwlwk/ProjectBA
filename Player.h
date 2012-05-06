#include "Windows.h"

class Player{
private:
	SOCKET hClnt;
	char name[30];
	Player* NextNode;
	bool connected;
	int Posx;
	int Posy;

public:
	Player(SOCKET hClnt, char* name);
	~Player(void);

	Player* GetNextNode();
	void SetNextNode(Player* NextNode);
	int GetClntMessage(char* buffer);
	char* GetClntId();
	void SendClntMessage(char* message);
	void DisConnect();
	void Connect();
	void SetSocket(SOCKET Socket);
	bool GetConnect();

	void setPos(int PosX, int PosY);
	int PlayerMessageProc(char* message);
	void SavePlayerData(char* query);
};
