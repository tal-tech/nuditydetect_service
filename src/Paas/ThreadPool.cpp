#include <assert.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include "ThreadPool.h"
#include <fstream> 

//template<class T1, class T2> int ThreadPool<T1, T2>::m_maxThreadsSize=10;
//template<class T1, class T2> int ThreadPool<T1, T2>::m_minThreadsSize=3;
//template<class T1, class T2> int ThreadPool<T1, T2>::m_TaskQueueSize=110;
//template<class T1, class T2> int ThreadPool<T1, T2>::m_ReservedQSizeForFirstTask=10;

template<class T1, class T2> ThreadPool<T1, T2>::ThreadPool() 
{
	//m_taskmutex();
	//m_cond(m_taskmutex);		
	m_isStarted=false;
	
	m_maxThreadsSize=10;
	m_minThreadsSize=3;
	m_TaskQueueSize=110;
	m_ReservedQSizeForFirstTask=10;
}

template<class T1, class T2> ThreadPool<T1, T2>::~ThreadPool()
{
  if(m_isStarted)
  {
    stop();
  }
}

template<class T1, class T2> void ThreadPool<T1, T2>::start()
{
	if(m_minThreadsSize>m_maxThreadsSize)
	{
		m_minThreadsSize=m_maxThreadsSize;
	}
   assert(m_threads.empty());
   m_isStarted = true;
 //m_threads.reserve(m_maxThreadsSize); 
 
  for (int i = 0; i <m_minThreadsSize; ++i)
  {
	 // std::cout << "add thrend - " << i << std::endl;
	 try
	 {
       m_threads.push_back(new std::thread(std::bind(&ThreadPool<T1, T2>::threadloop, this)));
     }
	 catch(...)
	{
		std::cout <<"### ThreadPool::start() add new thread failed \r\n";
	}
  }

}

template<class T1, class T2> void ThreadPool<T1, T2>::stop()
{  
    std::unique_lock<std::mutex> lock(m_threadmutex);
    m_isStarted = false;   
	m_cond.notify_all();   
   for (Threads::iterator it = m_threads.begin(); it != m_threads.end() ; ++it)
   {
     (*it)->join();
     delete *it;
   }
   m_threads.clear();
}


template<class T1, class T2> bool ThreadPool<T1, T2>::eraseCurrentThread()
{
	std::unique_lock<std::mutex> lock(m_threadmutex);
	bool isErased = false;
	
   if (m_threads.size() > m_minThreadsSize && m_tasks.size()==0)
   {	
     for (char i=0;i< m_threads.size();i++)
     {
	  std::ostringstream sin;
	  sin << m_threads[i]->get_id();
	  std::cout << "##m_threads[i]->get_id() : "+ sin.str() + "\r\n";
	  
	   if (m_threads[i]->get_id() == std::this_thread::get_id())
	   {		 
		 std::stringstream sin;
		 sin << std::this_thread::get_id();		
		 std::cout << "##ThreadPool::earse tid : " + sin.str() + "exit.\r\n";
		 
		  m_threads[i]->detach();
		 //m_threads[i]->join();		 
		 delete  m_threads[i];
		 m_threads.erase(m_threads.begin()+i);
		 isErased = true;
	  	 break; 
	   }	 
     }
   }
   return isErased;
}


template<class T1, class T2> void ThreadPool<T1, T2>::threadloop()
{  
  T1 m_instance;  
  
  while(m_isStarted)
  {
         std::cout<<"threadloop ---->0\n";
         std::ofstream ifs("data//thread.txt", std::ios::app);//追加写
         ifs << "----->0!!m_tasks.size():" << m_tasks.size() << " m_threads.size():" << m_threads.size() << " m_minThreadsSize:" << m_minThreadsSize << "\n";

         ifs.close();




	  T2 task;
	  if (take(task))
	  {
		  m_instance.set(task);
		  m_instance.process();
	  }	 

	   if (m_tasks.size()==0 && m_threads.size() > m_minThreadsSize)
	  {  		  
		  std::stringstream sin;
		  sin << std::this_thread::get_id();
	  std::cout << "###current thread" + sin.str()+" exit, m_threads.size() is" + std::to_string(m_threads.size()) +"\r\n";		  
		  if (eraseCurrentThread())
		  {
			  break;
		  }
	  }
	   
	 std::cout << "##m_tasks.size():" << m_tasks.size() << " m_threads.size():" << m_threads.size() << " m_minThreadsSize:" << m_minThreadsSize << std::endl;

  } 
  
}

