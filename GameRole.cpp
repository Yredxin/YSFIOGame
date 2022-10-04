/*************************************************************************************
 *
 * 文 件 名:	GameRole.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/3 19:18
 *
*************************************************************************************/
#include <ysfio_timer.h>
#include <algorithm>
#include <random>
#include "GameRole.h"
#include "GameProtocol.h"
#include "GameTcpChannel.h"
#include "GameExitTask.h"
#include "msg.pb.h"
#include "RandomName.h"
using namespace YSFIO;

static GameExitTask* task = nullptr;

GameMsg* GameRole::CreateLoginMsg()
{
	auto msg = new pb::SyncPid{};
	msg->set_pid(m_iPid);
	msg->set_username(m_sName);
	return new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, msg);
}

GameMsg* GameRole::CreateSelfPostion()
{
	auto msg = new pb::BroadCast{};
	msg->set_pid(m_iPid);
	msg->set_username(m_sName);
	msg->set_tp(2);
	auto pos = msg->mutable_p();
	pos->set_x(m_X);
	pos->set_y(m_Y);
	pos->set_z(m_Z);
	pos->set_v(m_V);
	return new GameMsg(GameMsg::MSG_TYPE_BROADCAST, msg);
}

GameMsg* GameRole::CreataSrdPlayers(std::list<GameAOIPlayer*>& _srdPlayers)
{
	auto msg = new pb::SynPlayers{};
	for (auto& single : _srdPlayers)
	{
		auto role = dynamic_cast<GameRole*>(single);
		auto player = msg->add_ps();
		player->set_pid(role->m_iPid);
		player->set_username(role->m_sName);
		auto pos = player->mutable_p();
		pos->set_x(role->m_X);
		pos->set_y(role->m_Y);
		pos->set_z(role->m_Z);
		pos->set_v(role->m_V);
	}
	return new GameMsg(GameMsg::MSG_TYPE_SRD_POSTION, msg);
}

GameMsg* GameRole::CreateLogoffMsg()
{
	auto msg = new pb::SyncPid{};
	msg->set_pid(m_iPid);
	msg->set_username(m_sName);
	return new GameMsg(GameMsg::MSG_TYPE_LOGOFF_ID_NAME, msg);
}

GameMsg* GameRole::CreateTalkBroadCast(std::string& _content)
{
	auto msg = new pb::BroadCast{};
	msg->set_pid(m_iPid);
	msg->set_username(m_sName);
	msg->set_tp(1);
	msg->set_content(_content);
	return new GameMsg(GameMsg::MSG_TYPE_BROADCAST, msg);
}

void GameRole::ProcTalkMsg(std::string _content)
{
	/* 向周围玩家发送消息 */
	auto& allRole = YSFIOKernel::GetAllRole();
	for (auto& single : allRole)
	{
		auto role = dynamic_cast<GameRole*>(single);
		if (nullptr == role)
		{
			continue;
		}
		auto msg = CreateTalkBroadCast(_content);
		/* 向所有玩家发送聊天消息 */
		YSFIOKernel::SendOut(*msg, role->m_protocol);
	}
}

void GameRole::ProcMovsMsg(float _x, float _y, float _z, float _v)
{
	auto& aoiWorld = GameAOIWorld::GetAOI();
	/* 获取周围玩家 */
	auto oldSrd = aoiWorld.GetSrdPlayers(this);
	/* 删除旧位置 */
	aoiWorld.DelPlayer2World(this);
	m_X = _x;
	m_Y = _y;
	m_Z = _z;
	m_V = _v;
	aoiWorld.AddPlayer2World(this);
	auto newSrd = aoiWorld.GetSrdPlayers(this);
	/* 玩家视野处理 */
	for (auto& single : newSrd)
	{
		/* 新视野玩家 */
		if (std::find(oldSrd.begin(), oldSrd.end(), single) == oldSrd.end())
		{
			ViewShow(dynamic_cast<GameRole*>(single));
		}
	}
	for (auto& single : oldSrd)
	{
		/* 消失视野玩家 */
		if (std::find(newSrd.begin(), newSrd.end(), single) == newSrd.end())
		{
			ViewLast(dynamic_cast<GameRole*>(single));
		}
	}
	/* 给周围玩家发送新位置 */
	auto srdPlayer = aoiWorld.GetSrdPlayers(this);
	for (auto& single : srdPlayer)
	{
		auto pMsg = new pb::BroadCast();
		auto pPos = pMsg->mutable_p();
		pPos->set_x(_x);
		pPos->set_y(_y);
		pPos->set_z(_z);
		pPos->set_v(_v);
		pMsg->set_pid(m_iPid);
		pMsg->set_tp(4);
		pMsg->set_username(m_sName);
		auto role = dynamic_cast<GameRole*>(single);
		YSFIOKernel::SendOut(*(new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg)), role->m_protocol);
	}
}

