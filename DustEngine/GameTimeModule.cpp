#include "GameTimeModule.h"
#include "GameBlockAllocator.h"

#include <unordered_set>

float GameTimer::GetDuration()
{
	return m_fTimeDuration;
}

void GameTimer::SetDuration(float fDuration)
{
	if (fDuration <= 0.0f)
	{
		m_bIsOver = true;
		m_fTimeDuration = 0.0f;
		return;
	}

	m_bIsOver = false;
	m_fTimeDuration = fDuration;
	m_nTimeCreated = GameTimeModule::GetInstance().GetCurrentTime();
}

const std::function<void()>& GameTimer::GetCallback()
{
	return m_funcCallback;
}

void GameTimer::SetCallback(const std::function<void()>& funcCallback)
{
	m_funcCallback = funcCallback;
}

GameTimer::GameTimer(const GameTimer::Def& defTimer)
{
	m_fTimeDuration = defTimer.fTimeDuration;
	m_funcCallback = defTimer.funcCallback;

	m_nTimeCreated = 0;
	m_bIsOver = true;
}

class GameTimeModule::Impl
{
public:
	uint8_t							m_nFrameRate;
	uint64_t						m_nFrameCount;
	std::unordered_set<GameTimer*>	m_setTimers;

public:
	Impl()
	{
		m_nFrameRate = 60;
		m_nFrameCount = 0;
	}
	~Impl()
	{
		for (std::unordered_set<GameTimer*>::iterator iter = m_setTimers.begin();
			iter != m_setTimers.end(); iter++)
		{
			GameTimeModule::GetInstance().DestroyTimer(*iter);
		}
	}
};

void GameTimeModule::Update()
{
	m_pImpl->m_nFrameCount++;

	for (std::unordered_set<GameTimer*>::iterator iter = m_pImpl->m_setTimers.begin();
		iter != m_pImpl->m_setTimers.end(); iter++)
	{
		if ((*iter)->m_bIsOver) continue;

		if (m_pImpl->m_nFrameCount - (*iter)->m_nTimeCreated >= (*iter)->m_fTimeDuration * m_pImpl->m_nFrameRate)
		{
			(*iter)->m_bIsOver = true;
			(*iter)->m_funcCallback();
		}
	}
}

uint64_t GameTimeModule::GetCurrentTime()
{
	return m_pImpl->m_nFrameCount;
}

uint8_t GameTimeModule::GetFrameRate()
{
	return m_pImpl->m_nFrameRate;
}

void GameTimeModule::SetFrameRate(uint8_t nFrameRate)
{
	m_pImpl->m_nFrameRate = nFrameRate;
}

GameTimer* GameTimeModule::CreateTimer(const GameTimer::Def& defTimer)
{
	if (defTimer.fTimeDuration < 0.0f)
	{
		return nullptr;
	}

	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameTimer));
	GameTimer* pTimer = new (pMem) GameTimer(defTimer);

	pTimer->m_bIsOver = false;
	pTimer->m_nTimeCreated = m_pImpl->m_nFrameCount;

	m_pImpl->m_setTimers.insert(pTimer);

	return pTimer;
}

void GameTimeModule::DestroyTimer(GameTimer* pTimer)
{
	if (!pTimer)
	{
		return;
	}

	m_pImpl->m_setTimers.erase(pTimer);

	pTimer->~GameTimer();
	GameBlockAllocator::GetInstance().Free(pTimer, sizeof(GameTimer));
}

GameTimeModule::GameTimeModule()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameTimeModule::~GameTimeModule()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}