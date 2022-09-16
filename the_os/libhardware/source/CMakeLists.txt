add_library(hardware hardware.cpp pic8259.cpp console.cpp kbdctrl.cpp atwenty.cpp x86arch.cpp)
target_include_directories(hardware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(hardware PUBLIC cimpl misc textio)
set_target_properties(hardware PROPERTIES CXX_STANDARD 23)