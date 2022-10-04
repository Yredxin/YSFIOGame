/*************************************************************************************
 *
 * 文 件 名:	GameTimer.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/4 11:06
 *
*************************************************************************************/
#include "GameExitTask.h"
#include <ysfio.h>

void GameExitTask::Proc()
{
	YSFIO::YSFIOKernel::Exit();
}

int GameExitTask::TimerSec()
{
	return 10;
}
