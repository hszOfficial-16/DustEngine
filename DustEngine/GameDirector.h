#ifndef _GAME_DIRECTOR_H_
#define _GAME_DIRECTOR_H_

#include "GameGraphicModule.h"
#include "GameMediaModule.h"
#include "GameScene.h"

class GameDirector
{
public:
	// 开始运行
	void Run();

	// 注册一个场景
	void RegisterScene(const GameScene::Def& defScene);
	// 切换为索引的场景
	void SwitchScene(size_t nIndex);

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
	~GameDirector();
	GameDirector(const GameDirector&) = delete;
	GameDirector& operator=(const GameDirector&) = delete;
	static GameDirector& GetInstance()
	{
		static GameDirector instance;
		return instance;
	}
private:
	GameDirector();
};

#endif // !_GAME_DIRECTOR_H_