#include "Player.h"
#include <stdio.h>
#include <stdlib.h>

void Player::Disconnect(){
	connected= false;
	close(hClnt);

	char query[100];
	memset(query, 0, sizeof(query));
	sprintf(query, "update br_characters set PosX= %d, PosY= %d where CharName= \'%s\';", Posx, Posy, name);
	SendQuery(query);
}

Player::Player(SOCKET hClnt, char* name){
	this->hClnt= hClnt;
	memset(this->name, 0, sizeof(this->name));
	memcpy(this->name, name, 30);
	NextNode= NULL;
	connected= true;
	Posx= Posy= 0;
}


Player::~Player(void)
{
}

Player* Player::GetNextNode(){
	return NextNode;
}

char* Player::GetClntId(){
	return name;
}

void Player::SetNextNode(Player* NextNode){
	this->NextNode= NextNode;
}

int Player::GetClntMessage(char* buffer){
	printf("%d", connected);
	while(connected== false){
		sleep(1); //connect는 false에서 true로 loginsystem에서 바꿔준다.
	}
	int index= 0;
	int result;
	memset(buffer, 0, sizeof(buffer));
	printf("before get message\n");
	do{
		result= read(hClnt, &buffer[index], sizeof(char));
		//printf("getmessage %c\n", buffer[index]);

	}while(buffer[index++]!= 0);
	printf("%s's result is %d",name, result);
	if(result== -1|| result== 0){
		Disconnect();
	}
}

void Player::SendClntMessage(char* message){
	int len=0;
	len= strlen(message);
	if(connected== true){
		printf("Send Message to %s: %s\n", name, message);
		send(hClnt, message, len+1, 0);
	}
}

void Player::SetSocket(SOCKET Socket){
	hClnt= Socket;
}

bool Player::GetConnect(){
	return connected;
}

void Player::SetConnection(bool connection){
	connected= connection;
}


int Player::PlayerMessageProc(char* message){
	char recvmes[5][30];
	memset(recvmes, 0, sizeof(recvmes));

	for(int i= 0, k= 0, m= 0; message[i]!= 0|| i<100; i++){
		if(message[i]== ' '){
			m= 0;
			k++;
			continue;
		}
		recvmes[k][m++]= message[i];
	}

	if(strcmp(recvmes[1], "MOVE")== 0){
		Posx= atoi(recvmes[3]);
		Posy= atoi(recvmes[4]);
	}
	return 0;
}


void Player::SetPos(int x, int y){
	Posx= x;
	Posy= y;
}

int Player::SendQuery(char* query){
	extern MYSQL mysql;
	mysql_real_query(&mysql, query, strlen(query));

	return 0;
}
