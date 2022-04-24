#ifndef _GAME_ENTITY_H_
#define _GAME_ENTITY_H_

#include <string>

class GameScene;
class GameComponent;

class GameEntity
{
public:
	struct Def
	{
		GameScene* pScene;
	};

	// ��ȡ��ʵ��
	GameEntity* GetParent();
	// ���ø�ʵ��
	void SetParent(GameEntity* pParent);

	// �����ʵ��
	void AddChild(GameEntity* pChild);
	// ɾ����ʵ��
	void DeleteChild(GameEntity* pChild);

	// ��ȡ��ʵ���б�ͷ
	GameEntity* GetChildList();
	// ��ȡ��һ����ʵ��
	GameEntity* GetNext();

	// ������
	void AddComponent(GameComponent* pComponent);
	// ɾ�����
	void DeleteComponent(const std::string& strName);

	// ��ȡ���
	GameComponent* GetComponent(const std::string& strName);

	// ��ȡʵ���ڳ����е���һ��ʵ��
	GameEntity* GetSceneNext();

	// ��ȡʵ�������ĳ���
	GameScene* GetScene();

protected:
	// ����ʵ���ڳ����е���һ��ʵ��
	void SetSceneNext(GameEntity* pEntity);

	// ����ʵ�������ĳ���
	void SetScene(GameScene* pScene);

	// ��ʵ����������ȫ����ӵ�������
	void AddComponentToScene(GameScene* pScene);

private:
	class Impl;
	Impl* m_pImpl;

private:
	GameEntity(const Def& defEntity);
	~GameEntity();

	friend class GameScene;
	friend class GameEntityFactory;
};

class GameEntityFactory
{
public:
	GameEntity* CreateEntity(GameScene* pScene);

	void DestroyEntity(GameEntity* pEntity);

	GameEntity* CloneEntity(GameEntity* pEntity);

public:
	~GameEntityFactory() = default;
	GameEntityFactory(const GameEntityFactory&) = delete;
	GameEntityFactory& operator=(const GameEntityFactory&) = delete;
	static GameEntityFactory& GetInstance()
	{
		static GameEntityFactory instance;
		return instance;
	}

private:
	GameEntityFactory() = default;
};

#endif // !_GAME_ENTITY_H_