#ifndef _GAME_SYSTEM_H_
#define _GAME_SYSTEM_H_

#include <string>

class GameScene;

class GameSystem
{
public:
	// 系统更新函数
	virtual void Update() = 0;

	// 获取系统的名称
	const std::string& GetName() { return m_strName; }

	// 获取系统所属场景
	GameScene* GetScene() { return m_pScene; }

	// 获取系统在场景中的下一个系统
	GameSystem* GetSceneNext() { return m_pSceneNext; }

protected:
	// 获取系统的大小
	uint16_t GetSize() { return m_nSize; }

	// 设置系统所属场景
	void SetScene(GameScene* pScene) { m_pScene = pScene; }

	// 设置系统在场景中的下一个系统
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