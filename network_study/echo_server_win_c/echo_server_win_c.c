﻿#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
//argv 값 넘기기
//프로젝트 우클릭 – 속성 – 구성 속성 – 디버깅 – 명령 인수에 값을 적으면 됨

#define BUF_SIZE 1024
void ErrorHandling( char *message );

int main( int argc, char *argv[] )
{

	WSADATA wsaData;
	SOCKET hServSock, hClintSock;
	char message[ BUF_SIZE ];
	int strLen, i;

	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSize;
	
	if(argc!=2){
		printf("Usage : %s <port>\n",argv[0]);
		//	printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}

	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0){
		ErrorHandling("WSAStartup() error!\n");
	}

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if(hServSock==INVALID_SOCKET)
			ErrorHandling("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family=AF_INET;
	servAdr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAdr.sin_port=htons(atoi(argv[1]));

	if(bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if(listen(hServSock,5)==SOCKET_ERROR)
		ErrorHandling("listen() error");

	clntAdrSize=sizeof(clntAdr);

	for(i=0; i<5; ++i)
	{

		hClintSock=accept(hServSock,(SOCKADDR*)&clntAdr,&clntAdrSize);
		if(hClintSock==-1)
			ErrorHandling("accept() error");
		else
			printf("Connected client %d\n",i+1);

		while((strLen=recv(hClintSock, message, BUF_SIZE, 0)) !=0 )
			send(hClintSock,message,strLen,0);

		closesocket(hClintSock);
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}