void GameRole::ViewShow(GameRole* _role)
{
	/* 向自己发送参数玩家的200消息 */
	auto msg = _role->CreateSelfPostion();
	YSFIOKernel::SendOut(*msg, m_protocol);

	/* 向参数玩家发送自己的200消息 */
	msg = CreateSelfPostion();
	YSFIOKernel::SendOut(*msg, _role->m_protocol);
}

void GameRole::ViewLast(GameRole* _role)
{
	/* 向自己发送参数玩家的201消息 */
	auto msg = _role->CreateLogoffMsg();
	YSFIOKernel::SendOut(*msg, m_protocol);

	/* 向参数玩家发送自己的201消息 */
	msg = CreateLogoffMsg();
	YSFIOKernel::SendOut(*msg, _role->m_protocol);
}

void GameRole::AddSelfProtocol(GameProtocol* _protocol)
{
	m_protocol = _protocol;
}

static std::default_random_engine randomEngine(time(nullptr));
GameRole::GameRole() :
	m_protocol{ nullptr },
	m_X{ static_cast<float>(100 + randomEngine() % 50 * 1.0) },
	m_Y{ 0 },
	m_Z{ static_cast<float>(100 + randomEngine() % 50 * 1.0) },
	m_V{ static_cast<float>(randomEngine() % 360 * 1.0) },
	m_iPid{ -1 },
	m_sName{ RandomName::GetName() }
{
}

GameRole::~GameRole()
{
	RandomName::Release(m_sName);
}

bool GameRole::Init()
{
	if (YSFIOKernel::GetAllRole().size() <= 0 && nullptr != task)
	{
		YSFIOTimerTaskProc::DelTask(task);
		task = nullptr;
	}
	/* 将自己添加到游戏世界 */
	auto& aoiWorld = GameAOIWorld::GetAOI();
	m_iPid = m_protocol->GetSelfChannel().GetFd();
	aoiWorld.AddPlayer2World(this);

	/* 向自己发送ID和姓名 */
	auto msg = CreateLoginMsg();
	YSFIOKernel::SendOut(*msg, m_protocol);
	auto srdPlayers = aoiWorld.GetSrdPlayers(this);
	/* 向自己发送周围玩家 */
	msg = CreataSrdPlayers(srdPlayers);
	YSFIOKernel::SendOut(*msg, m_protocol);
	/* 向周围玩家发送自己的位置 */
	for (auto& single : srdPlayers)
	{
		msg = CreateSelfPostion();
		auto role = dynamic_cast<GameRole*>(single);
		YSFIOKernel::SendOut(*msg, role->m_protocol);
	}

	return true;
}

void GameRole::Fini()
{
	auto& aoiWorld = GameAOIWorld::GetAOI();
	/* 向周围玩家发送下线消息 */
	auto srdPlayers = aoiWorld.GetSrdPlayers(this);
	for (auto& single : srdPlayers)
	{
		auto msg = CreateLogoffMsg();
		auto role = dynamic_cast<GameRole*>(single);
		if (nullptr == role)
		{
			continue;
		}
		YSFIOKernel::SendOut(*msg, role->m_protocol);
	}
	/* 将自己从世界中移除 */
	aoiWorld.DelPlayer2World(this);

	/* 最后一个玩家则创建定时任务 */
	if (YSFIOKernel::GetAllRole().size() <= 0)
	{
		task = new GameExitTask{};
		YSFIOTimerTaskProc::AddTask(task);
	}
}

UserData* GameRole::ProcMsg(UserData& _userData)
{
	GET_REF2DATA(MultiMsg, oMult, _userData);
	for (auto& gameMsg : oMult.m_msgs)
	{
		std::cout << "type is" << gameMsg->enMsgType << std::endl;
		std::cout << gameMsg->m_msg->Utf8DebugString() << std::endl;

		switch (gameMsg->enMsgType)
		{
		case GameMsg::MSG_TYPE_CHAT_CONTENT:
			/* 世界聊天消息 */
			ProcTalkMsg(dynamic_cast<pb::Talk*>(gameMsg->m_msg)->content());
			break;
		case GameMsg::MSG_TYPE_NEW_POSTION:
		{
			auto pos = dynamic_cast<pb::Position*>(gameMsg->m_msg);
			/* 移动到新位置 */
			ProcMovsMsg(pos->x(), pos->y(), pos->z(), pos->v());
			break;
		}
		default:
			break;
		}
	}
	return nullptr;
}

int GameRole::GetX()
{
	return (int)m_X;
}

int GameRole::GetY()
{
	return (int)m_Z;
}
