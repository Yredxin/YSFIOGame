/*************************************************************************************
 *
 * 文 件 名:	GameMsg.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/3 17:34
 *
*************************************************************************************/
#include "GameMsg.h"
#include "msg.pb.h"

GameMsg::GameMsg(MSG_TYPE _type, google::protobuf::Message* _msg) :
	enMsgType{ _type },
	m_msg{ _msg }
{
}

GameMsg::GameMsg(MSG_TYPE _type, std::string _msg) :
	enMsgType{ _type }
{
	switch (enMsgType)
	{
	case GameMsg::MSG_TYPE_LOGIN_ID_NAME:
		m_msg = new pb::SyncPid{};
		break;
	case GameMsg::MSG_TYPE_CHAT_CONTENT:
		m_msg = new pb::Talk{};
		break;
	case GameMsg::MSG_TYPE_NEW_POSTION:
		m_msg = new pb::Position{};
		break;
	case GameMsg::MSG_TYPE_BROADCAST:
		m_msg = new pb::BroadCast{};
		break;
	case GameMsg::MSG_TYPE_LOGOFF_ID_NAME:
		m_msg = new pb::SyncPid{};
		break;
	case GameMsg::MSG_TYPE_SRD_POSTION:
		m_msg = new pb::SynPlayers{};
		break;
	default:
		break;
	}
	m_msg->ParseFromString(_msg);
}

GameMsg::~GameMsg()
{
	if (nullptr != m_msg)
	{
		delete m_msg;
		m_msg = nullptr;
	}
}

std::string GameMsg::serialize()
{
	std::string buf;
	m_msg->SerializePartialToString(&buf);
	return buf;
}

MultiMsg::~MultiMsg()
{
	for (auto& msg : m_msgs)
	{
		if (nullptr != msg)
		{
			delete msg;
			msg = nullptr;
		}
	}
}
