add_executable(8as main.cpp file.cpp)
target_include_directories(8as PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_compile_features(8as PRIVATE cxx_std_17)