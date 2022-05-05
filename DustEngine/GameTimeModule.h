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

	// 获取计时器的持续时间(单位: 秒)
	float GetDuration();
	// 设置计时器的持续时间(单位: 秒)
	void SetDuration(float fTimeDuration);

	// 获取计时器的回调函数
	const std::function<void()>& GetCallback();
	// 设置计时器的回调函数
	void SetCallback(const std::function<void()>& funcCallback);

private:
	float					m_fTimeDuration;
	std::function<void()>	m_funcCallback;

	uint64_t				m_nTimeCreated;
	bool					m_bIsOver;

private:
	GameTimer(const GameTimer::Def& defTimer);
	~GameTimer() = default;

	friend class GameTimeModule;
};

class GameTimeModule
{
public:
	// 维护时间系统
	void Update();

	// 获取游戏从启动到现在经过的帧
	uint64_t GetCurrentTime();

	// 获取游戏帧率
	uint8_t	GetFrameRate();
	// 设置游戏帧率
	void SetFrameRate(uint8_t nFrameRate);

	// 创建计时器
	GameTimer* CreateTimer(const GameTimer::Def& defTimer);
	// 销毁计时器
	void DestroyTimer(GameTimer* pTimer);

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameTimeModule();
	GameTimeModule(const GameTimeModule&) = delete;
	GameTimeModule& operator=(const GameTimeModule&) = delete;
	static GameTimeModule& GetInstance()
	{
		static GameTimeModule instance;
		return instance;
	}
private:
	GameTimeModule();
};

#endif // !_GAME_TIME_MODULE_