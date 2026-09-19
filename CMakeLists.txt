cmake_minimum_required(VERSION 4.3)
project(simulated_autonomous_sensor_hub C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(${PROJECT_NAME}
        src/main.c
        include/data.h
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)
