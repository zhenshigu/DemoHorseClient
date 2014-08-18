// SimpleHorseClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <conio.h>
#pragma comment (lib,"ws2_32")
#define NAME_LEN 20

typedef struct _SYS_INFO
{
	OSVERSIONINFO OsVer;
	char szComputerName[NAME_LEN];
	char szUserName[NAME_LEN];
}SYS_INFO,*PSYS_INFO;

void ShowSysInfo(PSYS_INFO SysInfo)
{
	if (SysInfo->OsVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
	{
		if (SysInfo->OsVer.dwMajorVersion==5 && SysInfo->OsVer.dwMinorVersion==1)
		{
			printf("Window XP %s \r\n",SysInfo->OsVer.szCSDVersion);
		}
		else if (SysInfo->OsVer.dwMajorVersion==5 && SysInfo->OsVer.dwMinorVersion==0)
		{
			printf("Windows 2k \r\n");
		}
	}
	else
	{
		printf("other system \r\n");
	}
	printf("Computer Name is %s \r\n",SysInfo->szComputerName);
	printf("User Name is %s \r\n",SysInfo->szUserName);
}
int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
	SOCKET clientSock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in ServerAddr;
	ServerAddr.sin_family=PF_INET;
	ServerAddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
	ServerAddr.sin_port=htons(827);
	connect(clientSock,(SOCKADDR*)&ServerAddr,sizeof(SOCKADDR));
	while(TRUE){
		char Buff[MAXBYTE]={0};
		char Cmd[MAXBYTE]={0};
		recv(clientSock,Buff,MAXBYTE,0);
		printf("%s",Buff);
		scanf("%s",Cmd);
		printf("%s from client",Cmd);
		send(clientSock,Cmd,MAXBYTE,0);
		if (!strcmp(Cmd,"exit"))
		{
			printf("Login out!\r\n");
			break;
		}
		memset(Buff,0,MAXBYTE);
		recv(clientSock,Buff,MAXBYTE,0);
		if (!strcmp(Cmd,"getsysinfo"))
		{
			PSYS_INFO SysInfo=(PSYS_INFO)Buff;
			ShowSysInfo(SysInfo);
		}
		else{
			printf("%s \r\n",Buff);
		}
	}
	getch();
	WSACleanup();
	
	return 0;
}

