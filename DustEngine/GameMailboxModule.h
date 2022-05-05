#ifndef _GAME_MAILBOX_MODULE_H_
#define _GAME_MAILBOX_MODULE_H_

#include <string>
#include <initializer_list>

// ��ֵ������
struct GamePair
{
	std::string strKey;
	std::string strValue;

	// ��Ϊ��Ч�Ĺ�ϣ
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

// ��Ϣ(��ֵ�Լ�)
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
	
	// ��ѯ�����ĳ������
	const std::string& operator[](const std::string& strKey);

	// ��ȡ�����������
	const std::string& GetName();

	// ��ȡλ�� nIndex �µ���Ϣ
	GameMessage* GetMessage(size_t nIndex);

	// ��ȡ��ǰ��Ϣ��������
	size_t GetMessageCount();

	// ��������е�������Ϣ
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
	// ����һ����Ϣ
	void Publish(const std::initializer_list<GamePair>& listPair);

	// ����һ�����䲢����
	GameMailbox* CreateMailbox(const GameMailbox::Def& defMailbox);
	// ����һ�����䲢����仺��
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