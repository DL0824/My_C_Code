#include "LZ77.h"


const USH MIN_LOOKAHEAD = MAX_MATCH + 1;//��֤��ѹ������������һ���ֽ��Լ����ֽڵ�һ�����ƥ�䳤��
const USH MAX_DIST = WSIZE - MIN_LOOKAHEAD;//���ƥ�����

LZ77::LZ77()
:_pwin(new UCH[WSIZE * 2])
, _ht(WSIZE)
{}

LZ77::~LZ77()
{
	delete[] _pwin;
	_pwin = nullptr;
}

void LZ77::CompressFile(const std::string& strFilePath, std::string& postFix)
{
	FILE* fin = fopen(strFilePath.c_str(), "rb");
	if (nullptr == fin)
	{
		std::cout << "open failed" << std::endl;
		return;
	}

	//��ȡ�ļ���С
	//���Դ�ļ��Ĵ�СС��MIN_MATCH,�򲻽��д���
	fseek(fin, 0, SEEK_END);
	ULL fileSize = ftell(fin);
	if (fileSize <= MIN_MATCH)
		return;

	//�Ӵ�ѹ���ļ��ж�ȡһ�������������ݵ�������
	fseek(fin, 0, SEEK_SET);
	size_t lookAhead = fread(_pwin, 1, 2 * WSIZE, fin);
	USH hashAddr = 0;
	//������ʼ��hashAddr(ǰ�����ַ�)
	for (USH i = 0; i < MIN_MATCH - 1; ++i)
		_ht.HashFunc(hashAddr, _pwin[i]);

	//�ҳ��ļ���׺���ļ���׺��Ϊ���θ���ѹ������
	size_t pos = strFilePath.find('.');
	postFix = strFilePath.substr(pos + 1);
	std::string compression = strFilePath.substr(0, pos);
	//ѹ���ļ��ͱ���ļ�
	std::string compressFile = compression + ".lzp";
	std::string compressFlag = compression + "_F.lzp";

	//��ʼѹ��
	FILE *fout = fopen(compressFile.c_str(), "wb");
	assert(fout);
	//д��ǵ��ļ�
	FILE *foutF = fopen(compressFlag.c_str(), "wb");
	assert(foutF);

	//ƥ�������Ϣ
	USH start = 0;
	USH matchHead = 0;
	USH curMatchLength = 0;//һ��ƥ���ĳ���
	USH curMatchDist = 0;//һ��ƥ���ľ���

	//��������Ϣ
	UCH chFlag = 0;
	UCH bitCount = 0;
	bool isLen = false;
	//lookAhead��ʾ���л������д�ѹ���ַ�������
	while (lookAhead)
	{
		//����ǰ�����ַ����뵽��ϣ���У�����ȡƥ��ͷ
		_ht.Insert(matchHead, _pwin[start + 2], start, hashAddr);

		//��֤�ڲ��һ��������Ƿ��ҵ�ƥ��
		if (matchHead)
		{
			//˳��ƥ���������ƥ��,���մ������Ⱦ����
			curMatchLength = LongestMatch(matchHead, curMatchDist,start);
		}

		//��֤�Ƿ��ҵ�ƥ��
		if (curMatchLength < MIN_MATCH)
		{
			//δ�ҵ��ظ��ַ���
			//��startλ�õ��ַ�д��ѹ���ļ�
			fputc(_pwin[start], fout);
			//д��ǰ�ַ���Ӧ�ı��
			writeFlag(foutF, chFlag, bitCount, false);
			start++;
			lookAhead--;
		}
		else
		{
			//�ҵ�ƥ��
			//�����Ⱦ����д��ѹ���ļ�

			//д���ȣ�ʵ��ƥ��curMatchLength,��ѹ����ʱ��Ҫ��3��
			UCH chLen = curMatchLength - 3;
			fputc(chLen, fout);
			//д����
			fwrite(&curMatchDist, sizeof(curMatchDist), 1, fout);
			//д���
			writeFlag(foutF, chFlag, bitCount, true);
			//�������л�������ʣ����ֽ���
			lookAhead -= curMatchLength;
				

			//���Ѿ�ƥ����ַ�����������һ�齫����뵽��ϣ����
			--curMatchLength;//��ǰ�ַ��Ѿ�����
			while (curMatchLength)
			{
				start++;
				_ht.Insert(matchHead, _pwin[start + 2], start, hashAddr);
				curMatchLength--;
			}
			start++;
		}
		//������л�����ʣ���ַ����������Ƿ���Ҫ�������
		if (lookAhead <= MIN_LOOKAHEAD)
			FillWindow(fin,lookAhead, start);
	}

	//ѹ���������������8������λ
	if (bitCount > 0 && bitCount < 8)
	{
		chFlag <<= (8 - bitCount);
		fputc(chFlag, foutF);
	}
	fclose(foutF);
	//��ѹ���ļ������ļ��ϲ�
	MergeFile(fout, fileSize, compressFlag);
	
	fclose(fin);
	fclose(fout);
}

