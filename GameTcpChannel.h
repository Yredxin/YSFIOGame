/*************************************************************************************
 *
 * 文 件 名:	GameTcpChannel.h
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/3 17:11
 *
*************************************************************************************/
#pragma once
#include "ysfio.h"
#include "ysfio_tcp.h"

class GameProtocol;
class GameTcpChannel :
	public YSFIO::YSFIOTcpData
{
	GameProtocol* m_protocol;
public:
	GameTcpChannel(int _fd);
	~GameTcpChannel();
	void AddSelfProtocol(GameProtocol* _protocol);
	// 通过 YSFIOTcpData 继承
	virtual YSFIO::AYSFIOHandle* GetInputNextStage(YSFIO::BytesMsg& _input) override;
};

class GameTcpFactory :
	public YSFIO::IYSFIOTcpFactory
{
	// 通过 IYSFIOTcpFactory 继承
	virtual YSFIO::YSFIOTcpData* CreateTcpDataChannel(int _fd) override;
};


