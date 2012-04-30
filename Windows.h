#define SOCKET int
#define HANDLE pthread_t
#define WSADATA int
#define SOCKADDR_IN (struct sockaddr_in)
#define SOCK_ERROR -1
#define SOCKADDR (struct sockaddr)
#define INDVALID_SOCKET -1

#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

//HANDLE CreateThread(int nul,int nul2, void (*start_rtn)(void), 
//void *arg){
//	pthread_t thread;
//	pthread_create(&thread, NULL, &start_rtn, NULL )
//	return thread;
//}

//int WSAStartup(WORD a, WSADATA b){
//	return 0;
//}
//void send(SOCKET hSock, char* buffer, int size, int flag){
//	write(int hSock, char* buffer, int size);
//}

//void recv(SOCKET hSock, char* buffer, int size, int flag){
//	read(int hSock, char* buffer, int size);
//}
