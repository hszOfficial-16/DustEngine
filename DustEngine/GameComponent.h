#ifndef _GAME_COMPONENT_H_
#define _GAME_COMPONENT_H_

#include "GameBlockAllocator.h"

#include <new>
#include <string>

class GameEntity;
class GameScene;

class GameComponent
{
public:
	// ��ȡ���������
	const std::string& GetName() { return m_strName; }

	// ��ȡ���������ʵ��
	GameEntity* GetEntity() { return m_pEntity; }

	// ��ȡ����ڳ����е���һ�����(������ͬ���͵����)
	GameComponent* GetSceneNext() { return m_pSceneNext; }

	// ��ȡ�����ʵ���е���һ�����(���ڱ���ʵ������)
	GameComponent* GetEntityNext() { return m_pEntityNext; }

protected:
	// ��ȡ����Ĵ�С
	uint16_t GetSize() { return m_nSize; }

	// �������������ʵ��
	void SetEntity(GameEntity* pEntity) { m_pEntity = pEntity; }

	// ��������ڳ����е���һ�����
	void SetSceneNext(GameComponent* pComponent) { m_pSceneNext = pComponent; }

	// ���������ʵ���е���һ�����
	void SetEntityNext(GameComponent* pComponent) { m_pEntityNext = pComponent; }

private:
	std::string		m_strName;		// �������
	uint16_t		m_nSize;		// �����С
	GameEntity*		m_pEntity;		// �������ʵ��
	GameComponent*	m_pSceneNext;	// ����ڳ����е���һ�����
	GameComponent*  m_pEntityNext;	// �����ʵ���е���һ�����

protected:
	GameComponent(std::string strName, uint16_t nSize)
	{
		m_strName = strName;
		m_nSize = nSize;
		m_pEntity = nullptr;
		m_pSceneNext = nullptr;
	}
	virtual ~GameComponent() = default;

	friend class GameEntity;
	friend class GameScene;
	friend class GameFactory;
};

#endif // !_GAME_COMPONENT_H_