#include "Backup.h"

Player* BackupList= NULL;

void InitBackupProc(){

	printf("InitBackupProc called\n");
	Player Dummy;
	Player* BackupTail= BackupList;
	MYSQL_RES* res;
	int fields;

	char query[100];
	memset(query, 0, sizeof(query));

	sprintf(query, "select * from BR_Characters");

	printf("Send bakcup query\n");
	Dummy.SendQuery(query, &res, &fields);

	MYSQL_ROW row;

	printf("fetching backup rows\n");
	while(row=mysql_fetch_row(res)){
		printf("fetching\n");
		if(BackupList== NULL){
			BackupTail=BackupList= new Player(row[2]);
		}
		else{
			while(BackupTail->GetNextNode()!= NULL){
				BackupTail= BackupTail->GetNextNode();
			}
			BackupTail->SetNextNode(new Player(row[2]));
			BackupTail= BackupTail->GetNextNode();
		}

		BackupTail->SetPos(atoi(row[8]), atoi(row[9]));
		printf("Set BackupTail %s\n", BackupTail->GetClntId());

	}

	printf("call free reslut\n");

	mysql_free_result(res);

	BackupTail= BackupList;

	if(BackupTail== NULL){
		printf("error BackupTail is NULL\n");
	}

	printf("BackupTail is not NULL\n");
	while(BackupTail!= NULL){
		printf("Print Backup list\n");
		printf("Init Backup List name: %s", BackupTail->GetClntId());
		BackupTail= BackupTail->GetNextNode();
	}

	printf("end InitBackup\n");
}

void* BackupProc(void* trash){

	while(1){
		printf("---------------back up called-------------\n");
		Player* BackupTail= BackupList;

		while(BackupTail!= NULL){
			char query[100];
			memset(query, 0, sizeof(query));

			int Posx= BackupTail->GetPosX();
			int Posy= BackupTail->GetPosY();

			sprintf(query, 
"update BR_Characters set PosX= %d, Posy= %d where CharName= '%s'", 
Posx, Posy, BackupTail->GetClntId());
//			printf("Backup PocSend Query %s", query);

			MYSQL_RES* res;
			int fields;
			BackupTail->SendQuery(query, &res, &fields);

			mysql_free_result(res);

			BackupTail= BackupTail->GetNextNode();
		}

		printf("end backup proc");

		sleep(1);
	}

}
