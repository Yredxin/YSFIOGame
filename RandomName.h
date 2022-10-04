/*************************************************************************************
 *
 * 文 件 名:	RandomName.h
 * 描    述:
 *
 * 版 本 号：  	V1.0
 * 创 建 者：  	Yredxin -- 杨红心
 * 创建时间： 	2022/10/4 14:30
 *
*************************************************************************************/
#include <vector>
#include <list>
#include <string>
#include <memory>

struct FirstName
{
	std::string firstName;
	std::vector<std::string> lastNameList;
};

class RandomName
{
	RandomName() = default;
	bool LoadFile(const std::string& _firstNameFile, const std::string& _lastNameFile);
	std::vector<FirstName*> m_pool;
	static std::shared_ptr<RandomName> randomName;
public:
	~RandomName() = default;
	static bool Init(const std::string& _firstNameFile = "random_first.txt", const std::string& _lastNameFile = "random_last.txt");
	static void Fini();
	static std::string GetName();
	static void Release(std::string& _name);
};