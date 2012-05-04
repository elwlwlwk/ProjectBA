#include "MessageProcs.h"
#include <stdio.h>


extern Player* PlayerHead;
Player* Playerlist= PlayerHead;

void DefMessageProc(char* message, Player* threadPlayer){
	char order[30];
	memset(order, 0, sizeof(order));
	for(int i=0; message[i]!= ' '; i++){
		order[i]= message[i];
	}
	if(strcmp(order, "PRGTA")== 0){
		printf("call PropagateAllMessageProc()\n");
		PropagateAllProc(message, threadPlayer);
	}

	else if(strcmp(order, "PRGT")== 0){
		printf("Call PropagateMessageProc()\n");
		PropagateProc(message, threadPlayer);
	}
	else if(strcmp(order, "SSEND")== 0){
		printf("Call SsendMessageProc()\n");
		SsendProc(message, threadPlayer);
	}
}

void PropagateProc(char* message, Player* threadPlayer){
	printf("propagateProc() called\n");
	char Tempmessage[100];
	memset(Tempmessage, 0, sizeof(Tempmessage));
	for(int i= 0, k= 0, m= 0; message[i]!= 0; i++){
		if(message[i]== ' '&& k== 0){
			k++;
			i++;
		}
		if(k>0){
			Tempmessage[m++]= message[i];
		}
	}
	Player* PlayerTail= PlayerHead;
	printf("send except itself\n");
	while(PlayerTail!= NULL){
		if(strcmp(PlayerTail->GetClntId(), threadPlayer->GetClntId())
!= 0)		{
			PlayerTail->SendClntMessage(Tempmessage);
		}
		PlayerTail= PlayerTail->GetNextNode();
	}
}

void PropagateAllProc(char* message, Player* threadPlayer){
	char Tempmessage[100];
	memset(Tempmessage, 0, sizeof(Tempmessage));
	for(int i= 0, k= 0, m= 0; message[i]!= 0; i++){
		if(message[i]== ' '&& k== 0){
			k++;
			i++;
		}
		if(k>0){
			Tempmessage[m++]= message[i];
		}
	}
	Player* TempPlayer= PlayerHead;
	while(TempPlayer!= NULL){
		TempPlayer->SendClntMessage(Tempmessage);
		TempPlayer= TempPlayer->GetNextNode();
	}
}

void SsendProc(char* message, Player* threadPlayer){
	char targetname[30];
	char Tempmessage[100];
	memset(targetname, 0, sizeof(targetname));
	memset(Tempmessage, 0, sizeof(Tempmessage));

	Player* PlayerTail= PlayerHead;
	for(int i= 0, k=0, m= 0, n= 0; i<100&& message[i]!= 0; i++){
		if(message[i]== ' '&& k<2){
			k++;
			continue;
		}
		if(k== 1){
			targetname[m++]= message[i];
		}
		if(k>1){
			Tempmessage[n++]= message[i];
		}
	}
	printf("target name is %s\n", targetname);
	printf("cmpname is %s\n", PlayerTail->GetClntId());
	while(PlayerTail!= NULL){
	printf("terget name is %s, and cmpname is %s\n", targetname
, PlayerTail->GetClntId());
		if(strcmp(targetname, PlayerTail->GetClntId())== 0){
			printf("send message to %s: %s\n"
, PlayerTail->GetClntId(), Tempmessage);
			PlayerTail->SendClntMessage(Tempmessage);
			break;
		}
		PlayerTail= PlayerTail->GetNextNode();
	}
}

void MoveMessageProc(char* message, Player* threadPlayer){
	char x[30], y[30];
	memset(x, 0, sizeof(x));
	memset(y, 0, sizeof(y));
	Player* PlayerTail;

	for(int i=0, k=0, m=0; i<100&& message[i]!= 0; i++){
		if(message[i]==' '){
			k++;
			m= i;
			continue;
		}
		if(k== 1){
			x[i-m-1]= message[i];
		}
		if(k== 2){
			y[i-m-1]= message[i];
		}
	}
	char Tempmessage[100]="MOVE ";
	strcat(Tempmessage, threadPlayer->GetClntId());
	strcat(Tempmessage, " ");
	strcat(Tempmessage, x);
	strcat(Tempmessage, " ");
	strcat(Tempmessage, y);

	PlayerTail= Playerlist;

	while(PlayerTail!= NULL){
		if(strcmp(PlayerTail->GetClntId(), threadPlayer->GetClntId())!= 0)
			PlayerTail->SendClntMessage(Tempmessage);
		PlayerTail= PlayerTail->GetNextNode();
	}
	printf("send message: %s\n", Tempmessage);
}

void TrunMessageProc(char* message, Player* threadPlayer){
	char theta[30];
	Player* PlayerTail;
	memset(theta, 0, sizeof(theta));
	for(int i=0, k=0, m=0; i<100&& message[i]!= 0; i++){
		if(message[i]==' '){
			k++;
			m= i;
			continue;
		}
		if(k== 1){
			theta[i-m-1]= message[i];
		}
	}
	char Tempmessage[100]="TURN ";
	strcat(Tempmessage, threadPlayer->GetClntId());
	strcat(Tempmessage, " ");
	strcat(Tempmessage, theta);
	PlayerTail= Playerlist;

	while(PlayerTail!= NULL){
		if(strcmp(PlayerTail->GetClntId(), threadPlayer->GetClntId())!= 0)
			PlayerTail->SendClntMessage(Tempmessage);
		PlayerTail= PlayerTail->GetNextNode();
	}
	printf("send message: %s\n", Tempmessage);
}
