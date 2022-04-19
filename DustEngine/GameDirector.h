#ifndef _GAME_DIRECTOR_H_
#define _GAME_DIRECTOR_H_

#include "GameGraphicModule.h"
#include "GameMediaModule.h"
#include "GameScene.h"

class GameDirector
{
public:
	// ��ʼ����
	void Run();

	// ע��һ������
	void RegisterScene(const GameScene::Def& defScene);
	// �л�Ϊ�����ĳ���
	void SwitchScene(size_t nIndex);

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