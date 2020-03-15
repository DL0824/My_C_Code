#include "FileCompressHuff.h"
#include <algorithm>
#include <assert.h>


FileCompressHuff::FileCompressHuff()
{
	_fileInfo.resize(256);
	for (int i = 0; i < 256; ++i)
	{
		_fileInfo[i]._ch = i;//用字符对应的ASCLL码构建字符
		_fileInfo[i]._count = 0;//当前字符出现了0次
	}
}

void FileCompressHuff::CompressFile(const std::string& path)
{
	//1.统计源文件中每个字符出现的次数
	FILE  *fin = fopen(path.c_str(), "rb");
	if (nullptr == fin)
	{
		assert(false);
		return;
	}

	unsigned char *pReadBuff = new unsigned char[1024];//保存读到的内容
	size_t rdsize = 0;
	while (true)
	{
		rdsize = fread(pReadBuff, 1, 1024, fin);
		if (0 == rdsize)
			break;

		for (size_t i = 0; i < rdsize; ++i)
			_fileInfo[pReadBuff[i]]._count++;
	}

	//2.以字符出现次数为权值创建哈夫曼树
	HuffmanTree<CharInfo> t(_fileInfo,CharInfo(0));

	//3.获取每个字符的编码
	GenerateHuffmanCode(t.GetRoot());

	//4.用获取到的字符编码重新改写源文件
	size_t pos = path.find('.');
	std::string compressFile = path.substr(0, pos) + "_hf.lzp";
	FILE* fout = fopen(compressFile.c_str(), "wb");//存放压缩数据的文件
	if (nullptr == fout)
	{
		assert(false);
		return;
	}

	//写头部信息
	/*
	源文件后缀
	行数（1种字符占一行）
	字符信息 --->  [字符]:[字符对应次数]
	*/
	WriteHead(fout, path);

	fseek(fin, 0, SEEK_SET);
	char ch = 0;
	int bitcount = 0;
	while (true)
	{
		rdsize = fread(pReadBuff, 1, 1024, fin);
		if (0 == rdsize)
			break;

		//根据字节的编码对读取到的内容进行重写
		for (size_t i = 0; i < rdsize; ++i)
		{
			std::string strcode = _fileInfo[pReadBuff[i]]._strCode;
			for (size_t j = 0; j < strcode.size(); ++j)
			{
				ch <<= 1;
				if ('1' == strcode[j])
					ch |= 1;
				bitcount++;
				if (8 == bitcount)
				{
					fputc(ch, fout);
					ch = 0;
					bitcount = 0;
				}
			}
		}
	}

	//最后一次ch可能不够八个比特位
	if (bitcount != 0 && bitcount < 8)
	{
		ch <<= (8 - bitcount);
		fputc(ch, fout);
	}

	delete[] pReadBuff;
	fclose(fin);
	fclose(fout);
}

void FileCompressHuff::GenerateHuffmanCode(HuffmanTreeNode<CharInfo> *pRoot)
{
	if (nullptr == pRoot)
		return;

	GenerateHuffmanCode(pRoot->_pLeft);
	GenerateHuffmanCode(pRoot->_pRight);

	if (nullptr == pRoot->_pLeft && nullptr == pRoot->_pRight)
	{
		std::string& strcode = _fileInfo[pRoot->_weight._ch]._strCode;
		HuffmanTreeNode<CharInfo> *pCur = pRoot;
		HuffmanTreeNode<CharInfo> *pParent = pCur->_pParent;
		while (pParent)
		{
			if (pCur == pParent->_pLeft)
				strcode += '0';
			else
				strcode += '1';
			pCur = pParent;
			pParent = pCur->_pParent;
		}
		std::reverse(strcode.begin(), strcode.end());
		//_fileInfo[pRoot->_weight._ch]._strCode = strcode;
	}
}

std::string FileCompressHuff::GetFilePostfix(const std::string& filename)
{
	return filename.substr(filename.rfind('.'));
}

void FileCompressHuff::WriteHead(FILE* fout, const std::string& filename)
{
	assert(fout);
	//写文件后缀
	std::string strHead;
	strHead += GetFilePostfix(filename);
	strHead += '\n';

	//写行数
	size_t linecount = 0;
	std::string strcount;
	char sz[32] = { 0 };
	for (int i = 0; i < 256; ++i)
	{
		if (_fileInfo[i]._count)
		{
			linecount++;
			strcount += _fileInfo[i]._ch;
			strcount += ':';
			_itoa(_fileInfo[i]._count, sz, 10);
			strcount += sz; 
			strcount += '\n';
		}
	}
	_itoa(linecount, sz, 10);
	strHead += sz;//行数
	strHead += '\n';

	strHead += strcount;//字符次数信息
	//strHead += '\n';

	fwrite(strHead.c_str(), 1, strHead.size(), fout);

}

void FileCompressHuff::ReadLine(FILE* fin, std::string& strInfo)
{
	assert(fin);
	while (!feof(fin))
	{
		char ch = fgetc(fin);
		if (ch == '\n')
			break;

		strInfo += ch;
	}
}
void FileCompressHuff::UnCompressFile(const std::string& path)
{
	FILE* fin = fopen(path.c_str(), "rb");
	if (nullptr == fin)
	{
		assert(false);
		return;
	}
	//之前的信息在压缩文件中，一个写了一行
	//文件后缀
	std::string strFilePostfix;
	ReadLine(fin, strFilePostfix);

	//字符信息的总行数
	std::string strCount;
	ReadLine(fin, strCount);
	int lineCount = atoi(strCount.c_str());

	//读取lineCount行的字符信息
	for (int i = 0; i < lineCount; ++i)
	{
		std::string strCh;
		ReadLine(fin, strCh);
		//如果读取到的是'\n'
		if (strCh.empty())
		{
			strCh += '\n';
			ReadLine(fin, strCh);
		}
		//信息的格式--> A:1 (一个字符，一个冒号，一个次数)
		_fileInfo[(unsigned char)strCh[0]]._count = atoi(strCh.c_str() + 2);

	}
	//还原哈夫曼树
	HuffmanTree<CharInfo> t(_fileInfo, CharInfo(0));

	size_t pos = path.find('.');
	std::string uncompressFile = path.substr(0, pos) + strFilePostfix;
	FILE* fout = fopen(uncompressFile.c_str(), "wb");
	assert(fout);

	//解压缩
	char *pReadBuff = new char[1024];
	char ch = 0;
	HuffmanTreeNode<CharInfo>* pCur = t.GetRoot();
	size_t filesize = pCur->_weight._count;//根节点的权值就是文件总的字节数
	size_t count = 0;
	while (true)
	{
		size_t rdsize = fread(pReadBuff, 1, 1024, fin);
		if (rdsize == 0)
			break;

		for (size_t i = 0; i < rdsize; ++i)
		{
			//只需将一个字节中8个比特位单独处理
			ch = pReadBuff[i];
			for (int pos = 0; pos < 8; ++pos)
			{
				if (ch & 0x80)
					pCur = pCur->_pRight;
				else
					pCur = pCur->_pLeft;

				ch <<= 1;
				if (pCur->_pLeft == nullptr && pCur->_pRight == nullptr)
				{
					count++;
					fputc(pCur->_weight._ch, fout); 
					fflush(fout);
					if (count == filesize)
						break;
					
					pCur = t.GetRoot();
				}
			}
		}
	}

	delete[] pReadBuff;
	fclose(fin);
	fclose(fout);
}
