set (TEST_SOURCES 
  t000_simple_write.cpp 
  t001_simple_writeln.cpp
  t002_simple_integral.cpp
  t003_simple_fmt_repeat.cpp
  t004_simple_fmt_base.cpp
  t005_simple_fmt_address.cpp
  t006_simple_tuple.cpp
  t007_simple_random0.cpp
	t008_simple_gmkb.cpp
)

set (TEST_PREFIX textio)

foreach (test_item ${TEST_SOURCES})
  get_filename_component(test_name ${test_item} NAME_WE)
  set(TEST_TARGET ${TEST_PREFIX}.${test_name})
  add_executable(${TEST_TARGET} ${test_item})
  target_link_libraries(${TEST_TARGET} ${TEST_PREFIX})
  set_target_properties(${TEST_TARGET} PROPERTIES CXX_STANDARD 23)
  add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${TEST_TARGET}>)
  install (TARGETS ${TEST_TARGET} DESTINATION ${CMAKE_INSTALL_PREFIX})
endforeach()



