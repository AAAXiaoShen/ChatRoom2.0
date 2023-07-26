#include "ClientClass.h"
#pragma warning(disable:4996)
#include<windows.h>
using namespace std;

void Chat(ClientClass Client , char* buff , MsgType* msgtype) {
	while (true) {

		memset(buff, 0, sizeof(buff));

		cout << "请输入：" << endl;
		cin >> buff;
			if (strcmp(buff, "//leave") == 0) {//如果输入//leave,则离开房间
			MsgLeave msgleave(Client.GetRoomID(), Client.GetName());
			msgtype = &msgleave;
			send(Client.GetSocket(), (const char*)msgtype, msgtype->DataLen, 0);//将消息发给服务端
			break;
		}
		
		send(Client.GetSocket(), buff, strlen(buff), NULL);//发信息给服务器
	
		memset(buff, 0, sizeof(buff));
	}

}

void Send(LPVOID lp) {

	ClientClass Client = *(ClientClass*)lp;//客户端对象

	MsgType* msgtype;//消息基类

	while (true) {
		char buff[256];

		cout << "+------------------------------------------------+" << endl;
		cout << "+        欢迎来到C加加林会议聊天室               +" << endl;
		cout << "+        1.输入show查看所有聊天室                +" << endl;
		cout << "+        2.输入join加入该聊天室                  +" << endl;
		cout << "+        3.输入create创建聊天室                  +" << endl;
		cout << "+        4.输入exit退出聊天                      +" << endl;
		cout << "+------------------------------------------------+" << endl;
		cout << endl;

		cout << "请输入指令：" << endl;
		cin >> buff;

		//创建房间
		if (strcmp(buff, "create") == 0) {
			//加入一个聊天室，设置客户端的所属房间号

			string roomid;
			cout << "请输入房间号：" << endl;
			cin >> roomid;
			cout << "您所创建的房间号为：" << roomid << endl;
			Client.SetRoomID(roomid);

			//输入昵称，设置客户端的昵称
			cout << "请输入昵称：" << endl;
			string name;
			cin >> name;
			Client.SetName(name);

			MsgCreate msgcreate(roomid, name);
			msgtype = &msgcreate;//父类指针指向子类

			send(Client.GetSocket(), (const char*)msgtype, msgtype->DataLen, 0);//将消息发给服务端

			//加入聊天
			memset(buff, 0, sizeof(buff));
			Chat(Client, buff, msgtype);
			continue;
		}
		
		//加入房间
		else if (strcmp(buff, "join") == 0) {
			//加入一个聊天室，设置客户端的所属房间号
			cout << "请输入房间号：" << endl;
			string roomid;
			cin >> roomid;
			Client.SetRoomID(roomid);

			//输入昵称，设置客户端的昵称
			cout << "请输入昵称：" << endl;
			string name;
			cin >> name;
			Client.SetName(name);

			MsgJoin msgjoin(roomid, name);//创建join类
			msgtype = &msgjoin;

			send(Client.GetSocket(), (const char*)msgtype, msgtype->DataLen, 0);//将消息发给服务端

			memset(buff, 0, sizeof(buff));
			//聊天
			Chat(Client, buff, msgtype);
			continue;
		}

		//显示所有房间
		else if (strcmp(buff, "show") == 0) {
			MsgShow msgshow;
			msgtype = &msgshow;
			send(Client.GetSocket(), (const char*)msgtype, msgtype->DataLen, 0);//将消息发给服务端
			continue;
		}

		//退出程序
		else if (strcmp(buff, "exit") == 0) {
			exit(0);
		}


		memset(buff, 0, 256);//清空
	}
	
	
}

void ClientClass::SendData() {

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Send, (LPVOID)this, 0, 0);

}

void ClientClass::RecvData() {
	char buff[256];

	int n = 0;

	int Strlen;

	initgraph(300, 400, 1);//接收消息的窗口

	while (true) {
		Strlen = recv(ClntSock, buff, sizeof(buff) - 1, NULL);
		if (Strlen > 0) {
			//显示到图形界面窗口
			buff[Strlen] = 0;
			outtextxy(1, n * 20, buff);
			n++;
			/*
				聊天记录保存到文档
			*/
		}

		/*
			如果收到创建房间失败或者取昵称失败，该怎么处理?
		*/
	}

}




ClientClass::ClientClass() {
	//创建监听套接字
	ClntSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建TCP套接字
	if (ClntSock == SOCKET_ERROR) {
		cout << "监听套接字创建失败" << endl;
		return;
	}
	cout << "监听套接字创建成功" << endl;

}

ClientClass::~ClientClass() {
	closesocket(ClntSock);
}


void ClientClass::ConnectServer(const char* ip, unsigned short port) {//连接服务器

	sockaddr_in sock_in;
	sock_in.sin_family = AF_INET;
	sock_in.sin_port = htons(port);
	sock_in.sin_addr.S_un.S_addr = inet_addr(ip);

	if (connect(ClntSock, (const sockaddr*)&sock_in, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		cout << "连接服务器失败" << GetLastError() << endl;
		return;
	}

	cout << "与服务端建立连接" << endl;

}

SOCKET ClientClass::GetSocket() {
	return ClntSock;
}

void ClientClass::SetRoomID(string id) {
	m_RoomID = id;
}

string ClientClass::GetRoomID() {
	return m_RoomID;
}

void ClientClass::SetName(string name) {
	m_Name = name;
}

string ClientClass::GetName() {
	return m_Name;
}