add_library(vfsio 
	heapfile.cpp
	cstdfile.cpp
	archive.cpp 
	vfsio.cpp 
  archive_view.cpp
  "elf_load.cpp"
)

target_include_directories(vfsio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_compile_definitions(vfsio PUBLIC _CONTAINER_DEBUG_LEVEL=0 _ITERATOR_DEBUG_LEVEL=0)
set_target_properties(vfsio PROPERTIES CXX_STANDARD 23)
target_link_libraries(vfsio PUBLIC textio memory utils)
