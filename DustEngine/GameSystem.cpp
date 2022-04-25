#include "GameSystem.h"
#include "GameBlockAllocator.h"

#include <new>
#include <unordered_map>

class GameSystemFactory::Impl
{
public:
	std::unordered_map<std::string, GameSystemConstructor> m_mapSystemConstructors;

public:
	Impl() = default;
	~Impl() = default;
};

int GameSystemFactory::RegisterSystem(std::string strSystemName, GameSystemConstructor funcSystemConstructor)
{
	m_pImpl->m_mapSystemConstructors[strSystemName] = funcSystemConstructor;
	
	return -1;
}

GameSystem* GameSystemFactory::CreateSystem(const GameSystem::Def& defSystem)
{
	// 在表中查找是否注册了该系统，如果没注册则创建失败
	if (m_pImpl->m_mapSystemConstructors.find(defSystem.strName) == m_pImpl->m_mapSystemConstructors.end())
		return nullptr;

	// 根据系统提供的构造函数来构造该系统
	GameSystem* pSystem = m_pImpl->m_mapSystemConstructors[defSystem.strName](defSystem);
	return pSystem;
}

void GameSystemFactory::DestroySystem(GameSystem* pSystem)
{
	pSystem->~GameSystem();
	GameBlockAllocator::GetInstance().Free(pSystem, pSystem->GetSize());
}

GameSystemFactory::GameSystemFactory()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameSystemFactory::~GameSystemFactory()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}