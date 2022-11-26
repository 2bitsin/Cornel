add_executable(os99core core_entry.cpp)

target_link_libraries(os99core ${LIB_GCC_PATH})

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/.link)
set(OS99CORE_MAP ${CMAKE_BINARY_DIR}/os99core.map)  

set_target_properties(os99core PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT} SUFFIX ".exe")
target_link_options(os99core PUBLIC -Xlinker -Map=${OS99CORE_MAP} -T ${LINKER_SCRIPT})

install(FILES ${OS99CORE_MAP} DESTINATION ${CMAKE_INSTALL_PREFIX}/maps)
install(TARGETS os99core DESTINATION ${CMAKE_INSTALL_PREFIX}/os99)
