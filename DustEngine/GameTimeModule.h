#ifndef _GAME_TIME_MODULE_
#define _GAME_TIME_MODULE_

#include <functional>

class GameTimer
{
public:
	struct Def
	{
		float					fTimeDuration;
		std::function<void()>	funcCallback;

		Def() : fTimeDuration(0) {}

		Def(float fTimeDurationIn, const std::function<void()>& funcCallbackIn)
			: fTimeDuration(fTimeDurationIn), funcCallback(funcCallbackIn) {}
	};

	// ��ȡ��ʱ���ĳ���ʱ��(��λ: ��)
	float	GetDuration();
	// ���ü�ʱ���ĳ���ʱ��(��λ: ��)
	void	SetDuration(float fTimeDuration);

	const std::function<void()>& GetCallback();
	void SetCallback(const std::function<void()>& funcCallback);

private:
	float					m_fTimeDuration;
	std::function<void()>	m_funcCallback;

	uint64_t				m_nTimeCreated;
	bool					m_bIsOver;

private:
	GameTimer(const GameTimer::Def& defTimer);
	~GameTimer() = default;

	friend class GameTimeSystem;
};

class GameTimeSystem
{
public:
	// ά��ʱ��ϵͳ
	void		Update();

	// ��ȡ��Ϸ�����������ھ�����֡
	uint64_t	GetCurrentTime();

	// ��ȡ��Ϸ֡��
	uint8_t		GetFrameRate();
	// ������Ϸ֡��
	void		SetFrameRate(uint8_t nFrameRate);

	// ������ʱ��
	GameTimer*	CreateTimer(const GameTimer::Def& defTimer);
	// ���ټ�ʱ��
	void		DestroyTimer(GameTimer* pTimer);

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameTimeSystem();
	GameTimeSystem(const GameTimeSystem&) = delete;
	GameTimeSystem& operator=(const GameTimeSystem&) = delete;
	static GameTimeSystem& GetInstance()
	{
		static GameTimeSystem instance;
		return instance;
	}
private:
	GameTimeSystem();
};

#endif // !_GAME_TIME_MODULE_