void LZ77::FillWindow(FILE *fin, size_t& lookAhead,USH& start)
{
	//���ı��ʾ��ǽ��Ҵ������ݰ��Ƶ��󴰣��������Ѿ�ѹ����
	//WSIZE = MAX_DIST + MIN_LOOKAHEAD
	//�������Ѿ�ѹ������start�϶�����WSIZE
	//����start����WSIZE + MAX_DISTʱ��ôʣ��Ĵ�ѹ������
	if (start >= WSIZE + MAX_DIST)
	{
		//���Ҵ��е����ݰ��Ƶ���
		memcpy(_pwin, _pwin + WSIZE, WSIZE);
		memset(_pwin + WSIZE, 0, WSIZE);
		start -= WSIZE;

		//���¹�ϣ��
		_ht.UpDate();

		//���Ҵ��в���һ�������е�����
		if (!feof(fin))
			lookAhead += fread(_pwin + WSIZE, 1, WSIZE, fin);
	}
}

void LZ77::MergeFile(FILE *fout, ULL fileSize, std::string compressFlag)
{
	//��ѹ�������ļ��ͱ����Ϣ�ļ��ϲ�
	/*
	ѹ������
	�������
	��Ǵ�С
	Դ�ļ���С
	*/
	//��ȡ�����Ϣ�ļ��е����ݣ�Ȼ�󽫽��д�뵽ѹ���ļ���
	FILE *finF = fopen(compressFlag.c_str(), "rb");
	size_t flagSize = 0;
	UCH *pReadBuff = new UCH[1024];
	while (true)
	{
		size_t rdSize = fread(pReadBuff, 1, 1024, finF);
		if (rdSize == 0)
			break;

		fwrite(pReadBuff, 1, rdSize, fout);
		flagSize += rdSize;
	}
	fwrite(&flagSize, sizeof(flagSize), 1, fout);
	fwrite(&fileSize, sizeof(fileSize), 1, fout);
	fclose(finF);
	delete[] pReadBuff;
}

//chFlag:���ֽ��е�ÿ������λ������ȴ�ֵ�ǰ�ַ���ԭ�ַ����ǳ���
//0:ԭ�ַ�
//1:���ȣ�����������룬���Ծ���Ҳ���0��
//bitCount:���ֽ��ж��ٸ�����λ�Ѿ�������
void LZ77::writeFlag(FILE *fout, UCH& chFlag, UCH& bitCount, bool isLen)
{
	chFlag <<= 1;
	if (isLen)
		chFlag |= 1;
	bitCount++;
	if (bitCount == 8)
	{
		//���ñ��д�����ļ���
		fputc(chFlag, fout);
		chFlag = 0;
		bitCount = 0;
	}
}

