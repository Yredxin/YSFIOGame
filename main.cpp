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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "GameTcpChannel.h"
#include "RandomName.h"
#include "GameRole.h"
#include "GameExitTask.h"

void SetDaemon()
{
	auto pid = fork();
	if (0 > pid)
	{
		exit(-1);
	}
	else if (0 < pid)
	{
		/* 父进程 */
		exit(0);
	}
	/* 子进程 */
	setsid();
	auto nullfd = open("/dev/null", O_RDWR);
	if (nullfd >= 0)
	{
		dup2(nullfd, 0);
		dup2(nullfd, 1);
		dup2(nullfd, 2);
		close(nullfd);
	}
}

int main()
{
	SetDaemon();
	RandomName::Init();
	YSFIO::YSFIOKernel::Init();
	YSFIO::YSFIOKernel::AddChannel(new YSFIO::YSFIOTimerMngChannel{});
	if (nullptr == GameRole::task)
	{
		GameRole::task = new GameExitTask{};
	}
	YSFIO::YSFIOTimerTaskProc::AddTask(GameRole::task);
	YSFIO::YSFIOKernel::AddChannel(new YSFIO::YSFIOTcpListen{ 8989, new GameTcpFactory{} });
	YSFIO::YSFIOKernel::Run();
	YSFIO::YSFIOKernel::Fini();
	RandomName::Fini();
	return 0;
}