set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i386)

find_program(CMAKE_C_COMPILER gcc-12)
set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_C_COMPILER_WORKS 1)


find_program(CMAKE_CXX_COMPILER g++-12)
set(CMAKE_CXX_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_EXECUTABLE_SUFFIX_C   ".sys")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".sys")
set(CMAKE_EXECUTABLE_SUFFIX_ASM ".sys")
set(COMMON_FLAGS                "-m32 -march=i386 -fno-pie -ffreestanding -nostdlib -fbuiltin")
set(CMAKE_C_FLAGS               "${COMMON_FLAGS} -mno-red-zone -fno-stack-protector")
set(CMAKE_CXX_FLAGS             "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -std=c++23")
set(CMAKE_ASM_FLAGS             "${COMMON_FLAGS} -x assembler-with-cpp")
set(LIB_GCC_PATH                "/usr/lib/gcc/x86_64-linux-gnu/11/32/libgcc.a")
