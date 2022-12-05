#include <textio/format.hpp>
#include <textio/logger.hpp>

#include <cstdio>

textio::logger_module<meta::empty_string_v<char>, textio::detail::cstdio_iterator> Glog { stdout };

// TODO: hello
