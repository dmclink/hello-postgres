cmake_minimum_required(VERSION 3.25)

# fall back on directory name if args not passed with -D
if(NOT APP_NAME)
    get_filename_component(APP_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
endif()

project(${APP_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CSS_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE pqxx)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
