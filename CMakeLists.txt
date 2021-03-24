cmake_minimum_required(VERSION 3.10)
project(ECSGame VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# get all source files in the source directory
#file( GLOB_RECURSE  sourcefiles ${PROJECT_SOURCE_DIR}/src/*.cpp)
file( GLOB_RECURSE  sourcefiles ${PROJECT_SOURCE_DIR}/src/main.cpp ${PROJECT_SOURCE_DIR}/src/ECS/ECS.cpp)

#if (UNIX)
#    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#endif(UNIX)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

add_executable(game ${sourcefiles})
target_include_directories(game PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(game PRIVATE glm)
target_link_libraries(game PRIVATE sfml-graphics sfml-audio)
target_link_libraries(game PRIVATE Eigen3::Eigen)
target_link_libraries(game PRIVATE yaml-cpp)