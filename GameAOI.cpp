/*************************************************************************************
 *
 * 文 件 名:	GameAOIPlayer.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/3 19:50
 *
*************************************************************************************/
#include "GameAOI.h"

GameAOIWorld GameAOIWorld::AOI{ 0, 400, 0, 400, 20, 20 };

GameAOIWorld::GameAOIWorld(int _xBegin, int _xEnd, int _yBegin, int _yEnd, int _xCount, int _yCount) :
	m_xBegin{ _xBegin },
	m_xEnd{ _xEnd },
	m_yBegin{ _yBegin },
	m_yEnd{ _yEnd },
	m_xCount{ _xCount },
	m_yCount{ _yCount },
	m_xWidth{ (m_xEnd - m_xBegin) / m_xCount },
	m_yWidth{ (m_yEnd - m_yBegin) / m_yCount }
{
	for (size_t i = 0; i < m_xCount * m_yCount; ++i)
	{
		m_world.push_back(std::list<GameAOIPlayer*>{});
	}
}

GameAOIWorld& GameAOIWorld::GetAOI()
{
	return AOI;
}

std::list<GameAOIPlayer*> GameAOIWorld::GetSrdPlayers(GameAOIPlayer* _self)
{
	std::list<GameAOIPlayer*> srdPlayers;
	int pos = GetPlayerInWorldPos(_self->GetX(), _self->GetY());
	int xIndex = pos % m_xCount;
	int yIndex = pos / m_xCount;
	/* 左上 */
	if (xIndex > 0 && yIndex > 0)
	{
		std::list<GameAOIPlayer*>& posPlayerL = m_world[pos - 1 - m_xCount];
		srdPlayers.insert(srdPlayers.begin(), posPlayerL.begin(), posPlayerL.end());
	}
	/* 上 */
	if (yIndex > 0)
	{
		std::list<GameAOIPlayer*>& posPlayerL = m_world[pos - m_xCount];
		srdPlayers.insert(srdPlayers.begin(), posPlayerL.begin(), posPlayerL.end());
	}
	/* 右上 */
	if (xIndex < m_xCount - 1 && yIndex > 0)
	{
		std::list<GameAOIPlayer*>& posPlayerL = m_world[pos + 1 - m_xCount];
		srdPlayers.insert(srdPlayers.begin(), posPlayerL.begin(), posPlayerL.end());
	}
	/* 左 */
	if (xIndex > 0)
	{
		std::list<GameAOIPlayer*>& posPlayerL = m_world[pos - 1];
		srdPlayers.insert(srdPlayers.begin(), posPlayerL.begin(), posPlayerL.end());
	}
	/* 中 */
	if (true)
	{
		std::list<GameAOIPlayer*>& posPlayerL = m_world[pos];
		srdPlayers.insert(srdPlayers.begin(), posPlayerL.begin(), posPlayerL.end());
	}
	/* 右 */
	if (xIndex < m_xCount - 1)
	{
		std::list<GameAOIPlayer*>& posPlayerL = m_world[pos + 1];
		srdPlayers.insert(srdPlayers.begin(), posPlayerL.begin(), posPlayerL.end());
	}
	/* 左下 */
	if (xIndex > 0 && yIndex < m_yCount - 1)
	{
		std::list<GameAOIPlayer*>& posPlayerL = m_world[pos - 1 + m_xCount];
		srdPlayers.insert(srdPlayers.begin(), posPlayerL.begin(), posPlayerL.end());
	}
	/* 下 */
	if (yIndex < m_yCount - 1)
	{
		std::list<GameAOIPlayer*>& posPlayerL = m_world[pos + m_xCount];
		srdPlayers.insert(srdPlayers.begin(), posPlayerL.begin(), posPlayerL.end());
	}
	/* 右下 */
	if (xIndex < m_xCount - 1 && yIndex < m_yCount - 1)
	{
		std::list<GameAOIPlayer*>& posPlayerL = m_world[pos + 1 + m_xCount];
		srdPlayers.insert(srdPlayers.begin(), posPlayerL.begin(), posPlayerL.end());
	}
	return srdPlayers;
}

void GameAOIWorld::AddPlayer2World(GameAOIPlayer* _self)
{
	int pos = GetPlayerInWorldPos(_self->GetX(), _self->GetY());
	m_world[pos].push_back(_self);
}

void GameAOIWorld::DelPlayer2World(GameAOIPlayer* _self)
{
	int pos = GetPlayerInWorldPos(_self->GetX(), _self->GetY());
	m_world[pos].remove(_self);
}
