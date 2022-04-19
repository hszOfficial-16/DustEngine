#include "GameMediaModule.h"
#include "GameBlockAllocator.h"

#include <SDL.h>
#include <SDL_mixer.h>

#include <new>

class GameMusic::Impl
{
public:
	Mix_Music* m_pMusic;
	void* m_pData;

public:
	Impl()
	{
		m_pMusic = nullptr;
		m_pData = nullptr;
	}
	~Impl()
	{
		if (m_pMusic)
		{
			Mix_FreeMusic(m_pMusic);
		}
		if (m_pData)
		{
			delete[] m_pData;
		}
	}
};

GameMusic::Type GameMusic::GetType()
{
	switch (Mix_GetMusicType(m_pImpl->m_pMusic))
	{
	case MUS_WAV:	return GameMusic::Type::WAV; break;
	case MUS_MP3:	return GameMusic::Type::MP3; break;
	case MUS_OGG:	return GameMusic::Type::OGG; break;
	case MUS_MOD:	return GameMusic::Type::MOD; break;
	case MUS_MID:	return GameMusic::Type::MID; break;
	default:		return GameMusic::Type::UNKNOWN; break;
	}
}

GameMusic::GameMusic()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameMusic::~GameMusic()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}

class GameSound::Impl
{
public:
	Mix_Chunk* m_pSound;

public:
	Impl()
	{
		m_pSound = nullptr;
	}
	~Impl()
	{
		if (m_pSound)
		{
			Mix_FreeChunk(m_pSound);
		}
	}
};

GameSound::GameSound()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameSound::~GameSound()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}

class GameMediaModule::Impl
{
public:
	Impl()
	{
		Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	}
	~Impl()
	{
		Mix_CloseAudio();
		Mix_Quit();
	}
};

GameMusic* GameMediaModule::LoadMusic(GameData& dataMusic)
{
	Mix_Music* pMusic = nullptr;

	if (pMusic = Mix_LoadMUS_RW(SDL_RWFromMem(dataMusic.pData, dataMusic.nLength), 1))
	{
		dataMusic.bShouldDeleteData = false;

		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameMusic));
		GameMusic* pGameMusic = new (pMem) GameMusic();
		pGameMusic->m_pImpl->m_pMusic = pMusic;
		pGameMusic->m_pImpl->m_pData = dataMusic.pData;

		return pGameMusic;
	}
	return nullptr;
}

GameSound* GameMediaModule::LoadSound(const GameData& dataSound)
{
	Mix_Chunk* pSound = nullptr;
	if (pSound = Mix_LoadWAV_RW(SDL_RWFromMem(dataSound.pData, dataSound.nLength), 1))
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameSound));
		GameSound* pGameSound = new (pMem) GameSound();
		pGameSound->m_pImpl->m_pSound = pSound;

		return pGameSound;
	}

	return nullptr;
}

void GameMediaModule::PlayMusic(GameMusic* pGameMusic, int nLoop)
{
	Mix_PlayMusic(pGameMusic->m_pImpl->m_pMusic, nLoop);
}

void GameMediaModule::PlayMusic(GameMusic* pGameMusic, int nLoop, int msFading)
{
	Mix_FadeInMusic(pGameMusic->m_pImpl->m_pMusic, nLoop, msFading);
}

void GameMediaModule::StopMusic()
{
	Mix_HaltMusic();
}

void GameMediaModule::StopMusic(int msFading)
{
	Mix_FadeOutMusic(msFading);
}

void GameMediaModule::PauseMusic()
{
	Mix_PauseMusic();
}

void GameMediaModule::ResumeMusic()
{
	Mix_ResumeMusic();
}

void GameMediaModule::RewindMusic()
{
	Mix_RewindMusic();
}

bool GameMediaModule::CheckMusicPlaying()
{
	return Mix_PlayingMusic();
}

bool GameMediaModule::CheckMusicPaused()
{
	return Mix_PausedMusic();
}

GameMusic::Fading GameMediaModule::GetMusicFadingType()
{
	switch (Mix_FadingMusic())
	{
	case MIX_FADING_OUT:	return GameMusic::Fading::OUT; break;
	case MIX_FADING_IN:		return GameMusic::Fading::IN; break;
	default:				return GameMusic::Fading::NONE; break;
	}
}

void GameMediaModule::SetMusicVolume(int nVol)
{
	Mix_VolumeMusic(nVol);
}

int GameMediaModule::GetMusicVolume()
{
	return Mix_VolumeMusic(-1);
}

void GameMediaModule::PlaySound(GameSound* pGameSound, int nLoop)
{
	Mix_PlayChannel(-1, pGameSound->m_pImpl->m_pSound, nLoop);
}

void GameMediaModule::SetSoundVolume(GameSound* pGameSound, int nVol)
{
	Mix_VolumeChunk(pGameSound->m_pImpl->m_pSound, nVol);
}

int GameMediaModule::GetSoundVolume(GameSound* pGameSound)
{
	return pGameSound->m_pImpl->m_pSound->volume;
}

void GameMediaModule::DestroyMusic(GameMusic* pGameMusic)
{
	if (!pGameMusic) return;

	pGameMusic->~GameMusic();
	GameBlockAllocator::GetInstance().Free(pGameMusic, sizeof(GameMusic));
}

void GameMediaModule::DestroySound(GameSound* pGameSound)
{
	if (!pGameSound) return;

	pGameSound->~GameSound();
	GameBlockAllocator::GetInstance().Free(pGameSound, sizeof(GameSound));
}

GameMediaModule::GameMediaModule()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameMediaModule::~GameMediaModule()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}