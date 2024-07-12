add_library(hardware.ibm 
  console.cpp
  kbdctrl.cpp
  pic8259.cpp
  rtccmos.cpp
  atwenty.cpp)
target_link_libraries(hardware.ibm PRIVATE hardware hardware.x86)