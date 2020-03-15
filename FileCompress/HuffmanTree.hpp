#pragma once
#include <vector>
#include <queue>


//����������������Ȩ
template<class W>
struct HuffmanTreeNode
{//�������ڵ㣨��Ȩ��
	HuffmanTreeNode(const W& weight = W())
	:_pLeft(nullptr)
	, _pRight(nullptr)
	, _pParent(nullptr)
	, _weight(weight)
	{}
	HuffmanTreeNode<W>* _pLeft;
	HuffmanTreeNode<W>* _pRight;
	HuffmanTreeNode<W>* _pParent;
	W _weight;
};

template<class W>
class Less
{//���ȼ����бȽ���
	typedef HuffmanTreeNode<W> Node;
public:
	bool operator()(const Node *pLeft, const Node *pRight)
	{
		return pLeft->_weight > pRight->_weight;
	}
};

template<class W>
class HuffmanTree
{
	typedef HuffmanTreeNode<W> Node;
public:
	HuffmanTree()
		:_pRoot(nullptr)
	{}

	HuffmanTree(const std::vector<W>& vWeight,const W& invalued)
	{
		CreateHuffmanTree(vWeight, invalued);
	}

	~HuffmanTree()
	{
		DestroyTree(_pRoot);
	}

	Node* GetRoot()
	{
		return _pRoot;
	}

private:
	void CreateHuffmanTree(const std::vector<W>& vWeight,const W& invalued)
	{
		//1.����ɭ��
		std::priority_queue<Node *,std::vector<Node *>,Less<W>> q;
		for (auto e : vWeight)
		{
			if (e == invalued)
				continue;//���Ȩֵ��Ч���������ɭ�ֵĹ������ַ�����0�δ���Ȩֵ��Ч

			q.push(new Node(e));
		}

		//2.
		while (q.size() > 1)
		{
			Node *pLeft = q.top();
			q.pop();
			Node *pRight = q.top();
			q.pop();
			Node *pParent = new Node(pLeft->_weight + pRight->_weight);
			pParent->_pLeft = pLeft;
			pParent->_pRight = pRight;

			pLeft->_pParent = pParent;
			pRight->_pParent = pParent;
			q.push(pParent);
		}
		//��q.szie()==1��ʱ��Ҳ�������ȼ�������ֻ��һ������Ҳ�������յĹ�������
		_pRoot = q.top();
	}

	void DestroyTree(Node*& pRoot)
	{
		if (pRoot)
		{
			DestroyTree(pRoot->_pLeft);
			DestroyTree(pRoot->_pRight);
			delete pRoot;
			pRoot = nullptr;
		}
	}
private:
	Node *_pRoot;
};