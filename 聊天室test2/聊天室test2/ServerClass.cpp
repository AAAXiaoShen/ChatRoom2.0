#include "ServerClass.h"

using namespace std;

HANDLE HMute = CreateMutex(NULL,FALSE,NULL);//互斥锁

const int MaxNum = 100;

int CountNum = 0;//统计当前的客户端数量

set<string> RoomIDs;//存放所有房间号

class ClntObject {//存放客户端的socket，房间号，昵称
private:

	SOCKET m_Socket;//客户端的socket

	string m_RoomID;//所属房间号

	string m_Name;//昵称

public:
	ClntObject() {

	}
	ClntObject(SOCKET Socket) {
		m_Socket = Socket;
	}

	~ClntObject() {

	}

	SOCKET GetSocket() {//获取socket
		return m_Socket;
	}

	void SetRoomID(string id) {//设置客户端所属房间号
		m_RoomID = id;
	}

	string GetRoomID() {//获取客户端所属房间号
		return m_RoomID;
	}

	void SetName(string name) {//设置客户端昵称
		m_Name = name;
	}

	string GetName() {//获取客户端昵称
		return m_Name;
	}
};

ClntObject Clnts[MaxNum];//缓冲区，存放客户端对象的数组

unsigned WINAPI HandleClnt(int index) {//传入的参数是客户端对象在客户端对象数组中的索引
	char buff[256] = { 0 };//接收来自客户端的消息

	char temp1[256] = { 0 };//临时数组1，用于存放buff中的信息

	char temp2[256] = { 0 };//临时数组2，存放发给其他客户端的信息

	char temp3[256];//临时数组3，存放该客户加入或离开的消息

	int Len = -1;

	while (true) {
		Len = recv(Clnts[index].GetSocket(), buff, sizeof(buff) - 1, NULL);
		if (Len > 0) {

			buff[Len] = 0;//将最后一位的下一位置为0，才能正确读取数据

			memset(temp1, 0, 256);//清空

			strcpy_s(temp1, buff);

			MsgType* msg = (MsgType*)buff;//将接收到的消息转为MsgType类

			switch (msg->MsgType) {

			case MSG_SHOW: {

				string IDs;

				IDs.append("当前存在的房间号有：");

				//将所有房间号添加到IDs中
				for (auto it : RoomIDs) {
					IDs.append(it + "   ");
				}

				send(Clnts[index].GetSocket(), IDs.c_str(), sizeof(IDs), 0);//IDs发给客户端
			}
				break;

			case MSG_CREATE: {

				MsgCreate* create = (MsgCreate*)msg;//将父类MsgType指针转为子类指针

				if (RoomIDs.find(create->GetRoomID()) == RoomIDs.end()) {//如果没有该房间，则新建

					RoomIDs.insert(create->GetRoomID());//添加房间号到房间号容器中

					send(Clnts[index].GetSocket(), "新建房间成功", sizeof("新建房间成功"), 0);//发给客户端

					Clnts[index].SetRoomID(create->GetRoomID());//记录房间号

				}
				else {//房号重复则报错

					send(Clnts[index].GetSocket(), "新建房间失败", sizeof("加入房间成功"), 0);//发给客户端

				}
				for (int i = 0; i < CountNum; i++) {
					//如果在同一房间&&昵称无重复，则取昵称成功
					if (create->GetRoomID() == Clnts[index].GetRoomID() && create->GetName() != Clnts[index].GetName()) {

						send(Clnts[index].GetSocket(), "取昵称成功", sizeof("取昵称成功"), 0);//发给该客户端

						Clnts[index].SetName(create->GetName());//记录昵称

						cout << create->GetName() << "创建了" << create->GetRoomID() << "房间" << endl;//在服务端显示成功
					}
				}
				if (Clnts[index].GetName() == "") {//昵称重复则报错
					send(Clnts[index].GetSocket(), "取昵称失败，换一个吧", sizeof("取昵称失败，换一个吧"), 0);//发给客户端
				}
			}
				break;

			case MSG_JOIN: {

				MsgJoin* join = (MsgJoin*)msg;//将父类MsgType指针转为子类指针

				if (RoomIDs.find(join->GetRoomID()) != RoomIDs.end()) {//如果有该房间，则加入

					RoomIDs.insert(join->GetRoomID());//添加房间号到房间号容器中

					send(Clnts[index].GetSocket(), "加入房间成功", sizeof("加入房间成功"), 0);//发给客户端

					Clnts[index].SetRoomID(join->GetRoomID());//记录房间号

				}
				else {//房号重复则报错

					send(Clnts[index].GetSocket(), "加入房间失败", sizeof("加入房间成功"), 0);//发给客户端

				}
				for (int i = 0; i < CountNum; i++) {
					//如果在同一房间&&昵称无重复，则取昵称成功
					if (join->GetRoomID() == Clnts[index].GetRoomID() && join->GetName() != Clnts[index].GetName()) {

						send(Clnts[index].GetSocket(), "取昵称成功", sizeof("取昵称成功"), 0);//发给该客户端

						Clnts[index].SetName(join->GetName());//记录昵称

						cout << join->GetName() << "加入了" << join->GetRoomID() << "房间" << endl;//在服务端显示成功

						memset(temp3, 0, 256);//清空

						sprintf_s(temp3, "%s加入房间", Clnts[index].GetName().c_str());//temp3是发给该房间所有客户端的消息

						//将该客户端加入的消息发给该房间的所有成员
							if (join->GetRoomID() == Clnts[i].GetRoomID()) {//如果属于同一个房间，就发送信息
								send(Clnts[i].GetSocket(), temp3, strlen(temp3), NULL);
							}

					}
				}
				if (Clnts[index].GetName() == "") {//昵称重复则报错
					send(Clnts[index].GetSocket(), "取昵称失败，换一个吧", sizeof("取昵称失败，换一个吧"), 0);//发给客户端
				}

			}
				break;

			case MSG_LEAVE: {

				MsgLeave* leave = (MsgLeave*)msg;//将父类MsgType指针转为子类指针

				//消费者
				WaitForSingleObject(HMute, INFINITE);

				cout << Clnts[index].GetRoomID() << "的" << Clnts[index].GetName() << "离开了该房间" << endl;

				memset(temp3, 0, 256);//清空

				sprintf_s(temp3, "%s房间的%s离开该房间", Clnts[index].GetRoomID().c_str(), Clnts[index].GetName().c_str());//temp3是发给该房间所有客户端的消息

				
				WaitForSingleObject(HMute, INFINITE);
				//将该客户端离开的消息发给该房间的所有成员
				for (int j = 0; j < CountNum; j++) {
					if (Clnts[index].GetRoomID() == Clnts[j].GetRoomID()) {//如果属于同一个房间，就发送信息
						send(Clnts[j].GetSocket(), temp3, strlen(temp3), NULL);
					}
				}
				//消费者
				for (int j = 0; j < CountNum; j++) {
					Clnts[index] = Clnts[index + 1];//从客户端数组中移除该客户端
				}
				CountNum--;

				ReleaseMutex(HMute);

		}
				break;

			default://默认则不是指令，是发送的信息

				memset(temp2, 0, 256);//清空

				sprintf_s(temp2, "%s：%s", Clnts[index].GetName().c_str(), temp1);//temp2是发给该房间所有客户端的消息
				cout << "房间" << Clnts[index].GetRoomID() << "  " << temp2 << endl;

				//temp2发给该房间的所有客户端			
				for (int j = 0; j < CountNum; j++) {
					WaitForSingleObject(HMute, INFINITE);
					if (Clnts[index].GetRoomID() == Clnts[j].GetRoomID()) {//如果属于同一个房间，就发送信息
						send(Clnts[j].GetSocket(), temp2, strlen(temp2), NULL);
					}
					ReleaseMutex(HMute);
				}
				break;
			}

			}

		}

		return 0;
}

