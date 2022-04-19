#ifndef _GAME_FILE_MODULE_H_
#define _GAME_FILE_MODULE_H_

#include "GameData.h"

#include <string>

class GameFileSystem
{
public:
	// 将指定路径下的文件读取到内存中来
	// @param strFilePath: 路径+文件名
	// @param pData: 储存数据的指针
	// @param nLength: 数据长度
	// @return 包含二进制数据和其长度的对象
	GameData	Read(const std::string& strFilePath);

	// 将内存中的数据写入到指定文件中去
	// @param strFilePath: 路径+文件名
	// @param file: 包含二进制数据和其长度的结构体
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