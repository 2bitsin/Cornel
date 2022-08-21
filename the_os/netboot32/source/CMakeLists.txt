set(LINKER_SCRIPT               ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)
set(NETBOOT32_MAP               ${CMAKE_BINARY_DIR}/netboot32.map)
set(STARTUP_BINARY              ${CMAKE_BINARY_DIR}/startup.bin)
set(STARTUP_SOURCE              ${CMAKE_CURRENT_SOURCE_DIR}/startup.s)
set(CMAKE_EXECUTABLE_SUFFIX_C   ".sys")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".sys")
set(CMAKE_EXECUTABLE_SUFFIX_ASM ".sys")
set(NETBOOT_COMMON_FLAGS        "-m32 -march=i386 -ffreestanding -nostdlib -fbuiltin")
set(CMAKE_C_FLAGS               "${NETBOOT_COMMON_FLAGS} -mno-red-zone -fno-stack-protector")
set(CMAKE_CXX_FLAGS             "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions")
set(CMAKE_EXE_LINKER_FLAGS      "${NETBOOT_COMMON_FLAGS} -T ${LINKER_SCRIPT} -Xlinker -Map=${NETBOOT32_MAP}")
set(CMAKE_ASM_FLAGS             "${NETBOOT_COMMON_FLAGS} -x assembler-with-cpp")

find_program(FASM fasm)

add_custom_command(OUTPUT ${STARTUP_BINARY}
  BYPRODUCTS ${STARTUP_BINARY}
  COMMAND ${FASM} ${STARTUP_SOURCE} ${STARTUP_BINARY} 
  DEPENDS ${STARTUP_SOURCE}
  MAIN_DEPENDENCY  ${STARTUP_SOURCE}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Compiling startup.s to startup.bin"
  )  

add_custom_target(startup_bin DEPENDS ${STARTUP_BINARY})

add_executable(netboot32 preamble.s netmain.c)

set_property(SOURCE preamble.s PROPERTY OBJECT_DEPENDS ${STARTUP_BINARY})
set_property(SOURCE preamble.s PROPERTY COMPILE_DEFINITIONS "STARTUP_BINARY=\"${STARTUP_BINARY}\"")

set_target_properties(netboot32 PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

install(FILES ${NETBOOT32_MAP} DESTINATION ${CMAKE_INSTALL_PREFIX})