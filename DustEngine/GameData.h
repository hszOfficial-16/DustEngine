#ifndef _GAME_DATA_H_
#define _GAME_DATA_H_

struct GameData
{
	void*	pData;				// ��������
	int		nLength;			// ���ݳ���
	bool	bShouldDeleteData;	// �Ƿ�����������

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
		// ���û��������Ƿ�����������
		if (pData && bShouldDeleteData)
		{
			delete[] pData;
		}
	}

	// ɾ�����ƹ��캯�����Է�ֹ���ݸ��ƴ������������
	GameData(const GameData& copy) = delete;

	// �ṩ�ƶ��ƶ����캯�������������Ϊ����������Ч��
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