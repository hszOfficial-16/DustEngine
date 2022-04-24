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
	// �ڱ��в����Ƿ�ע���˸���������ûע���򴴽�ʧ��
	if (m_pImpl->m_mapComponentConstructors.find(defComponent.strName) == m_pImpl->m_mapComponentConstructors.end())
		return nullptr;

	// ��������ṩ�Ĺ��캯������������
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