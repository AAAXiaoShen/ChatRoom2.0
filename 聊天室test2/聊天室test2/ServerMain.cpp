#include<iostream>
#include "ServerClass.h"
#pragma warning(disable:4996)
using namespace std;

//���ÿ���̨�Ĵ�С
void setsize(int col, int row) {
	char cmd[64];
	sprintf(cmd, "mode con cols=%d lines=%d", col, row);
	system(cmd);
}

int main() {

	//����̨�Ĳ���
	SetConsoleTitleA("����������----�����");

	setsize(70, 20);

	ServerClass Server;
	Server.Bind("192.168.181.1", 12306);
	Server.Listen();


	//ѭ�����Ӷ���ͻ���
	Server.Communication();

	while (true);

	return 0;

}