#ifndef __TIMER_H__
#define __TIMER_H__

#include<memory>
#include"socks_thread.h"
#include<set>

namespace socks
{
class TimerManager;
class Timer //:public std::enable_share_from_this<Timer>
{
friend class TimerManager;
public:
    typedef std::shared_ptr<Timer> ptr;
private:
    Timer(uint64_t ms,std::function<void()> cb,
        bool recurring,TimerManager* manager);
private:
    struct Comparator{
        bool operator()(const Timer::ptr& lhs,const Timer::ptr& rhs) const;
    };
private:
    bool m_recurring=false;         //
    uint64_t m_ms=0;
    uint64_t m_next=0;
};
class TimerManager{
friend class Timer;
public:
    typedef RWMutex RWMutexType;
    TimerManager();
    virtual ~TimerManager();
    Timer::ptr addTimer(uint64_t ms,std::function<void()> cb,
            bool recurring=false);
    Timer::ptr addConditionTimer(uint64_t ms,std::function<void()> cb,
            std::weak_ptr<void> weak_cond,
            bool recurring=false);
private:
    RWMutexType m_mutex;
    std::set<Timer::ptr,Timer::Comparator> m_timers;
};

} // namespace socks



#endif
