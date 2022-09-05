#create_test_sourcelist (textio_all_tests
#	t000_basic_write.cpp 
#	t001_basic_writeln.cpp
#	t002_basic_write_int32.cpp)
#
#add_executable(textio_all_tests_run ${textio_all_tests})
#target_link_libraries(textio_all_tests_run PUBLIC textio)
#set_target_properties(textio_all_tests_run PROPERTIES CXX_STANDARD 23)
#
#foreach (test ${textio_all_tests})
#  get_filename_component (test_name ${test} NAME_WE)
#  add_test (NAME ${test_name} COMMAND textio_all_tests_run ${test_name})
#endforeach ()
#
set (TEST_SOURCES 
	t000_simple_write.cpp 
	t001_simple_writeln.cpp
	t002_simple_integral.cpp
	t003_simple_fmt_repeat.cpp)

foreach (test_item ${TEST_SOURCES})
	get_filename_component(test_name ${test_item} NAME_WE)
	add_executable(textio.${test_name} ${test_item})
	target_link_libraries(textio.${test_name} textio)
	set_target_properties(textio.${test_name} PROPERTIES CXX_STANDARD 23)
	add_test(NAME ${test_name} COMMAND $<TARGET_FILE:textio.${test_name}>)
	install (TARGETS textio.${test_name} DESTINATION ${CMAKE_INSTALL_PREFIX})
endforeach()

