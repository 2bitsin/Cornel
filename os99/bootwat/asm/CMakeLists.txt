find_program(FASM fasm)

file(GLOB STUB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.asi)


add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/stub.bin
  COMMAND ${FASM} ${CMAKE_CURRENT_SOURCE_DIR}/bootwat.asm ${CMAKE_CURRENT_BINARY_DIR}/stub.bin
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/bootwat.asm ${STUB_SOURCES}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Assembling stub"
)

add_custom_target(stub ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/stub.bin)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/stub.bin DESTINATION bin)