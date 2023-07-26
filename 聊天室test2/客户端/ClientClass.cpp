#include "ClientClass.h"
#pragma warning(disable:4996)
#include<windows.h>
using namespace std;

void Chat(ClientClass Client , char* buff , MsgType* msgtype) {
	while (true) {

		memset(buff, 0, sizeof(buff));

		cout << "�����룺" << endl;
		cin >> buff;
			if (strcmp(buff, "//leave") == 0) {//�������//leave,���뿪����
			MsgLeave msgleave(Client.GetRoomID(), Client.GetName());
			msgtype = &msgleave;
			send(Client.GetSocket(), (const char*)msgtype, msgtype->DataLen, 0);//����Ϣ���������
			break;
		}
		
		send(Client.GetSocket(), buff, strlen(buff), NULL);//����Ϣ��������
	
		memset(buff, 0, sizeof(buff));
	}

}

void Send(LPVOID lp) {

	ClientClass Client = *(ClientClass*)lp;//�ͻ��˶���

	MsgType* msgtype;//��Ϣ����

	while (true) {
		char buff[256];

		cout << "+------------------------------------------------+" << endl;
		cout << "+        ��ӭ����C�Ӽ��ֻ���������               +" << endl;
		cout << "+        1.����show�鿴����������                +" << endl;
		cout << "+        2.����join�����������                  +" << endl;
		cout << "+        3.����create����������                  +" << endl;
		cout << "+        4.����exit�˳�����                      +" << endl;
		cout << "+------------------------------------------------+" << endl;
		cout << endl;

		cout << "������ָ�" << endl;
		cin >> buff;

		//��������
		if (strcmp(buff, "create") == 0) {
			//����һ�������ң����ÿͻ��˵����������

			string roomid;
			cout << "�����뷿��ţ�" << endl;
			cin >> roomid;
			cout << "���������ķ����Ϊ��" << roomid << endl;
			Client.SetRoomID(roomid);

			//�����ǳƣ����ÿͻ��˵��ǳ�
			cout << "�������ǳƣ�" << endl;
			string name;
			cin >> name;
			Client.SetName(name);

			MsgCreate msgcreate(roomid, name);
			msgtype = &msgcreate;//����ָ��ָ������

			send(Client.GetSocket(), (const char*)msgtype, msgtype->DataLen, 0);//����Ϣ���������

			//��������
			memset(buff, 0, sizeof(buff));
			Chat(Client, buff, msgtype);
			continue;
		}
		
		//���뷿��
		else if (strcmp(buff, "join") == 0) {
			//����һ�������ң����ÿͻ��˵����������
			cout << "�����뷿��ţ�" << endl;
			string roomid;
			cin >> roomid;
			Client.SetRoomID(roomid);

			//�����ǳƣ����ÿͻ��˵��ǳ�
			cout << "�������ǳƣ�" << endl;
			string name;
			cin >> name;
			Client.SetName(name);

			MsgJoin msgjoin(roomid, name);//����join��
			msgtype = &msgjoin;

			send(Client.GetSocket(), (const char*)msgtype, msgtype->DataLen, 0);//����Ϣ���������

			memset(buff, 0, sizeof(buff));
			//����
			Chat(Client, buff, msgtype);
			continue;
		}

		//��ʾ���з���
		else if (strcmp(buff, "show") == 0) {
			MsgShow msgshow;
			msgtype = &msgshow;
			send(Client.GetSocket(), (const char*)msgtype, msgtype->DataLen, 0);//����Ϣ���������
			continue;
		}

		//�˳�����
		else if (strcmp(buff, "exit") == 0) {
			exit(0);
		}


		memset(buff, 0, 256);//���
	}
	
	
}

void ClientClass::SendData() {

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Send, (LPVOID)this, 0, 0);

}

void ClientClass::RecvData() {
	char buff[256];

	int n = 0;

	int Strlen;

	initgraph(300, 400, 1);//������Ϣ�Ĵ���

	while (true) {
		Strlen = recv(ClntSock, buff, sizeof(buff) - 1, NULL);
		if (Strlen > 0) {
			//��ʾ��ͼ�ν��洰��
			buff[Strlen] = 0;
			outtextxy(1, n * 20, buff);
			n++;
			/*
				�����¼���浽�ĵ�
			*/
		}

		/*
			����յ���������ʧ�ܻ���ȡ�ǳ�ʧ�ܣ�����ô����?
		*/
	}

}




ClientClass::ClientClass() {
	//���������׽���
	ClntSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//����TCP�׽���
	if (ClntSock == SOCKET_ERROR) {
		cout << "�����׽��ִ���ʧ��" << endl;
		return;
	}
	cout << "�����׽��ִ����ɹ�" << endl;

}

ClientClass::~ClientClass() {
	closesocket(ClntSock);
}


void ClientClass::ConnectServer(const char* ip, unsigned short port) {//���ӷ�����

	sockaddr_in sock_in;
	sock_in.sin_family = AF_INET;
	sock_in.sin_port = htons(port);
	sock_in.sin_addr.S_un.S_addr = inet_addr(ip);

	if (connect(ClntSock, (const sockaddr*)&sock_in, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		cout << "���ӷ�����ʧ��" << GetLastError() << endl;
		return;
	}

	cout << "�����˽�������" << endl;

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