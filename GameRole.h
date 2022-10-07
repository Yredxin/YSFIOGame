/*************************************************************************************
 *
 * 文 件 名:	GameRole.h
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/3 19:18
 *
*************************************************************************************/
#pragma once
#include <ysfio.h>
#include <list>
#include "GameAOI.h"
#include "GameMsg.h"
#include "GameExitTask.h"

class GameProtocol;
class GameRole :
	public YSFIO::IYSFIORole, public GameAOIPlayer
{
	float m_X;				/* X轴坐标 */
	float m_Y;				/* Y轴坐标 */
	float m_Z;				/* Z轴坐标 */
	float m_V;				/* 视角 */
	int m_iPid;				/* 人物编号 */
	std::string m_sName;	/* 人物名称 */
	GameProtocol* m_protocol;

	/* 生成登录消息 */
	GameMsg* CreateLoginMsg();
	/* 生成自己位置消息 */
	GameMsg* CreateSelfPostion();
	/* 生成周围玩家消息 */
	GameMsg* CreataSrdPlayers(std::list<GameAOIPlayer*>& _srdPlayers);
	/* 生成退出登录消息 */
	GameMsg* CreateLogoffMsg();
	/* 生成聊天内容消息 */
	GameMsg* CreateTalkBroadCast(std::string& _content);
	/* 处理世界聊天 */
	void ProcTalkMsg(std::string _content);
	/* 处理新位置 */
	void ProcMovsMsg(float _x, float _y, float _z, float _v);
	/* 相互移除视野 */
	void ViewShow(GameRole* _role);
	/* 相互添加视野 */
	void ViewLast(GameRole* _role);
public:
	/* 定时器任务 */
	static GameExitTask* task;
	void AddSelfProtocol(GameProtocol* _protocol);
	GameRole();
	~GameRole();
	// 通过 IYSFIORole 继承

	/* 人物上线 */
	virtual bool Init() override;

	/* 人物下线 */
	virtual void Fini() override;

	virtual YSFIO::UserData* ProcMsg(YSFIO::UserData& _userData) override;


	// 通过 GameAOIPlayer 继承
	virtual int GetX() override;

	virtual int GetY() override;

};

