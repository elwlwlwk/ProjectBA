#include "Player.h"
#include <stdio.h>

Player::Player(SOCKET hClnt, char* name){
	this->hClnt= hClnt;
	memset(this->name, 0, sizeof(this->name));
	memcpy(this->name, name, 30);
	NextNode= NULL;
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

void Player::GetClntMessage(char* buffer){
	int index= 0;
	memset(buffer, 0, sizeof(buffer));
	printf("before get message\n");
	do{
		read(hClnt, &buffer[index], sizeof(char));
		//printf("getmessage %c\n", buffer[index]);

	}while(buffer[index++]!= 0);
}

void Player::SendClntMessage(char* message){
	int len=0;
	len= strlen(message);
	send(hClnt, message, len+1, 0);
}
