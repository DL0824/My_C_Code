#include "LZ77.h"


const USH MIN_LOOKAHEAD = MAX_MATCH + 1;//保证待压缩区域至少有一个字节以及该字节的一个最大匹配长度
const USH MAX_DIST = WSIZE - MIN_LOOKAHEAD;//最大匹配距离

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

	//获取文件大小
	//如果源文件的大小小于MIN_MATCH,则不进行处理
	fseek(fin, 0, SEEK_END);
	ULL fileSize = ftell(fin);
	if (fileSize <= MIN_MATCH)
		return;

	//从待压缩文件中读取一个缓冲区的数据到窗口中
	fseek(fin, 0, SEEK_SET);
	size_t lookAhead = fread(_pwin, 1, 2 * WSIZE, fin);
	USH hashAddr = 0;
	//设置起始的hashAddr(前两个字符)
	for (USH i = 0; i < MIN_MATCH - 1; ++i)
		_ht.HashFunc(hashAddr, _pwin[i]);

	//找出文件后缀把文件后缀作为出参给解压缩利用
	size_t pos = strFilePath.find('.');
	postFix = strFilePath.substr(pos + 1);
	std::string compression = strFilePath.substr(0, pos);
	//压缩文件和标记文件
	std::string compressFile = compression + ".lzp";
	std::string compressFlag = compression + "_F.lzp";

	//开始压缩
	FILE *fout = fopen(compressFile.c_str(), "wb");
	assert(fout);
	//写标记的文件
	FILE *foutF = fopen(compressFlag.c_str(), "wb");
	assert(foutF);

	//匹配相关信息
	USH start = 0;
	USH matchHead = 0;
	USH curMatchLength = 0;//一次匹配后的长度
	USH curMatchDist = 0;//一次匹配后的距离

	//标记相关信息
	UCH chFlag = 0;
	UCH bitCount = 0;
	bool isLen = false;
	//lookAhead表示先行缓冲区中待压缩字符的数量
	while (lookAhead)
	{
		//将当前三个字符插入到哈希表中，并获取匹配头
		_ht.Insert(matchHead, _pwin[start + 2], start, hashAddr);

		//验证在查找缓冲区中是否找到匹配
		if (matchHead)
		{
			//顺着匹配链找最长的匹配,最终带出长度距离对
			curMatchLength = LongestMatch(matchHead, curMatchDist,start);
		}

		//验证是否找到匹配
		if (curMatchLength < MIN_MATCH)
		{
			//未找到重复字符串
			//将start位置的字符写入压缩文件
			fputc(_pwin[start], fout);
			//写当前字符对应的标记
			writeFlag(foutF, chFlag, bitCount, false);
			start++;
			lookAhead--;
		}
		else
		{
			//找到匹配
			//将长度距离对写入压缩文件

			//写长度（实际匹配curMatchLength,解压缩的时候要加3）
			UCH chLen = curMatchLength - 3;
			fputc(chLen, fout);
			//写距离
			fwrite(&curMatchDist, sizeof(curMatchDist), 1, fout);
			//写标记
			writeFlag(foutF, chFlag, bitCount, true);
			//更新先行缓冲区中剩余的字节数
			lookAhead -= curMatchLength;
				

			//将已经匹配的字符串按照三个一组将其插入到哈希表中
			--curMatchLength;//当前字符已经插入
			while (curMatchLength)
			{
				start++;
				_ht.Insert(matchHead, _pwin[start + 2], start, hashAddr);
				curMatchLength--;
			}
			start++;
		}
		//检测先行缓冲区剩余字符个数，看是否需要填充数据
		if (lookAhead <= MIN_LOOKAHEAD)
			FillWindow(fin,lookAhead, start);
	}

	//压缩到最后标记数不够8个比特位
	if (bitCount > 0 && bitCount < 8)
	{
		chFlag <<= (8 - bitCount);
		fputc(chFlag, foutF);
	}
	fclose(foutF);
	//将压缩文件与标记文件合并
	MergeFile(fout, fileSize, compressFlag);
	
	fclose(fin);
	fclose(fout);
}

void LZ77::FillWindow(FILE *fin, size_t& lookAhead,USH& start)
{
	//填充的本质就是将右窗的数据搬移到左窗，并且左窗已经压缩了
	//WSIZE = MAX_DIST + MIN_LOOKAHEAD
	//左窗数据已经压缩所以start肯定大于WSIZE
	//而当start大于WSIZE + MAX_DIST时那么剩余的待压缩数据
	if (start >= WSIZE + MAX_DIST)
	{
		//将右窗中的数据搬移到左窗
		memcpy(_pwin, _pwin + WSIZE, WSIZE);
		memset(_pwin + WSIZE, 0, WSIZE);
		start -= WSIZE;

		//更新哈希表
		_ht.UpDate();

		//向右窗中补充一个窗口中的数据
		if (!feof(fin))
			lookAhead += fread(_pwin + WSIZE, 1, WSIZE, fin);
	}
}