//ƥ�����ڲ��һ������н��еģ����һ������п��ܻ��ҵ����ƥ��
//����ƥ��
//���ܻ�������״��������Ҫ�������ƥ���������Ϊpos������һ��mask
USH LZ77::LongestMatch(USH matchHead, USH& curMatchDist, USH start)
{
	
	USH maxMatchLen = 0;
	UCH maxMatchCount = 255;//���ƥ����������������״��
	USH curMatchStart = 0;//��ǰƥ���ڲ��һ������е���ʼλ�ã��ж��ƥ��ͷ��

	//�����ڻ����Ĵ���,ƥ�����С�ڵ���MAX_DIST
	USH limit = start > MAX_DIST ? start - MAX_DIST : 0;

	do
	{
		//���л��������Խ���ƥ��ķ�Χ(��ѹ��)
		UCH *pstart = _pwin + start;
		UCH *pend = pstart + MAX_MATCH;
		UCH curMatchLen = 0;//��ǰƥ�䳤��

		//���һ�����ƥ�䴮����ʼ���Ѿ�ѹ����
		UCH *pMatchStart = _pwin + matchHead;

		//����ƥ��
		while (pstart <= pend && *pstart == *pMatchStart)
		{
			curMatchLen++;
			pstart++;
			pMatchStart++;
		}

		//һ��ƥ��������п��ܲ��������ƥ��
		if (curMatchLen > maxMatchLen)
		{
			maxMatchLen = curMatchLen;
			curMatchStart = matchHead;
		}
	} while ((matchHead = _ht.GetNext(matchHead)) > limit && maxMatchCount--);

	curMatchDist = start - curMatchStart;
	return maxMatchLen;
}


void LZ77::UNCompressFile(const std::string& strFilePath, std::string postFix)
{
	//��ѹ���ļ�
	FILE *finD = fopen(strFilePath.c_str(), "rb");
	if (nullptr == finD)
		return;
	//�ٴδ��ļ���Ϊ������ǵ�ָ��
	FILE *finF = fopen(strFilePath.c_str(), "rb");
	if (nullptr == finF)
		return;

	//��ȡԴ�ļ��Ĵ�С
	ULL fileSize = 0;
	fseek(finF, 0 - sizeof(fileSize), SEEK_END);
	fread(&fileSize, sizeof(fileSize), 1, finF);

	//��ȡ�����Ϣ�Ĵ�С
	size_t flagSize = 0;
	fseek(finF, 0 - sizeof(fileSize) - sizeof(flagSize), SEEK_END);
	fread(&flagSize, sizeof(flagSize), 1, finF);

	//����ȡ�����Ϣ���ļ�ָ���ƶ������������ݵ���ʼλ��
	fseek(finF, 0 - sizeof(flagSize) - sizeof(fileSize) - flagSize, SEEK_END);

	size_t pos = strFilePath.find('.');
	std::string uncompression = strFilePath.substr(0, pos);
	std::string uncompressFile = uncompression + "_un." + postFix;
	//��ʼ��ѹ��
	//д��ѹ��������
	FILE *fout = fopen(uncompressFile.c_str(), "wb");
	assert(fout);

	//������ѹ�����Ⱦ����
	FILE *f = fopen(uncompressFile.c_str(), "rb");

	UCH bitCount = 0;
	UCH chFlag = 0;
	ULL encodeCount = 0;
	while (encodeCount < fileSize)
	{
		//��ȡһ�����
		if (0 == bitCount)
		{
			chFlag = fgetc(finF);
			bitCount = 8;
		}

		if (chFlag & 0x80)
		{
			//���Ⱦ����
			USH matchLen = fgetc(finD) + 3;//����ѹ����ʱ���ȥ��3���Լ�3
			USH matchDist = 0;
			fread(&matchDist, sizeof(matchDist), 1, finD);
			fflush(fout);//��ջ�����
			//�����Ѿ������ַ�����
			encodeCount += matchLen;

			//f:��ȡǰ��ƥ�������
			fseek(f, 0 - matchDist, SEEK_END);

			while (matchLen)
			{
				UCH ch = fgetc(f);
				fputc(ch, fout);
				matchLen--;
				fflush(fout);
			}
		}
		else
		{
			//ԭ�ַ�
			UCH ch = fgetc(finD);
			fputc(ch, fout);
			encodeCount++;
		}
		chFlag <<= 1;
		bitCount--;
	}

	fclose(finD);
	fclose(finF);
	fclose(fout);
	fclose(f);
}