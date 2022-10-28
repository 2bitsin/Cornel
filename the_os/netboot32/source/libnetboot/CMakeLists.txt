
add_library(netboot
  interrupts.s 
  interrupts.cpp
  runtime.cpp
  memory.cpp
  panick.cpp
  main.cpp
  keyboard.cpp
  cstdio_impl.cpp
  progress.cpp) 

target_include_directories(netboot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(netboot PUBLIC utils textio pxenv hardware script)
  