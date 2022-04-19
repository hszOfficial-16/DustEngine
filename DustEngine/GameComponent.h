#ifndef _GAME_COMPONENT_H_
#define _GAME_COMPONENT_H_

#include <string>

class GameComponent
{
public:
	// 获取组件的名称
	const std::string& GetName()
	{
		return m_strName;
	}

	// 获取组件的大小
	uint16_t GetSize()
	{
		return m_nSize;
	}

private:
	std::string	m_strName;	// 组件名称
	uint16_t	m_nSize;	// 组件大小

private:
	GameComponent() : m_nSize(0) {}
	virtual ~GameComponent() = default;
};

#endif // !_GAME_COMPONENT_H_