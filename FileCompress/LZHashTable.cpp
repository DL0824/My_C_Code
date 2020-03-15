#include "LZHashTable.h"
#include <string.h>

const USH HASH_BITS = 15;
const USH HASH_SIZE = (1 << HASH_BITS);
const USH HASH_MASK = HASH_SIZE - 1;

LZHashTable::LZHashTable(USH size)
:_prev(new USH[size * 2])
, _head(_prev + size)
{
	memset(_prev, 0, size * 2 * sizeof(USH));
}

LZHashTable::~LZHashTable()
{
	delete[] _prev;
	_prev = nullptr;
}

//hashAddr：上一个“字符串”计算出的地址，ch是当前字符
void LZHashTable::HashFunc(USH& hashAddr, UCH ch)
{
	hashAddr = (((hashAddr) << H_SHIFT()) ^ (ch)) & HASH_MASK;
}
USH LZHashTable::H_SHIFT()
{
	return (HASH_BITS + MIN_MATCH - 1) / MIN_MATCH;
}


//hashAddr：上一个“字符串”计算出的地址，ch是当前字符串最后一个字符
//pos是当前待压缩字符的下标，matchhead：如果可以匹配，保存匹配串的起始位置
void LZHashTable::Insert(USH& matchHead, UCH ch, USH pos, USH& hashAddr)
{
	//先计算哈希地址
	HashFunc(hashAddr, ch);

	//找当前三个字符在查找缓冲区找到的最近的一个（匹配链的头）
	//_head[hashAddr]中初始时候是0，当插入对应字符之后变成该字符的下标
	//下一次再进行插入的时候先将_head[hashAddr]中的值给matchhead，也就是最近的匹配头
	matchHead = _head[hashAddr];

	//pos可能会超过32k，&MASK的目的就是为了不越界
	_prev[pos&HASH_MASK] = _head[hashAddr];
	_head[hashAddr] = pos;
} 

//得到下一个匹配头
//因为当初插入的时候为了解决越界问题&上了HASH_MASK所以此处也要&HASH_MASK
USH LZHashTable::GetNext(USH matchHead)
{
	return _prev[matchHead & HASH_MASK];
}

//更新哈希表
void LZHashTable::UpDate()
{
	for (USH i = 0; i < WSIZE; ++i)
	{
		//先更新head
		if (_head[i] >= WSIZE)
			_head[i] -= WSIZE;
		else
			_head[i] = 0;

		//更新prev
		if (_prev[i] >= WSIZE)
			_prev[i] -= WSIZE;
		else
			_prev[i] = 0;
	}
}