void LZ77::MergeFile(FILE *fout, ULL fileSize, std::string compressFlag)
{
	//将压缩数据文件和标记信息文件合并
	/*
	压缩数据
	标记数据
	标记大小
	源文件大小
	*/
	//读取标记信息文件中的内容，然后将结果写入到压缩文件中
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

//chFlag:该字节中的每个比特位是用来却分当前字符是原字符还是长度
//0:原字符
//1:长度（后面紧跟距离，所以距离也标记0）
//bitCount:该字节中多少个比特位已经被设置
void LZ77::writeFlag(FILE *fout, UCH& chFlag, UCH& bitCount, bool isLen)
{
	chFlag <<= 1;
	if (isLen)
		chFlag |= 1;
	bitCount++;
	if (bitCount == 8)
	{
		//将该标记写入标记文件中
		fputc(chFlag, fout);
		chFlag = 0;
		bitCount = 0;
	}
}

//匹配是在查找缓冲区中进行的，查找缓冲区中可能会找到多个匹配
//输出最长匹配
//可能会遇到环状的链，需要设置最大匹配次数，因为pos与上了一个mask
USH LZ77::LongestMatch(USH matchHead, USH& curMatchDist, USH start)
{
	
	USH maxMatchLen = 0;
	UCH maxMatchCount = 255;//最大匹配次数，用来解决环状链
	USH curMatchStart = 0;//当前匹配在查找缓冲区中的起始位置（有多个匹配头）

	//类似于滑动的窗口,匹配距离小于等于MAX_DIST
	USH limit = start > MAX_DIST ? start - MAX_DIST : 0;

	do
	{
		//先行缓冲区可以进行匹配的范围(待压缩)
		UCH *pstart = _pwin + start;
		UCH *pend = pstart + MAX_MATCH;
		UCH curMatchLen = 0;//当前匹配长度

		//查找缓冲区匹配串的起始（已经压缩）
		UCH *pMatchStart = _pwin + matchHead;

		//进行匹配
		while (pstart <= pend && *pstart == *pMatchStart)
		{
			curMatchLen++;
			pstart++;
			pMatchStart++;
		}

		//一次匹配结束，有可能并不是最佳匹配
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
	//打开压缩文件
	FILE *finD = fopen(strFilePath.c_str(), "rb");
	if (nullptr == finD)
		return;
	//再次打开文件，为操作标记的指针
	FILE *finF = fopen(strFilePath.c_str(), "rb");
	if (nullptr == finF)
		return;

	//获取源文件的大小
	ULL fileSize = 0;
	fseek(finF, 0 - sizeof(fileSize), SEEK_END);
	fread(&fileSize, sizeof(fileSize), 1, finF);

	//获取标记信息的大小
	size_t flagSize = 0;
	fseek(finF, 0 - sizeof(fileSize) - sizeof(flagSize), SEEK_END);
	fread(&flagSize, sizeof(flagSize), 1, finF);

	//将读取标记信息的文件指针移动到保存标记数据的起始位置
	fseek(finF, 0 - sizeof(flagSize) - sizeof(fileSize) - flagSize, SEEK_END);

	size_t pos = strFilePath.find('.');
	std::string uncompression = strFilePath.substr(0, pos);
	std::string uncompressFile = uncompression + "_un." + postFix;
	//开始解压缩
	//写解压缩的数据
	FILE *fout = fopen(uncompressFile.c_str(), "wb");
	assert(fout);

	//用来解压缩长度距离对
	FILE *f = fopen(uncompressFile.c_str(), "rb");

	UCH bitCount = 0;
	UCH chFlag = 0;
	ULL encodeCount = 0;
	while (encodeCount < fileSize)
	{
		//读取一个标记
		if (0 == bitCount)
		{
			chFlag = fgetc(finF);
			bitCount = 8;
		}

		if (chFlag & 0x80)
		{
			//长度距离对
			USH matchLen = fgetc(finD) + 3;//当初压缩的时候减去了3所以加3
			USH matchDist = 0;
			fread(&matchDist, sizeof(matchDist), 1, finD);
			fflush(fout);//清空缓冲区
			//更新已经解码字符数量
			encodeCount += matchLen;

			//f:读取前文匹配的内容
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
			//原字符
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