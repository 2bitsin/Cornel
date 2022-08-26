set(LINKER_SCRIPT               ${CMAKE_CURRENT_SOURCE_DIR}/.link)
set(NETBOOT32_MAP               ${CMAKE_BINARY_DIR}/netboot32.map)
set(CMAKE_EXECUTABLE_SUFFIX_C   ".sys")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".sys")
set(CMAKE_EXECUTABLE_SUFFIX_ASM ".sys")
set(NETBOOT_COMMON_FLAGS        "-m32 -march=i386 -fno-pie -ffreestanding -nostdlib -fbuiltin")
set(CMAKE_C_FLAGS               "${NETBOOT_COMMON_FLAGS} -mno-red-zone -fno-stack-protector")
set(CMAKE_CXX_FLAGS             "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -std=c++23")
set(CMAKE_EXE_LINKER_FLAGS      "${NETBOOT_COMMON_FLAGS} -T ${LINKER_SCRIPT} -Xlinker -Map=${NETBOOT32_MAP}")
set(CMAKE_ASM_FLAGS             "${NETBOOT_COMMON_FLAGS} -x assembler-with-cpp")
set(LIB_GCC_PATH                "/usr/lib/gcc/x86_64-linux-gnu/11/32/libgcc.a")

add_subdirectory(bootstrap)

add_subdirectory(${CMAKE_SOURCE_DIR}/../libcimpl ${CMAKE_BINARY_DIR}/libcimpl)

add_executable(netboot32 
  interrupts.s 
  interrupts.cpp
  main.cpp
  display.cpp
  globals.cpp
)

target_include_directories(netboot32 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(netboot32 libcimpl bootstrap ${LIB_GCC_PATH})

set_target_properties(netboot32 PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

install(FILES ${NETBOOT32_MAP} DESTINATION ${CMAKE_INSTALL_PREFIX})