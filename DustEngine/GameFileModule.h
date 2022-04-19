#ifndef _GAME_FILE_MODULE_H_
#define _GAME_FILE_MODULE_H_

#include "GameData.h"

#include <string>

class GameFileSystem
{
public:
	// ��ָ��·���µ��ļ���ȡ���ڴ�����
	// @param strFilePath: ·��+�ļ���
	// @param pData: �������ݵ�ָ��
	// @param nLength: ���ݳ���
	// @return �������������ݺ��䳤�ȵĶ���
	GameData	Read(const std::string& strFilePath);

	// ���ڴ��е�����д�뵽ָ���ļ���ȥ
	// @param strFilePath: ·��+�ļ���
	// @param file: �������������ݺ��䳤�ȵĽṹ��
	void		Write(const std::string& strFilePath, const GameData& file);

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameFileSystem();
	GameFileSystem(const GameFileSystem&) = delete;
	GameFileSystem& operator=(const GameFileSystem&) = delete;
	static GameFileSystem& GetInstance()
	{
		static GameFileSystem instance;
		return instance;
	}
private:
	GameFileSystem();
};

#endif // !_GAME_FILE_MODULE_H_