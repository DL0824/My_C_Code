#include "FileCompressHuff.h"
#include <algorithm>
#include <assert.h>


FileCompressHuff::FileCompressHuff()
{
	_fileInfo.resize(256);
	for (int i = 0; i < 256; ++i)
	{
		_fileInfo[i]._ch = i;//���ַ���Ӧ��ASCLL�빹���ַ�
		_fileInfo[i]._count = 0;//��ǰ�ַ�������0��
	}
}

void FileCompressHuff::CompressFile(const std::string& path)
{
	//1.ͳ��Դ�ļ���ÿ���ַ����ֵĴ���
	FILE  *fin = fopen(path.c_str(), "rb");
	if (nullptr == fin)
	{
		assert(false);
		return;
	}

	unsigned char *pReadBuff = new unsigned char[1024];//�������������
	size_t rdsize = 0;
	while (true)
	{
		rdsize = fread(pReadBuff, 1, 1024, fin);
		if (0 == rdsize)
			break;

		for (size_t i = 0; i < rdsize; ++i)
			_fileInfo[pReadBuff[i]]._count++;
	}

	//2.���ַ����ִ���ΪȨֵ������������
	HuffmanTree<CharInfo> t(_fileInfo,CharInfo(0));

	//3.��ȡÿ���ַ��ı���
	GenerateHuffmanCode(t.GetRoot());

	//4.�û�ȡ�����ַ��������¸�дԴ�ļ�
	size_t pos = path.find('.');
	std::string compressFile = path.substr(0, pos) + "_hf.lzp";
	FILE* fout = fopen(compressFile.c_str(), "wb");//���ѹ�����ݵ��ļ�
	if (nullptr == fout)
	{
		assert(false);
		return;
	}

	//дͷ����Ϣ
	/*
	Դ�ļ���׺
	������1���ַ�ռһ�У�
	�ַ���Ϣ --->  [�ַ�]:[�ַ���Ӧ����]
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

		//�����ֽڵı���Զ�ȡ�������ݽ�����д
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

	//���һ��ch���ܲ����˸�����λ
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
	//д�ļ���׺
	std::string strHead;
	strHead += GetFilePostfix(filename);
	strHead += '\n';

	//д����
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
	strHead += sz;//����
	strHead += '\n';

	strHead += strcount;//�ַ�������Ϣ
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
	//֮ǰ����Ϣ��ѹ���ļ��У�һ��д��һ��
	//�ļ���׺
	std::string strFilePostfix;
	ReadLine(fin, strFilePostfix);

	//�ַ���Ϣ��������
	std::string strCount;
	ReadLine(fin, strCount);
	int lineCount = atoi(strCount.c_str());

	//��ȡlineCount�е��ַ���Ϣ
	for (int i = 0; i < lineCount; ++i)
	{
		std::string strCh;
		ReadLine(fin, strCh);
		//�����ȡ������'\n'
		if (strCh.empty())
		{
			strCh += '\n';
			ReadLine(fin, strCh);
		}
		//��Ϣ�ĸ�ʽ--> A:1 (һ���ַ���һ��ð�ţ�һ������)
		_fileInfo[(unsigned char)strCh[0]]._count = atoi(strCh.c_str() + 2);

	}
	//��ԭ��������
	HuffmanTree<CharInfo> t(_fileInfo, CharInfo(0));

	size_t pos = path.find('.');
	std::string uncompressFile = path.substr(0, pos) + strFilePostfix;
	FILE* fout = fopen(uncompressFile.c_str(), "wb");
	assert(fout);

	//��ѹ��
	char *pReadBuff = new char[1024];
	char ch = 0;
	HuffmanTreeNode<CharInfo>* pCur = t.GetRoot();
	size_t filesize = pCur->_weight._count;//���ڵ��Ȩֵ�����ļ��ܵ��ֽ���
	size_t count = 0;
	while (true)
	{
		size_t rdsize = fread(pReadBuff, 1, 1024, fin);
		if (rdsize == 0)
			break;

		for (size_t i = 0; i < rdsize; ++i)
		{
			//ֻ�轫һ���ֽ���8������λ��������
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
