set (TEST_SOURCES 
  t000_allocator.cpp
	t001_ranges_overlap.cpp
	t002_align_range.cpp
	t003_insert_range.cpp
	t004_realloc.cpp
)

set (TEST_PREFIX memory)

add_subdirectory(${CMAKE_SOURCE_DIR}/../libtextio ${CMAKE_BINARY_DIR}/libtextio)

foreach (test_item ${TEST_SOURCES})
  get_filename_component(test_name ${test_item} NAME_WE)
  set(TEST_TARGET ${TEST_PREFIX}.${test_name})
  add_executable(${TEST_TARGET} ${test_item} ${SUPPORT_SOURCES})
	target_compile_definitions(${TEST_TARGET} PUBLIC _TESTING_=1)
  target_link_libraries(${TEST_TARGET} ${TEST_PREFIX})
  set_target_properties(${TEST_TARGET} PROPERTIES CXX_STANDARD 23)
  add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${TEST_TARGET}>)
  install (TARGETS ${TEST_TARGET} DESTINATION ${CMAKE_INSTALL_PREFIX})
endforeach()

