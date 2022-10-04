/*************************************************************************************
 *
 * 文 件 名:	GameMsg.h
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/3 17:34
 *
*************************************************************************************/
#pragma once
#include <ysfio.h>
#include <google/protobuf/message.h>

class GameMsg :
	public YSFIO::UserData
{
public:
	google::protobuf::Message* m_msg = nullptr;
	enum MSG_TYPE : int
	{
		// 玩家ID和玩家姓名
		MSG_TYPE_LOGIN_ID_NAME = 1,
		// 聊天内容
		MSG_TYPE_CHAT_CONTENT = 2,
		// 新位置
		MSG_TYPE_NEW_POSTION = 3,
		// 玩家ID，聊天内容/初始位置/动作（预留）/新位置
		MSG_TYPE_BROADCAST = 200,
		// 玩家ID和玩家姓名
		MSG_TYPE_LOGOFF_ID_NAME = 201,
		// 周围玩家们的位置
		MSG_TYPE_SRD_POSTION = 202
	} enMsgType;

	GameMsg(MSG_TYPE _type, google::protobuf::Message* _msg);
	GameMsg(MSG_TYPE _type, std::string _msg);
	~GameMsg();
	std::string serialize();
};

class MultiMsg :
	public YSFIO::UserData
{
public:
	MultiMsg() = default;
	std::list<GameMsg*> m_msgs;
	~MultiMsg();
};

