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

//hashAddr����һ�����ַ�����������ĵ�ַ��ch�ǵ�ǰ�ַ�
void LZHashTable::HashFunc(USH& hashAddr, UCH ch)
{
	hashAddr = (((hashAddr) << H_SHIFT()) ^ (ch)) & HASH_MASK;
}
USH LZHashTable::H_SHIFT()
{
	return (HASH_BITS + MIN_MATCH - 1) / MIN_MATCH;
}


//hashAddr����һ�����ַ�����������ĵ�ַ��ch�ǵ�ǰ�ַ������һ���ַ�
//pos�ǵ�ǰ��ѹ���ַ����±꣬matchhead���������ƥ�䣬����ƥ�䴮����ʼλ��
void LZHashTable::Insert(USH& matchHead, UCH ch, USH pos, USH& hashAddr)
{
	//�ȼ����ϣ��ַ
	HashFunc(hashAddr, ch);

	//�ҵ�ǰ�����ַ��ڲ��һ������ҵ��������һ����ƥ������ͷ��
	//_head[hashAddr]�г�ʼʱ����0���������Ӧ�ַ�֮���ɸ��ַ����±�
	//��һ���ٽ��в����ʱ���Ƚ�_head[hashAddr]�е�ֵ��matchhead��Ҳ���������ƥ��ͷ
	matchHead = _head[hashAddr];

	//pos���ܻᳬ��32k��&MASK��Ŀ�ľ���Ϊ�˲�Խ��
	_prev[pos&HASH_MASK] = _head[hashAddr];
	_head[hashAddr] = pos;
} 

//�õ���һ��ƥ��ͷ
//��Ϊ���������ʱ��Ϊ�˽��Խ������&����HASH_MASK���Դ˴�ҲҪ&HASH_MASK
USH LZHashTable::GetNext(USH matchHead)
{
	return _prev[matchHead & HASH_MASK];
}

//���¹�ϣ��
void LZHashTable::UpDate()
{
	for (USH i = 0; i < WSIZE; ++i)
	{
		//�ȸ���head
		if (_head[i] >= WSIZE)
			_head[i] -= WSIZE;
		else
			_head[i] = 0;

		//����prev
		if (_prev[i] >= WSIZE)
			_prev[i] -= WSIZE;
		else
			_prev[i] = 0;
	}
}