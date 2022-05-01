#ifndef _GAME_MAILBOX_H_
#define _GAME_MAILBOX_H_

#include <string>
#include <initializer_list>

class GameMessage
{
public:
	// 访问某个键下的值
	const std::string& operator[](const std::string strKey);

	// 添加键值对
	void AddPair(const std::string& strKey, const std::string& strValue);

	// 删除键值对
	void DeletePair(const std::string& strKey);

private:
	class Impl;
	Impl* m_pImpl;

private:
	GameMessage();
	~GameMessage();

	friend class GameFactory;
	friend class GameMailboxManager;
};

class GameMailbox
{
public:
	// 根据下标获取信箱中的消息
	GameMessage* GetMessage(size_t nIndex);

	// 清空整个信箱
	void Clear();

private:
	class Impl;
	Impl* m_pImpl;

private:
	// 创建一个关注该列表中消息的信箱
	GameMailbox(const std::initializer_list<std::pair<std::string, std::string>>& listPairs);
	~GameMailbox();

	friend class GameFactory;
	friend class GameMailboxManager;
};

class GameMailboxManager
{
public:
	// 推送消息，管理者会将其投递到符合条件的信箱中
	void Publish(GameMessage* pMessage);
	
	// 注册一个信箱
	void Register(GameMailbox* pMailbox);

	// 注销一个信箱
	void Unregister(GameMailbox* pMailbox);

	// 缓存用于筛选信箱的信息
	void Cache(GameMailbox* pMailbox);

	// 清空某一个信箱的所有缓存
	void ClearCache(GameMailbox* pMailbox);

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameMailboxManager();
	GameMailboxManager(const GameMailboxManager&) = delete;
	GameMailboxManager& operator=(const GameMailboxManager&) = delete;
	static GameMailboxManager& GetInstance()
	{
		static GameMailboxManager instance;
		return instance;
	}

private:
	GameMailboxManager();
};

#endif // !_GAME_EVENT_MODULE_H_