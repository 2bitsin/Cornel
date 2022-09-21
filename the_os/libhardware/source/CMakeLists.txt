add_subdirectory(call16_thunk)
add_library(hardware 
  hardware.cpp 
  pic8259.cpp 
  console.cpp 
  kbdctrl.cpp 
  atwenty.cpp 
  x86gdt.cpp 
  x86call16.cpp
  x86call16_stack.cpp
)

target_include_directories(hardware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(hardware PUBLIC cimpl utils textio memory call16_thunk)
set_target_properties(hardware PROPERTIES CXX_STANDARD 23)