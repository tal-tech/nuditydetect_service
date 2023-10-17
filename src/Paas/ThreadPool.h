#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <vector>
#include <utility>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

//#include "Condition.hh"
template<class T1 , class T2> 
class ThreadPool
{
 public:
	 ThreadPool();
	 ~ThreadPool();
  //static  int m_maxThreadsSize;
  //static  int m_minThreadsSize;
  //static  int m_TaskQueueSize;
  //static  int m_ReservedQSizeForFirstTask;//为优先级高的任务预留的队列空间
   int m_maxThreadsSize;
   int m_minThreadsSize;
   int m_TaskQueueSize;
   int m_ReservedQSizeForFirstTask;//为优先级高的任务预留的队列空间
   enum taskPriority { level0, level1, level2};  
 
  typedef T2 Task;  // typedef std::function<void()> Task;
  //typedef std::pair<taskPriority, T2> Task_Pair;
  typedef std::pair<int, T2> Task_Pair;

  void start();
  void stop();
  void addTask(const Task &);
  void addTask(const Task_Pair&);
  typedef std::vector<std::thread*> Threads;
  Threads m_threads;  
 
private:

   ThreadPool(const ThreadPool&) {};  //禁止复制拷贝   
   ThreadPool& operator = (const ThreadPool &) {};

   struct TaskPriorityCmp
   {
     bool operator()(const Task_Pair p1, const Task_Pair p2)
     {
         return p1.first > p2.first; //first的小值优先
     }
   };


   void threadloop();
   bool take( Task & task);
   bool tasksIsFull() const;
   bool haveQueueSizeForFirstTasks() const;
   bool eraseCurrentThread();
   
   typedef std::priority_queue<Task_Pair, std::vector<Task_Pair>, TaskPriorityCmp> Tasks;
    
   Tasks m_tasks;

   std::mutex m_taskmutex;  
   std::mutex m_threadmutex;
   std::condition_variable m_cond;
   std::condition_variable m_notEmpty;
   std::condition_variable m_notFull;
   bool m_isStarted;
     
};

#endif
