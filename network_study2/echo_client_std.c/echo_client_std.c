#pragma comment(lib, "ws2_32.lib")
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<winsock2.h>
void ErrorHandling(char *message);
#define BUF_SIZE 1024

int main(int argc, char *argv[]){
	SOCKET sock;
	char message[BUF_SIZE];
	int str_len;
	WSADATA wsaData;mn
	SOCKADDR_IN	serv_adr;
	FILE * readfp;
	FILE * writefp;
	if(argc!=3){
		printf("Usage: %s <IP><port>\n",argv[0]);
		exit(1);
	}
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0){
		ErrorHandling("WSAStartup() error!\n");
	}
	sock =socket(PF_INET,SOCK_STREAM,0);
	if(sock==-1)
		ErrorHandling("socket() error");

	memset(&serv_adr,0,sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
		ErrorHandling("connect() error!");
	else
		puts("Conneected......");

	readfp=fdopen(sock,"r");
	writefp = fdopen(sock,"w");
	while(1)
	{

		fputs("Input message(Q to quit): ", stdout);
		fgets(message,BUF_SIZE,stdin);
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
			break;

		fputs(message,writefp);
		fflush(writefp);
		fgets(message,BUF_SIZE,readfp);
		printf("Message from server : %s",message);
	}
	fclose(writefp);
	fclose(readfp);
	return 0;
}
void ErrorHandling(char *message)
{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}