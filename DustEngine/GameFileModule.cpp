#include "GameFileModule.h"

#include <fstream>

class GameFileSystem::Impl
{
public:
	std::ifstream m_finStream;
	std::ofstream m_foutStream;

public:
	~Impl()
	{
		m_finStream.clear();
		m_finStream.close();

		m_foutStream.clear();
		m_foutStream.close();
	}
};

GameData GameFileSystem::Read(const std::string& strFilePath)
{
	m_pImpl->m_finStream.open(strFilePath, std::ios_base::in | std::ios_base::binary);

	if (!m_pImpl->m_finStream.is_open())
	{
		return { nullptr, 0 };
	}

	m_pImpl->m_finStream.seekg(0, m_pImpl->m_finStream.end);
	int nLength = m_pImpl->m_finStream.tellg();
	char* pData = new char[nLength];
	m_pImpl->m_finStream.seekg(0, m_pImpl->m_finStream.beg);
	m_pImpl->m_finStream.read((char*)pData, nLength);

	m_pImpl->m_finStream.clear();
	m_pImpl->m_finStream.close();

	return { pData, nLength };
}

void GameFileSystem::Write(const std::string& strFilePath, const GameData& file)
{
	m_pImpl->m_foutStream.open(strFilePath, std::ios_base::out | std::ios_base::binary);

	if (!m_pImpl->m_foutStream.is_open())
	{
		return;
	}

	m_pImpl->m_foutStream.write((char*)file.pData, file.nLength) << std::flush;

	m_pImpl->m_foutStream.clear();
	m_pImpl->m_foutStream.close();
}

GameFileSystem::GameFileSystem()
{
	m_pImpl = new Impl();
}

GameFileSystem::~GameFileSystem()
{
	delete m_pImpl;
}