/*************************************************************************************
 *
 * 文 件 名:	main.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/3 14:39
 *
*************************************************************************************/
#include <iostream>
#include <string>
#include <ysfio.h>
#include <ysfio_tcp.h>
#include <ysfio_timer.h>
#include "GameTcpChannel.h"
#include "RandomName.h"

int main()
{
	RandomName::Init();
	YSFIO::YSFIOKernel::Init();
	YSFIO::YSFIOKernel::AddChannel(new YSFIO::YSFIOTimerMngChannel{});
	YSFIO::YSFIOKernel::AddChannel(new YSFIO::YSFIOTcpListen{ 8989, new GameTcpFactory{} });
	YSFIO::YSFIOKernel::Run();
	YSFIO::YSFIOKernel::Fini();
	RandomName::Fini();
	return 0;
}