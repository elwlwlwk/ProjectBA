#include "Windows.h"
#include <time.h>

class Player
{
private:
	SOCKET hClnt;
	char name[30];
	Player* NextNode;
	bool connected;

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
};
