#ifndef _GAME_ENTITY_H_
#define _GAME_ENTITY_H_

#include <string>

class GameScene;
class GameComponent;

class GameEntity
{
public:
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
	// ��ȡ����б�ͷ
	GameComponent* GetComponentList();

	// ��ȡʵ�������ĳ���
	GameScene* GetScene();
	// ��ȡʵ���ڳ����е���һ��ʵ��
	GameEntity* GetSceneNext();

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

protected:
	GameEntity();
	~GameEntity();

	friend class GameScene;
	friend class GameFactory;
};

#endif // !_GAME_ENTITY_H_