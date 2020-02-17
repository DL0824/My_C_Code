#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <queue>

//线程池的线程数量
#define THREAD_CAPACITY 5
//创建任务类：任务+如何处理(函数指针)
typedef bool (*handler_t)(int);
class ThreadTask
{
    private:
        int _data;//任务（数据）
        handler_t _handler;//函数指针
    public:
        ThreadTask(int data = -1,handler_t handler = NULL)
            :_data(data)
             ,_handler(handler)
    {}

        void Run()
        {
            _handler(_data);
        }
};

//创建线程池：有一个线程安全的队列+多个线程
class ThreadPool
{
    private:
        int _thread_capacity;//线程池线程容量
        int _thread_size;//线程池实时线程数量
        bool _tp_quit;//线程池退出标志

        //线程安全的队列要满足互斥和同步
        std::queue<ThreadTask *> _task_queue;
        pthread_mutex_t _mutex;
        pthread_cond_t _cond;
    private:
        void LockQueue()
        {
            pthread_mutex_lock(&_mutex);
        }
        void UnLockQueue()
        {
            pthread_mutex_unlock(&_mutex);
        }
        void WakeUpOne()
        {
            pthread_cond_signal(&_cond);
        }
        void WakeUpAll()
        {
            pthread_cond_broadcast(&_cond);
        }
        void ThreadQuit()
        {
            _thread_size--;
            UnLockQueue();
            pthread_exit(NULL);
        }
        void ThreadWait()
        {
            if(_tp_quit)
            {
                ThreadQuit();
            }
            pthread_cond_wait(&_cond,&_mutex);
        }
        bool IsEmpty()
        {
            return _task_queue.empty();
        }

        bool popTask(ThreadTask **tt)
        {//只在线程入口函数调用过，在访问入口函数的时候已经加锁，所以不用加锁
        //并且我们的线程池一般就是将任务压入队列让线程池处理，不需要pop所以将其变成私有函数，不让用户调用更好
            *tt=_task_queue.front();
            _task_queue.pop();
            return true;
        }

        static void *ThreadStart(void *arg)
        {//避免this指针所以用static
            ThreadPool *tp = (ThreadPool *)arg;
            while(1)
            {
                tp->LockQueue();
                while(tp->IsEmpty())
                {
                    tp->ThreadWait();
                }
                ThreadTask *tt;
                tp->popTask(&tt);
                tp->UnLockQueue();
                tt->Run();
                delete tt;
            }
            return NULL;
        }
    public:
        ThreadPool(int capacity=THREAD_CAPACITY)
            :_thread_capacity(capacity)
             ,_thread_size(capacity)
             ,_tp_quit(false)
    {
        pthread_mutex_init(&_mutex,NULL);
        pthread_cond_init(&_cond,NULL);
    }

        ~ThreadPool()
        {
            pthread_mutex_destroy(&_mutex);
            pthread_cond_destroy(&_cond);
        }

        bool PoolInit()
        {
            pthread_t tid;
            for(int i=0;i<THREAD_CAPACITY;++i)
            {
                int ret=pthread_create(&tid,NULL,ThreadStart,this);
                if(ret != 0)
                {
                    std::cout<<"create thread error\n";
                    return false; 
                }
            }
            return true;
        }

        bool pushTask(ThreadTask *tt)
        {
            //因为要实现线程安全的队列所以要加锁
            LockQueue();
            if(_tp_quit)
            {
                UnLockQueue();
                return false;
            }
            _task_queue.push(tt);
            WakeUpOne();
            UnLockQueue();
            return true;
        }
        bool poolQuit()
        {
            LockQueue();
            _tp_quit = true;
            UnLockQueue();
            if(_thread_size > 0)
            {
                WakeUpAll();
                usleep(1000);
            }
            return true;
        }
};

bool handler(int data)
{
    srand(time(NULL));
    int n=rand() % 5;
    printf("Thread: %p Run Tast: %d--sleep %d sec\n",pthread_self(),data,n);
    sleep(n);
    return true;
}

int main()
{
    int i;
    ThreadPool pool;
    pool.PoolInit();
    for(i = 0;i<10;i++)
    {
        ThreadTask *tt = new ThreadTask(i,handler);
        pool.pushTask(tt);
    }

    //pool.poolQuit();
    return 0;
}
