#pragma once

#include "LZHashTable.h"
#include <string>
#include <assert.h>
#include <iostream>

class LZ77
{
public:
	LZ77();
	~LZ77();
	void CompressFile(const std::string& strFilePath, std::string& postFix);
	void UNCompressFile(const std::string& strFilePath, std::string postFix);
private:
	USH LongestMatch(USH matchHead, USH& curMatchDist, USH start);
	void writeFlag(FILE *fout, UCH& chFlag, UCH& bitCount, bool isLen);
	void MergeFile(FILE *fout, ULL fileSize, std::string compressFlag);
	void FillWindow(FILE *fin, size_t& lookAhead, USH& start);
private:
	UCH* _pwin;//用来保存待压缩数据的缓冲区
	LZHashTable _ht; 
};