add_library(${LIB_NAME} "script.cpp")
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_compile_definitions(${LIB_NAME} PUBLIC _CONTAINER_DEBUG_LEVEL=0 _ITERATOR_DEBUG_LEVEL=0)
set_target_properties(${LIB_NAME} PROPERTIES CXX_STANDARD 23)