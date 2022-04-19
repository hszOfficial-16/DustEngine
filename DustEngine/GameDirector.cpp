#include "GameDirector.h"
#include "GameBlockAllocator.h"
#include "GameTimeModule.h"

#include <new>
#include <vector>
#include <thread>
#include <chrono>

class GameDirector::Impl
{
public:
	// ��ǰ���г���
	GameScene*	m_pCurrentScene;
	// �Ƿ��˳���ѭ��
	bool		m_bIsQuit;

	std::vector<GameTexture*>	m_vecTextures;
	std::vector<GameFont*>		m_vecFonts;

	std::vector<GameMusic*>		m_vecMusic;
	std::vector<GameSound*>		m_vecSounds;

	std::vector<GameScene::Def> m_vecScenes;

	// ���ڿ�����Ϸ֡�ʵı���
	std::chrono::steady_clock::time_point t1;		// ��֡�������ǰ��ʱ���
	std::chrono::steady_clock::time_point t2;		// ��֡������ɺ��ʱ���
	std::chrono::steady_clock::time_point t3;		// ��ʱ��ɺ��ʱ���
	std::chrono::duration<double> dFrameTime;
	std::chrono::duration<double> dSleepAdjust;
	std::chrono::duration<double> dTarget;
	std::chrono::duration<double> dTimeUsed;
	std::chrono::duration<double> dSleepTime;

public:
	Impl()
	{
		m_pCurrentScene = nullptr;
		m_bIsQuit = false;

		dFrameTime = std::chrono::duration<double>(0.0);	
		dSleepAdjust = std::chrono::duration<double>(0.0);	
		dTarget = std::chrono::duration<double>(0.0);
		dTimeUsed = std::chrono::duration<double>(0.0);
		dSleepTime = std::chrono::duration<double>(0.0);
	}

	~Impl()
	{

	}
};

void GameDirector::Run()
{
	while (!m_pImpl->m_bIsQuit)
	{
		// ��ʼΪ��֡��ʱ
		m_pImpl->t1 = std::chrono::steady_clock::now();

		m_pImpl->m_pCurrentScene->Update();

		GameGraphicModule::GetInstance().RenderClear();
		m_pImpl->m_pCurrentScene->Draw();
		GameGraphicModule::GetInstance().RenderPresent();

		// ��������ʱ��
		m_pImpl->t2 = std::chrono::steady_clock::now();
		m_pImpl->dTarget = std::chrono::duration<double>(1.0 / GameTimeSystem::GetInstance().GetFrameRate());
		m_pImpl->dTimeUsed = std::chrono::duration<double>(m_pImpl->t2 - m_pImpl->t1);
		m_pImpl->dSleepTime = std::chrono::duration<double>(m_pImpl->dTarget - m_pImpl->dTimeUsed + m_pImpl->dSleepAdjust);

		// ��ʼ����
		if (m_pImpl->dSleepTime > std::chrono::duration<double>(0))
		{
			std::this_thread::sleep_for(m_pImpl->dSleepTime);
		}

		// ����ֱ����ǰ��ʵ��֡����Ԥ��֡���Ĳ���������һ֡������ʱ��
		m_pImpl->t3 = std::chrono::steady_clock::now();
		m_pImpl->dFrameTime = m_pImpl->t3 - m_pImpl->t1;
		m_pImpl->dSleepAdjust = 0.9 * m_pImpl->dSleepAdjust + 0.1 * (m_pImpl->dTarget - m_pImpl->dFrameTime);
	}
}

void GameDirector::RegisterScene(const GameScene::Def& defScene)
{
	m_pImpl->m_vecScenes.push_back(defScene);
}

void GameDirector::SwitchScene(size_t nIndex)
{
	if (m_pImpl->m_pCurrentScene)
	{
		GameSceneSystem::GetInstance().DestroyScene(m_pImpl->m_pCurrentScene);
	}
	m_pImpl->m_pCurrentScene = GameSceneSystem::GetInstance().CreateScene(m_pImpl->m_vecScenes[nIndex]);
}

void GameDirector::AddTexture(GameTexture* pTexture)
{
	m_pImpl->m_vecTextures.push_back(pTexture);
}

GameTexture* GameDirector::GetTexture(size_t nIndex)
{
	return m_pImpl->m_vecTextures[nIndex];
}

void GameDirector::AddFont(GameFont* pFont)
{
	m_pImpl->m_vecFonts.push_back(pFont);
}

GameFont* GameDirector::GetFont(size_t nIndex)
{
	return m_pImpl->m_vecFonts[nIndex];
}

void GameDirector::AddMusic(GameMusic* pMusic)
{
	m_pImpl->m_vecMusic.push_back(pMusic);
}

GameMusic* GameDirector::GetMusic(size_t nIndex)
{
	return m_pImpl->m_vecMusic[nIndex];
}

void GameDirector::AddSound(GameSound* pSound)
{
	m_pImpl->m_vecSounds.push_back(pSound);
}

GameSound* GameDirector::GetSound(size_t nIndex)
{
	return m_pImpl->m_vecSounds[nIndex];
}

GameDirector::GameDirector()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameDirector::~GameDirector()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}