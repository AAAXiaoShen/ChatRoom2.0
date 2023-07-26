#pragma once
#include<iostream>
#include<string>
using namespace std;
const int PACKET_MAXSIZE = 1024;
const int ROOM_NUM = 30;
enum MSG_YTPE {
	MSG_SHOW,//չʾ���з���
	MSG_JOIN,
	MSG_CREATE,
	MSG_LEAVE
};

//��Ϣ����
class MsgType {//��Ϣ����
public:
	int MsgType;//��Ϣ����
	int DataLen;//��Ϣ����
};

//չʾ���з���
class MsgShow :public MsgType {
public:
	MsgShow() {
		MsgType = MSG_SHOW;
		DataLen = sizeof(MsgShow);
	}
};


//���뷿��
class MsgJoin :public MsgType {
private:
	string m_RoomID;
	string m_Name;
public:
	MsgJoin(string roomid, string name) {
		MsgType = MSG_JOIN;
		DataLen = sizeof(MsgJoin);
		m_RoomID = roomid;
		m_Name = name;
	}
	string GetRoomID() {
		return m_RoomID;
	}
	string GetName() {
		return m_Name;
	}

};

//��������
class MsgCreate :public MsgType {
private:
	string m_RoomID;
	string m_Name;
public:
	MsgCreate(string roomid, string name) {
		m_RoomID = roomid;
		MsgType = MSG_CREATE;
		DataLen = sizeof(MsgCreate);
		m_Name = name;
	}
	string GetRoomID() {
		return m_RoomID;
	}
	string GetName() {
		return m_Name;
	}
};

//�뿪����
class MsgLeave :public MsgType {
private:
	string m_RoomID;
	string m_Name;
public:
	MsgLeave(string roomid, string name) {
		m_RoomID = roomid;
		MsgType = MSG_LEAVE;
		DataLen = sizeof(MsgLeave);
		m_Name = name;
	}
	string GetRoomID() {
		return m_RoomID;
	}
	string GetName() {
		return m_Name;
	}
};

