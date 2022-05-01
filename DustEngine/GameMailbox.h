#ifndef _GAME_MAILBOX_H_
#define _GAME_MAILBOX_H_

#include <string>
#include <initializer_list>

class GameMessage
{
public:
	// ����ĳ�����µ�ֵ
	const std::string& operator[](const std::string strKey);

	// ��Ӽ�ֵ��
	void AddPair(const std::string& strKey, const std::string& strValue);

	// ɾ����ֵ��
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
	// �����±��ȡ�����е���Ϣ
	GameMessage* GetMessage(size_t nIndex);

	// �����������
	void Clear();

private:
	class Impl;
	Impl* m_pImpl;

private:
	// ����һ����ע���б�����Ϣ������
	GameMailbox(const std::initializer_list<std::pair<std::string, std::string>>& listPairs);
	~GameMailbox();

	friend class GameFactory;
	friend class GameMailboxManager;
};

class GameMailboxManager
{
public:
	// ������Ϣ�������߻Ὣ��Ͷ�ݵ�����������������
	void Publish(GameMessage* pMessage);
	
	// ע��һ������
	void Register(GameMailbox* pMailbox);

	// ע��һ������
	void Unregister(GameMailbox* pMailbox);

	// ��������ɸѡ�������Ϣ
	void Cache(GameMailbox* pMailbox);

	// ���ĳһ����������л���
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