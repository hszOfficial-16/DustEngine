#ifndef _GAME_RESOURCE_MANAGER_H_
#define _GAME_RESOURCE_MANAGER_H_

#include "GameGraphicModule.h"
#include "GameMediaModule.h"

class GameResourceManager
{
public:
	// ���һ������
	void AddTexture(GameTexture* pTexture);
	// ��ȡ����
	GameTexture* GetTexture(size_t nIndex);

	// ���һ������
	void AddFont(GameFont* pFont);
	// ��ȡ����
	GameFont* GetFont(size_t nIndex);

	// ���һ������
	void AddMusic(GameMusic* pMusic);
	// ��ȡ����
	GameMusic* GetMusic(size_t nIndex);

	// ���һ����Ч
	void AddSound(GameSound* pSound);
	// ��ȡ��Ч
	GameSound* GetSound(size_t nIndex);

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameResourceManager();
	GameResourceManager(const GameResourceManager&) = delete;
	GameResourceManager& operator=(const GameResourceManager&) = delete;
	static GameResourceManager& GetInstance()
	{
		static GameResourceManager instance;
		return instance;
	}
private:
	GameResourceManager();
};

#endif // !_GAME_RESOURCE_MANAGER_H_