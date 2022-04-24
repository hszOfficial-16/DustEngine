#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <string>
#include <functional>

class GameEntity;
class GameComponent;
class GameSystem;

class GameScene
{
public:
	// ���³���
	void Update();

	// ���һ��ʵ��
	void AddEntity(GameEntity* pEntity);
	// ɾ��һ��ʵ��
	void DeleteEntity(GameEntity* pEntity);
	// ��ȡʵ���б�
	GameEntity* GetEntityList();
	
	// ���һ�������ϵͳ����
	void AddComponent(GameComponent* pComponent);
	// ɾ��һ�����
	void DeleteComponent(GameComponent* pComponent);
	// ��ȡ����б�
	GameComponent* GetComponentList(std::string strComponentName);

	// ���һ��ϵͳ
	void AddSystem(GameSystem* pSystem);
	// ɾ��һ��ϵͳ
	void DeleteSystem(GameSystem* pSystem);
	// ��ȡϵͳ�б�
	GameSystem* GetSystemList();

private:
	class Impl;
	Impl* m_pImpl;

protected:
	GameScene();
	~GameScene();

	friend class GameDirector;
};

typedef GameScene* (*GameSceneConstructor)();

#endif // !_GAME_SCENE_SYSTEM_H_