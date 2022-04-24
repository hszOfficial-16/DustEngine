#include "GameComponent.h"

#include <unordered_map>

class GameComponentFactory::Impl
{
public:
	std::unordered_map<std::string, GameComponentConstructor> m_mapComponentConstructors;

public:
	Impl() = default;
	~Impl() = default;
};

int GameComponentFactory::RegisterComponent(std::string strComponentName, GameComponentConstructor funcComponentConstructor)
{
	m_pImpl->m_mapComponentConstructors[strComponentName] = funcComponentConstructor;

	return -1;
}

GameComponent* GameComponentFactory::CreateComponent(const GameComponent::Def& defComponent)
{
	// 在表中查找是否注册了该组件，如果没注册则创建失败
	if (m_pImpl->m_mapComponentConstructors.find(defComponent.strName) == m_pImpl->m_mapComponentConstructors.end())
		return nullptr;

	// 根据组件提供的构造函数来构造该组件
	GameComponent* pComponent = m_pImpl->m_mapComponentConstructors[defComponent.strName](defComponent);
	return pComponent;
}

void GameComponentFactory::DestroyComponent(GameComponent* pComponent)
{
	pComponent->~GameComponent();
	GameBlockAllocator::GetInstance().Free(pComponent, pComponent->GetSize());
}

GameComponentFactory::GameComponentFactory()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameComponentFactory::~GameComponentFactory()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}