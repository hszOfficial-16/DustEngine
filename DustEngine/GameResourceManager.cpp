#include "GameResourceManager.h"
#include "GameBlockAllocator.h"

#include <new>
#include <vector>

class GameResourceManager::Impl
{
public:
	std::vector<GameTexture*>	m_vecTextures;
	std::vector<GameFont*>		m_vecFonts;
	std::vector<GameMusic*>		m_vecMusic;
	std::vector<GameSound*>		m_vecSounds;

public:
	Impl()
	{

	}
	~Impl()
	{

	}
};

void GameResourceManager::AddTexture(GameTexture* pTexture)
{
	m_pImpl->m_vecTextures.push_back(pTexture);
}

GameTexture* GameResourceManager::GetTexture(size_t nIndex)
{
	return m_pImpl->m_vecTextures[nIndex];
}

void GameResourceManager::AddFont(GameFont* pFont)
{
	m_pImpl->m_vecFonts.push_back(pFont);
}

GameFont* GameResourceManager::GetFont(size_t nIndex)
{
	return m_pImpl->m_vecFonts[nIndex];
}

void GameResourceManager::AddMusic(GameMusic* pMusic)
{
	m_pImpl->m_vecMusic.push_back(pMusic);
}

GameMusic* GameResourceManager::GetMusic(size_t nIndex)
{
	return m_pImpl->m_vecMusic[nIndex];
}

void GameResourceManager::AddSound(GameSound* pSound)
{
	m_pImpl->m_vecSounds.push_back(pSound);
}

GameSound* GameResourceManager::GetSound(size_t nIndex)
{
	return m_pImpl->m_vecSounds[nIndex];
}

GameResourceManager::GameResourceManager()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameResourceManager::~GameResourceManager()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}