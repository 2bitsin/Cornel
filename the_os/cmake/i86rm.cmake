set(CMAKE_HOST_SYSTEM_NAME Linux)
set(CMAKE_HOST_SYSTEM_PROCESSOR x86_64)
set(CMAKE_SYSTEM_NAME DOS)
set(CMAKE_SYSTEM_PROCESSOR I86)

set(WATCOM_DIR $ENV{WATCOM})

set(CMAKE_C_COMPILER ${WATCOM_DIR}/binl/wcl)
set(CMAKE_C_COMPILER_ID OpenWatcom)
set(CMAKE_C_COMPILER_WORKS 1)

set(CMAKE_CXX_COMPILER ${WATCOM_DIR}/binl/wcl)
set(CMAKE_CXX_COMPILER_ID OpenWatcom)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_ASM_COMPILER ${WATCOM_DIR}/binl/wasm)
set(CMAKE_ASM_COMPILER_ID OpenWatcom)
set(CMAKE_ASM_COMPILER_WORKS 1)

set(CMAKE_ASM_FLAGS_INIT "-3 -mt")
set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -fo=<OBJECT> <SOURCE>")

set(CMAKE_FIND_ROOT_PATH ${WATCOM_DIR}/lib286;${WATCOM_DIR}/lh;${WATCOM_DIR}/h;${WATCOM_DIR}/binl)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_EXECUTABLE_SUFFIX_C ".sys")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".sys")
set(CMAKE_EXECUTABLE_SUFFIX_ASM ".sys")

