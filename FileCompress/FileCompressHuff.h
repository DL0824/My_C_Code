#pragma once
#include "HuffmanTree.hpp"
#include <string>
#include <vector>


struct CharInfo
{
	unsigned char _ch;//������ֵ��ַ�
	size_t _count;//�ַ����ֵĴ���,Ҳ����ȨֵW
	std ::string _strCode;//�ַ�����

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
	std::vector<CharInfo> _fileInfo;//���256���ַ�
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