#ifndef _GAME_ENTITY_H_
#define _GAME_ENTITY_H_

#include "GameComponent.h"

class GameEntity
{
public:
	struct Def
	{
		
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

private:
	class Impl;
	Impl* m_pImpl;

private:
	GameEntity(const Def& defEntity);
	~GameEntity();
};

#endif // !_GAME_ENTITY_H_