ServerClass::ServerClass() {
	//创建监听套接字
	ServSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建TCP套接字
	if (ServSock == SOCKET_ERROR) {
		cout << "监听套接字创建失败" << endl;
		return;
	}

}

ServerClass::~ServerClass() {
	closesocket(ServSock);
	delete[] Clnts;
}

void ServerClass::Bind(const char* ip, unsigned short port) {

	sockaddr_in sock_in;// sockaddr_in是保存IPv4地址信息的结构体
	sock_in.sin_family = AF_INET;
	sock_in.sin_port = htons(port);//端口
	sock_in.sin_addr.S_un.S_addr = inet_addr(ip);//ip,值为INADDR_ANY时设置的是服务器的IP地址

	int ret = bind(ServSock, (const sockaddr*)&sock_in, sizeof(sock_in));//绑定(ip 与 socket绑定)

	if (ret == SOCKET_ERROR) {
		cout << "绑定套接字失败" << endl;
		return;
	}
	cout << "绑定套接字成功" << endl;
}

void ServerClass::Listen() {

	if (listen(ServSock, 10) == SOCKET_ERROR) {//监听，此处第二个参数是说 内核为 此套接字维护的最大链接个数
		cout << "监听套接字失败" << endl;
		return;
	}
	cout << "监听套接字成功" << endl;
}

void ServerClass::Communication() {
	for (int i = 0; i < MaxNum; i++) {
		//接收客户端的连接
		sockaddr_in ClntAddr;
		int clen = sizeof(ClntAddr);

		//连接客户端，生产者
		SOCKET ClntSock = accept(ServSock, (sockaddr*)&ClntAddr, &clen);

		if (ClntSock == SOCKET_ERROR) {
			cout << "接收客户端失败" << GetLastError() << endl;
			closesocket(ClntSock);
			return;
		}
		cout << "连接客户端" << i << "成功" << endl;


		//将客户端的socket放入服务端，缓冲区
		WaitForSingleObject(HMute, INFINITE);
		ClntObject* Clnt = new ClntObject(ClntSock);//生产者
		Clnts[i] = *Clnt;//缓冲区
		CountNum++;
		ReleaseMutex(HMute);


		//为客户端分配线程，该线程负责接收消息

		HANDLE hTheard = (HANDLE)_beginthreadex(0, 0, (_beginthreadex_proc_type)HandleClnt, (void*)i, 0, 0);
	}
		
}

SOCKET ServerClass::GetSocket() {
	return ServSock;
}