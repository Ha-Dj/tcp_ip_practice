# 设置最低的 CMake 版本要求
cmake_minimum_required(VERSION 3.22)

# 设置项目名称
project(op_server)

# 设置 C 标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# 添加可执行文件
add_executable(client src/client.c)
add_executable(server src/server.c)

# 设置输出目录
set_target_properties(client server PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)
