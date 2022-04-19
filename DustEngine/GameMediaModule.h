#ifndef _GAME_MEDIA_MODULE_H_
#define _GAME_MEDIA_MODULE_H_

#include "GameData.h"

class GameMusic
{
public:
	enum class Type
	{
		WAV = 0,
		MP3 = 1,
		OGG = 2,
		MOD = 3,
		MID = 4,
		UNKNOWN = 5
	};

	enum class Fading
	{
		NONE = 0,
		OUT = 1,
		IN = 2
	};

	Type GetType();

private:
	class Impl;
	Impl* m_pImpl;

private:
	GameMusic();
	~GameMusic();

	friend class GameMediaModule;
};

class GameSound
{
private:
	class Impl;
	Impl* m_pImpl;

private:
	GameSound();
	~GameSound();

	friend class GameMediaModule;
};

class GameMediaModule
{
public:
	GameMusic* LoadMusic(GameData& dataMusic);
	GameSound* LoadSound(const GameData& dataSound);

	void				PlayMusic(GameMusic* pGameMusic, int nLoop);
	void				PlayMusic(GameMusic* pGameMusic, int nLoop, int msFading);
	void				StopMusic();
	void				StopMusic(int msFading);
	void				PauseMusic();
	void				ResumeMusic();
	void				RewindMusic();
	bool				CheckMusicPlaying();
	bool				CheckMusicPaused();
	GameMusic::Fading	GetMusicFadingType();

	void				SetMusicVolume(int nVol);
	int					GetMusicVolume();

	void				PlaySound(GameSound* pGameSound, int nLoop);

	void				SetSoundVolume(GameSound* pGameSound, int nVol);
	int					GetSoundVolume(GameSound* pGameSound);

	void				DestroyMusic(GameMusic* pGameMusic);
	void				DestroySound(GameSound* pGameSound);

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameMediaModule();
	GameMediaModule(const GameMediaModule&) = delete;
	GameMediaModule& operator=(const GameMediaModule&) = delete;
	static GameMediaModule& GetInstance()
	{
		static GameMediaModule instance;
		return instance;
	}

private:
	GameMediaModule();
};

#endif // !_GAME_MEDIA_MODULE_H_