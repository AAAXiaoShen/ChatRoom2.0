#pragma once
#pragma warning(disable:4996)
#include"../������test2/SocketInit.hpp"
#include <process.h>
#include<Windows.h>
#include<winsock2.h>
#include<WS2tcpip.h>
#include<graphics.h>
#include"MsgType.hpp"
using namespace std;
class ClientClass {
private:

    SocketInit socketinit;//ȷ������Э��

    SOCKET ClntSock;// �����׽���

    string m_RoomID;//����ķ����

    string m_Name;//�ǳ�

public:
    ClientClass();

    ~ClientClass();

    void ConnectServer(const char* ip, unsigned short port);//���ӷ�����

    void SendData();//��������

    void RecvData();//��������

    SOCKET GetSocket();//��ȡsocket

    void SetRoomID(string id);//���ÿͻ������������

    string GetRoomID();//��ȡ�ͻ������������

    void SetName(string name);//���ÿͻ����ǳ�

    string GetName();//��ȡ�ͻ����ǳ�
};

