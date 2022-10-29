#include <textio/format.hpp>
#include <textio/logger.hpp>

#include <cstdio>

textio::logger_module<meta::string<0u, char>{}, textio::detail::cstdio_iterator> Glog { stdout };
