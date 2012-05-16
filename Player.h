#include "Windows.h"
#include "SqlLib.h"

class Player{
private:
	SOCKET hClnt;
	char name[30];
	Player* NextNode;
	bool connected;
	int Posx;
	int Posy;
	char arena[30];

	void Disconnect();

public:
	Player();
	Player(SOCKET hClnt, char* name);
	~Player(void);

	Player* GetNextNode();
	void SetNextNode(Player* NextNode);
	int GetClntMessage(char* buffer);
	char* GetClntId();
	void SendClntMessage(char* message);
	void SetSocket(SOCKET Socket);
	bool GetConnect();
	int GetPosX();
	int GetPosY();
	void SetConnection(bool connection);

	void SendPosInfo(Player* targetPlayer);

	void SetPos(int PosX, int PosY);
	int PlayerMessageProc(char* message);
	int SendQuery(char* query, MYSQL_RES** res, int* fields);
};
