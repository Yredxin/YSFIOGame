/*************************************************************************************
 *
 * 文 件 名:	GameProtocol.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/3 17:31
 *
*************************************************************************************/
#include "GameProtocol.h"
#include "GameMsg.h"
#include "GameTcpChannel.h"
#include "GameRole.h"

using namespace YSFIO;

GameProtocol::GameProtocol() :
	m_channel{ nullptr },
	m_role{ nullptr }
{
}

GameProtocol::~GameProtocol()
{
	if (nullptr != m_role)
	{
		YSFIOKernel::DelRole(m_role);
	}
}

void GameProtocol::AddSelfChannel(GameTcpChannel* _channel)
{
	m_channel = _channel;
}

void GameProtocol::AddSelfRole(GameRole* _role)
{
	m_role = _role;
}

GameTcpChannel& GameProtocol::GetSelfChannel()
{
	return *m_channel;
}

GameRole& GameProtocol::GetSelfRole()
{
	return *m_role;
}

UserData* GameProtocol::Raw2Request(std::string& _input)
{
	m_reqBuf.append(_input);
	auto retLMsg = new MultiMsg{};
	/* 数据为TLV，长度4字节，为防止粘包设计 */
	while (true)
	{
		/* 此项目数据前8字节分别是: 4字节长度，4字节id值 */
		if (8 > m_reqBuf.size())
		{
			break;
		}

		/* 防止字节序错乱，读取长度 */
		int iLen = 0;
		iLen |= m_reqBuf[0] << 0;
		iLen |= m_reqBuf[1] << 8;
		iLen |= m_reqBuf[2] << 16;
		iLen |= m_reqBuf[3] << 24;

		/* 防止字节序错乱，读取id */
		int iId = 0;
		iId |= m_reqBuf[4] << 0;
		iId |= m_reqBuf[5] << 8;
		iId |= m_reqBuf[6] << 16;
		iId |= m_reqBuf[7] << 24;

		/* 一次消息未读取玩 */
		if (m_reqBuf.size() - 8 < iLen)
		{
			break;
		}

		auto msg = new GameMsg{ static_cast<GameMsg::MSG_TYPE>(iId), m_reqBuf.substr(8, iLen) };
		std::cout << msg->m_msg->Utf8DebugString() << std::endl;
		retLMsg->m_msgs.push_back(msg);
		m_reqBuf.erase(0, iLen + 8);
	}
	return retLMsg;
}

std::string* GameProtocol::Response2Raw(UserData& _userData)
{
	GET_REF2DATA(GameMsg, oMsg, _userData);
	std::cout << oMsg.m_msg->Utf8DebugString() << std::endl;
	auto retBuf = new std::string;
	auto strMsg = oMsg.serialize();
	int iId = static_cast<int>(oMsg.enMsgType);
	int iLen = strMsg.size();
	retBuf->push_back((iLen >> 0) & 0xff);
	retBuf->push_back((iLen >> 8) & 0xff);
	retBuf->push_back((iLen >> 16) & 0xff);
	retBuf->push_back((iLen >> 24) & 0xff);

	retBuf->push_back((iId >> 0) & 0xff);
	retBuf->push_back((iId >> 8) & 0xff);
	retBuf->push_back((iId >> 16) & 0xff);
	retBuf->push_back((iId >> 24) & 0xff);

	retBuf->append(strMsg);
	return retBuf;
}

IYSFIORole* GameProtocol::GetProcMsg(UserMsg& _msg)
{
	return m_role;
}

IYSFIOChannel* GameProtocol::GetSender(BytesMsg& _msg)
{
	return m_channel;
}
