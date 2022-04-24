#ifndef _GAME_RESOURCE_MANAGER_H_
#define _GAME_RESOURCE_MANAGER_H_

#include "GameGraphicModule.h"
#include "GameMediaModule.h"

class GameResourceManager
{
public:
	// 添加一个纹理
	void AddTexture(GameTexture* pTexture);
	// 获取纹理
	GameTexture* GetTexture(size_t nIndex);

	// 添加一个字体
	void AddFont(GameFont* pFont);
	// 获取字体
	GameFont* GetFont(size_t nIndex);

	// 添加一个音乐
	void AddMusic(GameMusic* pMusic);
	// 获取音乐
	GameMusic* GetMusic(size_t nIndex);

	// 添加一个音效
	void AddSound(GameSound* pSound);
	// 获取音效
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