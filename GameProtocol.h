/*************************************************************************************
 *
 * 文 件 名:	GameProtocol.h
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/3 17:31
 *
*************************************************************************************/
#pragma once
#include <string>
#include "ysfio.h"

class GameTcpChannel;
class GameRole;
class GameProtocol :
	public YSFIO::IYSFIOProtocol
{
	GameTcpChannel* m_channel;
	GameRole* m_role;
	std::string m_reqBuf;
public:
	GameProtocol();
	~GameProtocol();
	void AddSelfChannel(GameTcpChannel* _channel);
	void AddSelfRole(GameRole* _role);
	GameTcpChannel& GetSelfChannel();
	GameRole& GetSelfRole();

private:
	// 通过 IYSFIOProtocol 继承
	virtual YSFIO::UserData* Raw2Request(std::string& _input) override;
	virtual std::string* Response2Raw(YSFIO::UserData& _userData) override;
	virtual YSFIO::IYSFIORole* GetProcMsg(YSFIO::UserMsg& _msg) override;
	virtual YSFIO::IYSFIOChannel* GetSender(YSFIO::BytesMsg& _msg) override;
};

