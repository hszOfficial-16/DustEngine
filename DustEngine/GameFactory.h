#ifndef _GAME_FACTORY_H_
#define _GAME_FACTORY_H_

class GameFactory
{
public:


private:

	
public:
	~GameFactory();
	GameFactory(const GameFactory&) = delete;
	GameFactory& operator=(const GameFactory&) = delete;
	static GameFactory& GetInstance()
	{
		static GameFactory instance;
		return instance;
	}
private:
	GameFactory();
};

#endif // !_GAME_FACTORY_H_