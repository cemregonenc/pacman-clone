cmake_minimum_required(VERSION 3.16)
project(PacmanClone CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU|AppleClang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Mac'te brew ile kurulan SFML'i bulması için ipucu
if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH
        "/opt/homebrew/opt/sfml@2"
        "/opt/homebrew/opt/sfml"
        "/usr/local/opt/sfml@2"
        "/usr/local/opt/sfml"
    )
endif()

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Maze.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics sfml-window sfml-system sfml-audio)

# assets klasörünü çalıştırılabilir dosyanın yanına kopyala
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)