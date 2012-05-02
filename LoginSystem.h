#include <stdio.h>
#include "GameThread.h"

void LoginSystem(char* message, SOCKET hClntSock);

void NewMember(char* message, SOCKET* hClntSock);

void Login(char* message, SOCKET* hClntSock);

int Reconnect(char* id, SOCKET Socket);
