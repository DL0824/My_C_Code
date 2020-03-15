#pragma once
#include "HuffmanTree.hpp"
#include <string>
#include <vector>


struct CharInfo
{
	unsigned char _ch;//具体出现的字符
	size_t _count;//字符出现的次数,也就是权值W
	std ::string _strCode;//字符编码

	CharInfo(size_t count = 0)
		:_count(count)
	{}
	CharInfo operator+(const CharInfo& c)const
	{
		return CharInfo(_count + c._count);
	}

	bool operator>(const CharInfo& c)const
	{
		return _count > c._count;
	}

	bool operator==(const CharInfo& c)const
	{
		return _count == c._count;
	}
};


class FileCompressHuff
{
private:
	std::vector<CharInfo> _fileInfo;//最多256种字符
private:
	void GenerateHuffmanCode(HuffmanTreeNode<CharInfo> *pRoot);
	void WriteHead(FILE* fout, const std::string& filepostfix);
	std::string GetFilePostfix(const std::string& filename);
	void ReadLine(FILE* fin, std::string& strInfo);
public:
	FileCompressHuff();
	void CompressFile(const std::string& path);
	void UnCompressFile(const std::string& path);
	
};