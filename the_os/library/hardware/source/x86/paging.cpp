#include <hardware/x86/paging.hpp>

namespace x86arch
{
  auto Paging () noexcept -> PagingImpl& 
  {
    static PagingImpl paging;
    return paging;
  }
}

using x86arch::detail::PagingImpl;

auto PagingImpl::enable() noexcept -> bool
{


}