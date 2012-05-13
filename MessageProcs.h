#include "Player.h"
#include "SqlLib.h"

void DefMessageProc(char* message, Player* threadPlayer);

void PropagateProc(char* message, Player* threadPlayer);

void PropagateAllProc(char* message, Player* threadPlayer);

void SsendProc(char* message, Player* threadPlayer);

void MoveMessageProc(char* message, Player* threadPlayer);

void TrunMessageProc(char* message, Player* threadPlayer);

void InitPlayerProc(char* message, Player* threadPlayer);

void StartPlayProc(char* message, Player* threadPlayer);

void CHMapProc(char* message, Player* threadPlayer);
