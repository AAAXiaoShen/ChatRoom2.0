#include<iostream>
#include"ClientClass.h"
#pragma warning(disable:4996)
using namespace std;

//设置控制台的大小
void setsize(int col, int row) {
	char cmd[64];
	sprintf(cmd, "mode con cols=%d lines=%d", col, row);
	system(cmd);
}

int main() {

	//控制台相关操作
	SetConsoleTitle(LPCSTR("会议聊天室----客户端打字区"));

	setsize(70, 20);



	ClientClass Client;

	SOCKET ClntSock = Client.GetSocket();

	Client.ConnectServer("192.168.181.1", 12306);

	Client.SendData();

	Client.RecvData();

	while (true);//停顿

	return 0;
}