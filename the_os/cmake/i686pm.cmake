set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i686)

find_program(CMAKE_C_COMPILER gcc-12)
set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_C_COMPILER_WORKS 1)


find_program(CMAKE_CXX_COMPILER g++-12)
set(CMAKE_CXX_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(COMMON_FLAGS                  "-m32 -march=i686 -fno-pie -ffreestanding -nostdlib -fbuiltin -flto")
set(CMAKE_C_FLAGS                 "${COMMON_FLAGS} -mno-red-zone -fno-stack-protector -Wall -Wpedantic -Wextra -Werror -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS               "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -std=c++23")
set(CMAKE_ASM_FLAGS               "${COMMON_FLAGS} -x assembler-with-cpp")
set(CMAKE_CXX_FLAGS_DEBUG_INIT    "-Os")
set(CMAKE_CXX_FLAGS_RELEASE_INIT  "-Os")
set(LIB_GCC_PATH                  "/usr/lib/gcc/x86_64-linux-gnu/12/32/libgcc.a")

