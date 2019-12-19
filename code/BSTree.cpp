#include <iostream>
using namespace std;

//实现节点
template<class T>
struct BSTNode
{
    BSTNode(const T& data=T())
        :_pLeft(NULL)
         ,_pRight(NULL)
         ,_data(data)
    {}
    BSTNode<T> *_pLeft;
    BSTNode<T> *_pRight;
    T _data;
};

//实现二叉搜索树
//假设：二叉搜索树中的元素唯一(不能说明二叉搜索树中的数据一定是唯一的，可以重复)
template<class T>
class BSTree
{
    typedef BSTNode<T> Node;
    public:
    BSTree()
        :_pRoot(NULL)
    {}

    bool Insert(const T& data)
    {
        //空树
        if(_pRoot==NULL)
        {
            _pRoot=new Node(data);
            return true;
        }

        //非空
        //找到待插入节点的位置
        Node *pCur=_pRoot;
        Node *pPar=NULL;
        while(pCur)
        {
            pPar=pCur;
            if(data>pCur->_data)
            {
                pCur=pCur->_pRight;
            }
            else if(data<pCur->_data)
            {
                pCur=pCur->_pLeft;
            }
            else
            {
                return false;
            }
        }
        //插入节点
        pCur=new Node(data);
        if(pCur->_data<pPar->_data)
            pPar->_pLeft=pCur;
        else
            pPar->_pRight=pCur;
        return true;
    }

    bool Delete(const T& data)
    {
        if(_pRoot==NULL)
            return false;

        //找到待删除的节点
        Node *pCur=_pRoot;
        Node *pPar=NULL;
        while(pCur)
        {
            if(data==pCur->_data)
                break;//找到了就跳出
            else if(data>pCur->_data)
            {
                pPar=pCur;
                pCur=pCur->_pRight;
            }
            else
            {
                pPar=pCur;
                pCur=pCur->_pLeft;
            }
        }
        //而没找到要删除的就返回错误
        if(pCur==NULL)
            return false;

        //删除
        
    }
    private:
    Node *_pRoot;
};


int main()
{

    return 0;
}
