#include"timer.h"

namespace socks{

bool Timer::Comparator::operator()(const Timer::ptr& lhs,const Timer::ptr& rhs) const{

    return lhs.get()<rhs.get();
}


} // namespace socks
