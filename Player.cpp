#include "Player.h"
#include <stdio.h>

Player::Player(SOCKET hClnt, char* name){
	this->hClnt= hClnt;
	memset(this->name, 0, sizeof(this->name));
	memcpy(this->name, name, 30);
	NextNode= NULL;
	connected= true;
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
		this->connected= false;
		close(hClnt);
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

void Player::DisConnect(){
	connected= false;
}

void Player::Connect(){
	connected= true;
}

void Player::SetSocket(SOCKET Socket){
	hClnt= Socket;
}

bool Player::GetConnect(){
	return connected;
}
