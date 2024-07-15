add_executable(os99core main.cpp)
target_compile_features(os99core PRIVATE cxx_std_23)

set(OS99CORE_LINKER_SCRIPT    "${CMAKE_CURRENT_SOURCE_DIR}/.link")
set(OS99CORE_OUTPUT_MAP       "${CMAKE_BINARY_DIR}/os99core.map")
set(OS99CORE_COMPILE_OPTIONS  -fno-stack-protector -fno-exceptions -ffreestanding  -mno-red-zone -fno-rtti -nostdlib -fno-pic -m32)
set(OS99CORE_LINK_OPTIONS     -nostdlib -m32)

target_link_options(os99core PRIVATE "-T${OS99CORE_LINKER_SCRIPT}" "-Wl,-Map=${OS99CORE_OUTPUT_MAP}")
target_link_options(os99core PRIVATE "${OS99CORE_LINK_OPTIONS}")
target_compile_options(os99core PRIVATE "${OS99CORE_COMPILE_OPTIONS}")
set_target_properties(os99core PROPERTIES SUFFIX ".exe")
install(TARGETS os99core DESTINATION ${CMAKE_INSTALL_PREFIX}/core)