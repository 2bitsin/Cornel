set(LINKER_SCRIPT               ${CMAKE_CURRENT_SOURCE_DIR}/.link)
set(NETBOOT32_MAP               ${CMAKE_BINARY_DIR}/netboot32.map)

add_subdirectory(bootstrap)

add_executable(netboot32 
  interrupts.s 
  interrupts.cpp
  main.cpp
  runtime.cpp
)

target_include_directories(netboot32 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(netboot32 textio hardware bootstrap misc cimpl ${LIB_GCC_PATH})
set_target_properties(netboot32 PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
target_link_options(netboot32 PUBLIC -Xlinker -Map=${NETBOOT32_MAP} -T ${LINKER_SCRIPT})

install(FILES ${NETBOOT32_MAP} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(TARGETS netboot32 DESTINATION ${CMAKE_INSTALL_PREFIX})
