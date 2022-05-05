#include "GameDirector.h"

#include "GameBlockAllocator.h"
#include "GameTimeModule.h"
#include "GameGraphicModule.h"

#include "GameMailboxModule.h"

#include <new>
#include <thread>
#include <chrono>
#include <unordered_map>

// Ĭ�ϳ���(���� Director ��ת)
class GameDefaultScene : public GameScene
{
private:
	GameDefaultScene() : GameScene({}) {}
	~GameDefaultScene() = default;

public:
	static GameScene* Create()
	{
		return new GameDefaultScene;
	}
};

class GameDirector::Impl
{
public:
	// ��ǰ���г���
	GameScene*	m_pCurrentScene;

	// �Ƿ��˳���ѭ��
	bool		m_bIsQuit;

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
		m_bIsQuit = false;
		m_pCurrentScene = GameDefaultScene::Create();

		dFrameTime = std::chrono::duration<double>(0.0);	
		dSleepAdjust = std::chrono::duration<double>(0.0);	
		dTarget = std::chrono::duration<double>(0.0);
		dTimeUsed = std::chrono::duration<double>(0.0);
		dSleepTime = std::chrono::duration<double>(0.0);
	}
	~Impl() = default;
};

void GameDirector::Run()
{
	while (!m_pImpl->m_bIsQuit)
	{
		// ��ʼΪ��֡��ʱ
		m_pImpl->t1 = std::chrono::steady_clock::now();

		// ����ʱ��ϵͳ
		GameTimeModule::GetInstance().Update();

		// �����һ֡��ͼ�񻺴�
		GameGraphicModule::GetInstance().RenderClear();

		// ���³���ϵͳ
		m_pImpl->m_pCurrentScene->Update();

		// ���´���ͼ��
		GameGraphicModule::GetInstance().RenderPresent();

		// ��������ʱ��
		m_pImpl->t2 = std::chrono::steady_clock::now();
		m_pImpl->dTarget = std::chrono::duration<double>(1.0 / GameTimeModule::GetInstance().GetFrameRate());
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

void GameDirector::ShiftScene(const GameScene::Def& defScene)
{
	if (m_pImpl->m_pCurrentScene)
	{
		m_pImpl->m_pCurrentScene->~GameScene();
		GameBlockAllocator::GetInstance().Free(m_pImpl->m_pCurrentScene, sizeof(GameScene));
	}
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameScene));
	m_pImpl->m_pCurrentScene = new (pMem) GameScene(defScene);
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