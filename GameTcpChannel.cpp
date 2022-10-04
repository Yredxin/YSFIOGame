/*************************************************************************************
 *
 * 文 件 名:	GameTcpChannel.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/3 17:11
 *
*************************************************************************************/
#include "GameTcpChannel.h"
#include "GameProtocol.h"
#include "GameRole.h"
using namespace YSFIO;

YSFIOTcpData* GameTcpFactory::CreateTcpDataChannel(int _fd)
{
	auto channel = new GameTcpChannel{ _fd };
	auto protocol = new GameProtocol{};
	auto role = new GameRole{};
	channel->AddSelfProtocol(protocol);
	protocol->AddSelfChannel(channel);
	protocol->AddSelfRole(role);
	role->AddSelfProtocol(protocol);
	YSFIO::YSFIOKernel::AddProtocol(protocol);
	YSFIO::YSFIOKernel::AddRole(role);
	return channel;
}

GameTcpChannel::GameTcpChannel(int _fd) :
	YSFIO::YSFIOTcpData{ _fd },
	m_protocol{ nullptr }
{
}

GameTcpChannel::~GameTcpChannel()
{
	if (nullptr != m_protocol)
	{
		YSFIOKernel::DelProtocol(m_protocol);
	}
}

void GameTcpChannel::AddSelfProtocol(GameProtocol* _protocol)
{
	m_protocol = _protocol;
}

AYSFIOHandle* GameTcpChannel::GetInputNextStage(BytesMsg& _input)
{
	return m_protocol;
}
