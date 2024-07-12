set (TEST_SOURCES 
  t000_hello_world.cpp
  t001_extract.cpp
	t002_elfload.cpp
)

set (TEST_PREFIX vfsio)

add_subdirectory(../../textio ./textio)
add_subdirectory(../../memory ./memory)
add_subdirectory(../../utils  ./utils)

foreach (test_item ${TEST_SOURCES})
  get_filename_component(test_name ${test_item} NAME_WE)
  set(TEST_TARGET ${TEST_PREFIX}.${test_name})
  add_executable(${TEST_TARGET} ${test_item})
	install(TARGETS ${TEST_TARGET} DESTINATION ${CMAKE_INSTALL_PREFIX})
  target_link_libraries(${TEST_TARGET} ${TEST_PREFIX})
  set_target_properties(${TEST_TARGET} PROPERTIES CXX_STANDARD 23)
  add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${TEST_TARGET}>)
  install (TARGETS ${TEST_TARGET} DESTINATION ${CMAKE_INSTALL_PREFIX})
endforeach()




