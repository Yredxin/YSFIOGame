/*************************************************************************************
 *
 * 文 件 名:	GameAOIPlayer.h
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/3 19:50
 *
*************************************************************************************/
#pragma once
#include <memory>
#include <list>
#include <vector>

class GameAOIPlayer
{
public:
	GameAOIPlayer() = default;
	virtual ~GameAOIPlayer() = default;
	virtual int GetX() = 0;
	virtual int GetY() = 0;
};

class GameAOIWorld
{
	/* 世界范围 */
	int m_xBegin = 0;
	int m_xEnd = 0;
	int m_yBegin = 0;
	int m_yEnd = 0;
	int m_xCount = 0;
	int m_yCount = 0;
	int m_xWidth = 0;
	int m_yWidth = 0;
	GameAOIWorld(int _xBegin, int _xEnd, int _yBegin, int _yEnd, int _xCount, int _yCount);
	~GameAOIWorld() = default;
	static GameAOIWorld AOI;
	std::vector<std::list<GameAOIPlayer*>> m_world;
	inline int GetPlayerInWorldPos(const int& x, const int& y)
	{
		return (x - m_xBegin) / m_xWidth + (y - m_yBegin) / m_yWidth * m_xCount;
	};
public:
	static GameAOIWorld& GetAOI();
	/* 获取周围玩家 */
	std::list<GameAOIPlayer*> GetSrdPlayers(GameAOIPlayer* _self);
	/* 添加玩家到世界 */
	void AddPlayer2World(GameAOIPlayer* _self);
	/* 从世界中删除玩家 */
	void DelPlayer2World(GameAOIPlayer* _self);
};

