#ifndef _GAME_DATA_H_
#define _GAME_DATA_H_

struct GameData
{
	void*	pData;				// 数据内容
	int		nLength;			// 数据长度
	bool	bShouldDeleteData;	// 是否析构掉数据

	GameData()
	{
		pData = nullptr;
		nLength = 0;
		bShouldDeleteData = true;
	}

	GameData(void* data, int length)
	{
		pData = data;
		nLength = length;
		bShouldDeleteData = true;
	}

	~GameData()
	{
		// 由用户来决定是否析构掉数据
		if (pData && bShouldDeleteData)
		{
			delete[] pData;
		}
	}

	// 删除复制构造函数，以防止数据复制带来的性能损耗
	GameData(const GameData& copy) = delete;

	// 提供移动移动构造函数，提高数据作为参数带来的效率
	GameData(GameData&& move) noexcept
	{
		pData = move.pData;
		nLength = move.nLength;
		bShouldDeleteData = move.bShouldDeleteData;
		move.pData = nullptr;
		move.nLength = 0;
		move.bShouldDeleteData = true;
	}
};

#endif // !_GAME_DATA_H_