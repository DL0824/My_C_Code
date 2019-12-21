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

        Node *pDelete=pCur;//记录要删除的节点

        //删除
       if(pCur->_pLeft==NULL)
       {//只有右孩子或者为叶子节点
           if(pPar)
           {
               //删除的不是根节点
               if(pCur==pPar->_pLeft)
               {
                   pPar->_pLeft=pCur->_pRight;
               }
               else
               {
                   pPar->_pRight=pCur->_pRight;
               }
           }
           else
           {
               //删除的是根节点
               _pRoot=pCur->_pRight;
           }
       }
       else if(pCur->_pRight==NULL)
       {//只有左孩子
           if(NULL==pPar)
           {
               _pRoot=pCur->_pLeft;
           }
           else
           {
               if(pCur==pPar->_pLeft)
               {
                   pPar->_pLeft=pCur->_pLeft;
               }
               else
               {
                   pPar->_pRight=pCur->_pLeft;
               }
           }
       }
       else
       {//左右孩子均存在
           //在pCur的右子树找一个节点替代--->找右子树中值最小的（最左边的）
           //在pCur的左子树找一个节点替代--->找左子树中值最大的（最右边的）
           //这里找右子树中的节点
           Node *pDel=pCur->_pRight;
           pPar=pCur;
           while(pDel->_pLeft)
           {
               //找替代节点
               pPar=pDel;
               pDel=pDel->_pLeft;
           }
           pCur->_data=pDel->_data;

           //删除替代节点
           //替代节点一定没有左子树可能有右子树
           if(pDel==pPar->_pLeft)
           {
               pPar->_pLeft=pDel->_pRight;
           }
           else
           {
               pPar->_pRight=pDel->_pRight;
           }

           pDelete=pDel;
       }

       delete pDelete;
       return true;
    }

    Node *Find(const T& data)
    {
        Node *pCur=_pRoot;
        while(pCur)
        {
            if(data==pCur->_data)
                return pCur;
            else if(data<pCur->_pLeft)
                pCur=pCur->_pLeft;
            else
                pCur=pCur->_pRight;
        }
        return NULL;
    }
    //验证：二叉搜索的中序遍历一定是有序序列
    void InOrder()
    {//再给一层函数是为了防止用户去传参验证提高程序的安全性
        _InOrder(_pRoot);
    }

    Node *LeftMost()
    {
        if(NULL==_pRoot)
            return NULL;
        Node *pCur=_pRoot;
        while(pCur->_pLeft)
        {
            pCur=pCur->_pLeft;
        }
        return pCur;
    }
    Node *RightMost()
    {
        if(NULL==_pRoot)
            return NULL;
        Node *pCur=_pRoot->_pRight;
        while(pCur->_pRight)
        {
            pCur=pCur->_pRight;
        }
        return pCur;
    }
    private:
    void _InOrder(const Node *pRoot)
    {
        if(pRoot)
        {
            _InOrder(pRoot->_pLeft);
            cout<<pRoot->_data<<" ";
            _InOrder(pRoot->_pRight);
        }
    }
    private:
    Node *_pRoot;
};

void Test()
{
    BSTree<int> t;
    int a[]={5,4,3,1,7,8,2,2,2,1,0,9};
    for(int i=0;i<12;++i)
    {
        t.Insert(a[i]);
    }
    t.InOrder();
    cout<<endl;
    cout<<t.LeftMost()->_data<<endl;
    cout<<t.RightMost()->_data<<endl;
    t.Delete(8);
    t.InOrder();
    cout<<endl;
}
int main()
{
    Test();
    return 0;
}
