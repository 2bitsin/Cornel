set(WLINK_SCRIPT            ${CMAKE_CURRENT_SOURCE_DIR}/wlink.wl)
set(NETBOOT_MAP             ${CMAKE_BINARY_DIR}/netboot.map)
set(CMAKE_EXE_LINKER_FLAGS  "option map=${NETBOOT_MAP} @${WLINK_SCRIPT}")
set(CMAKE_C_FLAGS           "-3 -s")
set(CMAKE_CXX_FLAGS         "-3 -s")
set(CMAKE_ASM_FLAGS         "-3")

add_executable(netboot
  biosacpi.s
  desctab.c
  epilogue.s
  error.c
  flatreal.s
  longdiv.s
  prologue.s
  irqstubs.s
  atwenty.c
  irqctrl.c
  kbdctrl.c
  memory.c
  netmain.c
  print.c
  rtccmos.c
  serial.c
  smallcode.c
  timer.c
)

target_include_directories(netboot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(netboot PRIVATE FORCE_DBG_PRINT=1 G_LOAD_ADDRESS=0x7c00  G_STACK_SIZE=0x1c00 G_BASE_ADDRESS=0x6000)
