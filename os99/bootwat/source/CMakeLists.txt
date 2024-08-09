add_executable(bootwat main.cpp)
target_compile_features(bootwat PRIVATE cxx_std_23)
install(TARGETS bootwat DESTINATION bin)