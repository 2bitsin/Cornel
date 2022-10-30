add_subdirectory(call16_thunk)
add_library(hardware.x86 STATIC
  address16.cpp 
  bios.cpp 
  call16.cpp 
  call16_stack.cpp)
target_link_libraries(hardware.x86 PRIVATE hardware PUBLIC call16_thunk)