#ifndef _GAME_ENTITY_H_
#define _GAME_ENTITY_H_

#include "GameBlockAllocator.h"

#include <string>
#include <functional>

class GameScene;
class GameComponent;

class GameEntity
{
public:
	struct Def
	{
		std::function<void(GameEntity*)> funcInit;

		Def() : funcInit([](GameEntity*) {}) {}

		Def(std::function<void(GameEntity*)> funcInitIn) :
			funcInit(funcInitIn) {}
	};

	// ��ȡ��ʵ��
	GameEntity* GetParent();
	// ���ø�ʵ��
	void SetParent(GameEntity* pParent);

	// �����ʵ��
	void AddChild(GameEntity* pChild);
	// ɾ����ʵ��
	void DeleteChild(GameEntity* pChild);

	// ��ȡ��ʵ���б�ͷ
	GameEntity* GetChildList();
	// ��ȡ��һ����ʵ��
	GameEntity* GetNext();

	template<typename Component>
	GameComponent* CreateComponent(const typename Component::Def& defComponent)
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Component));
		typename Component::Def* pDefComponent = (typename Component::Def*) & defComponent;
		GameComponent* pComponent = new (pMem) Component(*pDefComponent);
		AddComponent(pComponent);
		return pComponent;
	}

	// �������
	void DestroyComponent(const std::string& strName);

	// ��ȡ���
	GameComponent* GetComponent(const std::string& strName);
	// ��ȡ����б�ͷ
	GameComponent* GetComponentList();

	// ��ȡʵ�������ĳ���
	GameScene* GetScene();
	// ��ȡʵ���ڳ����е���һ��ʵ��
	GameEntity* GetSceneNext();

protected:
	// ����ʵ���ڳ����е���һ��ʵ��
	void SetSceneNext(GameEntity* pEntity);

	// ����ʵ�������ĳ���
	void SetScene(GameScene* pScene);

	// ������
	void AddComponent(GameComponent* pComponent);

private:
	class Impl;
	Impl* m_pImpl;

protected:
	GameEntity(const GameEntity::Def& defEntity);
	~GameEntity();

	friend class GameScene;
};

#endif // !_GAME_ENTITY_H_