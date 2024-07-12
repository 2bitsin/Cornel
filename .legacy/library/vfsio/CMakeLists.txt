cmake_minimum_required(VERSION 3.22.1)
project(libvfsio LANGUAGES C CXX)

add_subdirectory(source)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Generic")
  enable_testing()
  add_subdirectory(tests)
	install(TARGETS vfsio DESTINATION ${CMAKE_INSTALL_PREFIX})
else()
  message(NOTICE "CMAKE_SYSTEM_NAME is ${CMAKE_SYSTEM_NAME}, skipping tests")
endif()

