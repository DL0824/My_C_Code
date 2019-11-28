#include <iostream>
#include <cstring>
#include <assert.h>
using namespace std;

namespace my
{
    class string 
    {
        public:
            string(const char* str="")
            {
                if(str==NULL)
                {
                    str="";
                }
                //开辟对象
                _size=strlen(str);
                _capacity=_size;
                _str=new char[_capacity+1];
                //拷贝元素
                strcpy(_str,str);
            }

            string(int n,char ch)
            :_size(n)
             ,_capacity(n)
             ,_str(new char[n+1])
        {
            memset(_str,ch,n);
            _str[n]='\0';
        }

            //[begin,end)
            string(char* begin,char* end)
            {
                _size=end-begin;
                _capacity=_size;
                _str=new char[_size+1];
                strncpy(_str,begin,_size);
                _str[_size]='\0';
           }
            
            string(const string& s)
                :_size(s._size)
                 ,_capacity(s._capacity)
                 ,_str(new char[_capacity+1])
        {
            strcpy(_str,s._str);
        }

            string& operator=(const string& s)
            {
                if(this!=&s)
                {
                    int len=strlen(s._str);
                    char* tmp=new char[len+1];
                    strcpy(tmp,s._str);
                    delete[] _str;//将原来的资源释放
                    _str=tmp;
                    _size=len;
                    _capacity=len;
                }
                return *this;
            }

            ~string()
            {
                if(_str)
                {
                    delete[] _str;
                    _str=NULL;
                    _capacity=_size=0;
                }
            }

            //与空间相关的操作
            int size()const //const修饰this指针
            {
                return _size;
            }

            int capacity()const
            {
                return _capacity;
            }

            bool empty()const
            {
                return 0==_size;
            }

            void resize(int newsize,char ch)
            {
                int oldsize=size();
                if(newsize>oldsize)
                {
                    //先检查是否需要扩容
                    if(newsize>_capacity)
                    {
                        reserve(newsize);//扩容
                    }
                    //原来_str[_size]是'\0'
                    memset(_str+_size,ch,newsize-oldsize);
                }
                _size=newsize;
                _str[_size]='\0';//如果newsize<oldsize,直接把newsize位置的内容编程'\0',那么newsize之后的内容就无效了
            }

            void reserve(int newcapacity)
            {
                int oldcapacity=capacity();
                if(newcapacity>oldcapacity)
                {
                    //申请新空间
                    char* tmp=new char[newcapacity+1];
                    //拷贝元素(转移)
                    strcpy(tmp,_str);
                    //释放旧空间
                    delete[] _str;
                    _str=tmp;
                    _capacity=newcapacity;
                }
            }

            //获取元素
            char& operator[](int index)
            {
                assert(index<_size);
                return _str[index]; 
            }
            
           const char& operator[](int index)const
           {
               assert(index<_size);
               return _str[index];
           }
           
           void push_back(char ch)
           {
               if(_size==_capacity)
                   reserve(2*_capacity);
               _str[_size++]=ch;
               _str[_size]='\0';
           }

           string& operator+=(char ch)
           {
               push_back(ch);
               return *this;
           }

           friend ostream& operator<<(ostream& _cout,const my::string& s)
           {//因为this指针默认占据第一个形参的位置,所以用friend
               _cout<<s.c_str();
               return _cout;
           }

           friend istream& operator>>(istream& _cin,my::string& s)
           {
               _cin>>s;
               return _cin;
           }

           const char* c_str()const
           {
               return _str;
           }

           int find(char ch,int pos=0)
           {
               for(int i=pos;i<_size;++i)
               {
                   if(ch==_str[i])
                   {
                       return i;
                   }
               }
               return npos;
           }

           int rfind(char ch,int pos=npos)
           {
               if(pos==npos)
                   pos=_size-1;//最后一个元素的下标,左开右闭
               for(int i=pos;i>=0;i--)
               {
                   if(ch==_str[i])
                   {
                       return i;
                   }
               }
               return npos;
           }

           string substr(int pos=0,int n=npos)
           {//返回的是栈上的内容所以不用引用类型的返回
               if(n==npos)
                   n=_size;
               string tmp(_str+pos,_str+pos+n);
               return tmp;
           }
        private:
            int _size;
            int _capacity;
            char* _str;
            static int npos;
    };
    int npos=-1;
}
int main()
{

    return 0;
}
