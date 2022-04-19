#include "GameScene.h"
#include "GameBlockAllocator.h"

#include <new>
#include <vector>
#include <functional>

class GameScene::Impl
{
public:
	

public:
	Impl(const GameScene::Def& defScene)
	{

	}
	~Impl()
	{

	}
};

void GameScene::Update()
{

}

void GameScene::Draw()
{

}

GameScene::GameScene(const GameScene::Def& defScene)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl(defScene);
}

GameScene::~GameScene()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}

GameScene* GameSceneSystem::CreateScene(const GameScene::Def& defScene)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameScene));
	GameScene* pScene = new (pMem) GameScene(defScene);
	return pScene;
}

void GameSceneSystem::DestroyScene(GameScene* pScene)
{
	pScene->~GameScene();
	GameBlockAllocator::GetInstance().Free(pScene, sizeof(GameScene));
}