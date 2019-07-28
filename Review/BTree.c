
/*
借助栈来实现非递归的前中后三序遍历


递归的思想就相当于入栈；
每一次递归要是终止条件不满足，就进行下一次递归；
就相当于先把第一次递归入栈，如果第二次递归还是不满足终止条件，继续入栈；
直到终止条件满足了，就从最后一次递归开始出栈；
所以要实现三序遍历的非递归遍历，那就要用到栈，把递归转换成栈来实现

比如中序遍历，是先左再根最后右，所以我们就要先打印根的左孩子，但是可以这样理解，根的左右子树就相当于左右孩子；
因为二叉树只能从根开始遍历，所以我们找左孩子的时候先找到根的左孩子；
但这个时候根的左孩子并不是叶子节点，而是另外一颗树，根的左树；
所以根据递归的思想我们就又要找这棵左树的左树，也就是把左树中那个和最开始的根直接相连的节点看作新的根，再找新根的左孩子；
就这样一直递归下去，直到找到叶子节点，那我们就可以打印这个叶子节点的数据了，这个叶子节点就相当于终止条件；
因为是递归的思想但是不能用递归，所以我们可以借助栈来实现；
每次找左孩子的时候，只要左孩子不是叶子节点，就将左孩子的根压栈，直到找到叶子节点就可以层层出栈了

前序遍历和后序遍历的大体思想和中序遍历差不多，只不过是谁先打印数据，谁先压栈的问题
*/


void BinaryTreePrevOrder(BTNode * root)
{//非递归实现前序遍历   根左右
	Stack st;//创建一个栈
	BTNode * tmp = root;
	StackInit(&st,100);//初始化栈
	while(tmp||!StackIsEmpty(&st))
	{
		printf("%c",tmp->data);//打印根的数据
		if(tmp->_right)
		{//如果当前根的右孩子存在就把右孩子压栈
			StackPush(&st,tmp->_right);
		}
		if(tmp->_left)
		{//如果当前根的左孩子存在就访问当前根的左孩子
			tmp=tmp->_left;
		}
		else
		{//如果当前根的左孩子不存在那么就让当前根变成栈顶的那个根 
	     //然后再弹栈
			tmp=StackFront(&st);
			StackPop(&st);
		}
	}
	StackDestory(&st);
}

void BinaryTreeInOrder(BTNode * root)
{
	Stack st;
	BTNode * tmp = root;
	StackInit(&st);
	
	while(tmp||!StackIsEmpty)
	{
		for(;tmp;tmp->_left)
		{
			StackPush(&st,tmp);
		}
		tmp=StackFront(&st);
		printf("%c",tmp->data);
		StackPop(&st);
		tmp=tmp->_right;
	}
	StackDestory(&st);
}

void BinaryTreePostOrder(BTNode * root)
{
	Stack st;
	BTNode * tmp = root;
	StackInit(&st);
	int tag[]={0};
	
	while(tmp||!StackIsEmpty)
	{
		for(;tmp;tmp=tmp->_left)
		{
			StackPush(&st,tmp);
			tag[st.size]=0;
		}
		while(!StackIsEmpty&&tag[st.size]==1)
		{
			printf("%c",StackFront(&st)->data);
			StackPop(&st);
			tmp=NULL;
		}
		if(!StackIsEmpty)
		{
			tag[st.size]=1;
			tmp=StackFront(&st)->_right;
		}
		


	