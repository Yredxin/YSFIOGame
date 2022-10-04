/*************************************************************************************
 *
 * 文 件 名:	RandomName.cpp
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/4 14:39
 *
*************************************************************************************/
#include <fstream>
#include <iostream>
#include <random>
#include "RandomName.h"

static std::default_random_engine randEngine(time(NULL));

std::shared_ptr<RandomName> RandomName::randomName = nullptr;

bool RandomName::LoadFile(const std::string& _firstNameFile, const std::string& _lastNameFile)
{
	std::ifstream first{ _firstNameFile };
	std::ifstream last{ _lastNameFile };

	std::string lastName;
	std::vector<std::string> vLast;
	while (std::getline(last, lastName))
	{
		vLast.push_back(lastName);
	}

	std::string firstName;
	while (std::getline(first, firstName))
	{
		auto vName = new FirstName{};
		vName->firstName = firstName;
		vName->lastNameList = vLast;
		m_pool.push_back(vName);
	}
	return true;
}

bool RandomName::Init(const std::string& _firstNameFile, const std::string& _lastNameFile)
{
	if (nullptr == randomName)
	{
		randomName.reset(new RandomName{});
	}
	return randomName->LoadFile(_firstNameFile, _lastNameFile);
}
void RandomName::Fini()
{
	for (auto& name : randomName->m_pool)
	{
		if (nullptr != name)
		{
			delete name;
			name = nullptr;
		}
	}
}


std::string RandomName::GetName()
{
	if (randomName->m_pool.size() <= 0)
	{
		return std::string();
	}
	/* 姓随机 */
	auto firstPos = randEngine() % randomName->m_pool.size();
	auto& firstName = randomName->m_pool[firstPos];
	auto first = firstName->firstName;
	/* 名随机 */
	auto last = firstName->lastNameList[randEngine() % firstName->lastNameList.size()];
	/* 如果姓对应容器为空，则删除姓 */
	if (firstName->lastNameList.size() <= 0)
	{
		delete randomName->m_pool[firstPos];
		randomName->m_pool.erase(randomName->m_pool.begin() + firstPos);
	}
	return first + " " + last;
}
void RandomName::Release(std::string& _name)
{
	auto splitPos = _name.find(" ");
	if (_name.npos == splitPos)
	{
		return;
	}
	auto first = _name.substr(0, splitPos);
	auto last = _name.substr(splitPos + 1, _name.size() - splitPos - 1);

	for (auto& firstName : randomName->m_pool)
	{
		if (first == firstName->firstName)
		{
			firstName->lastNameList.push_back(last);
			return;
		}
	}

	auto vName = new FirstName{};
	vName->firstName = first;
	vName->lastNameList.push_back(last);
	randomName->m_pool.push_back(vName);
}