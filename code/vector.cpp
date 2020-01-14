#include <iostream>

namespace my
{
	template<class T>
	class vector
	{
		typedef T* iterator;
		public:
		
		vector()
		:_start(nullptr)
		,_finish(nullptr)
		,_end(nullptr)
		{}
		
		
		vector(int n,const T& data)
		:_start(new T[n])
		{
			for(int i=0;i<n;++i)
			{
				_start[i]=data;
			}
			_finish=_start+n;
			_end=_finish;
		}
		
		
		//Iterator可能不是原生态的指针
		template<class Iterator>
		vector(Iterator first,Iterator last)
		{
			int n=0;
			auto it=first;
			while(it!=last)
			{//因为Iterator可能不是原生态的指针
			 //所以要计算first到last之间的元素个数不能用last-first
				++it;
				++n;
			}
			_start=new T[n];
			
			for(int i=0;i<n;++i)
			{
				_start[i]=*first++;
			}
			_finish=_start+n;
			_end=_finish;
		}
		
		vector(const vector<T>& v)
		:_start(new T[v.size()])
		{
			for(int i=0;i<v.size();++i)
			{
				_start[i]=v[i];
			}
			_finish=_start+v.size();
			_end=_finish;
		}
		
		
		vector<T>& operator=(const vector<T>& v);
		
		~vector()
		{
			if(_start)
			{
				delete[] _start;
				_start=_finish=_end=nullptr;
			}
		}
		
		iterator begin()
		{
			return _start;
		}
		
		iterator end()
		{
			return _finish;
		}
		
		int size()const
		{
			return _finish-_start;
		}
		
		int capacity()const
		{
			return _end-_start;
		}
		
		bool empty()const
		{
			return _start==_finish;
		}
		
		
		
		
		private:
		T *_start;
		T *_finish;
		T *_end;
	};
}
