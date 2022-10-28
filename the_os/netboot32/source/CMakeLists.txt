add_subdirectory(bootstrap)
add_subdirectory(pxenv)
add_subdirectory(impl)
add_subdirectory(netboot)

add_executable(netboot32 netboot32.cpp)
target_link_libraries(netboot32 netboot pxenv textio hardware bootstrap utils cimpl memory script impl ${LIB_GCC_PATH})

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/.link)
set(NETBOOT32_MAP ${CMAKE_BINARY_DIR}/netboot32.map)  

set_target_properties(netboot32 PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
target_link_options(netboot32 PUBLIC -Xlinker -Map=${NETBOOT32_MAP} -T ${LINKER_SCRIPT})

install(FILES ${NETBOOT32_MAP} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(TARGETS netboot32 DESTINATION ${CMAKE_INSTALL_PREFIX})
