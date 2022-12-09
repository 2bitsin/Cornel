set (TEST_SOURCES 
  t000_format_basics.cpp
  t001_format_statement.cpp
  t002_format_user_convert.cpp
  t003_format_helpers_repeat.cpp
  t004_logger_basic.cpp
  t005_logger_debug.cpp
)

set (TEST_PREFIX textio)

add_subdirectory(../../meta ./meta)

foreach (test_item ${TEST_SOURCES})
  get_filename_component(test_name ${test_item} NAME_WE)
  set(TEST_TARGET ${TEST_PREFIX}.${test_name})
  add_executable(${TEST_TARGET} ${test_item})
  target_link_libraries(${TEST_TARGET} ${TEST_PREFIX})
  set_target_properties(${TEST_TARGET} PROPERTIES CXX_STANDARD 23)
  set_target_properties(${TEST_TARGET} PROPERTIES COMPILE_FLAGS -m32)
  set_target_properties(${TEST_TARGET} PROPERTIES LINK_FLAGS -m32)
  add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${TEST_TARGET}>)
  install (TARGETS ${TEST_TARGET} DESTINATION ${CMAKE_INSTALL_PREFIX})
endforeach()




