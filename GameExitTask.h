/*************************************************************************************
 *
 * 文 件 名:	GameTimer.h
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/4 11:06
 *
*************************************************************************************/
#pragma once
#include <ysfio_timer.h>

class GameExitTask :
	public YSFIO::IYSFIOTask
{
	// 通过 IYSFIOTask 继承
	virtual void Proc() override;
	virtual int TimerSec() override;
};

