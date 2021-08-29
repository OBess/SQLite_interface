
find_package(Threads)

file(GLOB sources_c "src/*.c")
file(GLOB sources_cpp "src/*.cpp")

add_library(sqlite STATIC ${sources_c})
target_include_directories(sqlite PUBLIC include)

add_library(api STATIC ${sources_cpp})
target_include_directories(api PUBLIC include)
target_link_libraries(api PUBLIC sqlite Threads::Threads ${_lib64_path}libdl.so)