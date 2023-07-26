#include "ServerClass.h"

using namespace std;

HANDLE HMute = CreateMutex(NULL,FALSE,NULL);//������

const int MaxNum = 100;

int CountNum = 0;//ͳ�Ƶ�ǰ�Ŀͻ�������

set<string> RoomIDs;//������з����

class ClntObject {//��ſͻ��˵�socket������ţ��ǳ�
private:

	SOCKET m_Socket;//�ͻ��˵�socket

	string m_RoomID;//���������

	string m_Name;//�ǳ�

public:
	ClntObject() {

	}
	ClntObject(SOCKET Socket) {
		m_Socket = Socket;
	}

	~ClntObject() {

	}

	SOCKET GetSocket() {//��ȡsocket
		return m_Socket;
	}

	void SetRoomID(string id) {//���ÿͻ������������
		m_RoomID = id;
	}

	string GetRoomID() {//��ȡ�ͻ������������
		return m_RoomID;
	}

	void SetName(string name) {//���ÿͻ����ǳ�
		m_Name = name;
	}

	string GetName() {//��ȡ�ͻ����ǳ�
		return m_Name;
	}
};

ClntObject Clnts[MaxNum];//����������ſͻ��˶��������

unsigned WINAPI HandleClnt(int index) {//����Ĳ����ǿͻ��˶����ڿͻ��˶��������е�����
	char buff[256] = { 0 };//�������Կͻ��˵���Ϣ

	char temp1[256] = { 0 };//��ʱ����1�����ڴ��buff�е���Ϣ

	char temp2[256] = { 0 };//��ʱ����2����ŷ��������ͻ��˵���Ϣ

	char temp3[256];//��ʱ����3����Ÿÿͻ�������뿪����Ϣ

	int Len = -1;

	while (true) {
		Len = recv(Clnts[index].GetSocket(), buff, sizeof(buff) - 1, NULL);
		if (Len > 0) {

			buff[Len] = 0;//�����һλ����һλ��Ϊ0��������ȷ��ȡ����

			memset(temp1, 0, 256);//���

			strcpy_s(temp1, buff);

			MsgType* msg = (MsgType*)buff;//�����յ�����ϢתΪMsgType��

			switch (msg->MsgType) {

			case MSG_SHOW: {

				string IDs;

				IDs.append("��ǰ���ڵķ�����У�");

				//�����з������ӵ�IDs��
				for (auto it : RoomIDs) {
					IDs.append(it + "   ");
				}

				send(Clnts[index].GetSocket(), IDs.c_str(), sizeof(IDs), 0);//IDs�����ͻ���
			}
				break;

			case MSG_CREATE: {

				MsgCreate* create = (MsgCreate*)msg;//������MsgTypeָ��תΪ����ָ��

				if (RoomIDs.find(create->GetRoomID()) == RoomIDs.end()) {//���û�и÷��䣬���½�

					RoomIDs.insert(create->GetRoomID());//��ӷ���ŵ������������

					send(Clnts[index].GetSocket(), "�½�����ɹ�", sizeof("�½�����ɹ�"), 0);//�����ͻ���

					Clnts[index].SetRoomID(create->GetRoomID());//��¼�����

				}
				else {//�����ظ��򱨴�

					send(Clnts[index].GetSocket(), "�½�����ʧ��", sizeof("���뷿��ɹ�"), 0);//�����ͻ���

				}
				for (int i = 0; i < CountNum; i++) {
					//�����ͬһ����&&�ǳ����ظ�����ȡ�ǳƳɹ�
					if (create->GetRoomID() == Clnts[index].GetRoomID() && create->GetName() != Clnts[index].GetName()) {

						send(Clnts[index].GetSocket(), "ȡ�ǳƳɹ�", sizeof("ȡ�ǳƳɹ�"), 0);//�����ÿͻ���

						Clnts[index].SetName(create->GetName());//��¼�ǳ�

						cout << create->GetName() << "������" << create->GetRoomID() << "����" << endl;//�ڷ������ʾ�ɹ�
					}
				}
				if (Clnts[index].GetName() == "") {//�ǳ��ظ��򱨴�
					send(Clnts[index].GetSocket(), "ȡ�ǳ�ʧ�ܣ���һ����", sizeof("ȡ�ǳ�ʧ�ܣ���һ����"), 0);//�����ͻ���
				}
			}
				break;

			case MSG_JOIN: {

				MsgJoin* join = (MsgJoin*)msg;//������MsgTypeָ��תΪ����ָ��

				if (RoomIDs.find(join->GetRoomID()) != RoomIDs.end()) {//����и÷��䣬�����

					RoomIDs.insert(join->GetRoomID());//��ӷ���ŵ������������

					send(Clnts[index].GetSocket(), "���뷿��ɹ�", sizeof("���뷿��ɹ�"), 0);//�����ͻ���

					Clnts[index].SetRoomID(join->GetRoomID());//��¼�����

				}
				else {//�����ظ��򱨴�

					send(Clnts[index].GetSocket(), "���뷿��ʧ��", sizeof("���뷿��ɹ�"), 0);//�����ͻ���

				}
				for (int i = 0; i < CountNum; i++) {
					//�����ͬһ����&&�ǳ����ظ�����ȡ�ǳƳɹ�
					if (join->GetRoomID() == Clnts[index].GetRoomID() && join->GetName() != Clnts[index].GetName()) {

						send(Clnts[index].GetSocket(), "ȡ�ǳƳɹ�", sizeof("ȡ�ǳƳɹ�"), 0);//�����ÿͻ���

						Clnts[index].SetName(join->GetName());//��¼�ǳ�

						cout << join->GetName() << "������" << join->GetRoomID() << "����" << endl;//�ڷ������ʾ�ɹ�

						memset(temp3, 0, 256);//���

						sprintf_s(temp3, "%s���뷿��", Clnts[index].GetName().c_str());//temp3�Ƿ����÷������пͻ��˵���Ϣ

						//���ÿͻ��˼������Ϣ�����÷�������г�Ա
							if (join->GetRoomID() == Clnts[i].GetRoomID()) {//�������ͬһ�����䣬�ͷ�����Ϣ
								send(Clnts[i].GetSocket(), temp3, strlen(temp3), NULL);
							}

					}
				}
				if (Clnts[index].GetName() == "") {//�ǳ��ظ��򱨴�
					send(Clnts[index].GetSocket(), "ȡ�ǳ�ʧ�ܣ���һ����", sizeof("ȡ�ǳ�ʧ�ܣ���һ����"), 0);//�����ͻ���
				}

			}
				break;

			case MSG_LEAVE: {

				MsgLeave* leave = (MsgLeave*)msg;//������MsgTypeָ��תΪ����ָ��

				//������
				WaitForSingleObject(HMute, INFINITE);

				cout << Clnts[index].GetRoomID() << "��" << Clnts[index].GetName() << "�뿪�˸÷���" << endl;

				memset(temp3, 0, 256);//���

				sprintf_s(temp3, "%s�����%s�뿪�÷���", Clnts[index].GetRoomID().c_str(), Clnts[index].GetName().c_str());//temp3�Ƿ����÷������пͻ��˵���Ϣ

				
				WaitForSingleObject(HMute, INFINITE);
				//���ÿͻ����뿪����Ϣ�����÷�������г�Ա
				for (int j = 0; j < CountNum; j++) {
					if (Clnts[index].GetRoomID() == Clnts[j].GetRoomID()) {//�������ͬһ�����䣬�ͷ�����Ϣ
						send(Clnts[j].GetSocket(), temp3, strlen(temp3), NULL);
					}
				}
				//������
				for (int j = 0; j < CountNum; j++) {
					Clnts[index] = Clnts[index + 1];//�ӿͻ����������Ƴ��ÿͻ���
				}
				CountNum--;

				ReleaseMutex(HMute);

		}
				break;

			default://Ĭ������ָ��Ƿ��͵���Ϣ

				memset(temp2, 0, 256);//���

				sprintf_s(temp2, "%s��%s", Clnts[index].GetName().c_str(), temp1);//temp2�Ƿ����÷������пͻ��˵���Ϣ
				cout << "����" << Clnts[index].GetRoomID() << "  " << temp2 << endl;

				//temp2�����÷�������пͻ���			
				for (int j = 0; j < CountNum; j++) {
					WaitForSingleObject(HMute, INFINITE);
					if (Clnts[index].GetRoomID() == Clnts[j].GetRoomID()) {//�������ͬһ�����䣬�ͷ�����Ϣ
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
	//���������׽���
	ServSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//����TCP�׽���
	if (ServSock == SOCKET_ERROR) {
		cout << "�����׽��ִ���ʧ��" << endl;
		return;
	}

}

ServerClass::~ServerClass() {
	closesocket(ServSock);
	delete[] Clnts;
}

void ServerClass::Bind(const char* ip, unsigned short port) {

	sockaddr_in sock_in;// sockaddr_in�Ǳ���IPv4��ַ��Ϣ�Ľṹ��
	sock_in.sin_family = AF_INET;
	sock_in.sin_port = htons(port);//�˿�
	sock_in.sin_addr.S_un.S_addr = inet_addr(ip);//ip,ֵΪINADDR_ANYʱ���õ��Ƿ�������IP��ַ

	int ret = bind(ServSock, (const sockaddr*)&sock_in, sizeof(sock_in));//��(ip �� socket��)

	if (ret == SOCKET_ERROR) {
		cout << "���׽���ʧ��" << endl;
		return;
	}
	cout << "���׽��ֳɹ�" << endl;
}

void ServerClass::Listen() {

	if (listen(ServSock, 10) == SOCKET_ERROR) {//�������˴��ڶ���������˵ �ں�Ϊ ���׽���ά����������Ӹ���
		cout << "�����׽���ʧ��" << endl;
		return;
	}
	cout << "�����׽��ֳɹ�" << endl;
}

void ServerClass::Communication() {
	for (int i = 0; i < MaxNum; i++) {
		//���տͻ��˵�����
		sockaddr_in ClntAddr;
		int clen = sizeof(ClntAddr);

		//���ӿͻ��ˣ�������
		SOCKET ClntSock = accept(ServSock, (sockaddr*)&ClntAddr, &clen);

		if (ClntSock == SOCKET_ERROR) {
			cout << "���տͻ���ʧ��" << GetLastError() << endl;
			closesocket(ClntSock);
			return;
		}
		cout << "���ӿͻ���" << i << "�ɹ�" << endl;


		//���ͻ��˵�socket�������ˣ�������
		WaitForSingleObject(HMute, INFINITE);
		ClntObject* Clnt = new ClntObject(ClntSock);//������
		Clnts[i] = *Clnt;//������
		CountNum++;
		ReleaseMutex(HMute);


		//Ϊ�ͻ��˷����̣߳����̸߳��������Ϣ

		HANDLE hTheard = (HANDLE)_beginthreadex(0, 0, (_beginthreadex_proc_type)HandleClnt, (void*)i, 0, 0);
	}
		
}

SOCKET ServerClass::GetSocket() {
	return ServSock;
}