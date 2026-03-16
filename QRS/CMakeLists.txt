cmake_minimum_required(VERSION 3.16)
project(QRGenerate)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 查找依赖
find_package(OpenCV REQUIRED)
find_package(ZXing REQUIRED)

# 包含头文件目录
include_directories(include)

# 源文件
set(SOURCE_FILES
    src/main.cpp
    src/encoder.cpp
    src/decoder.cpp
    src/qrutils.cpp
    src/fileutils.cpp
    src/imageutils.cpp
)

# 头文件
set(HEADER_FILES
    include/encoder.h
    include/decoder.h
    include/qrutils.h
    include/fileutils.h
    include/imageutils.h
)

# 创建可执行文件
add_executable(QRGenerate ${SOURCE_FILES} ${HEADER_FILES})

# 链接依赖
target_link_libraries(QRGenerate PRIVATE ${OpenCV_LIBS} ZXing::ZXing)

# 复制测试文件到输出目录
file(COPY test DESTINATION ${CMAKE_BINARY_DIR}/bin)