#ifndef __ASSERT_HPP__
#define __ASSERT_HPP__

#include "termio.hpp"
#include "inlasm.hpp"

#define __STRINGIFY_INNER(X) #X
#define __STRINGIFY(X) __STRINGIFY_INNER(X)
#define __LINE_AS_STRING__ __STRINGIFY(__LINE__)

#ifndef NDEBUG
  #define assert(condition) co_assert(condition, __FILE__ ":" __LINE_AS_STRING__, #condition) 
#else 
  #define assert(condition) condition
#endif

inline void co_assert(bool cond, char const* location, char const* reason)
{
  if (!cond)
  {
    tty_put_string("Assertion failed at ");
    tty_put_string(location);
    tty_put_string(":\n");   
    tty_put_string(reason);
    tty_put_string("\n");
    irq_disable();
    irq_wait();
  }
}

#endif