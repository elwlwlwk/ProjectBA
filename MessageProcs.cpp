#include "MessageProcs.h"
#include <stdio.h>
#include <stdlib.h>


extern Player* PlayerHead;
Player* Playerlist= PlayerHead;

void DefMessageProc(char* message, Player* threadPlayer){
	char order[30];
	memset(order, 0, sizeof(order));
	for(int i=0; message[i]!= ' '&& i<30; i++){
		order[i]= message[i];
	}
	if(strcmp(order, "ASKINFO")== 0){
		printf("call InitPlayerProc()\n");
		InitPlayerProc(message, threadPlayer);
	}
	else if(strcmp(order, "PRGTA")== 0){
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
	else if(strcmp(order, "ADDHP")== 0){
		printf("Call AddHpProc()\n");
		AddHpProc(message, threadPlayer);
	}
	else if(strcmp(order, "STARTPLAY")== 0){
		printf("Call StartPlayProc()\n");
		StartPlayProc(message, threadPlayer);
	}
	else if(strcmp(order, "CHMAP")== 0){
		printf("Call CHMap()\n");
		CHMapProc(message, threadPlayer);
//		StartPlayProc(message, threadPlayer);
	}
	else{
		printf("Invalid Message\n");
	}
	printf("DefMessageProc() end\n");
}

void PropagateProc(char* message, Player* threadPlayer){
	printf("propagateProc() called\n");

	char query[100];
	MYSQL_RES* res;
	int fields;
	MYSQL_ROW row;
	memset(query, 0, sizeof(100));

	char Tempmessage[100];
	char order[30];
	char Posx[30];
	char Posy[30];
	char Name[30];
	memset(order, 0, sizeof(order));
	memset(Tempmessage, 0, sizeof(Tempmessage));
	memset(Posx, 0, sizeof(Posx));
	memset(Posy, 0, sizeof(Posy));
	memset(Name, 0, sizeof(Name));
	for(int i= 0, k= 0, m= 0,n= 0; message[i]!= 0; i++){
		if(message[i]== ' '&& k== 0){
			i++;
			k++;
			n= 0;
		}
		if(message[i]== ' '){
			k++;
			n= 0;
		}
		if(k== 1&& message[i]!= ' '){
			order[n++]= message[i];
		}
		else if(k== 2&& message[i]!= ' '){
			Name[n++]= message[i];
		}
		else if(k== 3&& message[i]!= ' '){
			Posx[n++]= message[i];
		}
		else if(k== 4&& message[i]!= ' '){
			Posy[n++]= message[i];
		}
		if(k>0){
			Tempmessage[m++]= message[i];
		}
	}
	Player* PlayerTail= PlayerHead;
	printf("send except itself\n");
	while(PlayerTail!= NULL){
		if(strcmp(PlayerTail->GetClntId(), threadPlayer->GetClntId())!= 0)		{
			PlayerTail->SendClntMessage(Tempmessage);
		}
		PlayerTail= PlayerTail->GetNextNode();
	}

	printf("PRGT UpdateMoveProc()\n");
	printf("order is %s\n", order);
	if(strcmp(order, "MOVE")== 0){
		PlayerTail= PlayerHead;

		while(PlayerTail!= NULL){
			if(strcmp(PlayerTail->GetClntId(), Name)== 0){
				printf("set %s's pos to %d %d\n", 
PlayerTail->GetClntId(), atoi(Posx), atoi(Posy));
				PlayerTail->SetPos(atoi(Posx), atoi(Posy));
			}
			PlayerTail= PlayerTail->GetNextNode();
		}
		sprintf(query, 
"update BR_Characters set PosX= %d, PosY= %d where CharName= '%s'", 
atoi(Posx), atoi(Posy), Name);
		printf("UPDATE POSITION QUERY: %s\n", query);
		threadPlayer->SendQuery(query, &res, &fields);
		mysql_free_result(res);

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

void InitPlayerProc(char* message, Player* threadPlayer){
	char id[30];
	char query[100];
	memset(id, 0, sizeof(id));
	memset(query, 0, sizeof(query));

	MYSQL_RES* res;
	int fields;

	sprintf(query, "select * from BR_Characters where CharName='%s'", 
threadPlayer->GetClntId());

	printf("send query: %s\n", query);

	threadPlayer->SendQuery(query, &res, &fields);

	MYSQL_ROW row;

	printf("call mysql_fetch_row()\n");
	row= mysql_fetch_row(res);

	printf("mysql_fetch_row() end\n");

	char tempMessage[100];
	memset(tempMessage, 0, sizeof(tempMessage));
	sprintf(tempMessage, "INITP LOCATION %s %s %s", row[7], row[8], row[9]);
	printf("send InitMessage: %s\n", tempMessage);
	threadPlayer->SendClntMessage(tempMessage);

	memset(tempMessage, 0, sizeof(tempMessage));
	sprintf(tempMessage, "INITP HP %s %s", row[4], row[3]);
	printf("send InitMessage: %s\n", tempMessage);
	threadPlayer->SendClntMessage(tempMessage);

	memset(tempMessage, 0, sizeof(tempMessage));
	sprintf(tempMessage, "INIT STA %s %s", row[6], row[5]);
	printf("send InitMessage: %s\n", tempMessage);
	threadPlayer->SendClntMessage(tempMessage);

	memset(tempMessage, 0, sizeof(tempMessage));
	sprintf(tempMessage, "INITP END");
	printf("send InitMEssage: %s\n", tempMessage);
	threadPlayer->SendClntMessage(tempMessage);

	printf("call mysql_free_result\n");

	mysql_free_result(res);

	printf("call mysql_free_result() end\n");
}

void StartPlayProc(char* message, Player* threadPlayer){
	char query[100];
	char toOthers[100];
	MYSQL_RES* res;
	MYSQL_ROW row;
	int fields;
	memset(query, 0, sizeof(query));

	sprintf(query, "select * from BR_Characters");
	threadPlayer->SendQuery(query, &res, &fields);

	while(row= mysql_fetch_row(res)){
		char tempMessage[100];
		memset(tempMessage, 0, sizeof(tempMessage));
		if(strcmp(threadPlayer->GetClntId(), row[2])== 0){
			sprintf(toOthers, "ENTERP %s %s %s %s", row[2], row[7], 
row[8], row[9]);
			continue;
		}
		sprintf(tempMessage, "ENTERP %s %s %s %s", row[2], row[7], 
row[8], row[9]);
		threadPlayer->SendClntMessage(tempMessage);

	}

	Player* PlayerTail= PlayerHead;
	while(PlayerTail!= NULL){
		if(strcmp(PlayerTail->GetClntId(), threadPlayer->GetClntId())
!= 0){
			PlayerTail->SendClntMessage(toOthers);
		}
		PlayerTail= PlayerTail->GetNextNode();
	}

	mysql_free_result(res);

	PlayerTail= PlayerHead;

	while(PlayerTail!= NULL){
		if(PlayerTail->GetConnect()== true){
			PlayerTail->SendPosInfo(threadPlayer);
		}
		PlayerTail= PlayerTail->GetNextNode();
	}

}

void CHMapProc(char* message, Player* threadPlayer){

	char arena[30];
	memset(arena, 0, sizeof(arena));

	for(int i= 0, k= 0, m= 0; i<100; i++){
		if(message[i]== ' '){
			k++;
			i++;
		}
		if(k== 2){
			arena[m++]= message[i];
		}
	}

	printf("CHMAP arena to %s\n", arena);

	char query[100];
	MYSQL_RES* res;
	int fields;
	memset(query, 0, sizeof(query));
	sprintf(query, 
"update BR_Characters set arena= '%s' where CharName= '%s'", arena
, threadPlayer->GetClntId());

	threadPlayer->SendQuery(query, &res, &fields);

	mysql_free_result(res);

	StartPlayProc(message, threadPlayer);
}

void AddHpProc(char* message, Player* threadPlayer){

	char victim[30];
	char amount[30];

	memset(victim, 0, sizeof(victim));
	memset(amount, 0, sizeof(amount));

	char query[100];
	MYSQL_RES *res;
	int fields;

	memset(query, 0, sizeof(query));


	for(int i= 0, k= 0, m= 0; i< 100; i++){
		if(message[i]== ' '){
			k++;
			m= 0;
		}
		if(k== 1){
			victim[m++]= message[i];
		}
		if(k== 2){
			amount[m++]= message[i];
		}
	}

	sprintf(query, "select * BR_Characters form CharName= '%s'", 
victim);

	threadPlayer->SendQuery(query, &res, &fields);
	MYSQL_ROW row;
	row= mysql_fetch_row(res);

	int victimhp= atoi(row[3]);
	int dishp= atoi(amount);

	mysql_free_result(res);
	memset(query, 0, sizeof(query));

	if(victimhp- dishp> 0){
		sprintf(query, 
"update BR_Characters set Hit= %d where CharName= '%s'", 
victimhp- dishp, victim);
		threadPlayer->SendQuery(query, &res, &fields);
		mysql_free_result(res);
	}
	else{
		sprintf(query, 
"update BR_Characters set Hit= %d, CharStatus= 'DEAD' where CharName= '%s'", 
0, victim);
		threadPlayer->SendQuery(query, &res, &fields);
		mysql_free_result(res);

		Player* PlayerTail= PlayerHead;

		char tempMessage[100];
		memset(tempMessage, 0, sizeof(tempMessage));

		sprintf(tempMessage, "CHWEAPON %s dead", victim);

		while(PlayerTail!= NULL){
			PlayerTail->SendClntMessage(tempMessage);
			PlayerTail= PlayerTail->GetNextNode();
		}
	}
}
