cmake_minimum_required(VERSION 3.10)
project(ECSGame VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# get all source files in the source directory
file( GLOB_RECURSE  sourcefiles ${PROJECT_SOURCE_DIR}/src/ECS/*.cpp
							${PROJECT_SOURCE_DIR}/src/Systems/*.cpp
							${PROJECT_SOURCE_DIR}/src/main.cpp
							${PROJECT_SOURCE_DIR}/src/App.cpp
							${PROJECT_SOURCE_DIR}/src/Factories.cpp
							${PROJECT_SOURCE_DIR}/src/Renderer/*.cpp
							${PROJECT_SOURCE_DIR}/src/Scene/*.cpp
							${PROJECT_SOURCE_DIR}/src/Utils/*.cpp
							${PROJECT_SOURCE_DIR}/src/Utils/*.c
)
#file( GLOB_RECURSE  sourcefiles ${PROJECT_SOURCE_DIR}/src/main.cpp
#							${PROJECT_SOURCE_DIR}/src/ECS/ECS.cpp
#							)

#if (UNIX)
#    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#endif(UNIX)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(ZLIB REQUIRED)


add_executable(game ${sourcefiles})
add_executable(tests ${PROJECT_SOURCE_DIR}/src/Tests/test_serialization.cpp
 ${PROJECT_SOURCE_DIR}/src/ECS/ECS.cpp)

target_include_directories(game PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(game PRIVATE glm GLEW::GLEW glfw nlohmann_json nlohmann_json::nlohmann_json ZLIB::ZLIB)
target_link_libraries(tests PRIVATE glm GLEW::GLEW sfml-graphics sfml-audio nlohmann_json nlohmann_json::nlohmann_json)

set_target_properties(game PROPERTIES CXX_STANDARD 17)
set_target_properties(game PROPERTIES CXX_STANDARD_REQUIRED ON)
target_compile_definitions(game PRIVATE "SOURCE_DIRECTORY=\"${CMAKE_SOURCE_DIR}\"")
