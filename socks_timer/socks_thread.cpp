#include <iostream>
#include <syscall.h>
#include <unistd.h>  

#include "socks_thread.h" 

static int count=0;

namespace socks
{
pid_t GetThreadId() {
    return syscall(SYS_gettid);
}
static thread_local Thread* t_thread=nullptr;
static thread_local std::string t_thread_name="UNKNOWN";


//Semaphore
Semaphore::Semaphore(uint32_t count){
    if(sem_init(&m_semaphore,0,count)){
        throw std::logic_error("sem_init error!");
    }
}
Semaphore::~Semaphore(){
    sem_destroy(&m_semaphore);
}

void Semaphore::wait(){
    if(sem_wait(&m_semaphore))
        throw std::logic_error("sem_wait error!!");
}
void Semaphore::notify(){
    if(sem_post(&m_semaphore)){
        throw std::logic_error("sem_post error!!");
    }
}

//Thread
Thread* Thread::GetThis()
{
    return t_thread;
}
void Thread::SetName(const std::string& name)
{
    if(name.empty())
        return;
    if(t_thread)
    {
        t_thread->m_name=name;
    }
    t_thread_name=name;
}

Thread::Thread(std::function<void()> cb,const std::string& name):m_cb(cb),m_name(name)
{
    if(name.empty())
        m_name="unknown";
    int rt=pthread_create(&m_thread,nullptr,&Thread::run,this);
    if(rt)
        std::cout<<"tr pthread create"<<std::endl;
  
    m_semaphore.wait();
    
}
Thread::~Thread()
{
    if(m_thread){
    }
}
void Thread::join()
{
    if(m_thread)
    {
        /*if(pthread_detach(m_thread)){
            std::cout<<"1111getName="<<getName()<<std::endl;    
        } */
        int rt=pthread_join(m_thread,nullptr);
        if(rt)
            std::cout<<"rt prhread_join"<<std::endl;
    }
}
void* Thread::run(void* arg)
{
    Thread *thread=(Thread*)arg;
    t_thread=thread;
    t_thread_name=thread->m_name;
    thread->m_id= socks::GetThreadId();
    
    pthread_setname_np(pthread_self(),thread->m_name.substr(0,15).c_str());     //给线程命名 1：线程 2：名称 

    std::function<void()> cb;
    cb.swap(thread->m_cb);                  //

    thread->m_semaphore.notify();

    cb();
    return 0;
}

} // namespace sylar
