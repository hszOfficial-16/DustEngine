#ifndef _GAME_MAILBOX_MODULE_H_
#define _GAME_MAILBOX_MODULE_H_

#include <string>
#include <initializer_list>

// 键值对条件
struct GamePair
{
	std::string strKey;
	std::string strValue;

	// 较为高效的哈希
	struct Hasher
	{
		size_t Combine(size_t seed, std::string strValue) const;

		size_t operator()(const GamePair& pair) const
		{
			size_t seed = 0;
			return Combine(Combine(seed, pair.strKey), pair.strValue);
		}
	};

	struct Equalizer
	{
		bool operator()(const GamePair& former, const GamePair& latter) const
		{
			return (former.strKey == latter.strKey && latter.strValue == latter.strValue);
		}
	};

	GamePair() : strKey("default"), strValue("default") {}

	GamePair(std::string strKeyIn, std::string strValueIn) :
		strKey(strKeyIn), strValue(strValueIn) {}

	~GamePair() = default;
};

// 消息(键值对集)
class GameMessage
{
public:
	struct Def
	{
		std::initializer_list<GamePair> listPair;
	};

	std::string& operator[](const std::string& strKey);

protected:
	uint16_t& GetReferenceCount();

private:
	class Impl;
	Impl* m_pImpl;

private:
	GameMessage(const Def& defMessage);
	~GameMessage();

	friend class GameMailbox;
	friend class GameMailboxManager;
};

class GameMailbox
{
public:
	struct Def
	{
		std::string	strName;
		std::initializer_list<GamePair> listRequirement;

		Def() = default;

		Def(const std::string& strNameIn, const std::initializer_list<GamePair>& listRequirementIn) :
			strName(strNameIn), listRequirement(listRequirementIn) {}
	};
	
	// 查询信箱的某个条件
	const std::string& operator[](const std::string& strKey);

	// 获取该信箱的名称
	const std::string& GetName();

	// 获取位于 nIndex 下的消息
	GameMessage* GetMessage(size_t nIndex);

	// 获取当前消息的总数量
	size_t GetMessageCount();

	// 清空信箱中的所有消息
	void Clear();

private:
	class Impl;
	Impl* m_pImpl;

private:
	GameMailbox(const Def& defMailbox);
	~GameMailbox();

	friend class GameMailboxManager;
};

class GameMailboxManager
{
public:
	// 发布一个消息
	void Publish(const std::initializer_list<GamePair>& listPair);

	// 创建一个信箱并缓存
	GameMailbox* CreateMailbox(const GameMailbox::Def& defMailbox);
	// 销毁一个信箱并清除其缓存
	void DestroyMailbox(GameMailbox* pMailbox);

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

#endif // !_GAME_MAILBOX_MODULE_H_