#include "GameThread.h"
#include <stdio.h>

HANDLE hThread[32];
int thread_index= 0;

void* ThreadProc(void* PlayerHead);

void MakeNewThread(Player** Playerlist, SOCKET newSocket, char* id){

	pthread_attr_t DetachedAttr;
	pthread_attr_init(&DetachedAttr);
	pthread_attr_setdetachstate(&DetachedAttr, PTHREAD_CREATE_DETACHED);

	printf("Called MakeNewThred()\n");
	Player* tempPlayer= *Playerlist;
	if((*Playerlist)== NULL){
		*Playerlist= new Player(newSocket, id);
	}
	else{
		while(tempPlayer->GetNextNode()!= NULL){
			tempPlayer= tempPlayer->GetNextNode();
		}
		tempPlayer->SetNextNode(new Player(newSocket, id));
	}

	if(thread_index<32){
		printf("Make New Thread\n");
		pthread_create( &(hThread[thread_index]), &DetachedAttr,
 (*ThreadProc), Playerlist);
	}
}

extern Player* PlayerHead;

void* ThreadProc(void* PlayerHead2){
	printf("New Thread Called \n");

	Player* PlayerTail;

	while(1){
		char message[100];
		char order[30];
		int message_index=0;

		memset(message, 0, sizeof(message));
		memset(order, 0, sizeof(order));

		Player* Playerlist= (Player*) PlayerHead;
		Player* threadPlayer;
		Player* temp= Playerlist;
		while(temp->GetNextNode()!= NULL){
			temp= temp->GetNextNode();
		}
		threadPlayer= temp;
		printf("threadPlayer is %s\n", threadPlayer->GetClntId());

		threadPlayer->SetMyThread();
		//int MinResult= MinGyuProc(threadPlayer);

		char Tempmessage[100]="NEWP ";
		strcat(Tempmessage, threadPlayer->GetClntId());

		PlayerTail= Playerlist;

		while(PlayerTail!= NULL){
			if(strcmp(PlayerTail->GetClntId(), threadPlayer->GetClntId())!= 0)
				PlayerTail->SendClntMessage(Tempmessage);
			PlayerTail= PlayerTail->GetNextNode();
		}
		printf("send message: %s\n", Tempmessage);

		PlayerTail= Playerlist;
		while(PlayerTail!= NULL){
			memset(Tempmessage, 0, sizeof(Tempmessage));
			strcpy(Tempmessage, "NEWP ");
			strcat(Tempmessage, PlayerTail->GetClntId());
			if(strcmp(PlayerTail->GetClntId(), threadPlayer->GetClntId())!= 0)
				threadPlayer->SendClntMessage(Tempmessage);
			PlayerTail= PlayerTail->GetNextNode();
		}

		while(1){
//			if(threadPlayer== NULL)
//				printf("threadPlayer is null\n");
			printf("Call GetClntMessage()\n");
			int result= threadPlayer->GetClntMessage(message);
			printf("incoming message: %s\n", message);
			if(result!= 0&& result!= -1){
				printf("Call DefMessageProc()\n");
				DefMessageProc(message, threadPlayer);
				printf("Call PlayerMessageProc()\n");
				threadPlayer->PlayerMessageProc(message);
			}
			/*
			for(int i= 0; i<30; i++){
				if(message[i]== ' ')
					break;
				order[i] =message[i];
			}
			if(strcmp(order, "MOVE")== 0){
				char x[30], y[30];
				memset(x, 0, sizeof(x));
				memset(y, 0, sizeof(y));

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
			if(strcmp(order, "TURN")== 0){
				char theta[30];
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
			*/

		}
	}
}
