cmake_minimum_required(VERSION 3.8...3.27)

project(MyProject VERSION 1.0
					DESCRIPTION "A test project"
					LANGUAGES CXX)


FILE(GLOB MySources src/*.cpp lib/glad.cpp)
add_library(calclib STATIC ${MySources})
target_include_directories(calclib PUBLIC include)
target_include_directories(calclib PUBLIC /usr/local/include)
target_include_directories(calclib PUBLIC lib)
target_compile_features(calclib PUBLIC cxx_std_17)



add_executable(out src/main.cpp)

target_link_libraries(out PUBLIC calclib)
target_link_libraries(out PUBLIC /opt/homebrew/opt/glfw/lib/libglfw.3.dylib)
