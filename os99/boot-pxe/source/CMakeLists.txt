cmake_minimum_required(VERSION 3.22.0)
project(boot-pxe LANGUAGES C CXX ASM)

add_executable(
  boot-pxe 
  core/error.cpp 
  core/sysinfo.cpp
  x86/flatmem.cpp
  x86/bios.cpp 
  pxe/pxe.cpp 
  main.cpp)
target_compile_options(boot-pxe PRIVATE -3)
target_include_directories(boot-pxe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_options(boot-pxe PRIVATE option stack=48k)
install(TARGETS boot-pxe DESTINATION ${CMAKE_INSTALL_PREFIX})
