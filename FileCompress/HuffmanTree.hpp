#pragma once
#include <vector>
#include <queue>


//构建哈夫曼树，带权
template<class W>
struct HuffmanTreeNode
{//哈夫曼节点（带权）
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
{//优先级队列比较器
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
		//1.构建森林
		std::priority_queue<Node *,std::vector<Node *>,Less<W>> q;
		for (auto e : vWeight)
		{
			if (e == invalued)
				continue;//如果权值无效就无需参与森林的构建，字符出现0次代表权值无效

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
		//当q.szie()==1的时候也就是优先级队列中只有一棵树，也就是最终的哈夫曼树
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