template<class T1, class T2> void ThreadPool<T1, T2>::addTask(const Task& task)
{
  std::unique_lock<std::mutex> lock(m_taskmutex);
  while(tasksIsFull())
 {	 
	  std::cout << "##addTask :  is full "+ std::to_string(m_tasks.size())+"\r\n";
	  m_notFull.wait(lock);
  }
   
  while( !haveQueueSizeForFirstTasks())
  {
	  m_notFull.wait(lock);
  }
   
  Task_Pair taskPair(level2, task);
  m_tasks.push(taskPair);

 // std::cout << "addTask  m_tasks.size()" << m_tasks.size()<< std::endl;
    m_notEmpty.notify_one(); //.notify_all();  
  
//添加线程数
  if (m_tasks.size() >=m_threads.size()*2 && m_threads.size()< m_maxThreadsSize)
  {  
     try
	 {
		std::unique_lock<std::mutex> lock(m_threadmutex);
	   std::cout << "@@ThreadPool::add new thread \r\n" + std::to_string(m_tasks.size()) + "\r\n";
	   m_threads.push_back(new std::thread(std::bind(&ThreadPool<T1, T2>::threadloop, this)));
	   std::cout << "@$ThreadPool::add new thread \r\n" + std::to_string(m_tasks.size()) + "\r\n";
	}
    catch(...)
	{
		std::cout <<"### ThreadPool::addTask add new thread failed \r\n";
	}
  }
}

template<class T1, class T2> void ThreadPool<T1, T2>::addTask(const Task_Pair& taskPair)
{
  std::unique_lock<std::mutex> lock(m_taskmutex);
  
  while( tasksIsFull())
  {	 
  	 m_notFull.wait(lock);
  }  
  //while( taskPair.first!=level0 && !haveQueueSizeForFirstTasks())
  //{	 
	//  m_notFull.wait(lock);
  //}
  
  
  
  m_tasks.push(taskPair);  
  m_notEmpty.notify_one();
  
  //添加线程数
  if (m_tasks.size() >=m_TaskQueueSize/2 && m_threads.size()< m_maxThreadsSize)
  {  
     try
	 {
		std::unique_lock<std::mutex> lock(m_threadmutex);
	    std::cout << "@@ThreadPool::add new thread \r\n" + std::to_string(m_tasks.size()) + "\r\n";
	    m_threads.push_back(new std::thread(std::bind(&ThreadPool<T1, T2>::threadloop, this)));
	    std::cout << "@$ThreadPool::add new thread \r\n" + std::to_string(m_tasks.size()) + "\r\n";
	}
    catch(...)
	{
		std::cout <<"### ThreadPool::addTask add new thread failed \r\n";
	}
  }
 
}


template<class T1, class T2> bool ThreadPool<T1, T2>::take(Task & task)
{
  std::unique_lock<std::mutex> lock(m_taskmutex);
  
  while(m_tasks.empty() && m_isStarted)
  { 	
	  std::stringstream sin;
	  sin << std::this_thread::get_id();
	  std::cout << "!!!ThreadPool::take() tid : " + sin.str()+ " wait.\r\n" ;	
	 
	 m_notEmpty.wait(lock);	 	 
  }
  
 // std::cout << "ThreadPool::take() tid : " << std::this_thread::get_id() << " wakeup." << std::endl; 
  int size = m_tasks.size();
  if(!m_tasks.empty() && m_isStarted)
  {
     task = m_tasks.top().second;
     m_tasks.pop();
     assert(size - 1 == m_tasks.size());
     if (m_TaskQueueSize > 0)
     {     
	  m_notFull.notify_one();
     }
  }
  return true;
}


template<class T1, class T2> bool ThreadPool<T1, T2>::tasksIsFull() const
{
	return (m_TaskQueueSize <= 0 || m_tasks.size() >= m_TaskQueueSize);
}


template<class T1, class T2> bool ThreadPool<T1, T2>:: haveQueueSizeForFirstTasks() const
{
	return m_TaskQueueSize > 0 && m_tasks.size() < m_TaskQueueSize-m_ReservedQSizeForFirstTask;
	
}


