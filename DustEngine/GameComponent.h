#ifndef _GAME_COMPONENT_H_
#define _GAME_COMPONENT_H_

#include <string>

class GameComponent
{
public:
	// ��ȡ���������
	const std::string& GetName()
	{
		return m_strName;
	}

	// ��ȡ����Ĵ�С
	uint16_t GetSize()
	{
		return m_nSize;
	}

private:
	std::string	m_strName;	// �������
	uint16_t	m_nSize;	// �����С

private:
	GameComponent() : m_nSize(0) {}
	virtual ~GameComponent() = default;
};

#endif // !_GAME_COMPONENT_H_