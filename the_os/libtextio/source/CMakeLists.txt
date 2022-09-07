add_library(textio textio.cpp)
target_include_directories(textio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(textio PROPERTIES CXX_STANDARD 23)
target_link_libraries(textio PRIVATE misc)