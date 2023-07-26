#include<iostream>
#include "ServerClass.h"
#pragma warning(disable:4996)
using namespace std;

//设置控制台的大小
void setsize(int col, int row) {
	char cmd[64];
	sprintf(cmd, "mode con cols=%d lines=%d", col, row);
	system(cmd);
}

int main() {

	//控制台的操作
	SetConsoleTitleA("会议聊天室----服务端");

	setsize(70, 20);

	ServerClass Server;
	Server.Bind("192.168.181.1", 12306);
	Server.Listen();


	//循环连接多个客户端
	Server.Communication();

	while (true);

	return 0;

}