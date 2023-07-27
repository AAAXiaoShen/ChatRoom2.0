#pragma once
#include<iostream>
#include<string>
using namespace std;
const int PACKET_MAXSIZE = 1024;
const int ROOM_NUM = 30;
enum MSG_YTPE {
	MSG_SHOW,//展示所有房间
	MSG_JOIN,
	MSG_CREATE,
	MSG_LEAVE
};

//消息基类
class MsgType {//消息基类
public:
	int MsgType;//消息类型
	int DataLen;//消息长度
};

//展示所有房间
class MsgShow :public MsgType {
public:
	MsgShow() {
		MsgType = MSG_SHOW;
		DataLen = sizeof(MsgShow);
	}
};


//加入房间
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

//创建房间
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

//离开房间
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

