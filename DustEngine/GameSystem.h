#ifndef _GAME_SYSTEM_H_
#define _GAME_SYSTEM_H_

#include <string>

class GameScene;

class GameSystem
{
public:
	// ϵͳ���º���
	virtual void Update() = 0;

	// ��ȡϵͳ������
	const std::string& GetName() { return m_strName; }

	// ��ȡϵͳ��������
	GameScene* GetScene() { return m_pScene; }

	// ��ȡϵͳ�ڳ����е���һ��ϵͳ
	GameSystem* GetSceneNext() { return m_pSceneNext; }

protected:
	// ��ȡϵͳ�Ĵ�С
	uint16_t GetSize() { return m_nSize; }

	// ����ϵͳ��������
	void SetScene(GameScene* pScene) { m_pScene = pScene; }

	// ����ϵͳ�ڳ����е���һ��ϵͳ
	void SetSceneNext(GameSystem* pSystem) { m_pSceneNext = pSystem; }

private:
	std::string		m_strName;
	uint16_t		m_nSize;
	GameScene*		m_pScene;
	GameSystem*		m_pSceneNext;

protected:
	GameSystem(std::string strName, uint16_t nSize)
	{
		m_strName = strName;
		m_nSize = nSize;
		m_pScene = nullptr;
		m_pSceneNext = nullptr;
	}
	virtual ~GameSystem() = default;

	friend class GameScene;
};

#endif // !_GAME_SYSTEM_H_