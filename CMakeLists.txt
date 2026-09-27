cmake_minimum_required(VERSION 4.3)
project(simulated_autonomous_sensor_hub C)

set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME}
        src/main.c
        include/data.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -O3 -lpthread)
