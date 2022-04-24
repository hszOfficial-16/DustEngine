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
	class Def
	{
	private:
		std::string		strName;
		uint16_t		nSize;

	public:
		Def(std::string strNameIn, uint16_t nSizeIn)
			: strName(strNameIn), nSize(nSizeIn) {}

		friend class GameComponent;
		friend class GameComponentFactory;
	};

	// ��ȡ���������
	const std::string& GetName() { return m_strName; }

	// ��ȡ���������ʵ��
	GameEntity* GetEntity() { return m_pEntity; }

	// ��ȡ����ڳ����е���һ�����(���ڱ������)
	GameComponent* GetSceneNext() { return m_pSceneNext; }

protected:
	// ��ȡ����Ĵ�С
	uint16_t GetSize() { return m_nSize; }

	// ��������ڳ����е���һ�����
	void SetSceneNext(GameComponent* pComponent) { m_pSceneNext = pComponent; }

	// �������������ʵ��
	void SetEntity(GameEntity* pEntity) { m_pEntity = pEntity; }

private:
	std::string		m_strName;		// �������
	uint16_t		m_nSize;		// �����С
	GameEntity*		m_pEntity;		// �������ʵ��
	GameComponent*	m_pSceneNext;	// ����ڳ����е���һ�����

protected:
	GameComponent(const GameComponent::Def& defComponent)
	{
		m_strName = defComponent.strName;
		m_nSize = defComponent.nSize;
		m_pEntity = nullptr;
		m_pSceneNext = nullptr;
	}
	virtual ~GameComponent() = default;

	friend class GameEntity;
	friend class GameScene;
	friend class GameEntityFactory;
	friend class GameComponentFactory;
};

// ������캯��
typedef GameComponent* (*GameComponentConstructor)(const GameComponent::Def&);

// ������칤��
class GameComponentFactory
{
public:
	// �ṩ��Ϣע��һ�������
	int RegisterComponent(std::string strComponentName, GameComponentConstructor funcComponentConstructor);

	// ���������Ϣ����һ�����
	GameComponent* CreateComponent(const GameComponent::Def& defComponent);

	// ����һ�����
	void DestroyComponent(GameComponent* pComponent);

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameComponentFactory();
	GameComponentFactory(const GameComponentFactory&) = delete;
	GameComponentFactory& operator=(const GameComponentFactory&) = delete;
	static GameComponentFactory& GetInstance()
	{
		static GameComponentFactory instance;
		return instance;
	}
private:
	GameComponentFactory();
};

#endif // !_GAME_COMPONENT_H_