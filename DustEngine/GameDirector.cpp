#include "GameDirector.h"

#include "GameBlockAllocator.h"
#include "GameTimeModule.h"
#include "GameGraphicModule.h"

#include "GameMailboxModule.h"

#include <new>
#include <thread>
#include <chrono>
#include <unordered_map>

// 默认场景(用于 Director 空转)
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
	// 当前运行场景
	GameScene*	m_pCurrentScene;

	// 是否退出主循环
	bool		m_bIsQuit;

	// 用于控制游戏帧率的变量
	std::chrono::steady_clock::time_point t1;		// 当帧工作完成前的时间点
	std::chrono::steady_clock::time_point t2;		// 当帧工作完成后的时间点
	std::chrono::steady_clock::time_point t3;		// 延时完成后的时间点
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
		// 开始为当帧计时
		m_pImpl->t1 = std::chrono::steady_clock::now();

		// 更新时间系统
		GameTimeModule::GetInstance().Update();

		// 清除上一帧的图像缓存
		GameGraphicModule::GetInstance().RenderClear();

		// 更新场景系统
		m_pImpl->m_pCurrentScene->Update();

		// 更新窗口图像
		GameGraphicModule::GetInstance().RenderPresent();

		// 计算休眠时间
		m_pImpl->t2 = std::chrono::steady_clock::now();
		m_pImpl->dTarget = std::chrono::duration<double>(1.0 / GameTimeModule::GetInstance().GetFrameRate());
		m_pImpl->dTimeUsed = std::chrono::duration<double>(m_pImpl->t2 - m_pImpl->t1);
		m_pImpl->dSleepTime = std::chrono::duration<double>(m_pImpl->dTarget - m_pImpl->dTimeUsed + m_pImpl->dSleepAdjust);

		// 开始休眠
		if (m_pImpl->dSleepTime > std::chrono::duration<double>(0))
		{
			std::this_thread::sleep_for(m_pImpl->dSleepTime);
		}

		// 根据直到当前的实际帧数和预期帧数的差来调整下一帧的休眠时间
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